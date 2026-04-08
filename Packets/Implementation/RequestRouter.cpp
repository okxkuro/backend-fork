#include <RequestRouter.h>
#include <SpectreWebsocket.h>
#include <restinio/all.hpp>
#include <restinio/websocket/websocket.hpp>
#include <string>
#include <utility>

std::unordered_map<uint16_t, std::unique_ptr<restinio::router::express_router_t<>>> RequestRouter::routers{};
std::vector<restinio::running_server_handle_t<RestinioServerTraits>> RequestRouter::servers{};
std::vector<std::shared_ptr<SpectreWebsocket>> RequestRouter::websocketConnections{};

restinio::request_handling_status_t RequestRouter::NonMatchedHTTPProcessor(const restinio::request_handle_t& req) {
    if (req->header().connection() == restinio::http_connection_header_t::upgrade) {
        // upgrade connection to websocket
        std::shared_ptr<SpectreWebsocket> newWS = std::make_shared<SpectreWebsocket>(req);
        websocketConnections.emplace_back(newWS);
        // Upgrade the websocket connection and bind the message handler to the new SpectreWebsocket instance
        rws::ws_handle_t websocketHandle = rws::upgrade<RestinioServerTraits>(*req, rws::activation_t::immediate,
                                                                              [newWS](rws::ws_handle_t wsHandle, rws::message_handle_t wsMessage) {
                                                                                  newWS->OnReceiveWebsocketMessage(std::move(wsHandle), std::move(wsMessage));
                                                                              });
        websocketConnections.back()->websocketHandle = websocketHandle;
        return restinio::request_accepted();
    }
    spdlog::warn("No processor found for a {} request to route {}", req->header().method().c_str(), req->header().request_target());
    return restinio::request_handling_status_t::not_handled;
}

void RequestRouter::CreateRouter(uint16_t port) {
    auto router = std::make_unique<restinio::router::express_router_t<>>();
    router->non_matched_request_handler([](auto req) {
        return NonMatchedHTTPProcessor(req);
    });
    routers[port] = std::move(router);
}

void RequestRouter::RegisterHTTPProcessor(uint16_t port, HTTPPacketProcessor* processor) {
    if (processor == nullptr) {
        spdlog::warn("Tried to register route for null http packet processor, ignoring");
        return;
    }
    auto router = routers.find(port);
    if (router == routers.end()) {
        spdlog::error("Failed to find router for port {}, packet processor for route {} will not be registered", port, processor->GetRoute());
        return;
    }
    if (processor->GetMethod() == HTTPRequestType::GET) {
        router->second->http_get(processor->GetRoute(), [processor](auto&& PH1, auto&& PH2) { return processor->ProcessResolveOptional(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); });
    } else if (processor->GetMethod() == HTTPRequestType::POST) {
        router->second->http_post(processor->GetRoute(), [processor](auto&& PH1, auto&& PH2) { return processor->ProcessResolveOptional(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); });
    } else if (processor->GetMethod() == HTTPRequestType::PUT) {
        router->second->http_put(processor->GetRoute(), [processor](auto&& PH1, auto&& PH2) { return processor->ProcessResolveOptional(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); });
    } else if (processor->GetMethod() == HTTPRequestType::DEL) {
        router->second->http_delete(processor->GetRoute(), [processor](auto&& PH1, auto&& PH2) { return processor->ProcessResolveOptional(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); });
    } else if (processor->GetMethod() == HTTPRequestType::HEAD) {
        router->second->http_head(processor->GetRoute(), [processor](auto&& PH1, auto&& PH2) { return processor->ProcessResolveOptional(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); });
    } else {
        spdlog::error("Failed to register HTTP processor for port {} and route {} due to an unrecognized HTTP method", port, processor->GetRoute());
    }
}

void RequestRouter::RegisterHTTPProcessor(HTTPPacketProcessor* processor) {
    for (auto& [port, router] : routers) {
        RegisterHTTPProcessor(port, processor);
    }
}

void RequestRouter::Start() {
    for (auto& [port, router] : routers) {
        servers.push_back(restinio::run_async<RestinioServerTraits>(
            restinio::own_io_context(),
            restinio::server_settings_t<RestinioServerTraits>{}
                .port(port)
                .address("0.0.0.0")
                .request_handler(std::move(router))
                .protocol(asio::ip::tcp::v4()),
            //.logger(spdlog_logger_t()),
            2));
    }
}

void RequestRouter::Shutdown() {
    for (restinio::running_server_handle_t<RestinioServerTraits>& server : servers) {
        server->stop();
        server->wait();
    }
}
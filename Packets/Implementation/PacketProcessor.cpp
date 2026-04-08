#include <PacketProcessor.h>
#include <RequestRouter.h>
#include <utility>

HTTPPacketProcessor::HTTPPacketProcessor(HTTPRequestIdentifier routeId)
    : routeId(std::move(std::move(routeId))) {
    RequestRouter::RegisterHTTPProcessor(this);
}

HTTPPacketProcessor::HTTPPacketProcessor(HTTPRequestIdentifier routeId, uint16_t port)
    : routeId(std::move(std::move(routeId))) {
    RequestRouter::RegisterHTTPProcessor(port, this);
}

restinio::request_handling_status_t HTTPPacketProcessor::ProcessResolveOptional(restinio::request_handle_t req, restinio::router::route_params_t params) {
    std::optional<restinio::response_builder_t<restinio::restinio_controlled_output_t>> response = Process(std::move(req), std::move(params));
    if (!response.has_value()) {
        return restinio::request_handling_status_t::not_handled;
    }
    return response.value().done();
}
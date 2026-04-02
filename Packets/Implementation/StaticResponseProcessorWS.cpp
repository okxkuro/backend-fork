#include <StaticResponseProcessorWS.h>
#include <nlohmann/json.hpp>

std::optional<WebsocketPayload> StaticResponseProcessorWS::Process(SpectreWebsocketRequest& packet) {
    nlohmann::json res = *staticRes;
    return res;
}
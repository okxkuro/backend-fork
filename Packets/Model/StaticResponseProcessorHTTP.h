#pragma once

#include <PacketProcessor.h>

class StaticResponseProcessorHTTP : public HTTPPacketProcessor {
  private:
    std::shared_ptr<nlohmann::json> staticRes;

  public:
    StaticResponseProcessorHTTP(HTTPRequestIdentifier id, const std::shared_ptr<nlohmann::json>& res)
        : HTTPPacketProcessor(id), staticRes(res){};

    std::optional<restinio::response_builder_t<restinio::restinio_controlled_output_t>> Process(restinio::request_handle_t req, restinio::router::route_params_t params) override;
};
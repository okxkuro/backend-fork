#pragma once
#include <google/protobuf/message.h>
#include <string>
#include <nlohmann/json.hpp>

class WebsocketPayload {
private:
    std::string payload;
    public:
    WebsocketPayload(const google::protobuf::Message& message);
    WebsocketPayload(std::string  payload);
    WebsocketPayload(const nlohmann::json& json);
    [[nodiscard]] const std::string& GetPayload() const;
};
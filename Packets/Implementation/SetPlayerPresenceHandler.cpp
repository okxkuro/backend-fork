#include <PlayerDatabase.h>
#include <SetPlayerPresenceHandler.h>
#include <SetPresenceRequest.pb.h>

SetPlayerPresenceHandler::SetPlayerPresenceHandler(SpectreRpcType rpcType) : WebsocketPacketProcessor(rpcType) {
}

std::optional<WebsocketPayload> SetPlayerPresenceHandler::Process(SpectreWebsocketRequest& packet) {
    std::unique_ptr<SetPresenceRequest> req = packet.GetPayloadAsMessage<SetPresenceRequest>();

    std::unique_ptr<PlayerPresence> presence = PlayerDatabase::Get().GetField<PlayerPresence>(FieldKey::PLAYER_PRESENCE, packet.GetPlayerId());
    presence->set_basicpresence(req->basicpresence());
    UpdatePlayerPresence(*presence, packet.GetPlayerId());

    nlohmann::json res{};
    res["response"] = "Ok";
    return res;
}

void UpdatePlayerPresence(PlayerPresence& newPresence, const std::string& playerId) {
    int version = std::stoi(newPresence.version());
    version++;
    newPresence.set_version(std::to_string(version));
    PlayerDatabase::Get().SetField(FieldKey::PLAYER_PRESENCE, &newPresence, playerId);
}
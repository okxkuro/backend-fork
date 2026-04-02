#include "PlayerDatabase.h"
#include "SetPlayerPresenceHandler.h"

#include <FriendsList.pb.h>
#include <GetFriendsListAndRegisterOnlineHandler.h>
#include <PlayerPresence.pb.h>

GetFriendsListAndRegisterOnlineHandler::GetFriendsListAndRegisterOnlineHandler(SpectreRpcType rpcType) : WebsocketPacketProcessor(rpcType) {

}

std::optional<WebsocketPayload> GetFriendsListAndRegisterOnlineHandler::Process(SpectreWebsocketRequest& packet) {
    std::unique_ptr<FriendsList> friendsList = PlayerDatabase::Get().GetField<FriendsList>(FieldKey::FRIENDS_LIST, packet.GetPlayerId());
    std::unique_ptr<PlayerPresence> presence = PlayerDatabase::Get().GetField<PlayerPresence>(FieldKey::PLAYER_PRESENCE, packet.GetPlayerId());
    presence->set_basicpresence(Online);
    UpdatePlayerPresence(*presence, packet.GetPlayerId());
    return *friendsList;
}
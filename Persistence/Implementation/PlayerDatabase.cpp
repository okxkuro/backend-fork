#include "PlayerDatabase.h"

#include "PersistenceUtilities.h"

#include <FriendsList.pb.h>
#include <Inventory.pb.h>
#include <LegacyPlayerData.pb.h>
#include <OutfitLoadout.pb.h>
#include <PlayerData.pb.h>
#include <PlayerPresence.pb.h>
#include <ProfileData.pb.h>
#include <ResourcesUtilities.h>
#include <SavedNotificationData.pb.h>
#include <WeaponLoadout.pb.h>

PlayerDatabase::PlayerDatabase(const fs::path& path)
    : Database(path, "players", "PlayerID", "TEXT") {
    AddPrototype(FieldKey::PLAYER_INVENTORY, DatabaseFieldData::WithDefaultFromPath<Inventory>(FieldKey::PLAYER_INVENTORY, "inventory", ResourcesUtilities::GetResourcesFolder() / "payloads" / "ws" / "game" / "DefaultInventory.json"));
    AddPrototype(FieldKey::PLAYER_OUTFIT_LOADOUT, DatabaseFieldData::WithDefaultFromPath<OutfitLoadouts>(FieldKey::PLAYER_OUTFIT_LOADOUT, "outfitLoadouts", ResourcesUtilities::GetResourcesFolder() / "payloads" / "ws" / "game" / "DefaultOutfitLoadout.json"));
    AddPrototype(FieldKey::PLAYER_WEAPON_LOADOUT, DatabaseFieldData::WithDefaultFromPath<WeaponLoadouts>(FieldKey::PLAYER_WEAPON_LOADOUT, "weaponLoadouts", ResourcesUtilities::GetResourcesFolder() / "payloads" / "ws" / "game" / "DefaultWeaponLoadout.json"));
    AddPrototype(FieldKey::PLAYER_DATA, DatabaseFieldData::WithDefaultFromPath<PlayerData>(FieldKey::PLAYER_DATA, "playerData", ResourcesUtilities::GetResourcesFolder() / "payloads" / "ws" / "game" / "DefaultPlayerData.json"));
    AddPrototype(FieldKey::PROFILE_DATA, DatabaseFieldData::WithDefaultFromPath<ProfileData>(FieldKey::PROFILE_DATA, "profileData", ResourcesUtilities::GetResourcesFolder() / "payloads" / "ws" / "game" / "DefaultProfile.json"));
    AddPrototype(FieldKey::PLAYER_LEGACY_DATA, DatabaseFieldData::WithDefaultFromPath<LegacyPlayerData>(FieldKey::PLAYER_LEGACY_DATA, "legacyData", ResourcesUtilities::GetResourcesFolder() / "payloads" / "ws" / "game" / "DefaultLegacyData.json"));
    AddPrototype(FieldKey::PLAYER_PRESENCE, DatabaseFieldData::WithDefaultFromPath<PlayerPresence>(FieldKey::PLAYER_PRESENCE, "presence", ResourcesUtilities::GetResourcesFolder() / "payloads" / "ws" / "game" / "DefaultPlayerPresence.json"));
    AddPrototype(FieldKey::FRIENDS_LIST, DatabaseFieldData::WithDefaultFromPath<FriendsList>(FieldKey::FRIENDS_LIST, "friendsList", ResourcesUtilities::GetResourcesFolder() / "payloads" / "ws" / "game" / "DefaultFriendsList.json"));
    AddPrototype(FieldKey::NOTIFICATION_DATA, DatabaseFieldData::WithDefaultFromPath<SavedNotificationData>(FieldKey::NOTIFICATION_DATA, "notificationData", ResourcesUtilities::GetResourcesFolder() / "payloads" / "ws" / "game" / "DefaultSavedNotifications.json"));
}

PlayerDatabase PlayerDatabase::inst(PersistenceUtilities::GetSavePath() / "playerdata.sqlite");

PlayerDatabase& PlayerDatabase::Get() {
    return inst;
}
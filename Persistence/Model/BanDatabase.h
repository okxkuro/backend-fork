#pragma once
#include "Database.h"

class BanDatabase : public Database {
private:
    static BanDatabase inst;
public:
    static BanDatabase& Get();
    BanDatabase(const fs::path& path);
    bool IsBanned(const std::string& playerId);
};
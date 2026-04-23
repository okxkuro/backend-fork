#include "PersistenceUtilities.h"

#include <BackendEnvironment.h>
#include <ResourcesUtilities.h>
#include <filesystem>
#include <thread>
#include <windows.h>

namespace fs = std::filesystem;

static void RunBackendWindows() {
    STARTUPINFOA si = {sizeof(si)};
    PROCESS_INFORMATION pi = {};
    std::filesystem::path exePath = ResourcesUtilities::GetCurrentExecutablePath().parent_path().parent_path() /
                                    "pragmabackend.exe";
    BOOL ok = CreateProcessA("C:\\Windows\\System32\\cmd.exe",
        (LPSTR)(std::string("/c ") + exePath.string()).c_str(),
        nullptr,
        nullptr,
        TRUE,
        CREATE_NEW_PROCESS_GROUP,
        nullptr,
        nullptr,
        &si,
        &pi);
    FreeConsole();
    AttachConsole(pi.dwProcessId);
    SetConsoleCtrlHandler(nullptr, TRUE);
}

void BackendEnvironment::SetUp() {
#if defined(_WIN32)
    std::filesystem::path exePath = ResourcesUtilities::GetCurrentExecutablePath().parent_path().parent_path() /
                                    "pragmabackend.exe";
    std::system("taskkill /f /im pragmabackend.exe"); // NOLINT
#elif defined(__linux__)
    std::filesystem::path exePath = ResourcesUtilities::GetCurrentExecutablePath().parent_path().parent_path() /
                                    "pragmabackend";
    std::system("pkill -9 pragmabackend"); // NOLINT
#endif
    std::filesystem::remove(PersistenceUtilities::GetSavePath() / "playerdata.sqlite");
    RunBackendWindows();
    while (true) {
        if (fs::exists(ResourcesUtilities::GetCurrentExecutablePath().parent_path().parent_path() / "server.lock")) {
            break;
        }
    }
}

void BackendEnvironment::TearDown() {
    GenerateConsoleCtrlEvent(NULL, FALSE);
    std::this_thread::sleep_for(std::chrono::seconds(1)); // wait for windows to release file handle
    std::filesystem::remove(PersistenceUtilities::GetSavePath() / "playerdata.sqlite");
    std::filesystem::remove(ResourcesUtilities::GetCurrentExecutablePath().parent_path().parent_path() / "server.lock");
}
// SPDX-License-Identifier: MIT
//
// src/PluginMain.cpp
//
// DLL entry point and the single MQGetPluginClass() export Metasequoia
// uses to discover our station plugin instance.

#include "PluginConfig.h"
#include "PhysicsStation.h"

#include <MQBasePlugin.h>

#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*reserved*/) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
#endif // _WIN32

namespace {
// Singleton plugin instance owned by the DLL. Metasequoia calls
// GetPluginClass() any number of times and always expects the same pointer.
mqphys::PhysicsStation& GetStationInstance() {
    static mqphys::PhysicsStation instance;
    return instance;
}
} // namespace

// Required global hook: returns the plugin's MQBasePlugin subclass.
MQBasePlugin* GetPluginClass() {
    return &GetStationInstance();
}

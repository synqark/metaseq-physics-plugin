// SPDX-License-Identifier: MIT
//
// tests/mock_sdk/mqsdk_mock.cpp
//
// **MOCK** SDK runtime — provides empty implementations for SDK symbols our
// plugin links against during CI compile checks.

#include "MQBasePlugin.h"

// Real SDK provides this; the plugin returns its own subclass.
extern MQBasePlugin* GetPluginClass();

extern "C" {

MQPLUGIN_EXPORT MQBasePlugin* MQGetPlugInClass() {
    return GetPluginClass();
}

MQPLUGIN_EXPORT void MQGetPlugInID(unsigned int* product, unsigned int* id) {
    auto* p = GetPluginClass();
    if (p) p->GetPlugInID(product, id);
}

MQPLUGIN_EXPORT const char* MQGetPlugInName() {
    auto* p = GetPluginClass();
    return p ? p->GetPlugInName() : "";
}

MQPLUGIN_EXPORT int MQGetPlugInType() {
    auto* p = GetPluginClass();
    return p ? p->GetPlugInType() : 0;
}

MQPLUGIN_EXPORT const char* MQEnumString() {
    auto* p = GetPluginClass();
    return p ? p->EnumString() : "";
}

MQPLUGIN_EXPORT bool MQInit() {
    auto* p = GetPluginClass();
    return p ? p->Initialize() : false;
}

MQPLUGIN_EXPORT void MQExit() {
    if (auto* p = GetPluginClass()) p->Exit();
}

} // extern "C"

// SPDX-License-Identifier: MIT
//
// tests/mock_sdk/MQBasePlugin.h
//
// **MOCK** header — see MQPlugin.h for context.

#pragma once

#include "MQPlugin.h"

class MQBasePlugin {
public:
    virtual ~MQBasePlugin() = default;

    // Plugin identity — overridden by the concrete plugin.
    virtual void GetPlugInID(unsigned int* product, unsigned int* id) = 0;
    virtual const char* GetPlugInName() = 0;
    virtual int GetPlugInType() = 0;
    virtual const char* EnumString() { return ""; }

    // Lifecycle.
    virtual bool Initialize() { return true; }
    virtual void Exit() {}
};

class MQStationPlugin : public MQBasePlugin {
public:
    int GetPlugInType() override { return MQPLUGIN_TYPE_STATION; }

    // The real SDK has many more hooks; we only declare the ones we use.
    virtual bool Activate(MQDocument /*doc*/, bool /*flag*/) { return false; }
    virtual bool IsActivated(MQDocument /*doc*/) { return false; }
    virtual void OnDraw(MQDocument /*doc*/, MQScene /*scene*/, int /*w*/, int /*h*/) {}
    virtual bool OnUpdateScene(MQDocument /*doc*/, MQScene /*scene*/) { return false; }
    virtual void OnNewDocument(MQDocument /*doc*/, const char* /*filename*/, MQXmlElement /*elem*/) {}
    virtual void OnEndDocument(MQDocument /*doc*/) {}
    virtual void OnSaveDocument(MQDocument /*doc*/, const char* /*filename*/, MQXmlElement /*elem*/) {}
    virtual bool ExecuteCallback(MQDocument /*doc*/, void* /*option*/) { return false; }
};

// Provided by the SDK runtime in real builds; mocked here.
MQBasePlugin* GetPluginClass();

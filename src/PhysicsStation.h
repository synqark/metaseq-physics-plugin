// SPDX-License-Identifier: MIT
//
// src/PhysicsStation.h
//
// MQStationPlugin subclass — owns the simulation world, the docked UI, and
// the per-document state. Implements the SDK hooks required to be loaded
// as a Station plugin.

#pragma once

#include "PluginConfig.h"
#include "PhysicsWorld.h"
#include "PropertyStore.h"
#include "SimulationCache.h"

#include <MQBasePlugin.h>

#include <memory>

namespace mqphys {

class PhysicsPanel; // forward (src/ui/PhysicsPanel.h)

class PhysicsStation final : public MQStationPlugin {
public:
    PhysicsStation();
    ~PhysicsStation() override;

    // --- MQBasePlugin -----------------------------------------------------
    void GetPlugInID(unsigned int* product, unsigned int* id) override;
    const char* GetPlugInName() override;
    const char* EnumString() override;
    bool Initialize() override;
    void Exit() override;

    // --- MQStationPlugin --------------------------------------------------
    bool Activate(MQDocument doc, bool flag) override;
    bool IsActivated(MQDocument doc) override;
    void OnDraw(MQDocument doc, MQScene scene, int w, int h) override;
    bool OnUpdateScene(MQDocument doc, MQScene scene) override;
    void OnNewDocument(MQDocument doc, const char* filename, MQXmlElement elem) override;
    void OnEndDocument(MQDocument doc) override;
    void OnSaveDocument(MQDocument doc, const char* filename, MQXmlElement elem) override;

    // --- Public API used by the UI panel ---------------------------------
    void Play();
    void Pause();
    void Reset(MQDocument doc);
    bool IsPlaying() const { return m_playing; }

    PhysicsWorld&  World()      { return *m_world; }
    PropertyStore& Properties() { return m_properties; }

private:
    void RebuildFromDocument(MQDocument doc);
    void StepSimulation(MQDocument doc, float dtSeconds);
    void WriteBackVertices(MQDocument doc);

    bool m_activated{false};
    bool m_playing{false};
    double m_accumulatorSeconds{0.0};

    std::unique_ptr<PhysicsWorld>  m_world;
    std::unique_ptr<PhysicsPanel>  m_panel;
    PropertyStore                  m_properties;
    SimulationCache                m_cache;
};

} // namespace mqphys

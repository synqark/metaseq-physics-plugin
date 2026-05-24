// SPDX-License-Identifier: MIT
//
// src/PhysicsStation.cpp

#include "PhysicsStation.h"
#include "ui/PhysicsPanel.h"

#include <algorithm>

namespace mqphys {

PhysicsStation::PhysicsStation()
    : m_world(std::make_unique<PhysicsWorld>()) {}

PhysicsStation::~PhysicsStation() = default;

// ---------------------------------------------------------------------------
// MQBasePlugin
// ---------------------------------------------------------------------------
void PhysicsStation::GetPlugInID(unsigned int* product, unsigned int* id) {
    if (product) *product = kPluginProductID;
    if (id)      *id      = kPluginSubID;
}

const char* PhysicsStation::GetPlugInName() {
    return kPluginName;
}

const char* PhysicsStation::EnumString() {
    // Station plugins typically return their menu label here.
    return kPluginName;
}

bool PhysicsStation::Initialize() {
    if (!m_panel) {
        m_panel = std::make_unique<PhysicsPanel>(*this);
    }
    return true;
}

void PhysicsStation::Exit() {
    m_panel.reset();
    m_world.reset();
}

// ---------------------------------------------------------------------------
// MQStationPlugin
// ---------------------------------------------------------------------------
bool PhysicsStation::Activate(MQDocument doc, bool flag) {
    m_activated = flag;
    if (m_panel) {
        m_panel->SetVisible(flag);
    }
    if (flag) {
        RebuildFromDocument(doc);
    } else {
        m_playing = false;
    }
    return m_activated;
}

bool PhysicsStation::IsActivated(MQDocument /*doc*/) {
    return m_activated;
}

void PhysicsStation::OnDraw(MQDocument /*doc*/, MQScene /*scene*/, int /*w*/, int /*h*/) {
    // Phase 2: debug draw of collision shapes / cloth pin points.
}

bool PhysicsStation::OnUpdateScene(MQDocument doc, MQScene /*scene*/) {
    if (!m_activated || !m_playing) {
        return false;
    }
    // Drive the simulation at a fixed step regardless of the host's frame
    // pacing — Metasequoia calls OnUpdateScene roughly per redraw.
    constexpr float fixed = kFixedTimeStep;
    StepSimulation(doc, fixed);
    WriteBackVertices(doc);
    return true; // request a redraw
}

void PhysicsStation::OnNewDocument(MQDocument doc, const char* /*filename*/, MQXmlElement /*elem*/) {
    m_properties.Clear();
    m_cache.Clear();
    if (m_world) m_world->Reset();
    RebuildFromDocument(doc);
}

void PhysicsStation::OnEndDocument(MQDocument /*doc*/) {
    m_playing = false;
    m_properties.Clear();
    m_cache.Clear();
    if (m_world) m_world->Reset();
}

void PhysicsStation::OnSaveDocument(MQDocument /*doc*/, const char* /*filename*/, MQXmlElement /*elem*/) {
    // Phase 4: persist physics roles & parameters as plugin-private chunk.
}

// ---------------------------------------------------------------------------
// Public control surface (used by PhysicsPanel)
// ---------------------------------------------------------------------------
void PhysicsStation::Play() {
    m_playing = true;
    m_accumulatorSeconds = 0.0;
}

void PhysicsStation::Pause() {
    m_playing = false;
}

void PhysicsStation::Reset(MQDocument doc) {
    m_playing = false;
    m_accumulatorSeconds = 0.0;
    if (m_world) m_world->Reset();
    m_cache.Clear();
    RebuildFromDocument(doc);
}

// ---------------------------------------------------------------------------
// Simulation pipeline
// ---------------------------------------------------------------------------
void PhysicsStation::RebuildFromDocument(MQDocument doc) {
    if (!doc || !m_world) return;
    m_world->Reset();

    const int objectCount = doc->GetObjectCount();
    for (int i = 0; i < objectCount; ++i) {
        MQObject obj = doc->GetObject(i);
        if (!obj) continue;
        const PhysicsRole role = m_properties.GetRole(obj);
        switch (role) {
        case PhysicsRole::None:
            break;
        case PhysicsRole::Cloth:
            m_world->AddCloth(obj, m_properties.GetClothParams(obj));
            break;
        case PhysicsRole::SoftBody:
            m_world->AddSoftBody(obj, m_properties.GetSoftBodyParams(obj));
            break;
        case PhysicsRole::RigidBody:
            m_world->AddRigidBody(obj, m_properties.GetRigidBodyParams(obj));
            break;
        case PhysicsRole::Collider:
            m_world->AddCollider(obj, m_properties.GetColliderParams(obj));
            break;
        }
    }
}

void PhysicsStation::StepSimulation(MQDocument /*doc*/, float dtSeconds) {
    if (!m_world) return;
    m_world->Step(dtSeconds, kDefaultSubsteps);
}

void PhysicsStation::WriteBackVertices(MQDocument doc) {
    if (!doc || !m_world) return;
    m_world->WriteBack(doc);
}

} // namespace mqphys

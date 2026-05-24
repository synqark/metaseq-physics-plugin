// SPDX-License-Identifier: MIT
//
// src/RigidBody.h

#pragma once

#include "PluginConfig.h"

#include <MQPlugin.h>

namespace mqphys {

struct RigidBodyParams {
    float mass{1.0f};         // 0 => static
    float friction{0.5f};
    float restitution{0.0f};
    float linearDamping{0.05f};
    float angularDamping{0.05f};
};

class RigidBody {
public:
    void Build(MQObject obj, const RigidBodyParams& params);
    void WriteBack(MQDocument doc);
    int  OwnerID() const { return m_ownerID; }
private:
    int m_ownerID{0};
    RigidBodyParams m_params{};
};

} // namespace mqphys

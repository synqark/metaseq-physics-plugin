// SPDX-License-Identifier: MIT
//
// src/Collider.h
//
// Static / kinematic obstacle that other bodies collide against. The
// underlying shape is a triangle mesh built from the MQObject.

#pragma once

#include "PluginConfig.h"

#include <MQPlugin.h>

namespace mqphys {

struct ColliderParams {
    bool  kinematic{false};   // follow MQObject transforms between frames
    float friction{0.5f};
    float restitution{0.0f};
    float collisionMargin{0.005f};
};

class Collider {
public:
    void Build(MQObject obj, const ColliderParams& params);
    int  OwnerID() const { return m_ownerID; }
private:
    int m_ownerID{0};
    ColliderParams m_params{};
};

} // namespace mqphys

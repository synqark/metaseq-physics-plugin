// SPDX-License-Identifier: MIT
//
// src/ClothBody.h
//
// Conversion layer between an MQObject and a Bullet btSoftBody configured
// as a cloth (deformable surface with edge constraints).

#pragma once

#include "PluginConfig.h"

#include <MQPlugin.h>

#include <vector>

#if MQ_HAS_BULLET
class btSoftBody;
class btSoftBodyWorldInfo;
class btSoftRigidDynamicsWorld;
#endif

namespace mqphys {

struct ClothParams {
    float mass{0.5f};           // total mass in kg
    float stiffness{0.9f};      // stretch stiffness [0..1]
    float damping{0.05f};       // velocity damping [0..1]
    float friction{0.5f};       // dynamic friction [0..1]
    int   pinMaterialID{-1};    // pin vertices belonging to this material (-1 = none)
    bool  pinSelected{false};   // pin currently selected vertices
    bool  selfCollision{false}; // self-collision (perf cost)
    float collisionMargin{0.005f};
};

class ClothBody {
public:
    ClothBody() = default;
    ClothBody(ClothBody&&) noexcept = default;
    ClothBody& operator=(ClothBody&&) noexcept = default;

    // Construct the soft body from the MQObject's mesh and add it to the
    // dynamics world. Stores the originating object id so WriteBack can
    // resolve the destination.
    void Build(MQObject obj, const ClothParams& params
#if MQ_HAS_BULLET
        , btSoftBodyWorldInfo* worldInfo
        , btSoftRigidDynamicsWorld* world
#endif
    );

    // Copy simulated vertex positions back into the MQObject.
    void WriteBack(MQDocument doc);

    int OwnerID() const { return m_ownerID; }

private:
    int m_ownerID{0};
    ClothParams m_params{};

#if MQ_HAS_BULLET
    btSoftBody* m_softBody{nullptr};
#endif
    // For mock/no-bullet builds, retain a static-rest copy so WriteBack is
    // still well-defined.
    std::vector<MQPoint> m_restPositions;
};

} // namespace mqphys

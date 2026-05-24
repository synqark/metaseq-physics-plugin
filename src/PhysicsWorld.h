// SPDX-License-Identifier: MIT
//
// src/PhysicsWorld.h
//
// Thin wrapper around a Bullet dynamics world. When Bullet is not linked
// (MQ_HAS_BULLET == 0) the implementation degrades to a no-op so the
// plugin still loads and the UI still works.

#pragma once

#include "PluginConfig.h"
#include "ClothBody.h"
#include "Collider.h"
#include "RigidBody.h"
#include "SoftBody.h"

#include <MQPlugin.h>

#include <memory>
#include <unordered_map>
#include <vector>

#if MQ_HAS_BULLET
class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDefaultCollisionConfiguration;
class btSoftBodyRigidBodyCollisionConfiguration;
class btSoftRigidDynamicsWorld;
class btSoftBodyWorldInfo;
#endif

namespace mqphys {

class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    PhysicsWorld(const PhysicsWorld&) = delete;
    PhysicsWorld& operator=(const PhysicsWorld&) = delete;

    // Tear down all bodies/constraints and re-create an empty world.
    void Reset();

    void SetGravity(float x, float y, float z);

    // Add objects taken from the Metasequoia document.
    void AddCloth(MQObject obj, const ClothParams& p);
    void AddSoftBody(MQObject obj, const SoftBodyParams& p);
    void AddRigidBody(MQObject obj, const RigidBodyParams& p);
    void AddCollider(MQObject obj, const ColliderParams& p);

    // Advance the simulation. `substeps` is forwarded to Bullet's
    // internal sub-stepping for stability.
    void Step(float dtSeconds, int substeps);

    // Copy simulated positions back into the MQObject vertices.
    void WriteBack(MQDocument doc);

private:
    void DestroyWorld();
    void CreateWorld();

#if MQ_HAS_BULLET
    std::unique_ptr<btDefaultCollisionConfiguration>                 m_config;
    std::unique_ptr<btSoftBodyRigidBodyCollisionConfiguration>       m_softConfig;
    std::unique_ptr<btCollisionDispatcher>                           m_dispatcher;
    std::unique_ptr<btBroadphaseInterface>                           m_broadphase;
    std::unique_ptr<btConstraintSolver>                              m_solver;
    std::unique_ptr<btSoftRigidDynamicsWorld>                        m_world;
    std::unique_ptr<btSoftBodyWorldInfo>                             m_softInfo;
#endif

    // Keyed by MQObject unique-id so we can write positions back.
    std::vector<ClothBody>   m_cloths;
    std::vector<SoftBody>    m_softBodies;
    std::vector<RigidBody>   m_rigidBodies;
    std::vector<Collider>    m_colliders;

    float m_gravity[3]{0.f, kDefaultGravityY, 0.f};
};

} // namespace mqphys

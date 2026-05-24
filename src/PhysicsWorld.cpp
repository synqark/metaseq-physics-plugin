// SPDX-License-Identifier: MIT
//
// src/PhysicsWorld.cpp

#include "PhysicsWorld.h"

#if MQ_HAS_BULLET
#  include <btBulletDynamicsCommon.h>
#  include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#  include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#  include <BulletSoftBody/btDefaultSoftBodySolver.h>
#  include <BulletSoftBody/btSoftBody.h>
#  include <BulletSoftBody/btSoftBodyHelpers.h>
#endif

namespace mqphys {

PhysicsWorld::PhysicsWorld() {
    CreateWorld();
}

PhysicsWorld::~PhysicsWorld() {
    DestroyWorld();
}

void PhysicsWorld::Reset() {
    DestroyWorld();
    m_cloths.clear();
    m_softBodies.clear();
    m_rigidBodies.clear();
    m_colliders.clear();
    CreateWorld();
}

void PhysicsWorld::SetGravity(float x, float y, float z) {
    m_gravity[0] = x;
    m_gravity[1] = y;
    m_gravity[2] = z;
#if MQ_HAS_BULLET
    if (m_world) {
        m_world->setGravity(btVector3(x, y, z));
        if (m_softInfo) {
            m_softInfo->m_gravity.setValue(x, y, z);
        }
    }
#endif
}

void PhysicsWorld::CreateWorld() {
#if MQ_HAS_BULLET
    m_softConfig = std::make_unique<btSoftBodyRigidBodyCollisionConfiguration>();
    m_dispatcher = std::make_unique<btCollisionDispatcher>(m_softConfig.get());
    m_broadphase = std::make_unique<btDbvtBroadphase>();
    m_solver     = std::make_unique<btSequentialImpulseConstraintSolver>();
    m_world      = std::make_unique<btSoftRigidDynamicsWorld>(
        m_dispatcher.get(), m_broadphase.get(),
        m_solver.get(),     m_softConfig.get());
    m_world->setGravity(btVector3(m_gravity[0], m_gravity[1], m_gravity[2]));

    m_softInfo = std::make_unique<btSoftBodyWorldInfo>();
    m_softInfo->m_broadphase = m_broadphase.get();
    m_softInfo->m_dispatcher = m_dispatcher.get();
    m_softInfo->m_gravity.setValue(m_gravity[0], m_gravity[1], m_gravity[2]);
    m_softInfo->m_sparsesdf.Initialize();
#endif
}

void PhysicsWorld::DestroyWorld() {
#if MQ_HAS_BULLET
    m_world.reset();
    m_solver.reset();
    m_broadphase.reset();
    m_dispatcher.reset();
    m_softConfig.reset();
    m_config.reset();
    m_softInfo.reset();
#endif
}

void PhysicsWorld::AddCloth(MQObject obj, const ClothParams& p) {
    ClothBody body;
    body.Build(obj, p
#if MQ_HAS_BULLET
        , m_softInfo.get(), m_world.get()
#endif
    );
    m_cloths.push_back(std::move(body));
}

void PhysicsWorld::AddSoftBody(MQObject obj, const SoftBodyParams& p) {
    SoftBody body;
    body.Build(obj, p);
    m_softBodies.push_back(std::move(body));
}

void PhysicsWorld::AddRigidBody(MQObject obj, const RigidBodyParams& p) {
    RigidBody body;
    body.Build(obj, p);
    m_rigidBodies.push_back(std::move(body));
}

void PhysicsWorld::AddCollider(MQObject obj, const ColliderParams& p) {
    Collider c;
    c.Build(obj, p);
    m_colliders.push_back(std::move(c));
}

void PhysicsWorld::Step(float dtSeconds, int substeps) {
#if MQ_HAS_BULLET
    if (m_world) {
        // Bullet expects (timeStep, maxSubSteps, fixedTimeStep).
        m_world->stepSimulation(dtSeconds, substeps, kFixedTimeStep);
    }
#else
    (void)dtSeconds;
    (void)substeps;
#endif
}

void PhysicsWorld::WriteBack(MQDocument doc) {
    if (!doc) return;
    for (auto& c : m_cloths) {
        c.WriteBack(doc);
    }
    for (auto& s : m_softBodies) {
        s.WriteBack(doc);
    }
    for (auto& r : m_rigidBodies) {
        r.WriteBack(doc);
    }
    // Colliders are inputs only; no write-back.
}

} // namespace mqphys

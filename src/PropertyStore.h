// SPDX-License-Identifier: MIT
//
// src/PropertyStore.h
//
// Stores the user-assigned physics role (cloth / soft body / rigid /
// collider) and per-role parameters for each MQObject.
//
// The store is intentionally SDK-light: it keys by the object's stable
// unique id (MQObject::GetUniqueID), so it survives object re-ordering
// inside the document. Phase 4 will additionally serialize this state to
// a sidecar JSON next to the .mqo file.

#pragma once

#include "ClothBody.h"
#include "Collider.h"
#include "RigidBody.h"
#include "SoftBody.h"

#include <MQPlugin.h>

#include <cstdint>
#include <unordered_map>

namespace mqphys {

enum class PhysicsRole : std::uint8_t {
    None = 0,
    Cloth,
    SoftBody,
    RigidBody,
    Collider,
};

class PropertyStore {
public:
    void Clear();

    PhysicsRole GetRole(MQObject obj) const;
    void        SetRole(MQObject obj, PhysicsRole role);

    ClothParams     GetClothParams(MQObject obj) const;
    void            SetClothParams(MQObject obj, const ClothParams& p);

    SoftBodyParams  GetSoftBodyParams(MQObject obj) const;
    void            SetSoftBodyParams(MQObject obj, const SoftBodyParams& p);

    RigidBodyParams GetRigidBodyParams(MQObject obj) const;
    void            SetRigidBodyParams(MQObject obj, const RigidBodyParams& p);

    ColliderParams  GetColliderParams(MQObject obj) const;
    void            SetColliderParams(MQObject obj, const ColliderParams& p);

private:
    using Key = std::int32_t;
    static Key MakeKey(MQObject obj);

    std::unordered_map<Key, PhysicsRole>     m_role;
    std::unordered_map<Key, ClothParams>     m_cloth;
    std::unordered_map<Key, SoftBodyParams>  m_soft;
    std::unordered_map<Key, RigidBodyParams> m_rigid;
    std::unordered_map<Key, ColliderParams>  m_collider;
};

} // namespace mqphys

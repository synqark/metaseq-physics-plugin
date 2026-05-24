// SPDX-License-Identifier: MIT
//
// src/PropertyStore.cpp

#include "PropertyStore.h"

namespace mqphys {

PropertyStore::Key PropertyStore::MakeKey(MQObject obj) {
    return obj ? obj->GetUniqueID() : 0;
}

void PropertyStore::Clear() {
    m_role.clear();
    m_cloth.clear();
    m_soft.clear();
    m_rigid.clear();
    m_collider.clear();
}

PhysicsRole PropertyStore::GetRole(MQObject obj) const {
    auto it = m_role.find(MakeKey(obj));
    return it == m_role.end() ? PhysicsRole::None : it->second;
}

void PropertyStore::SetRole(MQObject obj, PhysicsRole role) {
    const Key k = MakeKey(obj);
    if (role == PhysicsRole::None) {
        m_role.erase(k);
    } else {
        m_role[k] = role;
    }
}

ClothParams PropertyStore::GetClothParams(MQObject obj) const {
    auto it = m_cloth.find(MakeKey(obj));
    return it == m_cloth.end() ? ClothParams{} : it->second;
}
void PropertyStore::SetClothParams(MQObject obj, const ClothParams& p) {
    m_cloth[MakeKey(obj)] = p;
}

SoftBodyParams PropertyStore::GetSoftBodyParams(MQObject obj) const {
    auto it = m_soft.find(MakeKey(obj));
    return it == m_soft.end() ? SoftBodyParams{} : it->second;
}
void PropertyStore::SetSoftBodyParams(MQObject obj, const SoftBodyParams& p) {
    m_soft[MakeKey(obj)] = p;
}

RigidBodyParams PropertyStore::GetRigidBodyParams(MQObject obj) const {
    auto it = m_rigid.find(MakeKey(obj));
    return it == m_rigid.end() ? RigidBodyParams{} : it->second;
}
void PropertyStore::SetRigidBodyParams(MQObject obj, const RigidBodyParams& p) {
    m_rigid[MakeKey(obj)] = p;
}

ColliderParams PropertyStore::GetColliderParams(MQObject obj) const {
    auto it = m_collider.find(MakeKey(obj));
    return it == m_collider.end() ? ColliderParams{} : it->second;
}
void PropertyStore::SetColliderParams(MQObject obj, const ColliderParams& p) {
    m_collider[MakeKey(obj)] = p;
}

} // namespace mqphys

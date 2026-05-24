// SPDX-License-Identifier: MIT
//
// src/RigidBody.cpp — Phase 1 placeholder. Full convex-hull / btRigidBody
// construction follows in Phase 2 alongside SoftBody.

#include "RigidBody.h"

namespace mqphys {

void RigidBody::Build(MQObject obj, const RigidBodyParams& params) {
    m_params  = params;
    m_ownerID = obj ? obj->GetUniqueID() : 0;
}

void RigidBody::WriteBack(MQDocument /*doc*/) {
    // Phase 2: apply Bullet world transform to MQObject local matrix.
}

} // namespace mqphys

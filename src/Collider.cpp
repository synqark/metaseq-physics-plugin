// SPDX-License-Identifier: MIT
//
// src/Collider.cpp

#include "Collider.h"

namespace mqphys {

void Collider::Build(MQObject obj, const ColliderParams& params) {
    m_params  = params;
    m_ownerID = obj ? obj->GetUniqueID() : 0;
    // Phase 2: build btBvhTriangleMeshShape and add as static body.
}

} // namespace mqphys

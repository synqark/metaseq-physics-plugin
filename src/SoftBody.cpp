// SPDX-License-Identifier: MIT
//
// src/SoftBody.cpp — Phase 2 stub: records ownership but does not run a
// simulation yet. The full volumetric soft body path is planned for the
// next phase; keeping the type lets PhysicsWorld treat all role kinds
// uniformly today.

#include "SoftBody.h"

namespace mqphys {

void SoftBody::Build(MQObject obj, const SoftBodyParams& params) {
    m_params = params;
    m_ownerID = obj ? obj->GetUniqueID() : 0;
}

void SoftBody::WriteBack(MQDocument /*doc*/) {
    // Intentionally empty until Phase 2.
}

} // namespace mqphys

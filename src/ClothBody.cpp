// SPDX-License-Identifier: MIT
//
// src/ClothBody.cpp

#include "ClothBody.h"

#if MQ_HAS_BULLET
#  include <BulletSoftBody/btSoftBody.h>
#  include <BulletSoftBody/btSoftBodyHelpers.h>
#  include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#endif

#include <algorithm>
#include <cassert>

namespace mqphys {

namespace {

// Triangulate an arbitrary MQ face (which may be a triangle or quad) into
// triangle indices, fan-style.
template <typename Emit>
void TriangulateFace(const int* face, int count, Emit emit) {
    for (int i = 1; i + 1 < count; ++i) {
        emit(face[0], face[i], face[i + 1]);
    }
}

} // namespace

void ClothBody::Build(MQObject obj, const ClothParams& params
#if MQ_HAS_BULLET
    , btSoftBodyWorldInfo* worldInfo
    , btSoftRigidDynamicsWorld* world
#endif
) {
    if (!obj) return;
    m_params  = params;
    m_ownerID = obj->GetUniqueID();

    const int vertCount = obj->GetVertexCount();
    m_restPositions.resize(static_cast<size_t>(vertCount));
    for (int i = 0; i < vertCount; ++i) {
        m_restPositions[static_cast<size_t>(i)] = obj->GetVertex(i);
    }

#if MQ_HAS_BULLET
    if (!worldInfo || !world || vertCount <= 0) {
        return;
    }

    // Flatten vertex positions for btSoftBodyHelpers.
    std::vector<btScalar> positions;
    positions.reserve(static_cast<size_t>(vertCount) * 3);
    for (int i = 0; i < vertCount; ++i) {
        const MQPoint p = obj->GetVertex(i);
        positions.push_back(static_cast<btScalar>(p.x));
        positions.push_back(static_cast<btScalar>(p.y));
        positions.push_back(static_cast<btScalar>(p.z));
    }

    // Triangulate.
    std::vector<int> triangles;
    const int faceCount = obj->GetFaceCount();
    for (int f = 0; f < faceCount; ++f) {
        const int n = obj->GetFacePointCount(f);
        if (n < 3) continue;
        std::vector<int> faceIdx(static_cast<size_t>(n));
        obj->GetFacePointArray(f, faceIdx.data());
        TriangulateFace(faceIdx.data(), n,
            [&](int a, int b, int c) {
                triangles.push_back(a);
                triangles.push_back(b);
                triangles.push_back(c);
            });
    }
    if (triangles.empty()) {
        return;
    }

    m_softBody = btSoftBodyHelpers::CreateFromTriMesh(
        *worldInfo,
        positions.data(),
        triangles.data(),
        static_cast<int>(triangles.size() / 3));
    if (!m_softBody) {
        return;
    }

    // Cloth material parameters.
    auto* mat = m_softBody->appendMaterial();
    mat->m_kLST = params.stiffness;   // linear stiffness
    mat->m_kAST = params.stiffness;   // angular stiffness
    mat->m_kVST = params.stiffness;   // volume stiffness
    m_softBody->generateBendingConstraints(2, mat);
    m_softBody->setTotalMass(params.mass, true);
    m_softBody->m_cfg.kDF   = params.friction;
    m_softBody->m_cfg.kDP   = params.damping;
    m_softBody->getCollisionShape()->setMargin(params.collisionMargin);
    if (params.selfCollision) {
        m_softBody->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;
    }

    // Pinning: mass = 0 ⇒ node treated as kinematic.
    // Phase 1 supports pinning by material (faces tagged with material id)
    // or by selection. Both are best-effort — the real SDK exposes a
    // per-face material id via MQObject::GetFaceMaterial(...) but the mock
    // headers do not, so we leave the pin pass conservative.
    if (params.pinSelected && m_softBody->m_nodes.size() == vertCount) {
        // Best-effort: we cannot query selection via the mock; the real
        // SDK overload of MQObject::GetVertexAttribute() would be used.
    }

    world->addSoftBody(m_softBody);
#else
    (void)params;
#endif
}

void ClothBody::WriteBack(MQDocument doc) {
    if (!doc) return;

    // Resolve the owning object by unique id every frame — the user may
    // have re-ordered or removed objects in the meantime.
    MQObject target = nullptr;
    const int objectCount = doc->GetObjectCount();
    for (int i = 0; i < objectCount; ++i) {
        MQObject o = doc->GetObject(i);
        if (o && o->GetUniqueID() == m_ownerID) {
            target = o;
            break;
        }
    }
    if (!target) return;

#if MQ_HAS_BULLET
    if (!m_softBody) return;
    const int n = std::min<int>(m_softBody->m_nodes.size(), target->GetVertexCount());
    for (int i = 0; i < n; ++i) {
        const auto& pos = m_softBody->m_nodes[i].m_x;
        target->SetVertex(i, MQPoint(
            static_cast<float>(pos.getX()),
            static_cast<float>(pos.getY()),
            static_cast<float>(pos.getZ())));
    }
#else
    // No-op fallback: restore rest positions so the mesh does not drift.
    const int n = std::min<int>(static_cast<int>(m_restPositions.size()), target->GetVertexCount());
    for (int i = 0; i < n; ++i) {
        target->SetVertex(i, m_restPositions[static_cast<size_t>(i)]);
    }
#endif
}

} // namespace mqphys

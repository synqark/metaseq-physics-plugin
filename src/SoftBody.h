// SPDX-License-Identifier: MIT
//
// src/SoftBody.h — Phase 2 stub.

#pragma once

#include "PluginConfig.h"

#include <MQPlugin.h>

namespace mqphys {

struct SoftBodyParams {
    float mass{1.0f};
    float volumeStiffness{0.5f};
    float shapeMatch{0.5f};
    float damping{0.05f};
    float friction{0.5f};
};

class SoftBody {
public:
    void Build(MQObject obj, const SoftBodyParams& params);
    void WriteBack(MQDocument doc);
    int  OwnerID() const { return m_ownerID; }
private:
    int m_ownerID{0};
    SoftBodyParams m_params{};
};

} // namespace mqphys

// SPDX-License-Identifier: MIT
//
// src/SimulationCache.h
//
// Per-frame vertex cache used for "bake to frames" and for restoring the
// rest pose on Reset. Kept SDK-independent so it can be unit-tested.

#pragma once

#include <MQPlugin.h>

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace mqphys {

class SimulationCache {
public:
    using ObjectID = std::int32_t;

    void Clear();

    // Record the rest positions of an object so they can be restored later.
    void StoreRest(ObjectID id, const std::vector<MQPoint>& positions);
    bool HasRest(ObjectID id) const;
    const std::vector<MQPoint>* GetRest(ObjectID id) const;

    // Append a frame's worth of positions for a given object.
    void StoreFrame(ObjectID id, int frame, std::vector<MQPoint> positions);
    const std::vector<MQPoint>* GetFrame(ObjectID id, int frame) const;

    int  FrameCount(ObjectID id) const;

private:
    std::unordered_map<ObjectID, std::vector<MQPoint>> m_rest;
    std::unordered_map<ObjectID, std::unordered_map<int, std::vector<MQPoint>>> m_frames;
};

} // namespace mqphys

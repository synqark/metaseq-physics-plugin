// SPDX-License-Identifier: MIT
//
// src/SimulationCache.cpp

#include "SimulationCache.h"

#include <utility>

namespace mqphys {

void SimulationCache::Clear() {
    m_rest.clear();
    m_frames.clear();
}

void SimulationCache::StoreRest(ObjectID id, const std::vector<MQPoint>& positions) {
    m_rest[id] = positions;
}

bool SimulationCache::HasRest(ObjectID id) const {
    return m_rest.find(id) != m_rest.end();
}

const std::vector<MQPoint>* SimulationCache::GetRest(ObjectID id) const {
    auto it = m_rest.find(id);
    return it == m_rest.end() ? nullptr : &it->second;
}

void SimulationCache::StoreFrame(ObjectID id, int frame, std::vector<MQPoint> positions) {
    m_frames[id][frame] = std::move(positions);
}

const std::vector<MQPoint>* SimulationCache::GetFrame(ObjectID id, int frame) const {
    auto it = m_frames.find(id);
    if (it == m_frames.end()) return nullptr;
    auto fit = it->second.find(frame);
    return fit == it->second.end() ? nullptr : &fit->second;
}

int SimulationCache::FrameCount(ObjectID id) const {
    auto it = m_frames.find(id);
    return it == m_frames.end() ? 0 : static_cast<int>(it->second.size());
}

} // namespace mqphys

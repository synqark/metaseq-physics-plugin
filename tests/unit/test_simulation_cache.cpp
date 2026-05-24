// SPDX-License-Identifier: MIT

#include "test_main.h"

#include "SimulationCache.h"

MQPHYS_TEST(SimulationCache_StartsEmpty) {
    mqphys::SimulationCache cache;
    MQPHYS_EXPECT(!cache.HasRest(1));
    MQPHYS_EXPECT(cache.GetRest(1) == nullptr);
    MQPHYS_EXPECT(cache.FrameCount(1) == 0);
}

MQPHYS_TEST(SimulationCache_StoresRestPositions) {
    mqphys::SimulationCache cache;
    std::vector<MQPoint> rest = { {0,0,0}, {1,0,0}, {0,1,0} };
    cache.StoreRest(42, rest);

    MQPHYS_EXPECT(cache.HasRest(42));
    auto* got = cache.GetRest(42);
    MQPHYS_EXPECT(got != nullptr);
    MQPHYS_EXPECT(got->size() == 3);
    MQPHYS_EXPECT((*got)[1].x == 1.0f);
}

MQPHYS_TEST(SimulationCache_StoresFrames) {
    mqphys::SimulationCache cache;
    cache.StoreFrame(7, 0, { {0,0,0} });
    cache.StoreFrame(7, 1, { {0,0,1} });
    cache.StoreFrame(7, 2, { {0,0,2} });
    MQPHYS_EXPECT(cache.FrameCount(7) == 3);
    auto* f = cache.GetFrame(7, 2);
    MQPHYS_EXPECT(f != nullptr);
    MQPHYS_EXPECT((*f)[0].z == 2.0f);
    MQPHYS_EXPECT(cache.GetFrame(7, 9) == nullptr);
}

MQPHYS_TEST(SimulationCache_ClearDropsAll) {
    mqphys::SimulationCache cache;
    cache.StoreRest(1, { {0,0,0} });
    cache.StoreFrame(1, 0, { {0,0,0} });
    cache.Clear();
    MQPHYS_EXPECT(!cache.HasRest(1));
    MQPHYS_EXPECT(cache.FrameCount(1) == 0);
}

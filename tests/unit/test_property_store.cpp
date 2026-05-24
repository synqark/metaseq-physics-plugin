// SPDX-License-Identifier: MIT

#include "test_main.h"

#include "PropertyStore.h"

#include <MQPlugin.h>

namespace {
class FakeObject : public MQCObject {
public:
    explicit FakeObject(int id) : m_id(id) {}
    int GetUniqueID() const override { return m_id; }
private:
    int m_id;
};
} // namespace

MQPHYS_TEST(PropertyStore_DefaultsToNoneRole) {
    mqphys::PropertyStore store;
    FakeObject a(42);
    MQPHYS_EXPECT(store.GetRole(&a) == mqphys::PhysicsRole::None);
}

MQPHYS_TEST(PropertyStore_RoundtripsRole) {
    mqphys::PropertyStore store;
    FakeObject a(1), b(2);
    store.SetRole(&a, mqphys::PhysicsRole::Cloth);
    store.SetRole(&b, mqphys::PhysicsRole::Collider);
    MQPHYS_EXPECT(store.GetRole(&a) == mqphys::PhysicsRole::Cloth);
    MQPHYS_EXPECT(store.GetRole(&b) == mqphys::PhysicsRole::Collider);
}

MQPHYS_TEST(PropertyStore_SettingNoneRemoves) {
    mqphys::PropertyStore store;
    FakeObject a(1);
    store.SetRole(&a, mqphys::PhysicsRole::Cloth);
    store.SetRole(&a, mqphys::PhysicsRole::None);
    MQPHYS_EXPECT(store.GetRole(&a) == mqphys::PhysicsRole::None);
}

MQPHYS_TEST(PropertyStore_RoundtripsClothParams) {
    mqphys::PropertyStore store;
    FakeObject a(7);
    mqphys::ClothParams p;
    p.mass = 1.25f;
    p.stiffness = 0.4f;
    p.pinMaterialID = 3;
    store.SetClothParams(&a, p);

    const auto got = store.GetClothParams(&a);
    MQPHYS_EXPECT(got.mass == 1.25f);
    MQPHYS_EXPECT(got.stiffness == 0.4f);
    MQPHYS_EXPECT(got.pinMaterialID == 3);
}

MQPHYS_TEST(PropertyStore_ClearRemovesEverything) {
    mqphys::PropertyStore store;
    FakeObject a(1);
    store.SetRole(&a, mqphys::PhysicsRole::Cloth);
    store.SetClothParams(&a, mqphys::ClothParams{});
    store.Clear();
    MQPHYS_EXPECT(store.GetRole(&a) == mqphys::PhysicsRole::None);
}

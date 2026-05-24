// SPDX-License-Identifier: MIT
//
// tests/unit/test_main.h — minimal home-grown test harness so we do not
// pull in another dependency just for a handful of unit tests.

#pragma once

#include <cstdio>
#include <cstdlib>
#include <functional>
#include <string>
#include <vector>

namespace mqphys_test {

struct TestCase {
    std::string name;
    std::function<void()> fn;
};

inline std::vector<TestCase>& AllTests() {
    static std::vector<TestCase> v;
    return v;
}

struct Registrar {
    Registrar(const char* name, std::function<void()> fn) {
        AllTests().push_back({name, std::move(fn)});
    }
};

#define MQPHYS_TEST(name)                                                    \
    static void name();                                                      \
    static ::mqphys_test::Registrar reg_##name(#name, &name);                \
    static void name()

#define MQPHYS_EXPECT(cond)                                                  \
    do {                                                                     \
        if (!(cond)) {                                                       \
            std::fprintf(stderr, "FAIL: %s:%d: %s\n",                        \
                __FILE__, __LINE__, #cond);                                  \
            std::exit(1);                                                    \
        }                                                                    \
    } while (0)

} // namespace mqphys_test

// SPDX-License-Identifier: MIT

#include "test_main.h"

int main() {
    for (auto& t : mqphys_test::AllTests()) {
        std::printf("[ RUN  ] %s\n", t.name.c_str());
        t.fn();
        std::printf("[  OK  ] %s\n", t.name.c_str());
    }
    std::printf("All %zu tests passed.\n", mqphys_test::AllTests().size());
    return 0;
}

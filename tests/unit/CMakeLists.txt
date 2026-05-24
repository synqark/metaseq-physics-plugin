# Unit tests cover the SDK-independent logic only (PropertyStore,
# SimulationCache, …). The Metasequoia SDK and Bullet are intentionally
# not required to build or run these tests.

add_executable(mqphys_unit_tests
    test_property_store.cpp
    test_simulation_cache.cpp
    test_main.cpp
)

target_include_directories(mqphys_unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests/mock_sdk
)

target_sources(mqphys_unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/PropertyStore.cpp
    ${CMAKE_SOURCE_DIR}/src/SimulationCache.cpp
)

target_compile_definitions(mqphys_unit_tests PRIVATE MQ_HAS_BULLET=0)

if(MSVC)
    target_compile_options(mqphys_unit_tests PRIVATE /W4 /permissive- /utf-8 /EHsc)
else()
    target_compile_options(mqphys_unit_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_test(NAME mqphys_unit_tests COMMAND mqphys_unit_tests)

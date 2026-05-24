cmake_minimum_required(VERSION 3.21)
project(MetaseqPhysics
    VERSION 0.1.0
    DESCRIPTION "Physics simulation plugin for Metasequoia 4"
    LANGUAGES CXX
)

# ---------------------------------------------------------------------------
# Project-wide settings
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(MQ_USE_MOCK_SDK
    "Use stubbed Metasequoia SDK headers from tests/mock_sdk/ for CI compile checks."
    OFF)
option(MQ_BUILD_TESTS
    "Build unit tests for the physics plugin (logic only, no SDK/Bullet)."
    OFF)
option(MQ_USE_BULLET
    "Link against Bullet Physics from extern/bullet3 (required for full runtime)."
    ON)

# ---------------------------------------------------------------------------
# Metasequoia SDK detection
# ---------------------------------------------------------------------------
if(MQ_USE_MOCK_SDK)
    message(STATUS "Metasequoia: using MOCK SDK headers (tests/mock_sdk).")
    set(MQSDK_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests/mock_sdk")
    set(MQSDK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/mock_sdk/mqsdk_mock.cpp")
    set(MQSDK_FOUND TRUE)
else()
    find_package(MQSDK QUIET)
endif()

# ---------------------------------------------------------------------------
# Bullet Physics
# ---------------------------------------------------------------------------
set(BULLET_TARGETS "")
if(MQ_USE_BULLET)
    set(_bullet_root "${CMAKE_CURRENT_SOURCE_DIR}/extern/bullet3")
    if(EXISTS "${_bullet_root}/CMakeLists.txt")
        # Bullet's own CMake spawns a lot of subprojects we do not need.
        set(BUILD_BULLET2_DEMOS    OFF CACHE BOOL "" FORCE)
        set(BUILD_BULLET3          OFF CACHE BOOL "" FORCE)
        set(BUILD_CPU_DEMOS        OFF CACHE BOOL "" FORCE)
        set(BUILD_EXTRAS           OFF CACHE BOOL "" FORCE)
        set(BUILD_OPENGL3_DEMOS    OFF CACHE BOOL "" FORCE)
        set(BUILD_UNIT_TESTS       OFF CACHE BOOL "" FORCE)
        set(BUILD_PYBULLET         OFF CACHE BOOL "" FORCE)
        set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "" FORCE)
        set(INSTALL_LIBS           OFF CACHE BOOL "" FORCE)
        set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE BOOL "" FORCE)
        add_subdirectory(extern/bullet3 EXCLUDE_FROM_ALL)
        set(BULLET_TARGETS BulletSoftBody BulletDynamics BulletCollision LinearMath)
        set(BULLET_INCLUDE_DIR "${_bullet_root}/src")
        set(BULLET_FOUND TRUE)
    else()
        message(WARNING
            "Bullet Physics not found at extern/bullet3. Run:\n"
            "    git submodule update --init --recursive\n"
            "Building without Bullet — physics will be a no-op stub.")
        set(BULLET_FOUND FALSE)
    endif()
else()
    set(BULLET_FOUND FALSE)
endif()

# ---------------------------------------------------------------------------
# Plugin DLL
# ---------------------------------------------------------------------------
set(PLUGIN_SOURCES
    src/PluginMain.cpp
    src/PhysicsStation.cpp
    src/PhysicsWorld.cpp
    src/ClothBody.cpp
    src/SoftBody.cpp
    src/RigidBody.cpp
    src/Collider.cpp
    src/SimulationCache.cpp
    src/PropertyStore.cpp
    src/ui/PhysicsPanel.cpp
    src/ui/ResourceStrings.cpp
)

set(PLUGIN_HEADERS
    src/PhysicsStation.h
    src/PhysicsWorld.h
    src/ClothBody.h
    src/SoftBody.h
    src/RigidBody.h
    src/Collider.h
    src/SimulationCache.h
    src/PropertyStore.h
    src/PluginConfig.h
    src/ui/PhysicsPanel.h
    src/ui/ResourceStrings.h
)

if(WIN32)
    list(APPEND PLUGIN_SOURCES res/plugin.rc)
endif()

if(MQSDK_FOUND)
    add_library(MetaseqPhysics SHARED
        ${PLUGIN_SOURCES}
        ${PLUGIN_HEADERS}
        ${MQSDK_SOURCES}
    )

    target_include_directories(MetaseqPhysics
        PRIVATE
            src
            ${MQSDK_INCLUDE_DIR}
    )

    if(BULLET_FOUND)
        target_include_directories(MetaseqPhysics PRIVATE ${BULLET_INCLUDE_DIR})
        target_link_libraries(MetaseqPhysics PRIVATE ${BULLET_TARGETS})
        target_compile_definitions(MetaseqPhysics PRIVATE MQ_HAS_BULLET=1)
    else()
        target_compile_definitions(MetaseqPhysics PRIVATE MQ_HAS_BULLET=0)
    endif()

    if(MQ_USE_MOCK_SDK)
        target_compile_definitions(MetaseqPhysics PRIVATE MQ_USING_MOCK_SDK=1)
    endif()

    if(MSVC)
        target_compile_definitions(MetaseqPhysics PRIVATE
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX
            WIN32_LEAN_AND_MEAN
            UNICODE
            _UNICODE
        )
        target_compile_options(MetaseqPhysics PRIVATE /W4 /permissive- /utf-8 /EHsc)
    else()
        target_compile_options(MetaseqPhysics PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    set_target_properties(MetaseqPhysics PROPERTIES
        OUTPUT_NAME "MetaseqPhysics"
        PREFIX ""
        SUFFIX ".dll"
    )
else()
    message(WARNING
        "Metasequoia SDK not found. Place the SDK headers under sdk/mqsdk/ "
        "or configure with -DMQ_USE_MOCK_SDK=ON to perform a syntax-only build.")
endif()

# ---------------------------------------------------------------------------
# Tests (logic-only, no SDK / Bullet required)
# ---------------------------------------------------------------------------
if(MQ_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/unit)
endif()

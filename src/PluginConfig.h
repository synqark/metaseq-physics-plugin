// SPDX-License-Identifier: MIT
//
// src/PluginConfig.h
//
// Compile-time configuration / feature flags for the plugin.

#pragma once

#ifndef MQ_HAS_BULLET
#  define MQ_HAS_BULLET 0
#endif

#ifndef MQ_USING_MOCK_SDK
#  define MQ_USING_MOCK_SDK 0
#endif

namespace mqphys {

// Plugin identity — change these only when bumping a published release.
//
// Per Metasequoia SDK convention, the product/ID pair is a 32+32 bit GUID
// that uniquely identifies your plugin. Treat both halves as opaque.
constexpr unsigned int kPluginProductID = 0x4D515048u; // 'MQPH'
constexpr unsigned int kPluginSubID     = 0x59530001u; // 'YS' + version slot

constexpr const char* kPluginName    = "Metaseq Physics";
constexpr const char* kPluginVersion = "0.1.0";
constexpr const char* kPluginVendor  = "synqark";

// Simulation defaults.
constexpr float kDefaultGravityY = -9.81f;
constexpr int   kDefaultSubsteps = 4;
constexpr float kFixedTimeStep   = 1.0f / 60.0f;

} // namespace mqphys

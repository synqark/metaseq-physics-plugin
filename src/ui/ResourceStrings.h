// SPDX-License-Identifier: MIT
//
// src/ui/ResourceStrings.h
//
// Localized UI strings. Simple compile-time selection between Japanese
// and English; future work can switch to a runtime locale lookup that
// reads from the OS / Metasequoia language setting.

#pragma once

#include <string>

namespace mqphys {

#ifndef MQ_LANG_EN
#  define MQ_LANG_EN 0   // 1 = English, 0 = Japanese (default)
#endif

struct R {
    static std::string Title();
    static std::string RoleGroup();
    static std::string RoleNone();
    static std::string RoleCloth();
    static std::string RoleSoftBody();
    static std::string RoleRigidBody();
    static std::string RoleCollider();
    static std::string ParamGroup();
    static std::string Mass();
    static std::string Stiffness();
    static std::string Damping();
    static std::string SelfCollision();
    static std::string Play();
    static std::string Pause();
    static std::string Reset();
};

} // namespace mqphys

// SPDX-License-Identifier: MIT
//
// src/ui/ResourceStrings.cpp

#include "ResourceStrings.h"

namespace mqphys {

#if MQ_LANG_EN
#  define MQ_STR(jp, en) en
#else
#  define MQ_STR(jp, en) jp
#endif

std::string R::Title()         { return MQ_STR("Metaseq Physics",      "Metaseq Physics"); }
std::string R::RoleGroup()     { return MQ_STR("ロール",                "Role"); }
std::string R::RoleNone()      { return MQ_STR("なし",                  "None"); }
std::string R::RoleCloth()     { return MQ_STR("布",                    "Cloth"); }
std::string R::RoleSoftBody()  { return MQ_STR("ソフトボディ",          "Soft body"); }
std::string R::RoleRigidBody() { return MQ_STR("剛体",                  "Rigid body"); }
std::string R::RoleCollider()  { return MQ_STR("衝突体",                "Collider"); }
std::string R::ParamGroup()    { return MQ_STR("パラメータ",            "Parameters"); }
std::string R::Mass()          { return MQ_STR("質量",                  "Mass"); }
std::string R::Stiffness()     { return MQ_STR("剛性",                  "Stiffness"); }
std::string R::Damping()       { return MQ_STR("減衰",                  "Damping"); }
std::string R::SelfCollision() { return MQ_STR("自己衝突",              "Self collision"); }
std::string R::Play()          { return MQ_STR("再生",                  "Play"); }
std::string R::Pause()         { return MQ_STR("一時停止",              "Pause"); }
std::string R::Reset()         { return MQ_STR("リセット",              "Reset"); }

} // namespace mqphys

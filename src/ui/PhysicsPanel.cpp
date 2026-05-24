// SPDX-License-Identifier: MIT
//
// src/ui/PhysicsPanel.cpp

#include "PhysicsPanel.h"
#include "ResourceStrings.h"
#include "../PhysicsStation.h"

namespace mqphys {

PhysicsPanel::PhysicsPanel(PhysicsStation& owner)
    : m_owner(owner) {
    BuildUi();
}

PhysicsPanel::~PhysicsPanel() = default;

void PhysicsPanel::BuildUi() {
    auto* parent = MQWindow::GetMainWindow();
    m_window = std::make_unique<MQDockWindow>(parent);
    m_window->SetTitle(R::Title());

    m_roleGroup = std::make_unique<MQGroupBox>(m_window.get(), R::RoleGroup());
    m_roleCombo = std::make_unique<MQComboBox>(m_roleGroup.get());
    m_roleCombo->AddItem(R::RoleNone());
    m_roleCombo->AddItem(R::RoleCloth());
    m_roleCombo->AddItem(R::RoleSoftBody());
    m_roleCombo->AddItem(R::RoleRigidBody());
    m_roleCombo->AddItem(R::RoleCollider());

    m_paramGroup    = std::make_unique<MQGroupBox>(m_window.get(), R::ParamGroup());
    m_mass          = std::make_unique<MQDoubleSpinBox>(m_paramGroup.get());
    m_mass->SetRange(0.0, 1000.0);
    m_mass->SetValue(0.5);
    m_stiffness     = std::make_unique<MQDoubleSpinBox>(m_paramGroup.get());
    m_stiffness->SetRange(0.0, 1.0);
    m_stiffness->SetValue(0.9);
    m_damping       = std::make_unique<MQDoubleSpinBox>(m_paramGroup.get());
    m_damping->SetRange(0.0, 1.0);
    m_damping->SetValue(0.05);
    m_selfCollision = std::make_unique<MQCheckBox>(m_paramGroup.get(), R::SelfCollision());

    m_play  = std::make_unique<MQButton>(m_window.get(), R::Play());
    m_pause = std::make_unique<MQButton>(m_window.get(), R::Pause());
    m_reset = std::make_unique<MQButton>(m_window.get(), R::Reset());

    m_play->onClick  = [this]() { OnPlay(); };
    m_pause->onClick = [this]() { OnPause(); };
    m_reset->onClick = [this]() { OnReset(); };
}

void PhysicsPanel::SetVisible(bool v) {
    if (m_window) m_window->SetVisible(v);
}

void PhysicsPanel::OnPlay()  { m_owner.Play(); }
void PhysicsPanel::OnPause() { m_owner.Pause(); }
void PhysicsPanel::OnReset() {
    // The current document pointer is not directly accessible from the
    // panel; the station already has it via its hooks. We pass nullptr
    // and the station will short-circuit when no document is loaded.
    m_owner.Reset(nullptr);
}

void PhysicsPanel::OnRoleChanged() {
    // Phase 2: write the new role for the selected object back to
    // PropertyStore using the panel widgets.
}

} // namespace mqphys

// SPDX-License-Identifier: MIT
//
// src/ui/PhysicsPanel.h
//
// Docked control window for the plugin. The widget hierarchy is built on
// the Metasequoia MQWidget API. The panel calls back into PhysicsStation
// to start / stop the simulation and to mutate the property store.

#pragma once

#include <MQWidget.h>

#include <memory>

namespace mqphys {

class PhysicsStation;

class PhysicsPanel {
public:
    explicit PhysicsPanel(PhysicsStation& owner);
    ~PhysicsPanel();

    PhysicsPanel(const PhysicsPanel&) = delete;
    PhysicsPanel& operator=(const PhysicsPanel&) = delete;

    void SetVisible(bool v);

private:
    void BuildUi();
    void OnPlay();
    void OnPause();
    void OnReset();
    void OnRoleChanged();

    PhysicsStation& m_owner;

    std::unique_ptr<MQDockWindow>    m_window;
    std::unique_ptr<MQGroupBox>      m_roleGroup;
    std::unique_ptr<MQComboBox>      m_roleCombo;
    std::unique_ptr<MQGroupBox>      m_paramGroup;
    std::unique_ptr<MQDoubleSpinBox> m_mass;
    std::unique_ptr<MQDoubleSpinBox> m_stiffness;
    std::unique_ptr<MQDoubleSpinBox> m_damping;
    std::unique_ptr<MQCheckBox>      m_selfCollision;
    std::unique_ptr<MQButton>        m_play;
    std::unique_ptr<MQButton>        m_pause;
    std::unique_ptr<MQButton>        m_reset;
};

} // namespace mqphys

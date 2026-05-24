// SPDX-License-Identifier: MIT
//
// tests/mock_sdk/MQWidget.h
//
// **MOCK** header — see MQPlugin.h for context. Only members that our UI
// code touches are declared; everything is a no-op.

#pragma once

#include "MQPlugin.h"
#include <functional>
#include <string>

class MQWidgetBase {
public:
    virtual ~MQWidgetBase() = default;
    void SetVisible(bool /*v*/) {}
    bool GetVisible() const { return false; }
};

class MQWindow : public MQWidgetBase {
public:
    static MQWindow* GetMainWindow() { return nullptr; }
};

class MQDockWindow : public MQWindow {
public:
    explicit MQDockWindow(MQWindow* /*parent*/) {}
    void SetTitle(const std::string& /*t*/) {}
};

class MQFrame : public MQWidgetBase {
public:
    explicit MQFrame(MQWidgetBase* /*parent*/) {}
};

class MQGroupBox : public MQWidgetBase {
public:
    MQGroupBox(MQWidgetBase* /*parent*/, const std::string& /*title*/) {}
};

class MQLabel : public MQWidgetBase {
public:
    MQLabel(MQWidgetBase* /*parent*/, const std::string& /*text*/) {}
    void SetText(const std::string& /*t*/) {}
};

class MQButton : public MQWidgetBase {
public:
    MQButton(MQWidgetBase* /*parent*/, const std::string& /*text*/) {}
    std::function<void()> onClick;
};

class MQComboBox : public MQWidgetBase {
public:
    explicit MQComboBox(MQWidgetBase* /*parent*/) {}
    void AddItem(const std::string& /*text*/) {}
    int  GetCurrentIndex() const { return 0; }
    void SetCurrentIndex(int /*i*/) {}
};

class MQDoubleSpinBox : public MQWidgetBase {
public:
    explicit MQDoubleSpinBox(MQWidgetBase* /*parent*/) {}
    void   SetRange(double /*lo*/, double /*hi*/) {}
    void   SetValue(double /*v*/) {}
    double GetValue() const { return 0.0; }
};

class MQCheckBox : public MQWidgetBase {
public:
    MQCheckBox(MQWidgetBase* /*parent*/, const std::string& /*text*/) {}
    void SetChecked(bool /*v*/) {}
    bool GetChecked() const { return false; }
};

// SPDX-License-Identifier: MIT
//
// tests/mock_sdk/MQPlugin.h
//
// **MOCK** Metasequoia SDK header used ONLY for compile-only CI builds.
// This is NOT the real SDK and the resulting DLL will NOT load in
// Metasequoia. Real builds must point at the official SDK from
// https://www.metaseq.net/jp/download/sdk/ .
//
// The shapes below model just enough of the public SDK surface to let our
// plugin source compile. They intentionally lack any implementation.

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#if defined(_WIN32)
#  ifndef MQPLUGIN_EXPORT
#    define MQPLUGIN_EXPORT __declspec(dllexport)
#  endif
#else
#  ifndef MQPLUGIN_EXPORT
#    define MQPLUGIN_EXPORT __attribute__((visibility("default")))
#  endif
#endif

using MQFileHandle = void*;

struct MQPoint {
    float x{0.f}, y{0.f}, z{0.f};
    MQPoint() = default;
    MQPoint(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    MQPoint operator+(const MQPoint& o) const { return {x + o.x, y + o.y, z + o.z}; }
    MQPoint operator-(const MQPoint& o) const { return {x - o.x, y - o.y, z - o.z}; }
    MQPoint operator*(float s) const { return {x * s, y * s, z * s}; }
};

struct MQColor {
    float r{1.f}, g{1.f}, b{1.f};
};

struct MQCoordinate {
    float u{0.f}, v{0.f};
};

// --- Plugin identification GUID ---------------------------------------------
struct MQPLUGIN_ID {
    std::uint32_t Product;
    std::uint32_t ID;
};

// --- MQObject ---------------------------------------------------------------
class MQCObject {
public:
    virtual ~MQCObject() = default;
    virtual int GetVertexCount() const { return 0; }
    virtual MQPoint GetVertex(int /*idx*/) const { return {}; }
    virtual void SetVertex(int /*idx*/, MQPoint /*p*/) {}
    virtual int GetFaceCount() const { return 0; }
    virtual int GetFacePointCount(int /*face*/) const { return 0; }
    virtual void GetFacePointArray(int /*face*/, int* /*out*/) const {}
    virtual const char* GetName() const { return ""; }
    virtual int GetUniqueID() const { return 0; }
    virtual bool GetVisible() const { return true; }
};
using MQObject = MQCObject*;

// --- MQMaterial -------------------------------------------------------------
class MQCMaterial {
public:
    virtual ~MQCMaterial() = default;
    virtual const char* GetName() const { return ""; }
    virtual int GetUniqueID() const { return 0; }
};
using MQMaterial = MQCMaterial*;

// --- MQDocument -------------------------------------------------------------
class MQCDocument {
public:
    virtual ~MQCDocument() = default;
    virtual int GetObjectCount() const { return 0; }
    virtual MQObject GetObject(int /*idx*/) const { return nullptr; }
    virtual int GetMaterialCount() const { return 0; }
    virtual MQMaterial GetMaterial(int /*idx*/) const { return nullptr; }
    virtual int GetCurrentObjectIndex() const { return -1; }
};
using MQDocument = MQCDocument*;

// --- MQScene ---------------------------------------------------------------
class MQCScene {
public:
    virtual ~MQCScene() = default;
};
using MQScene = MQCScene*;

// --- MQXmlElement / MQSetting (forward stubs) ------------------------------
class MQXmlElement {};
class MQSetting {
public:
    void Load(const char* /*key*/, int& v, int def = 0) { v = def; }
    void Load(const char* /*key*/, float& v, float def = 0.f) { v = def; }
    void Load(const char* /*key*/, bool& v, bool def = false) { v = def; }
    void Save(const char* /*key*/, int /*v*/) {}
    void Save(const char* /*key*/, float /*v*/) {}
    void Save(const char* /*key*/, bool /*v*/) {}
};

// --- MQ_PLUGIN_TYPE enum (subset) ------------------------------------------
enum MQ_PLUGIN_TYPE {
    MQPLUGIN_TYPE_OBJECT  = 1,
    MQPLUGIN_TYPE_SELECT  = 2,
    MQPLUGIN_TYPE_CREATE  = 3,
    MQPLUGIN_TYPE_IMPORT  = 4,
    MQPLUGIN_TYPE_EXPORT  = 5,
    MQPLUGIN_TYPE_COMMAND = 6,
    MQPLUGIN_TYPE_STATION = 7,
};

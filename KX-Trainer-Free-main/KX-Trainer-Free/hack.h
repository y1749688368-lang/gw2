#pragma once

#include "process_memory_manager.h"
#include <Windows.h>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>
#include <tuple>

class HackInitializationError : public std::runtime_error {
public:
    HackInitializationError(const std::string& message)
        : std::runtime_error(message) {}
};

class Hack {
public:
    Hack(std::function<void(const std::string&)> statusCallback);
    ~Hack();
    bool Initialize(); // Performs process attachment and initial scans. Returns true on success.

    void refreshAddresses();

    // Feature toggles/handlers
    void toggleFog(bool enable);
    void toggleObjectClipping(bool enable);
    void toggleFullStrafe(bool enable);
    void handleSprint(bool userPrefersSprint); // Apply sprint based on user preference
    void handleSuperSprint(bool enable);
    void toggleInvisibility(bool enable);
    void toggleWallClimb(bool enable);
    void toggleClipping(bool enable);
    void handleFly(bool enable);

    // Position saving/loading
    void savePosition1();
    void loadPosition1();
    void savePosition2();
    void loadPosition2();
    void savePosition3();
    void loadPosition3();
    void Maploadposition();
    void Mapcreateposition();
    //mapload
    bool m_showJsonViewer = false; 
    std::vector<std::tuple<std::string, float, float, float>> m_jsonPoints;
    void loadPoint(float x, float y, float z) {
        writeXYZmap(x, y, z);
    }


    // --- State Getters ---
    bool IsFogEnabled() const;
    bool IsObjectClippingEnabled() const;
    bool IsFullStrafeEnabled() const;
    bool IsSuperSprinting() const; // Reflects if key was held/active
    bool IsInvisibilityEnabled() const;
    bool IsWallClimbEnabled() const;
    bool IsClippingEnabled() const;
    bool IsFlying() const;         // Reflects if key was held/active
    // --- End State Getters ---

private:



    ProcessMemoryManager m_memoryManager;
    std::function<void(const std::string&)> m_statusCallback;

    uintptr_t m_baseAddressLocation = 0;
    uintptr_t m_fogAddress = 0;
    uintptr_t m_objectClippingAddress = 0;
    uintptr_t m_fullStrafeAddress = 0;

    // Pointer chain offsets
    std::vector<unsigned int> m_xOffsets;
    std::vector<unsigned int> m_yOffsets;
    std::vector<unsigned int> m_zOffsets;
    std::vector<unsigned int> m_zHeight1Offsets; // Invisibility
    std::vector<unsigned int> m_zHeight2Offsets; // Clipping
    std::vector<unsigned int> m_gravityOffsets;    // Fly
    std::vector<unsigned int> m_speedOffsets;
    std::vector<unsigned int> m_wallClimbOffsets;

    // Resolved dynamic addresses
    uintptr_t m_xAddr = 0;
    uintptr_t m_yAddr = 0;
    uintptr_t m_zAddr = 0;
    uintptr_t m_zHeight1Addr = 0; // Invisibility
    uintptr_t m_zHeight2Addr = 0; // Clipping
    uintptr_t m_gravityAddr = 0;    // Fly
    uintptr_t m_speedAddr = 0;
    uintptr_t m_wallClimbAddr = 0;

    // Core memory values / state
    float m_xValue = 0.0f, m_yValue = 0.0f, m_zValue = 0.0f;
    float m_xSave = 0.0f, m_ySave = 0.0f, m_zSave = 0.0f;
    float m_xValue1 = 0.0f, m_yValue1 = 0.0f, m_zValue1 = 0.0f;
    float m_xSave1 = 0.0f, m_ySave1 = 0.0f, m_zSave1 = 0.0f;
    float m_xValue2 = 0.0f, m_yValue2 = 0.0f, m_zValue2 = 0.0f;
    float m_xSave2 = 0.0f, m_ySave2 = 0.0f, m_zSave2 = 0.0f;
    float m_xValue3 = 0.0f, m_yValue3 = 0.0f, m_zValue3 = 0.0f;

    float m_xValuemap = 0.0f, m_yValuemap = 0.0f, m_zValuemap = 0.0f;
    float m_xSave3 = 0.0f, m_ySave3 = 0.0f, m_zSave3 = 0.0f;
    float m_speed = 0.0f, m_savedSpeed = 0.0f;
    float m_invisibilityValue = 0.0f;
    float m_wallClimbValue = 0.0f;
    float m_clippingValue = 0.0f;
    float m_flyValue = 0.0f;
    byte m_objectClippingByte = 0; // Cached byte written
    byte m_fogByte = 0;            // Cached byte written
    byte m_fullStrafeByte = 0;     // Cached byte written

    // Simple feature active flags (Single Source of Truth)
    bool m_isInvisibilityActive = false;
    bool m_isClippingActive = false;
    bool m_isWallClimbActive = false;
    bool m_isFlyingActive = false;
    bool m_wasSuperSprinting = false; // Tracks hold state from last frame
    bool m_wasSprinting = false;      // Tracks if normal sprint *logic* was applied last frame

    // Helpers
    void initializeOffsets();
    void findProcess();
    void performBaseScan();
    void scanForPatterns();
    void readXYZ1();
    void writeXYZ1(float xValue, float yValue, float zValue);
    void readXYZ2();
    void writeXYZ2(float xValue, float yValue, float zValue);
    void readXYZ3();
    void writeXYZ3(float xValue, float yValue, float zValue);
    void readXYZmap();
    void writeXYZmap(float xValue, float yValue, float zValue);
    uintptr_t refreshAddr(const std::vector<unsigned int>& offsets);
    void reportStatus(const std::string& message);
};
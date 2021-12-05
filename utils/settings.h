/**
 * @file    Settings.h
 *
 * This file contains various settings and enumerations that you will need to use in the various
 * assignments. The settings are bound to the GUI via static data bindings.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

/**
 * @struct Settings
 *
 * Stores application settings for the CS123 GUI.
 *
 * You can access all app settings through the "settings" global variable.
 * The settings will be automatically updated when things are changed in the
 * GUI (the reverse is not true however: changing the value of a setting does
 * not update the GUI).
*/
struct Settings {
    // Loads settings from disk, or fills in default values if no saved settings exist.
    void loadSettingsOrDefaults();

    // Saves the current settings to disk.
    void saveSettings();

    // Core feature switches
    bool antiAliasing;
    bool depthOfField;
    bool hdr;
    bool shadowMapping;
    bool textureMapping;

    // Camtrans
    float cameraFov;            // The camera's field of view, which is twice the height angle.
    float cameraNear;           // The distance from the camera to the near clipping plane.
    float cameraFar;            // The distance from the camera to the far clipping plane.
    float cameraDof;

    // HDR
    float exposure;
    float gamma;

    bool drawWireframe;
    bool drawNormals;

    int shapeParameter1;
    int shapeParameter2;
    float shapeParameter3;
};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H

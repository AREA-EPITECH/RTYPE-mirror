/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Window_systems
*/

#include "ecs/Systems.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

namespace ecs {

    void init_settings(Registry &ecs, const std::string &filePath) {
        nlohmann::json settingsJson;
        try {
            std::ifstream settingsFile(filePath);
            if (!settingsFile.is_open()) {
                throw std::runtime_error("Failed to open settings file: " + filePath);
            }
            settingsFile >> settingsJson;
        } catch (const std::exception &e) {
            std::cerr << "Error loading settings: " << e.what() << std::endl;
            return;
        }

        ecs.register_component<ecs::SettingsComponent>();
        ecs.register_component<ecs::SoundComponent>();
        ecs.register_component<ecs::MusicComponent>();
        ecs.register_event<ecs::DisplaySettingEvent>();
        ecs.subscribe<ecs::DisplaySettingEvent>(ecs::display_settings_system);
        ecs.register_component<ecs::KeyBindingComponent>();
        ecs.register_component<ecs::FilterComponent>();

        auto filter = ecs.spawn_entity();
        ecs::ColorBlindMode filterMode = ecs::ColorBlindMode::NONE;
        if (settingsJson.contains("filter")) {
            std::string filterModeStr = settingsJson["filter"];
            if (filterModeStr == "Protanopia") {
                filterMode = ecs::ColorBlindMode::PROTANOPIA;
            } else if (filterModeStr == "Deuteranopia") {
                filterMode = ecs::ColorBlindMode::DEUTERANOPIA;
            } else if (filterModeStr == "Tritanopia") {
                filterMode = ecs::ColorBlindMode::TRITANOPIA;
            }
        }
        ecs.add_component<ecs::FilterComponent>(filter, {filterMode});

        auto keys = ecs.spawn_entity();
        ecs::KeyBindingComponent keyBindings;
        for (auto &[action, key] : settingsJson.items()) {
            if (action == "Sound" || action == "Music" || action == "ResolutionW" || action == "ResolutionH" || action == "filter") {
                continue;
            }
            keyBindings.setKey(action, key);
        }
        ecs.add_component<ecs::KeyBindingComponent>(keys, std::move(keyBindings));

        auto musicSound = ecs.spawn_entity();
        ecs::MusicComponent musicComponent;
        if (settingsJson.contains("Music")) {
            musicComponent.setVolume(settingsJson["Music"]);
        }
        ecs.add_component<ecs::MusicComponent>(musicSound, std::move(musicComponent));

        auto sound = ecs.spawn_entity();
        ecs::SoundComponent soundComponent;
        if (settingsJson.contains("Sound")) {
            soundComponent.setVolume(settingsJson["Sound"]);
        }
        ecs.add_component<ecs::SoundComponent>(sound, std::move(soundComponent));

        if (settingsJson.contains("ResolutionW") && settingsJson.contains("ResolutionH")) {
            int resolutionWidth = settingsJson["ResolutionW"];
            int resolutionHeight = settingsJson["ResolutionH"];
            SetWindowSize(resolutionWidth, resolutionHeight);
        }
    }

    /**
     * Create window
     * @param ecs
     */
    void init_window_system(Registry &ecs, const CreateWindowEvent &) {
        auto &windows = ecs.get_components<Window>();
        for (size_t i = 0; i < windows.size(); ++i) {
            if (windows[i]) {
                auto &win = windows[i].value();
                if (!win.isOpen) {
                    TraceLog(LOG_WARNING,
                             TextFormat("Initiating window %dx%d named %s.", win.width, win.height, win.title.c_str()));
                    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
                    InitWindow(win.width, win.height, win.title.c_str());
                    SetTargetFPS(144);
                    InitAudioDevice();
                    win.isOpen = true;
                    init_settings(ecs, "./client/settings/settings.json");
                    break;
                }
            }
        }
    }

    /**
     * Close the window
     * @param ecs
     */
    void close_window_system(Registry &ecs, const WindowCloseEvent &) {
        auto &windows = ecs.get_components<Window>();
        for (size_t i = 0; i < windows.size(); ++i) {
            if (windows[i]) {
                auto &win = windows[i].value();
                if (win.isOpen) {
                    CloseWindow();
                    win.isOpen = false;
                    break;
                }
            }
        }
    }
}
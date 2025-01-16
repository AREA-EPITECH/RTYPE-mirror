//
// Created by lferraro on 12/19/24.
//

#include "ecs/Systems.hpp"
#include "raylib/kbd_layout.h"

namespace ecs {

    std::string getPrintableKeyName(int key) {
        if (nonPrintableKeyNames.find(key) != nonPrintableKeyNames.end()) {
            return nonPrintableKeyNames.at(key);
        }

        if (key >= 32 && key <= 126) {
            return std::string(1, static_cast<char>(key));
        }

        return "Unknown";
    }


    void drawSoundAndMusicSliders(Registry &ecs, int xOffset, int yOffset, int sliderWidth, int sliderHeight) {
        Vector2 mousePosition = GetMousePosition();

        auto &soundLevels = ecs.get_components<SoundComponent>();

        for (auto &soundLevel : soundLevels) {
            if (soundLevel.has_value()) {
                Rectangle soundBar = {static_cast<float>(xOffset), static_cast<float>(yOffset + sliderHeight + 20),
                                      static_cast<float>(sliderWidth), static_cast<float>(sliderHeight)};
                DrawRectangleRec(soundBar, LIGHTGRAY);
                DrawRectangle(static_cast<int>(soundBar.x), static_cast<int>(soundBar.y),
                              static_cast<int>(soundLevel.value().volume / 100.0f * sliderWidth), sliderHeight, {148, 113, 150, 200});
                DrawRectangleLines(static_cast<int>(soundBar.x), static_cast<int>(soundBar.y),
                                   static_cast<int>(soundBar.width), static_cast<int>(soundBar.height), BLACK);
                DrawText("Sound", static_cast<int>(soundBar.x), static_cast<int>(yOffset), 30, WHITE);

                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, soundBar)) {
                    float newVolume = (mousePosition.x - soundBar.x) / soundBar.width * 100.0f;
                    newVolume = std::clamp(newVolume, 0.0f, 100.0f);
                    soundLevel.value().setVolume(newVolume);
                }
                std::string soundText = std::to_string(static_cast<int>(soundLevel.value().volume)) + "%";
                DrawText(soundText.c_str(), xOffset + sliderWidth + 20, yOffset + sliderHeight + 10, 30, WHITE);
                yOffset += 100;
            }
        }

        auto &musicsLevel = ecs.get_components<MusicComponent>();

        for (auto &soundLevel : musicsLevel) {
            if (soundLevel.has_value()) {
                auto &musicComponent = soundLevel.value();

                Rectangle musicBar = {static_cast<float>(xOffset), static_cast<float>(yOffset + sliderHeight + 20),
                                      static_cast<float>(sliderWidth), static_cast<float>(sliderHeight)};
                DrawRectangleRec(musicBar, LIGHTGRAY);
                DrawRectangle(static_cast<int>(musicBar.x), static_cast<int>(musicBar.y),
                              static_cast<int>(musicComponent.volume / 100.0f * sliderWidth), sliderHeight, {148, 113, 150, 200});
                DrawRectangleLines(static_cast<int>(musicBar.x), static_cast<int>(musicBar.y),
                                   static_cast<int>(musicBar.width), static_cast<int>(musicBar.height), BLACK);
                DrawText("Music", static_cast<int>(musicBar.x), static_cast<int>(yOffset), 30, WHITE);

                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, musicBar)) {
                    float newVolume = (mousePosition.x - musicBar.x) / musicBar.width * 100.0f;
                    newVolume = std::clamp(newVolume, 0.0f, 100.0f);
                    musicComponent.setVolume(newVolume);
                }

                std::string musicText = std::to_string(static_cast<int>(musicComponent.volume)) + "%";
                DrawText(musicText.c_str(), xOffset + sliderWidth + 20, yOffset + sliderHeight + 10, 30, WHITE);
                yOffset += 100;
            }
        }

    }

    void drawResolutionButton(int x, int y, int width, int height, int res_w, int res_h) {
        Rectangle buttonBounds = {static_cast<float>(x), static_cast<float>(y),
                                  static_cast<float>(width), static_cast<float>(height)};
        std::string res_text = "Set Resolution " + std::to_string(res_w) + "x" + std::to_string(res_h);
        if (GuiButton(buttonBounds, res_text.c_str())) {
            SetWindowSize(res_w, res_h);
        }
    }

    void createColorBlindModeButton(Registry &ecs, const Rectangle &buttonRect,
                                    const std::string &buttonText,
                                    ecs::ColorBlindMode mode) {
        auto &filterComponents = ecs.get_components<ecs::FilterComponent>();
        for (auto &filterComponent : filterComponents) {
            if (filterComponent.has_value()) {
                auto &filter = filterComponent.value();

                if (GuiButton(buttonRect, buttonText.c_str())) {
                    filter.removeFilter();
                    filter.setMode(mode);
                }
            }
        }
    }

    void display_settings_system (Registry &ecs, const DisplaySettingEvent &) {
        int s_width = GetScreenWidth();
        int s_height = GetScreenHeight();
        if (GuiButton({100, 100, 200, 100},
                      "Back")) {
           auto & settings = ecs.get_components<SettingsComponent>();
           for (int i = 0; i < settings.size(); i++) {
               if (settings[i].has_value()) {
                   ecs.kill_entity(i);
                   ecs.run_event(ChangeFocusEvent{MENU_FOCUS});
                   return;
               }
           }
        }

        auto &keyBindingsArray = ecs.get_components<KeyBindingComponent>();
        for (int i = 0; i < keyBindingsArray.size(); i++) {
            if (!keyBindingsArray[i].has_value())
                continue;

            auto &keyBindings = keyBindingsArray[i].value();

            int yOffset = s_height / 4;
            int xOffset = s_width / 4;
            int textBoxWidth = 200;
            int textBoxHeight = 50;

            auto actions = keyBindings.getActions();

            static std::unordered_map<std::string, bool> isFocusedMap;
            for (const auto &action : actions) {
                if (isFocusedMap.find(action) == isFocusedMap.end()) {
                    isFocusedMap[action] = false;
                }
            }

            DrawText("Controls", xOffset, 125, 50, WHITE);

            for (const auto &action : actions) {
                int currentKey = keyBindings.getKey(action);
                std::string keyText = getPrintableKeyName(currentKey);

                          DrawText(action.c_str(), xOffset, yOffset, 30, WHITE);
                yOffset += 50;

                Rectangle textBox = {static_cast<float>(xOffset), static_cast<float>(yOffset),
                                     static_cast<float>(textBoxWidth), static_cast<float>(textBoxHeight)};
                Vector2 mousePosition = GetMousePosition();

                static std::unordered_map<std::string, std::string> currentInputMap;
                if (currentInputMap.find(action) == currentInputMap.end()) {
                    currentInputMap[action] = "";
                }

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    isFocusedMap[action] = CheckCollisionPointRec(mousePosition, textBox);
                    if (isFocusedMap[action]) {
                        currentInputMap[action].clear();
                    }
                }

                DrawRectangleRec(textBox, isFocusedMap[action] ?  Color({148, 113, 150, 200}) : Color({195, 122, 164, 200}));
                DrawRectangleLinesEx(textBox, 1, BLACK);
                DrawText(currentInputMap[action].empty() ? keyText.c_str() : currentInputMap[action].c_str(),
                         static_cast<int>(textBox.x + 5), static_cast<int>(textBox.y + 10), 30, BLACK);

                if (isFocusedMap[action]) {
                    int key = Kbd_GetKeyPressed(KBD_Layout::FR);
                    if (key != 0) {
                        if (key >= 32 && key <= 126) {
                            currentInputMap[action] = std::string(1, static_cast<char>(key));
                        } else {
                            for (const auto &[nonPrintableKey, name] : nonPrintableKeyNames) {
                                if (key == nonPrintableKey) {
                                    currentInputMap[action] = name;
                                    break;
                                }
                            }
                        }

                        if (!currentInputMap[action].empty()) {
                            keyBindings.setKey(action, key);
                            isFocusedMap[action] = false;
                        }
                    }
                }

                yOffset += 70;
            }
        }

        DrawText("General", s_width / 2, 125, 50, WHITE);
        drawSoundAndMusicSliders(ecs, s_width / 2, s_height / 4 + 10, 300, 20);
        drawResolutionButton(s_width / 2, s_height / 2, 300, 100, 2560, 1440);
        drawResolutionButton(s_width / 2, s_height / 2 + 150, 300, 100, 1920, 1080);
        drawResolutionButton(s_width / 2, s_height / 2 + 300, 300, 100, 1680, 1050);

        DrawText("Filters", (int)(s_width / 1.3), 125, 50, WHITE);
        createColorBlindModeButton(ecs, {(float)(s_width / 1.3), (float)(s_height / 4), 300, 100}, "Normal", ecs::ColorBlindMode::NONE);
        createColorBlindModeButton(ecs, {(float)(s_width / 1.3), (float)(s_height / 4 + 150), 300, 100}, "Protanopia", ecs::ColorBlindMode::PROTANOPIA);
        createColorBlindModeButton(ecs, {(float)(s_width / 1.3), (float)(s_height / 4 + 300), 300, 100}, "Deuteranopia", ecs::ColorBlindMode::DEUTERANOPIA);
        createColorBlindModeButton(ecs, {(float)(s_width / 1.3), (float)(s_height / 4 + 450), 300, 100}, "Tritanopia", ecs::ColorBlindMode::TRITANOPIA);

    }
}

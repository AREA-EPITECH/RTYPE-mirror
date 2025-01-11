//
// Created by lferraro on 12/19/24.
//

#include "ecs/Systems.hpp"
#include "list"

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
            if (soundLevel.has_value() && soundLevel.value().type == 0) {
                Rectangle soundBar = {static_cast<float>(xOffset), static_cast<float>(yOffset),
                                      static_cast<float>(sliderWidth), static_cast<float>(sliderHeight)};
                DrawRectangleRec(soundBar, LIGHTGRAY);
                DrawRectangle(static_cast<int>(soundBar.x), static_cast<int>(soundBar.y),
                              static_cast<int>(soundLevel.value().volume / 100.0f * sliderWidth), sliderHeight, GREEN);
                DrawRectangleLines(static_cast<int>(soundBar.x), static_cast<int>(soundBar.y),
                                   static_cast<int>(soundBar.width), static_cast<int>(soundBar.height), BLACK);
                DrawText("Sound", static_cast<int>(soundBar.x - 60), static_cast<int>(soundBar.y + sliderHeight / 4), 20, WHITE);

                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, soundBar)) {
                    soundLevel.value().volume = (mousePosition.x - soundBar.x) / soundBar.width * 100.0f;
                    soundLevel.value().volume = std::clamp(soundLevel.value().volume, 0.0f, 100.0f);
                }
                std::string soundText = "Sound: " + std::to_string(static_cast<int>(soundLevel.value().volume)) + "%";
                DrawText(soundText.c_str(), xOffset + sliderWidth + 20, yOffset + sliderHeight / 4, 20, WHITE);
            }
            if (soundLevel.has_value() && soundLevel.value().type == 1) {
                Rectangle musicBar = {static_cast<float>(xOffset), static_cast<float>(yOffset + sliderHeight + 20),
                                      static_cast<float>(sliderWidth), static_cast<float>(sliderHeight)};
                DrawRectangleRec(musicBar, LIGHTGRAY);
                DrawRectangle(static_cast<int>(musicBar.x), static_cast<int>(musicBar.y),
                              static_cast<int>(soundLevel.value().volume / 100.0f * sliderWidth), sliderHeight, BLUE);
                DrawRectangleLines(static_cast<int>(musicBar.x), static_cast<int>(musicBar.y),
                                   static_cast<int>(musicBar.width), static_cast<int>(musicBar.height), BLACK);
                DrawText("Music", static_cast<int>(musicBar.x - 60), static_cast<int>(musicBar.y + sliderHeight / 4), 20, WHITE);

                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, musicBar)) {
                    soundLevel.value().volume = (mousePosition.x - musicBar.x) / musicBar.width * 100.0f;
                    soundLevel.value().volume = std::clamp(soundLevel.value().volume, 0.0f, 100.0f);
                }
                std::string musicText = "Music: " + std::to_string(static_cast<int>(soundLevel.value().volume)) + "%";
                DrawText(musicText.c_str(), xOffset + sliderWidth + 20, yOffset + sliderHeight + 20 + sliderHeight / 4, 20, WHITE);
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

    void display_settings_system (Registry &ecs, const DisplaySettingEvent &) {
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

            int yOffset = 250;
            int xOffset = 100;
            int textBoxWidth = 150;
            int textBoxHeight = 30;

            auto actions = keyBindings.getActions();

            static std::unordered_map<std::string, bool> isFocusedMap;
            for (const auto &action : actions) {
                if (isFocusedMap.find(action) == isFocusedMap.end()) {
                    isFocusedMap[action] = false;
                }
            }

            for (const auto &action : actions) {
                int currentKey = keyBindings.getKey(action);
                std::string keyText = getPrintableKeyName(currentKey);

                DrawText(action.c_str(), xOffset, yOffset, 20, WHITE);

                Rectangle textBox = {static_cast<float>(xOffset + 200), static_cast<float>(yOffset),
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

                DrawRectangleRec(textBox, isFocusedMap[action] ? GRAY : LIGHTGRAY);
                DrawRectangleLinesEx(textBox, 1, BLACK);
                DrawText(currentInputMap[action].empty() ? keyText.c_str() : currentInputMap[action].c_str(),
                         static_cast<int>(textBox.x + 5), static_cast<int>(textBox.y + 5), 20, BLACK);

                if (isFocusedMap[action]) {
                    int key = GetKeyPressed();
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

                yOffset += 50;
            }
        }
        drawSoundAndMusicSliders(ecs, 100, 600, 300, 20);
        drawResolutionButton(100, 800, 300, 50, 2560, 1440);
        drawResolutionButton(100, 900, 300, 50, 1920, 1080);
        drawResolutionButton(100, 1000, 300, 50, 1600, 900);
    }
}

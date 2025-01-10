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
    }
}

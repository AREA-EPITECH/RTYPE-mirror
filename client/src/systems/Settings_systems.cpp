//
// Created by lferraro on 12/19/24.
//

#include "ecs/Systems.hpp"

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

            for (const auto &action : keyBindings.getActions()) {
                int currentKey = keyBindings.getKey(action);

                std::string keyText = getPrintableKeyName(currentKey);

                DrawText(action.c_str(), xOffset, yOffset, 20, WHITE);

                char keyBuffer[32];
                strncpy(keyBuffer, keyText.c_str(), sizeof(keyBuffer));

                Rectangle textBox = {static_cast<float>(xOffset + 200), static_cast<float>(yOffset),
                                     static_cast<float>(textBoxWidth), static_cast<float>(textBoxHeight)};
                if (GuiTextBox(textBox, keyBuffer, sizeof(keyBuffer), true)) {
                    std::string newKeyText(keyBuffer);

                    if (newKeyText.length() == 1) {
                        int newKey = static_cast<int>(newKeyText[0]);
                        std::cout << newKey << std::endl;
                        keyBindings.setKey(action, newKey);
                    } else {
                        for (const auto &[key, name] : nonPrintableKeyNames) {
                            if (newKeyText == name) {
                                keyBindings.setKey(action, key);
                                break;
                            }
                        }
                    }
                }

                yOffset += 50;
            }
        }
    }
}

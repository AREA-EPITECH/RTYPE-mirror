//
// Created by lferraro on 12/19/24.
//

#include "ecs/Systems.hpp"

namespace ecs {
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



    }
}

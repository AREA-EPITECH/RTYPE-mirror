//
// Created by lferraro on 12/17/24.
//

#include "ecs/Systems.hpp"

namespace ecs {
    /**
 * change the focus of the window
 * @param ecs
 * @param event
 */
    void change_focus_system(Registry &ecs, const ChangeFocusEvent &event) {
        auto &focus = ecs.get_components<FocusComponent>();

        for (auto &elem : focus) {
            if (elem.has_value()) {
                elem.value().focus = event.newFocus;
            }
        }
    }

    /**
* get the focus of the window
* @param ecs
*/
    WindowFocus get_focus(Registry &ecs) {
        auto &focus = ecs.get_components<FocusComponent>();

        for (auto &elem : focus) {
            if (elem.has_value()) {
                return elem.value().focus;
            }
        }
        return DEFAULT_FOCUS;
    }
}
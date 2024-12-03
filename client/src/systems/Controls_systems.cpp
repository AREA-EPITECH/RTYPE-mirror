//
// Created by lferraro on 12/2/24.
//

#include "include/Systems.hpp"
namespace ecs {
    void change_window(Registry &ecs) {
        ecs.run_event(WindowCloseEvent{});

        ecs.unsubscribe_all<WindowOpenEvent>();
        ecs.unsubscribe_all<WindowCloseEvent>();
        ecs.unsubscribe_all<WindowDrawEvent>();
        ecs.unsubscribe_all<ControlsEvent>();

        ecs.subscribe<ControlsEvent>(menu_controls_system);
        ecs.subscribe<WindowOpenEvent>(init_window_system);
        ecs.subscribe<WindowCloseEvent>(close_window_system);
        ecs.subscribe<WindowDrawEvent>(draw_menu_system);

        ecs.run_event(WindowOpenEvent{});
    }

    void menu_controls_system(Registry &ecs, const ControlsEvent &) {
        if (IsKeyPressed(KEY_ENTER)) {
            std::cout << "menu" << std::endl;
            ecs.unsubscribe_all<ControlsEvent>();
            ecs.subscribe<ControlsEvent>(selector_controls_system);
            //for (std::size_t i = 0;  ;i++) {
            //
            //}
            //currentModel = (currentModel + 1) % nb_vox;
        }

        if (IsKeyPressed(KEY_LEFT)) {
            //if (currentModel != 0)
            //{
            //    currentModel = (currentModel - 1) % nb_vox;
            //}
            //else
            //{
            //    currentModel = nb_vox - 1;
            //}
        }
    }

    void selector_controls_system(Registry &ecs, const ControlsEvent &) {
        auto &models = ecs.get_components<ModelComponent>();
        if (IsKeyPressed(KEY_ENTER)) {
            std::cout << "selector" << std::endl;
            change_window(ecs);
            //for (std::size_t i = 0;  ;i++) {
            //
            //}
            //currentModel = (currentModel + 1) % nb_vox;
        }

        if (IsKeyPressed(KEY_LEFT)) {
            //if (currentModel != 0)
            //{
            //    currentModel = (currentModel - 1) % nb_vox;
            //}
            //else
            //{
            //    currentModel = nb_vox - 1;
            //}
        }
    }

    void game_controls_system(Registry &ecs, const ControlsEvent &) {
        auto &models = ecs.get_components<ModelComponent>();
        if (IsKeyPressed(KEY_ENTER)) {
            std::cout << "game" << std::endl;
            ecs.unsubscribe_all<ControlsEvent>();
            ecs.subscribe<ControlsEvent>(menu_controls_system);
            //for (std::size_t i = 0;  ;i++) {
            //
            //}
            //currentModel = (currentModel + 1) % nb_vox;
        }

        if (IsKeyPressed(KEY_LEFT)) {
            //if (currentModel != 0)
            //{
            //    currentModel = (currentModel - 1) % nb_vox;
            //}
            //else
            //{
            //    currentModel = nb_vox - 1;
            //}
        }
    }
}
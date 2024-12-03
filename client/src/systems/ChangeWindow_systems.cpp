//
// Created by lferraro on 12/2/24.
//

#include "ecs/Systems.hpp"
namespace ecs {
    void change_window(Registry &ecs, WindowType type) {
        ecs.run_event(WindowCloseEvent{});

        ecs.unsubscribe_all<WindowOpenEvent>();
        ecs.unsubscribe_all<WindowCloseEvent>();
        ecs.unsubscribe_all<WindowDrawEvent>();
        ecs.unsubscribe_all<ControlsEvent>();

        switch (type) {
            case WindowType::MENU:
                ecs.subscribe<ControlsEvent>(menu_controls_system);
                ecs.subscribe<WindowOpenEvent>(init_window_system);
                ecs.subscribe<WindowCloseEvent>(close_window_system);
                ecs.subscribe<WindowDrawEvent>(draw_menu_system);
                break;

            case WindowType::SELECTOR:
                ecs.subscribe<ControlsEvent>(selector_controls_system);
                ecs.subscribe<ecs::WindowOpenEvent>([](Registry &ecs, const ecs::WindowOpenEvent &event) {
                    init_window_system(ecs, event);
                    open_lobby_system(ecs, event);
                });
                ecs.subscribe<ecs::WindowCloseEvent>([](Registry &e, const ecs::WindowCloseEvent &event) {
                    close_lobby_system(e, event);
                    close_window_system(e, event);
                });
                ecs.subscribe<ecs::WindowDrawEvent>(ecs::draw_lobby_system);
                break;

            case WindowType::GAME:
                break;
        }

        ecs.run_event(WindowOpenEvent{});
    }

    void menu_controls_system(Registry &ecs, const ControlsEvent &) {
        if (IsKeyPressed(KEY_ENTER)) {
            std::cout << "go to selector" << std::endl;
            change_window(ecs, WindowType::SELECTOR);
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
            std::cout << "go to menu" << std::endl;
            change_window(ecs, WindowType::MENU);
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

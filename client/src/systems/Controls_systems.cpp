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
                ecs.subscribe<ControlsEvent>(game_controls_system);
                ecs.subscribe<WindowOpenEvent>(init_window_system);
                ecs.subscribe<WindowCloseEvent>(close_window_system);
                ecs.subscribe<WindowDrawEvent>(draw_game_system);
                break;
        }

        ecs.run_event(WindowOpenEvent{});
    }

    void menu_controls_system(Registry &ecs, const ControlsEvent &) {
        if (IsKeyPressed(KEY_ENTER)) {
            change_window(ecs, WindowType::SELECTOR);
        }

        if (IsKeyPressed(KEY_LEFT)) {
        }
    }

    void selector_controls_system(Registry &ecs, const ControlsEvent &) {
        auto &models = ecs.get_components<ModelComponent>();
        if (IsKeyPressed(KEY_ENTER)) {
            change_window(ecs, WindowType::GAME);
        }

        if (IsKeyPressed(KEY_LEFT)) {
            size_t current = -1;
            size_t to_change = 0;
            for (size_t i = 0; i < models.size() ;i++) {
                if (models[i].has_value()) {
                    if (models[i].value().drawable)
                        if (current == -1) {
                            to_change = i;
                            continue;
                        } else {
                            models[current].value().drawable = true;
                            models[i].value().drawable = false;
                            return;
                        }
                    if (!models[i].value().drawable)
                        current = i;
                }
            }
            if (models[current].has_value()) {
                models[current].value().drawable = true;
                models[to_change].value().drawable = false;
            }
        }

        if (IsKeyPressed(KEY_RIGHT)) {
            size_t current = -1;
            size_t to_change = 0;

            for (size_t i = models.size(); i-- > 0;) {
                if (models[i].has_value()) {
                    if (models[i].value().drawable) {
                        if (current == -1) {
                            to_change = i;
                            continue;
                        } else {
                            models[current].value().drawable = true;
                            models[i].value().drawable = false;
                            return;
                        }
                    }
                    if (!models[i].value().drawable)
                        current = i;
                }
            }

            if (models[current].has_value()) {
                models[current].value().drawable = true;
                models[to_change].value().drawable = false;
            }
        }
    }

    void game_controls_system(Registry &ecs, const ControlsEvent &) {
        if (IsKeyPressed(KEY_ENTER)) {
            change_window(ecs, WindowType::MENU);
        }

        if (IsKeyPressed(KEY_LEFT)) {
        }
    }
}

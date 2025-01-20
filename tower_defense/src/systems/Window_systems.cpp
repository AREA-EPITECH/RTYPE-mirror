/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Window_systems
*/

#include "ecs/Systems.hpp"

namespace ecs {

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
                    InitWindow(win.width, win.height, win.title.c_str());
                    SetTargetFPS(144);
                    InitAudioDevice();
                    win.isOpen = true;
                    break;
                }
            }
        }
    }

    void init_register_system(Registry &ecs, const InitRegisterEvent &)
    {
        ecs.subscribe<ControlsEvent>(game_controls_system);
        ecs.subscribe<WindowDrawEvent>(draw_game_system);
        ecs.subscribe<CreateTextEvent>(create_text_component);
        ecs.subscribe<CreateTowerEvent>(create_tower_component);
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
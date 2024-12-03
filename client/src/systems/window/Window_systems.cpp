//
// Created by lferraro on 12/2/24.
//
#include "ecs/Systems.hpp"
namespace ecs {
    void init_window_system(Registry &ecs, const WindowOpenEvent &) {
        auto &windows = ecs.get_components<Window>();
        for (size_t i = 0; i < windows.size(); ++i) {
            if (windows[i]) {
                auto &win = windows[i].value();
                if (!win.isOpen) {
                    TraceLog(LOG_WARNING,
                             TextFormat("Initiating window %dx%d named %s.", win.width, win.height, win.title.c_str()));
                    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
                    InitWindow(win.width, win.height, win.title.c_str());
                    SetTargetFPS(144);
                    win.isOpen = true;
                    break;
                }
            }
        }
    }

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
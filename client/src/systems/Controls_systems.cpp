//
// Created by lferraro on 12/2/24.
//

#include "include/Systems.hpp"

void menu_controls_system(Registry &e, const ControlsEvent &) {
    if (IsKeyPressed(KEY_ENTER))
    {
        std::cout << "menu" << std::endl;
        e.unsubscribe_all<ControlsEvent>();
        e.subscribe<ControlsEvent>(selector_controls_system);
        //for (std::size_t i = 0;  ;i++) {
        //
        //}
        //currentModel = (currentModel + 1) % nb_vox;
    }

    if (IsKeyPressed(KEY_LEFT))
    {
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

void selector_controls_system(Registry &e, const ControlsEvent&) {
    auto &models = e.get_components<ModelComponent>();
    if (IsKeyPressed(KEY_ENTER))
    {
        std::cout << "selector" << std::endl;
        e.unsubscribe_all<ControlsEvent>();
        e.subscribe<ControlsEvent>(game_controls_system);
        //for (std::size_t i = 0;  ;i++) {
        //
        //}
        //currentModel = (currentModel + 1) % nb_vox;
    }

    if (IsKeyPressed(KEY_LEFT))
    {
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

void game_controls_system(Registry &e, const ControlsEvent&) {
    auto &models = e.get_components<ModelComponent>();
    if (IsKeyPressed(KEY_ENTER))
    {
        std::cout << "game" << std::endl;
        e.unsubscribe_all<ControlsEvent>();
        e.subscribe<ControlsEvent>(menu_controls_system);
        //for (std::size_t i = 0;  ;i++) {
        //
        //}
        //currentModel = (currentModel + 1) % nb_vox;
    }

    if (IsKeyPressed(KEY_LEFT))
    {
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

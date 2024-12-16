/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Controls_systems
*/

#include "ecs/Systems.hpp"

namespace ecs {

    /**
     * Change the window
     * @param ecs
     * @param type
     */
    void change_window(Registry &ecs, WindowType type) {
        ecs.run_event(WindowCloseEvent{});

        ecs.unsubscribe_all<WindowOpenEvent>();
        ecs.unsubscribe_all<WindowCloseEvent>();
        ecs.unsubscribe_all<WindowDrawEvent>();
        ecs.unsubscribe_all<ControlsEvent>();

        switch (type) {
            case MENU:
                init_menu_window(ecs);
                break;

            case LOBBY:
                init_lobby_window(ecs);
                break;

            case GAME:
                init_game_window(ecs);
                break;
        }

        ecs.run_event(WindowOpenEvent{});
    }

    /**
     * Get controls in the menu
     * @param ecs
     */
    void menu_controls_system(Registry &ecs, const ControlsEvent &) {

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePosition = GetMousePosition();
            auto &buttons = ecs.get_components<ButtonComponent>();
            auto &box = ecs.get_components<ShowBoxComponent>();

            for (auto &button : buttons) {
                if (button.has_value()) {
                    button->isButtonPressed(mousePosition);
                }
            }

            for (auto &b : box) {
                if (b.has_value()) {
                    b.value().handleClick(mousePosition);
                }
            }
        }
    }

    /**
     * Get controls in the lobby
     * @param ecs
     */
    void lobby_controls_system(Registry &ecs, const ControlsEvent &) {
        auto &models = ecs.get_components<VesselsComponent>();

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
            if (current != -1 && models[current].has_value()) {
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

            if (current != -1 && models[current].has_value()) {
                models[current].value().drawable = true;
                models[to_change].value().drawable = false;
            }
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePosition = GetMousePosition();
            auto &buttons = ecs.get_components<ButtonComponent>();

            for (auto &button : buttons) {
                if (button.has_value()) {
                    button->isButtonPressed(mousePosition);
                }
            }
        }
    }

    /**
     * @brief Create projectile entity in ecs
     * @param ecs
     * @param model
     * @param position
     * @param velocity
     * @param player
     * @param path
     */
    void create_projectile(Registry &ecs, Model model, Vector3 position, Vector3 velocity, bool player,
        std::string &path) {
        auto entity = ecs.spawn_entity();
        ecs.add_component<ProjectilesComponent>(entity, {model, true, path, position, player, velocity});
    }

    /**
     * @brief Get controls in the game
     * @param ecs
     */
    void game_controls_system(Registry &ecs, const ControlsEvent &) {
        auto &models = ecs.get_components<VesselsComponent>();
        VesselsComponent *modelComponent = nullptr;
        for (auto & model : models) {
            if (model.has_value()) {
                modelComponent = &model.value();
                break;
            }
        }
        if (modelComponent == nullptr)
            return;
        if (IsKeyPressed(KEY_ENTER)) {
            change_window(ecs, MENU);
        }
        if (IsKeyPressed(KEY_SPACE)) {
            auto &projectiles = ecs.get_components<ProjectilesComponent>();
            for (auto &projectile : projectiles) {
                if (projectile.has_value()) {
                    if (projectile->player && !projectile->drawable) {
                        create_projectile(ecs, projectile->model,
                            {modelComponent->position.x + 10, modelComponent->position.y + 2, 0},
                            {0.5, 0, 0}, true, projectile->path);
                    }
                }
            }
        }
        if (IsKeyPressed(KEY_LEFT) || IsKeyDown(KEY_LEFT)) {
            modelComponent->position.x -= 0.1f;
        }
        if (IsKeyPressed(KEY_RIGHT) || IsKeyDown(KEY_RIGHT)) {
            modelComponent->position.x += 0.1f;
        }
        if (IsKeyPressed(KEY_UP) || IsKeyDown(KEY_UP)) {
            modelComponent->position.y += 0.1f;
        }
        if (IsKeyPressed(KEY_DOWN) || IsKeyDown(KEY_DOWN)) {
            modelComponent->position.y -= 0.1f;
        }

    }
}

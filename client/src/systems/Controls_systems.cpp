/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Controls_systems
*/

#include "ecs/Systems.hpp"

namespace ecs
{

    /**
     * Change the window
     * @param ecs
     * @param type
     */
    void change_window(Registry &ecs, WindowType type)
    {
        ecs.run_event(WindowCloseEvent{});

        ecs.unsubscribe_all<WindowOpenEvent>();
        ecs.unsubscribe_all<WindowCloseEvent>();
        ecs.unsubscribe_all<WindowDrawEvent>();
        ecs.unsubscribe_all<ControlsEvent>();

        switch (type)
        {
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
    void menu_controls_system(Registry &ecs, const ControlsEvent &)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            auto &images = ecs.get_components<ImageComponent>();
            Vector2 mousePosition = GetMousePosition();
            for (auto &image : images)
            {
                if (image.has_value())
                {
                    image.value().handleClick(mousePosition, get_focus(ecs));
                }
            }
        }
    }

    /**
     * Get controls in the lobby
     * @param ecs
     */
    void lobby_controls_system(Registry &ecs, const ControlsEvent &)
    {
        auto &models = ecs.get_components<VesselsComponent>();

        if (IsKeyPressed(KEY_LEFT))
        {
            size_t current = -1;
            size_t to_change = 0;
            for (size_t i = 0; i < models.size(); i++)
            {
                if (models[i].has_value())
                {
                    if (models[i].value().drawable)
                        if (current == -1)
                        {
                            to_change = i;
                            continue;
                        }
                        else
                        {
                            models[current].value().drawable = true;
                            models[i].value().drawable = false;
                            return;
                        }
                    if (!models[i].value().drawable)
                        current = i;
                }
            }
            if (current != -1 && models[current].has_value())
            {
                models[current].value().drawable = true;
                models[to_change].value().drawable = false;
            }
        }

        if (IsKeyPressed(KEY_RIGHT))
        {
            size_t current = -1;
            size_t to_change = 0;

            for (size_t i = models.size(); i-- > 0;)
            {
                if (models[i].has_value())
                {
                    if (models[i].value().drawable)
                    {
                        if (current == -1)
                        {
                            to_change = i;
                            continue;
                        }
                        else
                        {
                            models[current].value().drawable = true;
                            models[i].value().drawable = false;
                            return;
                        }
                    }
                    if (!models[i].value().drawable)
                        current = i;
                }
            }

            if (current != -1 && models[current].has_value())
            {
                models[current].value().drawable = true;
                models[to_change].value().drawable = false;
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
     * @param target
     * @param nb
     * @param shader
     */
    void create_player_basic_projectile(Registry &ecs, Model model, Vector3 position, Vector3 velocity, bool player,
                                        std::string &path, Vector3 target, int nb, Shader shader)
    {
        auto entity = ecs.spawn_entity();
        ecs.add_component<ProjectilesComponent>(entity,
                                                {
                                                    model,
                                                    true,
                                                    path,
                                                    position,
                                                    player,
                                                    velocity,
                                                    target,
                                                    DARKBLUE,
                                                    nb,
                                                    shader
                                                });
    }

    /**
     * @brief Get controls in the game
     * @param ecs
     */
    void game_controls_system(Registry &ecs, const ControlsEvent &)
    {
        auto &cameras = ecs.get_components<CameraComponent>();
        auto &models = ecs.get_components<VesselsComponent>();
        auto &controllables = ecs.get_components<ControllableComponent>();
        auto &lights = ecs.get_components<LightComponent>();
        auto &projectiles = ecs.get_components<ProjectilesComponent>();
        auto &shaders = ecs.get_components<ShaderComponent>();
        int nb_lights= 0;

        VesselsComponent *modelComponent = nullptr;
        for (auto &model : models)
        {
            for (auto &controllable : controllables)
            {
                if (controllable.has_value())
                {
                    if (model.has_value())
                    {
                        modelComponent = &model.value();
                        break;
                    }
                }
            }
        }
        CameraComponent *cameraComponent = nullptr;
        for (auto &camera : cameras)
        {
            if (camera.has_value())
            {
                cameraComponent = &camera.value();
                break;
            }
        }

        Shader shader = {};
        for (auto & shader_i : shaders) {
            if (shader_i.has_value()) {
                shader = shader_i->shader;
                break;
            }
        }

        for (auto &light : lights)
        {
            if (light.has_value())
            {
                nb_lights += 1;
            }
        }

        for (auto &projectile : projectiles)
        {
            if (projectile.has_value())
            {
                if (projectile->drawable)
                {
                    nb_lights += 1;
                }
            }
        }

        if (modelComponent == nullptr || cameraComponent == nullptr)
            return;
        if (IsKeyPressed(KEY_ENTER))
        {
            change_window(ecs, MENU);
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            for (auto &projectile : projectiles)
            {
                if (projectile.has_value())
                {
                    if (projectile->player && !projectile->drawable)
                    {
                        create_player_basic_projectile(
                            ecs, projectile->model,
                            {modelComponent->position.x + 10, modelComponent->position.y + 2, 0}, {0.5, 0, 0}, true,
                            projectile->path,
                            Vector3Zero(), nb_lights, shader);
                    }
                }
            }
        }
        if (IsKeyPressed(KEY_LEFT) || IsKeyDown(KEY_LEFT))
        {
            modelComponent->Move(client::Direction::LEFT, cameraComponent->camera);
        }
        if (IsKeyPressed(KEY_RIGHT) || IsKeyDown(KEY_RIGHT))
        {
            modelComponent->Move(client::Direction::RIGHT, cameraComponent->camera);
        }
        if (IsKeyPressed(KEY_UP) || IsKeyDown(KEY_UP))
        {
            modelComponent->Move(client::Direction::UP, cameraComponent->camera);
        }
        if (IsKeyPressed(KEY_DOWN) || IsKeyDown(KEY_DOWN))
        {
            modelComponent->Move(client::Direction::DOWN, cameraComponent->camera);
        }
    }
} // namespace ecs

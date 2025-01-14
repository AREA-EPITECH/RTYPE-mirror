/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Controls_systems
*/

#include "ecs/Systems.hpp"
#include "raylib/kbd_layout.h"

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
        if (Kbd_IsKeyPressed(KBD_Layout::FR, KEY_LEFT))
        {
            previous_ship(ecs);
        }

        if (Kbd_IsKeyPressed(KBD_Layout::FR, KEY_RIGHT))
        {
            next_ship(ecs);
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
    void create_player_basic_projectile(Registry &ecs, uint32_t id, Model model, Vector3 position, Vector3 velocity, bool player,
                                        std::string &path, Vector3 target, int nb, Shader shader)
    {
        auto entity = ecs.spawn_entity();
        ecs.add_component<ProjectilesComponent>(
            entity, {id, model, true, path, position, player, velocity, target, DARKBLUE, nb, shader});
    }

    /**
     * @brief Get controls in the game
     * @param ecs
     */
    void game_controls_system(Registry &ecs, const ControlsEvent &)
    {
        struct network::InputPacket input_packet;
        input_packet.move = network::MoveDirection::NoneDirection;
        input_packet.fire = network::FireType::NoneFire;
        auto &cameras = ecs.get_components<CameraComponent>();
        auto &models = ecs.get_components<VesselsComponent>();
        auto &controllables = ecs.get_components<ControllableComponent>();
        auto &lights = ecs.get_components<LightComponent>();
        auto &projectiles = ecs.get_components<ProjectilesComponent>();
        auto &shaders = ecs.get_components<ShaderComponent>();
        int nb_lights = 0;

        auto &controls = ecs.get_components<KeyBindingComponent>();

        VesselsComponent *modelComponent = nullptr;
        for (int i = 0; i < models.size(); i++) {
            if (models[i].has_value() && controllables[i].has_value()) {
                modelComponent = &models[i].value();
                break;
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
        for (auto &shader_i : shaders)
        {
            if (shader_i.has_value())
            {
                shader = *shader_i->shader;
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
        if (Kbd_IsKeyPressed(KBD_Layout::FR, KEY_ENTER))
        {
            change_window(ecs, MENU);
        }
        for (auto &key : controls)
        {
            if (key.has_value())
            {
                int move_up = key.value().getKey("Move Up");
                int move_down = key.value().getKey("Move Down");
                int move_left = key.value().getKey("Move Left");
                int move_right = key.value().getKey("Move Right");

                if (Kbd_IsKeyPressed(KBD_Layout::FR, move_left) || Kbd_IsKeyDown(KBD_Layout::FR, move_left))
                {
                    modelComponent->Move(client::Direction::LEFT, cameraComponent->camera);
                    input_packet.move = network::MoveDirection::LeftDirection;
                }
                if (Kbd_IsKeyPressed(KBD_Layout::FR, move_right) || Kbd_IsKeyDown(KBD_Layout::FR, move_right))
                {
                    modelComponent->Move(client::Direction::RIGHT, cameraComponent->camera);
                    input_packet.move = network::MoveDirection::RightDirection;
                }
                if (Kbd_IsKeyPressed(KBD_Layout::FR, move_up) || Kbd_IsKeyDown(KBD_Layout::FR, move_up))
                {
                    modelComponent->Move(client::Direction::UP, cameraComponent->camera);
                    input_packet.move = network::MoveDirection::UpDirection;
                }
                if (Kbd_IsKeyPressed(KBD_Layout::FR, move_down) || Kbd_IsKeyDown(KBD_Layout::FR, move_down))
                {
                    modelComponent->Move(client::Direction::DOWN, cameraComponent->camera);
                    input_packet.move = network::MoveDirection::DownDirection;
                }

                if (Kbd_IsKeyPressed(KBD_Layout::FR, key.value().getKey("Basic Shoot")))
                {
                    input_packet.fire = network::FireType::NormalFire;
                }
            }
        }
        ecs.run_event(input_packet);
    }
} // namespace ecs

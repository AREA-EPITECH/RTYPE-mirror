/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Menu_systems
*/
#include "ecs/Systems.hpp"

namespace ecs
{
    /**
     * Draw the menu window
     * @param ecs
     */
    void draw_menu_system(Registry &ecs, const WindowDrawEvent &)
    {
        ecs.run_event(ControlsEvent{});
        auto &backgrounds = ecs.get_components<BackgroundComponent>();
        auto &decors = ecs.get_components<DecorElementComponent>();
        auto &musics = ecs.get_components<MusicComponent>();

        auto &lights = ecs.get_components<LightComponent>();
        auto &shaders = ecs.get_components<ShaderComponent>();

        Shader shader = {};

        for (auto &shader_i : shaders) {
            if (shader_i.has_value()) {
                shader = *shader_i->shader;
                break;
            }
        }

        for (auto &light : lights) {
            if (light.has_value()) {
                light->light->UpdateLightValues(shader, light->light->_enabled);
            }
        }

        for (auto &music : musics) {
            if (music.has_value()) {
                music.value().update("menu_music");
            }
        }

        for (auto &background : backgrounds)
        {
            if (background.has_value())
            {
                background->offset -= background->speed * GetFrameTime();
                float textureWidthOnScreen = static_cast<float>(background->texture.width) *
                    (static_cast<float>(GetScreenHeight()) / static_cast<float>(background->texture.height));
                if (background->offset <= -textureWidthOnScreen)
                {
                    background->offset = 0;
                }
            }
        }
        float deltaTime = GetFrameTime();
        for (auto &decor : decors)
        {
            if (decor.has_value())
            {
                decor->Update(deltaTime, GetScreenWidth(), GetScreenHeight());
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);


        for (auto &background : backgrounds)
        {
            if (background.has_value())
            {
                background->DrawLayer(GetScreenWidth(), GetScreenHeight());
            }
        }
        for (auto &decor : decors)
        {
            if (decor.has_value())
            {
                decor->DrawDecorElement(GetScreenWidth(), GetScreenHeight());
            }
        }

        const int screenWidth = GetScreenWidth();
        const int screenHeight = GetScreenHeight();
        auto &cameras = ecs.get_components<CameraComponent>();

        for (auto &camera_i : cameras)
        {
            if (camera_i.has_value())
            {
                Camera &camera = camera_i->camera;

                BeginMode3D(camera);

                auto &models = ecs.get_components<MenuText>();
                for (auto &model : models)
                {
                    if (model.has_value())
                    {
                        MenuText &modelComponent = model.value();
                        DrawModel(modelComponent.model, {0, 0, 0}, 1.5f, WHITE);
                    }
                }

                EndMode3D();
                break;
            }
        }

        update_menu_selectors(ecs, screenWidth, screenHeight);
        display_menu_selectors(ecs);

        auto &popups = ecs.get_components<PopupComponent>();
        ecs::WindowFocus focus = ecs::get_focus(ecs);
        for (int i = 0; i < popups.size(); i++) {
            if (popups[i].has_value()) {
                if (popups[i]->isVisible) {
                    popups[i]->updateBox(screenWidth, screenHeight);
                    popups[i]->draw(focus);
                } else {
                    ecs.kill_entity(i);
                }
            }
        }
        auto &filters = ecs.get_components<FilterComponent>();
        for (auto &filter : filters) {
            if (filter.has_value()) {
                filter.value().applyFilter();
            }
        }
        EndDrawing();

        if (WindowShouldClose())
        {
            ecs.run_event(WindowCloseEvent{true});
        }
    }

    void open_menu_system(Registry &ecs, const WindowOpenEvent &)
    {
        ecs.run_event(InitCameraEvent{
            {40.0f, -10.0f, 0.0f}, {0.0f, -10.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f, CAMERA_PERSPECTIVE});
        auto &cameras = ecs.get_components<CameraComponent>();
        Camera camera = {};
        for (auto &camera_i : cameras)
        {
            if (camera_i.has_value())
            {
                camera = camera_i->camera;
                break;
            }
        }
        ecs.run_event(InitLightEvent{client::LIGHT_DIRECTIONAL,
                                     {0, 0, 0},
                                     Vector3Normalize(Vector3Subtract(camera.target, camera.position)),
                                     WHITE,
                                     0});

        ecs.run_event(InitModelEvent{});
        ecs.run_event(InitShaderEvent{"client/assets/voxels/shaders/voxel_lighting.vs",
                                      "client/assets/voxels/shaders/voxel_lighting.fs"});

        auto settingsIcons = ecs.spawn_entity();
        std::string icon_path = ASSET_FILE("images/settings.png");
        float width = 100.0f;
        float height = 100.0f;


        ecs.add_component<ImageComponent>(
            settingsIcons,
            {icon_path, MENU_FOCUS, [width](int screenWidth, int screenHeight) { return screenWidth - 50 - width; },
             [height](int screenWidth, int screenHeight) { return screenHeight - 50 - height; }, width, height,
             []() { std::cout << "Image clicked!" << std::endl; }});
        ecs.add_component<ecs::ImageComponent>(settingsIcons, {icon_path, MENU_FOCUS,
                                                                [width](int screenWidth, int screenHeight) { return screenWidth - 50 - width; },
                                                                [height](int screenWidth, int screenHeight) { return screenHeight - 50 - height; },
                                                                width, height, [&ecs]() {
                                                                    auto settingEntity = ecs.spawn_entity();

                                                                    std::string back_path = ASSET_FILE("backgrounds/menu/setting_back.jpg");
                                                                    ImageComponent setting_back(back_path, SETTINGS_FOCUS,
                                                                                               [](int screenWidth, int screenHeight) {return 0;},
                                                                                                 [](int screenWidth, int screenHeight) {return 0;}, GetScreenWidth(), GetScreenHeight());
                                                                    ecs.add_component<SettingsComponent>(settingEntity, {setting_back});
                                                                    ecs.run_event(ChangeFocusEvent{SETTINGS_FOCUS});
                                                                }});
        // Init background
        ecs.run_event(InitBackgroundEvent{"client/assets/backgrounds/game/space_background.png", 1, 50, 0});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_midground.png", 1, 75});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_midground_2.png", 2, 100});
        ecs.run_event(InitDecorElementEvent{"client/assets/backgrounds/game/space_foreground.png", 3, 125});

        auto &musics = ecs.get_components<ecs::MusicComponent>();
        for (auto &music : musics) {
            if (music.has_value()) {
                music.value().addMusic("menu_music", "./client/assets/sound/menu_music.wav");
                music.value().play("menu_music");
                break;
            }
        }

    }

    void close_menu_system(Registry &ecs, const WindowCloseEvent &)
    {
        auto &images = ecs.get_components<ImageComponent>();
        for (std::size_t i = 0; i < images.size(); ++i)
        {
            if (images[i].has_value())
            {
                UnloadTexture(images[i]->texture);
                ecs.kill_entity(i);
            }
        }
        auto &texts = ecs.get_components<MenuText>();
        for (std::size_t i = 0; i < texts.size(); ++i)
        {
            if (texts[i].has_value())
            {
                UnloadModel(texts[i]->model);
                ecs.kill_entity(i);
            }
        }
        kill_entities_with_component<LightComponent>(ecs);
        kill_entities_with_component<ButtonComponent>(ecs);
        kill_entities_with_component<TextInputComponent>(ecs);
        kill_entities_with_component<ShowBoxComponent>(ecs);
    }
} // namespace ecs

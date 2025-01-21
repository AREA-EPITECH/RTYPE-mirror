/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Window_systems
*/

#include "ecs/Systems.hpp"

namespace ecs
{
    /**
     * @brief Create window
     * @param ecs
     */
    void init_window_system(Registry &ecs, const CreateWindowEvent &)
    {
        auto &windows = ecs.get_components<Window>();
        for (size_t i = 0; i < windows.size(); ++i)
        {
            if (windows[i])
            {
                auto &win = windows[i].value();
                if (!win.isOpen)
                {
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


    /**
     * @brief Register all the systems
     * @param ecs
     */
    void init_register_system(Registry &ecs, const InitRegisterEvent &)
    {
        ecs.subscribe<ControlsEvent>(game_controls_system);
        ecs.subscribe<WindowDrawEvent>(draw_game_system);
        ecs.subscribe<CreateTextEvent>(create_text_component);
        ecs.subscribe<CreateTowerEvent>(create_tower_component);
        ecs.subscribe<CreateEnemyEvent>(create_enemy_component);
        ecs.subscribe<CreateSoundEvent>([](Registry &e, const CreateSoundEvent &event)
        {
            create_sound_component(e, event);
            add_sounds(e, event);
        });
        ecs.subscribe<CreateMusicEvent>([](Registry &e, const CreateMusicEvent &event)
        {
            create_music_component(e, event);
            add_musics(e, event);
        });

        ecs.run_event(CreateSoundEvent{});
        ecs.run_event(CreateMusicEvent{});
    }

    /**
     * @brief Close the window
     * @param ecs
     */
    void close_window_system(Registry &ecs, const WindowCloseEvent &)
    {
        //auto &musics = ecs.get_components<MusicComponent>();
//
        //for (auto &music : musics)
        //{
        //    if (music.has_value())
        //    {
        //        StopMusicStream(music.value().musics[tower_defense::GAME_MUSIC]);
        //    }
        //}

        auto &windows = ecs.get_components<Window>();
        for (size_t i = 0; i < windows.size(); ++i)
        {
            if (windows[i])
            {
                auto &win = windows[i].value();
                if (win.isOpen)
                {
                    CloseWindow();
                    win.isOpen = false;
                    break;
                }
            }
        }
    }
} // namespace ecs

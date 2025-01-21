/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** Game_components
*/

#include "ecs/Systems.hpp"

namespace ecs
{
    /**
     * @brief Create tower component
     * @param ecs
     * @param event
     */
    void create_tower_component(Registry &ecs, const CreateTowerEvent &event)
    {
        const auto entity = ecs.spawn_entity();
        ecs.add_component<Tower>(entity,
                                 {event._range, event._damage, event._fire_rate, std::chrono::steady_clock::now(),
                                  event._cost, event._name, event._texture, 0, 0.0f, event._tower_type,
                                  event._position});
    }

    /**
     * @brief Create text component
     * @param ecs
     * @param event
     */
    void create_text_component(Registry &ecs, const CreateTextEvent &event)
    {
        const auto entity = ecs.spawn_entity();
        ecs.add_component<TextComponent>(entity,
                                         {event._text,
                                          std::chrono::steady_clock::now(),
                                          event._duration,
                                          {event._position_x, event._position_y},
                                          {event._velocity_x, event._velocity_y},
                                          event._color,
                                          event._font_size});
    }

    /**
     * @brief Create enemy component
     * @param ecs
     * @param event
     */
    void create_enemy_component(Registry &ecs, const CreateEnemyEvent &event)
    {
        const auto entity = ecs.spawn_entity();
        ecs.add_component<EnemyComponent>(entity,
                                          {event._health, event._speed, event._damage, event._reward, event._texture,
                                           event._position, event._type, 0, 0.0f, std::chrono::steady_clock::now()});
    }

    /**
     * @brief Create sound component
     * @param ecs
     */
    void create_sound_component(Registry &ecs, const CreateSoundEvent &)
    {
        const auto entity = ecs.spawn_entity();
        ecs.add_component<SoundComponent>(entity, {});
    }

    /**
     * @brief Add sounds to the sound component
     * @param ecs
     */
    void add_sounds(Registry &ecs, const CreateSoundEvent &)
    {
        auto &sounds = ecs.get_components<SoundComponent>();

        for (auto &sound : sounds)
        {
            if (sound.has_value())
            {
                sound.value().addSound(tower_defense::ARCHER_SHOT, "tower_defense/assets/sounds/archer_shot.wav");
                sound.value().addSound(tower_defense::BAT_DEATH, "tower_defense/assets/sounds/bat_death.wav");
                sound.value().addSound(tower_defense::BASIC_SLIME_DEATH,
                                       "tower_defense/assets/sounds/basic_slime_death.wav");
                sound.value().addSound(tower_defense::ZOMBIE_DEATH,
                       "tower_defense/assets/sounds/zombie_death.wav");
                sound.value().addSound(tower_defense::TOWER_BUILT, "tower_defense/assets/sounds/tower_built.wav");
                sound.value().addSound(tower_defense::LIFE_LOST, "tower_defense/assets/sounds/life_lost.wav");
            }
        }
    }

    /**
     * @brief Create music component
     * @param ecs
     */
    void create_music_component(Registry &ecs, const CreateMusicEvent &)
    {
        const auto entity = ecs.spawn_entity();
        ecs.add_component<MusicComponent>(entity, {});
    }

    /**
     * @brief Add musics to the music component
     * @param ecs
     */
    void add_musics(Registry &ecs, const CreateMusicEvent &)
    {
        auto &musics = ecs.get_components<MusicComponent>();

        for (auto &music : musics)
        {
            if (music.has_value())
            {
                music.value().addMusic(tower_defense::GAME_MUSIC, "tower_defense/assets/musics/game.wav");
                music.value().play(tower_defense::GAME_MUSIC);
            }
        }
    }
} // namespace ecs

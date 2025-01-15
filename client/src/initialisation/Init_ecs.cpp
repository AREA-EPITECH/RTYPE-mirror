//
// Created by lferraro on 12/24/24.
//

#include "Main.hpp"
#include "game/GameState.hpp"

float map_value(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void init_settings(Registry &ecs)
{
    ecs.register_component<ecs::SettingsComponent>();

    ecs.register_event<ecs::DisplaySettingEvent>();

    ecs.subscribe<ecs::DisplaySettingEvent>(ecs::display_settings_system);
    ecs.register_component<ecs::KeyBindingComponent>();

    auto keys = ecs.spawn_entity();
    ecs.add_component<ecs::KeyBindingComponent>(keys, {});
}

/**
 * Init the ecs
 * @return
 */
Registry init_ecs()
{
    Registry ecs;

    ecs.register_component<ecs::Window>();
    ecs.register_component<ecs::VesselsComponent>();
    ecs.register_component<ecs::ShaderComponent>();
    ecs.register_component<ecs::CameraComponent>();
    ecs.register_component<ecs::ParticleSystemComponent>();
    ecs.register_component<ecs::LightComponent>();
    ecs.register_component<ecs::TextComponent>();
    ecs.register_component<ecs::ButtonComponent>();
    ecs.register_component<ecs::MenuText>();
    ecs.register_component<ecs::BackgroundComponent>();
    ecs.register_component<ecs::DecorElementComponent>();
    ecs.register_component<ecs::TextInputComponent>();
    ecs.register_component<ecs::ShowBoxComponent>();
    ecs.register_component<ecs::ProjectilesComponent>();
    ecs.register_component<ecs::ControllableComponent>();
    ecs.register_component<ecs::EnemyComponent>();
    ecs.register_component<ecs::FocusComponent>();
    ecs.register_component<ecs::ImageComponent>();
    ecs.register_component<ecs::HealthBarComponent>();
    ecs.register_component<game::GameState>();
    ecs.register_component<ecs::SoundComponent>();
    ecs.register_component<ecs::MusicComponent>();

    ecs.register_event<ecs::CreateWindowEvent>();
    ecs.register_event<ecs::WindowOpenEvent>();
    ecs.register_event<ecs::WindowCloseEvent>();
    ecs.register_event<ecs::WindowDrawEvent>();
    ecs.register_event<ecs::InitCameraEvent>();
    ecs.register_event<ecs::InitModelEvent>();
    ecs.register_event<ecs::ControlsEvent>();
    ecs.register_event<ecs::ParticleSystemEvent>();
    ecs.register_event<ecs::InitLightEvent>();
    ecs.register_event<ecs::InitShaderEvent>();
    ecs.register_event<ecs::InitBackgroundEvent>();
    ecs.register_event<ecs::InitDecorElementEvent>();
    ecs.register_event<ecs::ChangeFocusEvent>();
    ecs.register_event<ecs::HealthBarEvent>();

    ecs.register_event<network::ClientEvent>();
    ecs.register_event<struct network::LobbyActionPacket>();
    ecs.register_event<struct network::InputPacket>();

    ecs.subscribe<network::ClientEvent>(
        [](Registry &ecs, const network::ClientEvent &event)
        {
            switch (event.type)
            {
            case network::ClientEventType::DataReceive:
                {
                    if (event.packetType == network::PacketType::LobbySnapshotPacket)
                    {
                        auto received_packet = std::any_cast<struct network::LobbySnapshotPacket>(event.data);
                        auto &gameStateCps = ecs.get_components<game::GameState>();
                        auto gameState = getGameState(ecs);
                        auto user = gameState->get().getUser();
                        std::vector<game::GameState::Player> other_players;
                        gameState->get().setRoomId(received_packet.roomId);
                        for (auto player: received_packet.players) {
                            if (player.id != user.id) {
                                other_players.push_back({
                                    player.id,
                                    0,
                                    MAX_HEALTH,
                                    player.name,
                                    player.shipId,
                                    player.ready,
                                    {0, 0}
                                });
                            }
                        }
                        gameState->get().updateOtherPlayer(other_players);
                        gameState->get().updateGameState(static_cast<game::GameState::LobbyGameState>(received_packet.gameState));
                        if (received_packet.gameState == network::LobbyGameState::Playing) {
                            ecs::change_window(ecs, ecs::WindowType::GAME);
                        }
                    }
                    else if (event.packetType == network::PacketType::SnapshotPacket)
                    {
                        auto received_packet = std::any_cast<struct network::SnapshotPacket>(event.data);
                        auto gameState = getGameState(ecs);
                        if (gameState->get().getGameState() == game::GameState::LobbyGameState::Menu) {
                            return;
                        }
                        if (gameState->get().getGameState() == game::GameState::LobbyGameState::Waiting) {
                            auto user = gameState->get().getUser();
                            user.id = received_packet.entities[0].entityId;
                            gameState->get().updateUser(user);
                            return;
                        }
                        auto &vessels = ecs.get_components<ecs::VesselsComponent>();
                        auto &sounds = ecs.get_components<ecs::SoundComponent>();
                        auto &projectiles = ecs.get_components<ecs::ProjectilesComponent>();
                        auto &lights = ecs.get_components<ecs::LightComponent>();
                        auto &shaders = ecs.get_components<ecs::ShaderComponent>();
                        auto user = gameState->get().getUser();
                        auto players = gameState->get().getOtherPlayer();
                        std::vector<uint32_t> actual_opponents;
                        actual_opponents.push_back(0);
                        actual_opponents.push_back(user.id);
                        vessels[user.entity]->drawable = false;
                        for (auto &player: players) {
                            if (vessels[player.entity].has_value()) {
                                vessels[player.entity]->drawable = false;
                                actual_opponents.push_back(player.id);
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
                        int nb_lights = 0;
                        for (auto &light : lights)
                        {
                            if (light.has_value())
                            {
                                nb_lights += 1;
                            }
                        }

                        for (auto &projectile: projectiles)
                        {
                            if (projectile.has_value())
                            {
                                if (projectile->drawable)
                                {
                                    nb_lights += 1;
                                }
                            }
                        }
                        std::vector<uint32_t> actual_projectiles;
                        std::map<uint32_t, entity_t> enemy_entities = gameState->get().getEnemyEntities();
                        actual_projectiles.push_back(0);
                        for (auto &entity: received_packet.entities) {
                            if (entity.type == network::EntityType::Player) {
                                if (entity.entityId == user.id) {
                                    float posX = map_value(entity.posX, 0, 350, -27.30, 7.79);
                                    float posY = map_value(entity.posY, 0, 332, 16.60, -16.60);
                                    user.position = {posX, posY};
                                    if (vessels[user.entity].has_value()) {
                                        vessels[user.entity]->position = {posX, posY, 0};
                                        vessels[user.entity]->drawable = true;
                                    }
                                    gameState->get().updateUser(user);
                                } else {
                                    float posX = map_value(entity.posX, 0, 350, -27.30, 7.79);
                                    float posY = map_value(entity.posY, 0, 332, 16.60, -16.60);
                                    for (auto &player: players) {
                                        if (player.id == entity.entityId) {
                                            player.position = {posX, posY};
                                            if (vessels[player.entity].has_value()) {
                                                vessels[player.entity]->position = {posX, posY, 0};
                                                vessels[player.entity]->drawable = true;
                                            }
                                            break;
                                        }
                                    }
                                }
                            }
                            if (entity.type == network::EntityType::Rocket) {
                                bool newShoot = true;
                                actual_projectiles.push_back(entity.entityId);
                                for (int i = 0; i < projectiles.size(); i++) {
                                    if (projectiles[i].has_value()) {
                                        if (projectiles[i]->id == entity.entityId) {
                                            newShoot = false;
                                            float posX = map_value(entity.posX, 0, 500, -27.30, 7.79);
                                            float posY = map_value(entity.posY, 0, 332, 16.60, -16.60);
                                            projectiles[i]->position = {posX, posY, 0};
                                            projectiles[i]->drawable = true;
                                            break;
                                        }
                                    }
                                }

                                if (newShoot) {
                                    for (auto &sound : sounds) {
                                        if (sound.has_value()) {
                                            sound.value().play("shoot");
                                        }
                                    }

                                    for (auto &projectile : projectiles)
                                    {
                                        if (projectile.has_value())
                                        {
                                            if (projectile->player && !projectile->drawable)
                                            {
                                                float posX = map_value(entity.posX, 0, 500, -27.30, 7.79);
                                                float posY = map_value(entity.posY, 0, 332, 16.60, -16.60);
                                                ecs::create_player_basic_projectile(
                                                    ecs, entity.entityId, projectile->model,
                                                    {posX, posY, 0}, {35, 0, 0},
                                                    true, projectile->path, Vector3Zero(), nb_lights, shader);
                                            }
                                        }
                                    }
                                }
                            }
                            if (entity.type == network::EntityType::OpponentRocket) {
                                bool newShoot = true;
                                actual_projectiles.push_back(entity.entityId);
                                for (int i = 0; i < projectiles.size(); i++) {
                                    if (projectiles[i].has_value()) {
                                        if (projectiles[i]->id == entity.entityId) {
                                            newShoot = false;
                                            float posX = map_value(entity.posX, 0, 500, -27.30, 7.79);
                                            float posY = map_value(entity.posY, 0, 332, 16.60, -16.60);
                                            projectiles[i]->position = {posX, posY, 0};
                                            projectiles[i]->drawable = true;
                                            break;
                                        }
                                    }
                                }

                                if (newShoot) {
                                    for (auto &sound : sounds) {
                                        if (sound.has_value()) {
                                            sound.value().play("shoot");
                                        }
                                    }

                                    for (auto &projectile : projectiles)
                                    {
                                        if (projectile.has_value())
                                        {
                                            if (!projectile->player && !projectile->drawable)
                                            {
                                                float posX = map_value(entity.posX, 0, 500, -27.30, 7.79);
                                                float posY = map_value(entity.posY, 0, 332, 16.60, -16.60);
                                                ecs::create_player_basic_projectile(
                                                    ecs, entity.entityId, projectile->model,
                                                    {posX, posY, 0}, {-35, 0, 0},
                                                    false, projectile->path, Vector3Zero(), nb_lights, shader);
                                            }
                                        }
                                    }
                                }
                            }
                            if (entity.type == network::EntityType::Opponent) {
                                bool newOpponent = true;
                                actual_opponents.push_back(entity.entityId);
                                for (int i = 0; i < vessels.size(); i++) {
                                    if (vessels[i].has_value()) {
                                        if (vessels[i]->id == entity.entityId && vessels[i]->is_enemy) {
                                            newOpponent = false;
                                            float posX = map_value(entity.posX, 0, 500, -27.30, 7.79);
                                            float posY = map_value(entity.posY, 0, 332, 16.60, -16.60);
                                            vessels[i]->position = {posX, posY, 0};
                                            vessels[i]->drawable = true;
                                            break;
                                        }
                                    }
                                }

                                if (newOpponent) {
                                    float posX = map_value(entity.posX, 0, 500, -27.30, 7.79);
                                    float posY = map_value(entity.posY, 0, 332, 16.60, -16.60);
                                    auto EnemyEntity = ecs.spawn_entity();
                                    ecs::VesselsComponent selectedOpponent = vessels[enemy_entities[entity.shipId]].value();
                                    ecs.add_component<ecs::VesselsComponent>(EnemyEntity, {entity.entityId, selectedOpponent.model, true, selectedOpponent.path, selectedOpponent.name, selectedOpponent.ship_id, true});
                                    vessels[EnemyEntity]->position = {posX, posY, 0};
                                }
                            }
                        }

                        for (int i = 0; i < projectiles.size(); i++) {
                            if (projectiles[i].has_value()) {
                                bool found = false;
                                for (auto &actual_projectile: actual_projectiles) {
                                    if (projectiles[i]->id == actual_projectile) {
                                        found = true;
                                    }
                                }
                                if (!found) {
                                    projectiles[i]->light->UpdateLightValues(shader, false);
                                    ecs.kill_entity(i);
                                }
                            }
                        }

                        for (int i = 0; i < vessels.size(); i++) {
                            if (vessels[i].has_value()) {
                                bool found = false;
                                for (auto &actual_opponent: actual_opponents) {
                                    if (vessels[i]->id == actual_opponent) {
                                        found = true;
                                    }
                                }
                                if (!found) {
                                    ecs.kill_entity(i);
                                }
                            }
                        }
                        gameState->get().updateOtherPlayer(players);
                    }
                    else
                    {
                        spdlog::info("Data received");
                    }
                    break;
                }
            case network::ClientEventType::ServerDisconnect:
                std::cout << "Disconnected from server!" << std::endl;
                break;
            }
        });

    auto focus = ecs.spawn_entity();
    ecs.add_component<ecs::FocusComponent>(focus, {ecs::DEFAULT_FOCUS});
    ecs.subscribe<ecs::ChangeFocusEvent>(ecs::change_focus_system);

    auto gameState = ecs.spawn_entity();
    ecs.add_component<game::GameState>(gameState, {});

    init_settings(ecs);
    init_menu_window(ecs);

    return ecs;
}

//
// Created by lferraro on 12/24/24.
//

#include "Main.hpp"
#include "game/GameState.hpp"

float map_value(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
    ecs.register_component<ecs::ScoreComponent>();

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
                        gameState->get().updateGameState(received_packet.gameState);
                        if (received_packet.gameState == network::LobbyGameState::Playing) {
                            ecs::change_window(ecs, ecs::WindowType::GAME);
                        }
                    }
                    else if (event.packetType == network::PacketType::SnapshotPacket)
                    {
                        auto received_packet = std::any_cast<struct network::SnapshotPacket>(event.data);
                        auto gameState = getGameState(ecs);
                        if (gameState->get().getGameState() == network::LobbyGameState::Waiting) {
                            auto user = gameState->get().getUser();
                            user.id = received_packet.entities[0].entityId;
                            gameState->get().updateUser(user);
                            return;
                        }
                        auto &cameras = ecs.get_components<ecs::CameraComponent>();
                        auto &vessels = ecs.get_components<ecs::VesselsComponent>();
                        auto &sounds = ecs.get_components<ecs::SoundComponent>();
                        auto &projectiles = ecs.get_components<ecs::ProjectilesComponent>();
                        auto &lights = ecs.get_components<ecs::LightComponent>();
                        auto &shaders = ecs.get_components<ecs::ShaderComponent>();
                        auto user = gameState->get().getUser();
                        auto players = gameState->get().getOtherPlayer();
                        for (auto &player: players) {
                            if (vessels[player.entity].has_value()) {
                                vessels[player.entity]->drawable = false;
                            }
                        }
                        Shader shader = {};
                        for (auto &shader_i : shaders)
                        {
                            if (shader_i.has_value())
                            {
                                shader = shader_i->shader;
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
                        //std::vector<uint32_t> actual_projectiles;
                        for (auto &entity: received_packet.entities) {
                            if (entity.type == network::EntityType::Player) {
                                if (entity.entityId == user.id) {
                                    float posX = map_value(entity.posX, 0, 350, -27.30, 7.79);
                                    float posY = map_value(entity.posY, 0, 332, 16.60, -16.60);
                                    user.position = {posX, posY};
                                    if (vessels[user.entity].has_value()) {
                                        vessels[user.entity]->position = {posX, posY, 0};
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
                                spdlog::info("Projectile with id: {} pos: {} {}", entity.entityId, entity.posX, entity.posY);
                                //actual_projectiles.push_back(entity.entityId);

                                for (auto &projectile : projectiles) {
                                    if (projectile.has_value()) {
                                        if (projectile->id == entity.entityId) {
                                            spdlog::info("Existing projectile with id: {}", entity.entityId);
                                            newShoot = false;
                                            float posX = map_value(entity.posX, 0, 500, -27.30, 7.79);
                                            float posY = map_value(entity.posY, 0, 332, 16.60, -16.60);
                                            projectile->position = {posX, posY, 0};
                                            projectile->drawable = true;
                                            break;
                                        }
                                    }
                                }

                                if (newShoot) {
                                    spdlog::info("New projectile with id: {}", entity.entityId);
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
                                                    {posX, posY, 0}, {0.5, 0, 0},
                                                    true, projectile->path, Vector3Zero(), nb_lights, shader);
                                            }
                                        }
                                    }
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

    init_menu_window(ecs);

    return ecs;
}

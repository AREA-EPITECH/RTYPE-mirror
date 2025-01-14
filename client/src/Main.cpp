/*
** EPITECH PROJECT, 2024
** client
** File description:
** Main
*/

#include "Main.hpp"

#include "queue/MessageQueue.hpp"
#include "game/GameState.hpp"

#include <variant>

std::atomic<bool> shutdown_requested(false);

/**
 * @brief Empty ecs from all entities and components
 * @param ecs
 */
void empty_ecs(Registry &ecs)
{
    kill_entities_with_component<ecs::Window>(ecs);

    auto &vessels = ecs.get_components<ecs::VesselsComponent>();
    for (std::size_t i = 0; i < vessels.size(); ++i)
    {
        if (vessels[i].has_value())
        {
            UnloadModel(vessels[i]->model);
            ecs.kill_entity(i);
        }
    }

    auto &shaders = ecs.get_components<ecs::ShaderComponent>();
    for (std::size_t i = 0; i < shaders.size(); ++i)
    {
        if (shaders[i].has_value())
        {
            UnloadShader(*shaders[i]->shader);
            ecs.kill_entity(i);
        }
    }

    auto &health_bars = ecs.get_components<ecs::HealthBarComponent>();
    for (std::size_t i = 0; i < health_bars.size(); ++i)
    {
        if (health_bars[i].has_value())
        {
            for (const auto &texture : health_bars[i]->textures)
            {
                UnloadTexture(texture);
            }
            ecs.kill_entity(i);
        }
    }

    kill_entities_with_component<ecs::CameraComponent>(ecs);
    kill_entities_with_component<ecs::ParticleSystemComponent>(ecs);
    kill_entities_with_component<ecs::LightComponent>(ecs);
    kill_entities_with_component<ecs::TextComponent>(ecs);
    kill_entities_with_component<ecs::ButtonComponent>(ecs);
    kill_entities_with_component<ecs::MenuText>(ecs);

    auto &backgrounds = ecs.get_components<ecs::BackgroundComponent>();
    for (std::size_t i = 0; i < backgrounds.size(); ++i)
    {
        if (backgrounds[i].has_value())
        {
            UnloadTexture(backgrounds[i]->texture);
            ecs.kill_entity(i);
        }
    }

    auto &decors = ecs.get_components<ecs::DecorElementComponent>();
    for (std::size_t i = 0; i < decors.size(); ++i)
    {
        if (decors[i].has_value())
        {
            UnloadTexture(decors[i]->texture);
            ecs.kill_entity(i);
        }
    }
    kill_entities_with_component<ecs::TextInputComponent>(ecs);
    kill_entities_with_component<ecs::ShowBoxComponent>(ecs);

    auto &projectiles = ecs.get_components<ecs::ProjectilesComponent>();
    for (std::size_t i = 0; i < projectiles.size(); ++i)
    {
        if (projectiles[i].has_value())
        {
            UnloadModel(projectiles[i]->model);
            ecs.kill_entity(i);
        }
    }

    auto &images = ecs.get_components<ecs::ImageComponent>();
    for (std::size_t i = 0; i < images.size(); ++i) {
        if (images[i].has_value()) {
            UnloadTexture(images[i]->texture);
            ecs.kill_entity(i);
        }
    }

    kill_entities_with_component<ecs::ControllableComponent>(ecs);
    kill_entities_with_component<ecs::EnemyComponent>(ecs);
    kill_entities_with_component<ecs::FocusComponent>(ecs);
    kill_entities_with_component<ecs::KeyBindingComponent>(ecs);
}

/**
 * @brief Handle events sent by server
 * @param ecs
 * @param host
 * @param port
 */
void handle_network_event(
    const std::string &host, int port, client::MessageQueue<network::ClientEvent> &receiveQueue,
    client::MessageQueue<std::variant<struct network::LobbyActionPacket, struct network::InputPacket>> &sendQueue)
{
    network::NetworkClient network_client;

    if (!network_client.connectToServer(host, port))
    {
        spdlog::error("Failed to connect to server!");
        return ;
    }

    network::ClientEvent event;
    while (!shutdown_requested.load())
    {
        while (network_client.pollEvent(event))
        {
            receiveQueue.push(event);
        }
        while (!sendQueue.empty())
        {
            auto message = sendQueue.pop();

            std::visit(
                [&network_client](auto &&arg)
                {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, struct network::LobbyActionPacket>)
                    {
                        network_client.sendLobbyPacket(arg);
                    }
                    else if constexpr (std::is_same_v<T, struct network::InputPacket>)
                    {
                        network_client.sendInputPacket(arg);
                    }
                },
                message);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


/**
 * @brief Main function, creates network thread and start game loop
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "USAGE: ./r-type_client <HOST> <PORT>" << std::endl;
        return 84;
    }
    std::string host = argv[1];
    int port = static_cast<int>(strtol(argv[2], nullptr, 10));

    client::MessageQueue<network::ClientEvent> receiveQueue;
    client::MessageQueue<std::variant<struct network::LobbyActionPacket, struct network::InputPacket>> sendQueue;

    std::cout << "Game will run on " << host << " using port " << port << std::endl;

    Registry ecs = init_ecs();
    std::thread thread_network(handle_network_event, std::ref(host), port, std::ref(receiveQueue), std::ref(sendQueue));

    auto windowEntity = ecs.spawn_entity();
    ecs.add_component<ecs::Window>(windowEntity, {1920, 1080, "ECS Raylib - Multi Events", false});

    ecs.subscribe<ecs::CreateWindowEvent>(ecs::init_window_system);

    ecs.subscribe<ecs::InitLightEvent>(ecs::create_light_system);

    ecs.run_event(ecs::CreateWindowEvent{});
    ecs.run_event(ecs::WindowOpenEvent{});

    ecs.subscribe<struct network::LobbyActionPacket>(
        [&sendQueue](Registry &ecs, const struct network::LobbyActionPacket &packet) {
            sendQueue.push(packet);
        });
    ecs.subscribe<struct network::InputPacket>([&sendQueue](Registry &ecs, const struct network::InputPacket &packet)
                                               { sendQueue.push(packet); });

    while (!WindowShouldClose())
    {
        while (!receiveQueue.empty())
        {
            ecs.run_event(receiveQueue.pop());
        }
        ecs.run_event(ecs::WindowDrawEvent{});
    }
    shutdown_requested.store(true);
    thread_network.join();
    empty_ecs(ecs);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

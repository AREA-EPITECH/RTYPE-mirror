/*
** EPITECH PROJECT, 2024
** client
** File description:
** Main
*/

#include "Main.hpp"

#include "queue/MessageQueue.hpp"

#include <variant>

std::atomic<bool> shutdown_requested(false);

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
                        spdlog::info("Received lobby snapshot packet: {}", received_packet.header.packetId);
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

    init_menu_window(ecs);

    return ecs;
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
        std::cerr << "Failed to connect to server!" << std::endl;
        exit(84);
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
    //std::thread thread_network(handle_network_event, std::ref(host), port, std::ref(receiveQueue), std::ref(sendQueue));

    auto windowEntity = ecs.spawn_entity();
    ecs.add_component<ecs::Window>(windowEntity, {1920, 1080, "ECS Raylib - Multi Events", false});

    ecs.subscribe<ecs::CreateWindowEvent>(ecs::init_window_system);

    ecs.subscribe<ecs::InitLightEvent>(ecs::create_light_system);

    ecs.run_event(ecs::CreateWindowEvent{});
    ecs.run_event(ecs::WindowOpenEvent{});

    ecs.subscribe<struct network::LobbyActionPacket>(
        [&sendQueue](Registry &ecs, const struct network::LobbyActionPacket &packet) { sendQueue.push(packet); });
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
    //thread_network.join();

    return 0;
}

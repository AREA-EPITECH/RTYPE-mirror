/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Client.cpp
*/

#include <spdlog/spdlog.h>

#include "network/Client.hpp"
#include "network/NetworkWrapper.hpp"

/**
 * @brief Constructs a NetworkClient instance.
 *
 * Initializes the ENet library and prepares the client for use.
 */
network::NetworkClient::NetworkClient() : client(nullptr, enet_host_destroy), serverPeer(nullptr)
{
    NetworkWrapper::initialize();
}

/**
 * @brief Destroys the NetworkClient instance.
 *
 * Disconnects from the server if connected and shuts down the ENet library.
 */
network::NetworkClient::~NetworkClient()
{
    disconnect();
    NetworkWrapper::shutdown();
}

/**
 * @brief Connects to a server.
 *
 * Attempts to establish a connection to the specified server address and port.
 *
 * @param host The server's hostname or IP address.
 * @param port The server's port number.
 * @return True if the connection is successful, false otherwise.
 */
bool network::NetworkClient::connectToServer(const std::string &host, uint16_t port)
{
    ENetAddress address;
    enet_address_set_host(&address, host.c_str());
    address.port = port;

    client.reset(enet_host_create(nullptr, 1, 2, 0, 0));
    if (!client)
    {
        spdlog::error("Failed to create client!");
        return false;
    }

    serverPeer = std::make_unique<PeerWrapper>(enet_host_connect(client.get(), &address, 2, 0));
    if (!serverPeer)
    {
        spdlog::error("Failed to connect to server!");
        return false;
    }

    ENetEvent event;
    if (enet_host_service(client.get(), &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
    {
        return true;
    }

    return false;
}

/**
 * @brief Disconnects from the server.
 *
 * Closes the connection to the server and releases associated resources.
 */
void network::NetworkClient::disconnect()
{
    if (client)
    {
        client.reset();
        serverPeer = nullptr;
    }
}

/**
 * @brief Sends a raw packet to the server.
 *
 * @param data The binary data to send.
 * @return True if the packet is sent successfully, false otherwise.
 */
bool network::NetworkClient::sendPacket(const std::vector<uint8_t> &data)
{
    if (serverPeer)
    {
        ENetPacket *packet = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);
        return enet_peer_send(serverPeer->getPeer().get(), 0, packet) >= 0;
    }
    return false;
}

/**
 * @brief Sends an input packet to the server.
 *
 * @param packet The InputPacket containing user input data.
 * @return True if the packet is sent successfully, false otherwise.
 */
bool network::NetworkClient::sendInputPacket(const struct InputPacket &packet)
{
    struct InputPacket newPacket;

    lastPacketId++;
    newPacket.header.packetId = lastPacketId;

    auto now = std::chrono::steady_clock::now();
    newPacket.header.timestamp = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());

    newPacket.header.type = PacketType::InputPacket;

    newPacket.fire = packet.fire;
    newPacket.move = packet.move;

    const std::vector<uint8_t> binary = Packet::serializeInputPacket(newPacket);
    return sendPacket(binary);
}

/**
 * @brief Sends a lobby action packet to the server.
 *
 * @param packet The LobbyActionPacket containing lobby action details.
 * @return True if the packet is sent successfully, false otherwise.
 */
bool network::NetworkClient::sendLobbyPacket(const struct LobbyActionPacket &packet)
{
    struct LobbyActionPacket newPacket;

    lastPacketId++;
    newPacket.header.packetId = lastPacketId;

    auto now = std::chrono::steady_clock::now();
    newPacket.header.timestamp = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());

    newPacket.header.type = PacketType::LobbyActionPacket;

    newPacket.actionType = packet.actionType;
    newPacket.roomId = packet.roomId;
    newPacket.name = packet.name;
    newPacket.shipId = packet.shipId;
    newPacket.readyState = packet.readyState;

    const std::vector<uint8_t> binary = Packet::serializeLobbyActionPacket(newPacket);
    return sendPacket(binary);
}

/**
 * @brief Polls for incoming events from the server.
 *
 * Checks for incoming events such as data reception or server disconnection.
 * Populates the provided ClientEvent with event details if an event is received.
 *
 * @param event The ClientEvent structure to populate with event details.
 * @return True if an event is successfully polled, false otherwise.
 */
bool network::NetworkClient::pollEvent(ClientEvent &event)
{
    if (!client)
        return false;

    ENetEvent enetEvent;
    std::vector<uint8_t> rawPacket;
    if (enet_host_service(client.get(), &enetEvent, 0) > 0)
    {
        switch (enetEvent.type)
        {
        case ENET_EVENT_TYPE_RECEIVE:
            {
                event.type = ClientEventType::DataReceive;
                rawPacket.assign(enetEvent.packet->data, enetEvent.packet->data + enetEvent.packet->dataLength);
                try
                {
                    auto [packetType, packet] = network::Packet::deserializePacket(rawPacket);
                    event.packetType = packetType;
                    event.data = packet;
                }
                catch (const std::exception &e)
                {
                    spdlog::error("Failed to deserialize packet: {}", e.what());
                }
                if (enetEvent.packet) {
                    enet_packet_destroy(enetEvent.packet);
                }
                break;
            }

        case ENET_EVENT_TYPE_DISCONNECT:
            {
                event.type = ClientEventType::ServerDisconnect;
                event.packetType = PacketType::NoPacket;
                event.data = nullptr;
                break;
            }

        default:
            return false;
        }
        return true;
    }
    return false;
}

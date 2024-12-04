/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Client.cpp
*/

#include <spdlog/spdlog.h>

#include "network/Client.hpp"
#include "network/NetworkWrapper.hpp"

// Client Constructor
network::NetworkClient::NetworkClient() : client(nullptr, enet_host_destroy), serverPeer(nullptr)
{
    NetworkWrapper::initialize();
}

network::NetworkClient::~NetworkClient()
{
    disconnect();
    NetworkWrapper::shutdown();
}

// Connect to the server
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

// Disconnect from the server
void network::NetworkClient::disconnect()
{
    if (client)
    {
        client.reset();
        serverPeer = nullptr;
    }
}

// Send a packet to the server
bool network::NetworkClient::sendPacket(const std::vector<uint8_t> &data)
{
    if (serverPeer)
    {
        ENetPacket *packet = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);
        return enet_peer_send(serverPeer->getPeer().get(), 0, packet) >= 0;
    }
    return false;
}

bool network::NetworkClient::sendInputPacket(const InputPacket &packet)
{
    InputPacket newPacket;

    lastPacketId++;
    newPacket.header.packetId = lastPacketId;

    auto now = std::chrono::steady_clock::now();
    newPacket.header.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    newPacket.header.type = PacketType::InputPacket;

    newPacket.fire = packet.fire;
    newPacket.move = packet.move;

    const std::vector<uint8_t> binary = Packet::serializeInputPacket(newPacket);
    return sendPacket(binary);
}

bool network::NetworkClient::sendLobbyPacket(const LobbyActionPacket &packet)
{
    LobbyActionPacket newPacket;

    lastPacketId++;
    newPacket.header.packetId = lastPacketId;

    auto now = std::chrono::steady_clock::now();
    newPacket.header.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    newPacket.header.type = PacketType::LobbyActionPacket;

    newPacket.actionType = packet.actionType;
    newPacket.roomId = packet.roomId;
    newPacket.name = packet.name;
    newPacket.shipId = packet.shipId;
    newPacket.readyState = packet.readyState;

    const std::vector<uint8_t> binary = Packet::serializeLobbyActionPacket(newPacket);
    return sendPacket(binary);
}

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
                    event.packetType = PacketType::RawPacket;
                    event.data = rawPacket;
                    spdlog::error("Failed to deserialize packet: {}", e.what());
                }
                enet_packet_destroy(enetEvent.packet);
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

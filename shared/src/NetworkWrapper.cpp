/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** NetworkWrapper.cpp
*/

#include "NetworkWrapper.hpp"
#include "Packet.hpp"
#include "spdlog/spdlog.h"

bool network::NetworkWrapper::initialized = false;

// Initialize ENet
bool network::NetworkWrapper::initialize()
{
    if (initialized)
        return true;

    if (enet_initialize() != 0)
    {
        spdlog::error("Network initialization failed!");
        return false;
    }

    initialized = true;
    return true;
}

// Shutdown ENet
void network::NetworkWrapper::shutdown()
{
    if (initialized)
    {
        enet_deinitialize();
        initialized = false;
    }
}

// Server Constructor
network::NetworkServer::NetworkServer() : host(nullptr) { NetworkWrapper::initialize(); }

network::NetworkServer::~NetworkServer()
{
    stop();
    NetworkWrapper::shutdown();
}

// Start the server
bool network::NetworkServer::start(uint16_t port)
{
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;

    host = enet_host_create(&address, 32, 2, 0, 0);
    if (!host)
    {
        spdlog::error("Failed to create server!");
        return false;
    }
    return true;
}

// Stop the server
void network::NetworkServer::stop()
{
    if (host)
    {
        enet_host_destroy(host);
        host = nullptr;
    }
}

// Send a packet to a specific peer
bool network::NetworkServer::sendPacket(const std::vector<uint8_t> &data, ENetPeer *peer)
{
    ENetPacket *packet = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);
    return enet_peer_send(peer, 0, packet) >= 0;
}

bool network::NetworkServer::sendSnapshotPacket(const SnapshotPacket& packet, ENetPeer* peer)
{
    SnapshotPacket newPacket;

    lastPacketId++;
    newPacket.header.packetId = lastPacketId;

    auto now = std::chrono::steady_clock::now();
    newPacket.header.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    newPacket.numEntities = packet.entities.size();
    newPacket.entities = packet.entities;

    const std::vector<uint8_t> binary = Packet::serializeSnapshotPacket(packet);
    return sendPacket(binary, peer);
}

bool network::NetworkServer::pollEvent(network::ServerEvent &event)
{
    if (!host)
    {
        return false;
    }

    ENetEvent enetEvent;
    if (enet_host_service(host, &enetEvent, 0) > 0)
    {
        switch (enetEvent.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            event.type = ServerEventType::ClientConnect;
            event.peer = enetEvent.peer;
            break;

        case ENET_EVENT_TYPE_RECEIVE:
            event.type = ServerEventType::DataReceive;
            event.peer = enetEvent.peer;
            event.data.assign(enetEvent.packet->data, enetEvent.packet->data + enetEvent.packet->dataLength);
            enet_packet_destroy(enetEvent.packet);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            event.type = ServerEventType::ClientDisconnect;
            event.peer = enetEvent.peer;
            break;

        default:
            return false;
        }
        return true;
    }
    return false;
}

// Client Constructor
network::NetworkClient::NetworkClient() : client(nullptr), serverPeer(nullptr) { NetworkWrapper::initialize(); }

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

    client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!client)
    {
        spdlog::error("Failed to create client!");
        return false;
    }

    serverPeer = enet_host_connect(client, &address, 2, 0);
    if (!serverPeer)
    {
        spdlog::error("Failed to connect to server!");
        return false;
    }

    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
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
        enet_host_destroy(client);
        client = nullptr;
        serverPeer = nullptr;
    }
}

// Send a packet to the server
bool network::NetworkClient::sendPacket(const std::vector<uint8_t> &data)
{
    if (serverPeer)
    {
        ENetPacket *packet = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);
        return enet_peer_send(serverPeer, 0, packet) >= 0;
    }
    return false;
}

bool network::NetworkClient::sendInputPacket(const InputPacket& packet)
{
    InputPacket newPacket;

    lastPacketId++;
    newPacket.header.packetId = lastPacketId;

    auto now = std::chrono::steady_clock::now();
    newPacket.header.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    newPacket.fire = packet.fire;
    newPacket.move = packet.move;

    const std::vector<uint8_t> binary = Packet::serializeInputPacket(newPacket);
    return sendPacket(binary);
}

bool network::NetworkClient::pollEvent(ClientEvent &event)
{
    if (!client)
        return false;

    ENetEvent enetEvent;
    std::vector<uint8_t> rawPacket;
    if (enet_host_service(client, &enetEvent, 0) > 0)
    {
        switch (enetEvent.type)
        {
        case ENET_EVENT_TYPE_RECEIVE:
            event.type = ClientEventType::DataReceive;
            rawPacket.assign(enetEvent.packet->data, enetEvent.packet->data + enetEvent.packet->dataLength);
            event.data = Packet::deserializeSnapshotPacket(rawPacket);
            enet_packet_destroy(enetEvent.packet);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            event.type = ClientEventType::ServerDisconnect;
            break;

        default:
            return false;
        }
        return true;
    }
    return false;
}

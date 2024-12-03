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
network::NetworkServer::NetworkServer() : host(nullptr, enet_host_destroy) { NetworkWrapper::initialize(); }

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

    host.reset(enet_host_create(&address, 32, 2, 0, 0));
    if (!host)
    {
        spdlog::error("Failed to create server!");
        return false;
    }
    return true;
}

// Stop the server
void network::NetworkServer::stop() {
    host.reset();
    peerMap.clear();
}

// Send a packet to a specific peer
bool network::NetworkServer::sendPacket(const std::vector<uint8_t> &data, std::shared_ptr<PeerWrapper> peer)
{
    ENetPacket *packet = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);
    return enet_peer_send(peer->getPeer().get(), 0, packet) >= 0;
}

bool network::NetworkServer::sendSnapshotPacket(const SnapshotPacket &packet, std::shared_ptr<PeerWrapper> peer)
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
    if (enet_host_service(host.get(), &enetEvent, 0) > 0)
    {
        switch (enetEvent.type)
        {
        case ENET_EVENT_TYPE_CONNECT: {
            auto wrapper = std::make_shared<PeerWrapper>(enetEvent.peer);
            peerMap[enetEvent.peer] = wrapper;

            event.type = ServerEventType::ClientConnect;
            event.peer = wrapper;
            break;
        }

        case ENET_EVENT_TYPE_RECEIVE: {
            auto it = peerMap.find(enetEvent.peer);
            if (it != peerMap.end()) {
                event.type = ServerEventType::DataReceive;
                event.peer = it->second;
                event.data.assign(enetEvent.packet->data, enetEvent.packet->data + enetEvent.packet->dataLength);
            }
            enet_packet_destroy(enetEvent.packet);
            break;
        }

        case ENET_EVENT_TYPE_DISCONNECT: {
            auto it = peerMap.find(enetEvent.peer);
            if (it != peerMap.end()) {
                event.type = ServerEventType::ClientDisconnect;
                event.peer = it->second;
                peerMap.erase(it);
            }
            break;
        }

        default:
            return false;
        }
        return true;
    }
    return false;
}

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
    if (enet_host_service(client.get(), &enetEvent, 0) > 0)
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

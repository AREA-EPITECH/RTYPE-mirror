/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Server.cpp
*/

#include <spdlog/spdlog.h>

#include "network/NetworkWrapper.hpp"
#include "network/Server.hpp"

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
void network::NetworkServer::stop()
{
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

    newPacket.header.type = PacketType::SnapshotPacket;

    newPacket.numEntities = packet.entities.size();
    newPacket.entities = packet.entities;

    const std::vector<uint8_t> binary = Packet::serializeSnapshotPacket(packet);
    return sendPacket(binary, peer);
}

bool network::NetworkServer::sendLobbyPacket(const LobbySnapshotPacket &packet, std::shared_ptr<PeerWrapper> peer)
{
    LobbySnapshotPacket newPacket;

    lastPacketId++;
    newPacket.header.packetId = lastPacketId;

    auto now = std::chrono::steady_clock::now();
    newPacket.header.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    newPacket.header.type = PacketType::LobbySnapshotPacket;

    newPacket.roomId = packet.roomId;
    newPacket.players = packet.players;
    newPacket.gameState = packet.gameState;

    const std::vector<uint8_t> binary = Packet::serializeLobbySnapshotPacket(packet);
    return sendPacket(binary, peer);
}

bool network::NetworkServer::pollEvent(network::ServerEvent &event)
{
    if (!host)
    {
        return false;
    }

    ENetEvent enetEvent;
    std::vector<uint8_t> rawPacket;
    if (enet_host_service(host.get(), &enetEvent, 0) > 0)
    {
        switch (enetEvent.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            {
                auto wrapper = std::make_shared<PeerWrapper>(enetEvent.peer);
                peerMap[enetEvent.peer] = wrapper;

                event.type = ServerEventType::ClientConnect;
                event.peer = wrapper;
                event.packetType = PacketType::NoPacket;
                event.data = nullptr;
                break;
            }

        case ENET_EVENT_TYPE_RECEIVE:
            {
                auto it = peerMap.find(enetEvent.peer);
                if (it != peerMap.end())
                {
                    event.type = ServerEventType::DataReceive;
                    event.peer = it->second;
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
                }
                enet_packet_destroy(enetEvent.packet);
                break;
            }

        case ENET_EVENT_TYPE_DISCONNECT:
            {
                auto it = peerMap.find(enetEvent.peer);
                if (it != peerMap.end())
                {
                    event.type = ServerEventType::ClientDisconnect;
                    event.peer = it->second;
                    event.packetType = PacketType::NoPacket;
                    event.data = nullptr;
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

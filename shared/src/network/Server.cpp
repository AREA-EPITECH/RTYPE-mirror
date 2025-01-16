/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** Server.cpp
*/

#include <spdlog/spdlog.h>
#include <chrono>

#include "network/NetworkWrapper.hpp"
#include "network/Server.hpp"

/**
 * @brief Constructs a new NetworkServer instance.
 *
 * Initializes the internal ENet host and other required resources.
 */
network::NetworkServer::NetworkServer() : host(nullptr, enet_host_destroy) { NetworkWrapper::initialize(); }

/**
 * @brief Destroys the NetworkServer instance.
 *
 * Cleans up the ENet host and other allocated resources.
 */
network::NetworkServer::~NetworkServer()
{
    stop();
    NetworkWrapper::shutdown();
}

/**
 * @brief Starts the server and binds it to a specific port.
 *
 * @param port The port number to bind the server to.
 * @return True if the server starts successfully, false otherwise.
 */
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

/**
 * @brief Stops the server and closes all active connections.
 */
void network::NetworkServer::stop()
{
    host.reset();
    for (auto &peer: peerMap) {
        peer.second.reset();
    }
    peerMap.clear();
}

/**
 * @brief Sends a raw packet of data to a specific peer.
 *
 * @param data The data to send.
 * @param peer A shared pointer to the target PeerWrapper.
 * @return True if the packet is sent successfully, false otherwise.
 */
bool network::NetworkServer::sendPacket(const std::vector<uint8_t> &data, std::shared_ptr<PeerWrapper> peer)
{
    ENetPacket *packet = enet_packet_create(data.data(), data.size(), ENET_PACKET_FLAG_RELIABLE);
    return enet_peer_send(peer->getPeer().get(), 0, packet) >= 0;
}

/**
 * @brief Sends a snapshot packet to a specific peer.
 *
 * @param packet The snapshot packet to send.
 * @param peer A shared pointer to the target PeerWrapper.
 * @return True if the packet is sent successfully, false otherwise.
 */
bool network::NetworkServer::sendSnapshotPacket(const struct SnapshotPacket &packet, std::shared_ptr<PeerWrapper> peer)
{
    struct SnapshotPacket newPacket;

    lastPacketId++;
    newPacket.header.packetId = lastPacketId;

    auto now = std::chrono::steady_clock::now();
    newPacket.header.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    newPacket.header.type = PacketType::SnapshotPacket;

    newPacket.numEntities = packet.entities.size();
    newPacket.entities = packet.entities;

    newPacket.maxScore = packet.maxScore;
    newPacket.level = packet.level;

    const std::vector<uint8_t> binary = Packet::serializeSnapshotPacket(newPacket);
    return sendPacket(binary, peer);
}

/**
 * @brief Sends a lobby snapshot packet to a specific peer.
 *
 * @param packet The lobby snapshot packet to send.
 * @param peer A shared pointer to the target PeerWrapper.
 * @return True if the packet is sent successfully, false otherwise.
 */
bool network::NetworkServer::sendLobbyPacket(const struct LobbySnapshotPacket &packet, std::shared_ptr<PeerWrapper> peer)
{
    struct LobbySnapshotPacket newPacket;

    lastPacketId++;
    newPacket.header.packetId = lastPacketId;

    auto now = std::chrono::steady_clock::now();
    newPacket.header.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    newPacket.header.type = PacketType::LobbySnapshotPacket;

    newPacket.roomId = packet.roomId;
    newPacket.players = packet.players;
    newPacket.gameState = packet.gameState;

    const std::vector<uint8_t> binary = Packet::serializeLobbySnapshotPacket(newPacket);
    return sendPacket(binary, peer);
}

/**
 * @brief Sends a lobby snapshot packet to a specific peer.
 *
 * @param packet The lobby snapshot packet to send.
 * @param peer A shared pointer to the target PeerWrapper.
 * @return True if the packet is sent successfully, false otherwise.
 */
bool network::NetworkServer::sendErrorPacket(const struct ErrorPacket &packet, std::shared_ptr<PeerWrapper> peer)
{
    struct ErrorPacket newPacket;

    lastPacketId++;
    newPacket.header.packetId = lastPacketId;

    auto now = std::chrono::steady_clock::now();
    newPacket.header.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    newPacket.header.type = PacketType::ErrorPacket;

    newPacket.type = packet.type;
    newPacket.message = packet.message;

    const std::vector<uint8_t> binary = Packet::serializeErrorPacket(newPacket);
    return sendPacket(binary, peer);
}

/**
 * @brief Polls the server for new network events.
 *
 * Processes connection, disconnection, and data reception events,
 * populating the ServerEvent object with the event details.
 *
 * @param event A reference to a ServerEvent object to store the next event.
 * @return True if an event is successfully polled, false otherwise.
 */
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
                    it->second.reset();
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

/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Server
*/

#include "Server.hpp"
#include <ThreadPool.hpp>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <enet/enet.h>
#include <memory>
#include <network/Server.hpp>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include "ClientData.hpp"
#include "Game.hpp"
#include "Lobby.hpp"
#include "Room.hpp"
#include "Utils.hpp"
#include "spdlog/spdlog.h"

namespace server
{
    /**
     * Server constructor
     * @param argv - command line argument corresponding to port number
     */
    Server::Server(char *argv[]) : _thread_pool(std::thread::hardware_concurrency())
    {
        if (argv[1] == nullptr)
        {
            throw ServerException("Port argument is missing");
        }
        try
        {
            const std::string port_string(argv[1]);
            this->_port = std::stoi(port_string);
        }
        catch ([[maybe_unused]] std::invalid_argument const &ex)
        {
            throw ServerException("Wrong port argument");
        }
    }

    /**
     * Init server with network ENet library
     * Port listening
     */
    void Server::initServer()
    {
        if (!this->_server.start(this->_port))
        {
            throw ServerException("Failed to start server");
        }
        spdlog::info("Server running on port {}...", this->_port);
    }

    /**
     * Polling events that catch different type of Paquets received by ENet library
     * ClientConnect: a client connects to server
     * ClientDisconnect: a client disconnects from server
     * DataReceive: a client sends data to server
     */
    void Server::pollEvent()
    {
        network::ServerEvent event;
        while (this->_server.pollEvent(event))
        {
            switch (event.type)
            {
            case network::ServerEventType::ClientConnect:
                {
                    const uint32_t unique_id = utils::Utils::hash_pointer(event.peer);
                    ClientData data(unique_id);
                    event.peer->setData<ClientData>(std::move(data));
                    spdlog::info("Client connected: {}",event.peer->getData<ClientData>().getId());
                    struct network::SnapshotPacket snapshot;
                    snapshot.numEntities = 1;
                    snapshot.entities.push_back({unique_id, network::EntityType::Player, 0, 0, 0});
                    _server.sendSnapshotPacket(snapshot, event.peer);
                    break;
                }
            case network::ServerEventType::ClientDisconnect:
                {
                    spdlog::info("Client disconnected: {}", event.peer->getData<ClientData>().getId());
                    this->clientDisconnect(event.peer);
                    break;
                }
            case network::ServerEventType::DataReceive:
                /*this->_thread_pool.enqueueTask(
                    [server = this, &peer = event.peer, data = std::move(event.data), dataType = event.packetType]
                    { handleClientData(*server, peer, data, dataType); });*/
                handleClientData(*this, event.peer, std::move(event.data), event.packetType);
                break;
            }
        }
    }

    /**
     * Disconnect a client from its Room and call sendUpdateRoom method
     * @param peer - client
     */
    void Server::clientDisconnect(std::shared_ptr<network::PeerWrapper> &peer)
    {
        const auto &data = peer->getData<ClientData>();
        auto client_id = peer->getData<ClientData>().getId();
        for (auto &room: this->getAllRooms()) {
            if (room->getClient(client_id)) {
                room->removeClient(client_id);
                spdlog::info("Client {} left room {}", client_id, room->getId());
                if (room->getState() == network::Starting) {
                    room->setState(network::Waiting);
                }
                room->sendUpdateRoom(*this);
                return;
            }
        }
    }

    /**
     * Moves a room from the waiting rooms to the playing rooms if all clients in the room are ready.
     * @param room_id The ID of the room to be moved to the playing rooms.
     */
    void Server::changeRoomToPlaying(uint32_t room_id)
    {
        auto it = std::find_if(this->_waiting_rooms.begin(), this->_waiting_rooms.end(),
            [room_id](const std::shared_ptr<Room> &room) {
                return room->getId() == room_id;
            });

        if (it != this->_waiting_rooms.end()) {
            this->_playing_rooms.push_back(*it);
            this->_waiting_rooms.erase(it);
        }
    }

    /**
     * Moves a room from the playing rooms to the waiting rooms if all clients in the room are ready.
     * @param room_id The ID of the room to be moved to the waiting rooms.
     */
    void Server::changeRoomToWaiting(uint32_t room_id)
    {
        auto it = std::find_if(this->_playing_rooms.begin(), this->_playing_rooms.end(),
            [room_id](const std::shared_ptr<Room> &room) {
                return room->getId() == room_id;
            });

        if (it != this->_playing_rooms.end()) {
            this->_waiting_rooms.push_back(*it);
            this->_playing_rooms.erase(it);
        }
    }

    /**
     * Client creates a room
     * @param client
     */
    void Server::createClientRoom(std::shared_ptr<network::PeerWrapper> &client)
    {
        Room room(this->_waiting_rooms.size() + 1);
        const auto room_ptr = std::make_shared<Room>(room);
        auto &data = client->getData<ClientData>();
        data.setRoom(room_ptr);
        const auto client_id = data.getId();
        client->setData<ClientData>(std::move(data));
        room_ptr->addClient(client);
        this->_waiting_rooms.push_back(room_ptr);
        const auto ecs_client = room_ptr->getClient(client_id);
        spdlog::info("Client {} created room {}", ecs_client->getData<ClientData>().getId(), room_ptr->getId());
        struct network::LobbySnapshotPacket lobby_snapshot_packet;
        lobby_snapshot_packet.roomId = room_ptr->getId();
        lobby_snapshot_packet.gameState = room_ptr->getState();
        this->getServer().sendLobbyPacket(lobby_snapshot_packet, ecs_client);
    }

    /**
     * Assign a Client to a room when a client wants to join an existent room
     * @param client
     * @param room_id
     */
    bool Server::assignClientToRoom(std::shared_ptr<network::PeerWrapper> &client, const uint32_t room_id)
    {
        for (auto &room: this->_waiting_rooms) {
            if (room->getId() == room_id) {
                auto &data = client->getData<ClientData>();
                data.setRoom(room);
                const auto client_id = data.getId();
                client->setData<ClientData>(std::move(data));
                room->addClient(client);
                const auto ecs_client = room->getClient(client_id);
                spdlog::info("Client {} joined room {} with name {}", ecs_client->getData<ClientData>().getId(), room_id, ecs_client->getData<ClientData>().getName());
                struct network::LobbySnapshotPacket lobby_snapshot_packet;
                lobby_snapshot_packet.roomId = room_id;
                lobby_snapshot_packet.gameState = room->getState();
                this->getServer().sendLobbyPacket(lobby_snapshot_packet, ecs_client);
                return true;
            }
        }
        spdlog::error("Room doesn't exist {}", room_id);
        return false;
    }

    /**
     * @brief Removes a client from its room.
     * This function removes the client from the room's clients vector.
     * @param client The client to be removed from the room.
     * @param room_id The ID of the room from which the client should be removed.
     */
    void Server::leaveClientRoom(std::shared_ptr<network::PeerWrapper> &client, uint32_t room_id)
    {
        for (const auto &room: this->getAllRooms()) {
            if (room->getId() == room_id) {
                auto client_id = client->getData<ClientData>().getId();
                const auto ecs_client = room->getClient(client_id);
                if (ecs_client) {
                    auto &data = client->getData<ClientData>();
                    data.unsetRoom();
                    client->setData<ClientData>(std::move(data));
                    room->removeClient(client_id);
                    spdlog::info("Client {} left room {}", ecs_client->getData<ClientData>().getId(), room_id);
                } else {
                    spdlog::error("Client {} doesn't belong to room {}", ecs_client->getData<ClientData>().getId(), room_id);
                }
                return;
            }
        }
        spdlog::error("Room doesn't exist {}", room_id);
    }

    void Server::moveActionRoom(uint32_t client_id, uint32_t room_id, network::MoveDirection type) {
        for (const auto &room: this->_playing_rooms) {
            if (room->getId() == room_id) {
                room->addPos(client_id, type);
            }
        }
    }

    void Server::fireActionRoom(uint32_t client_id, uint32_t room_id, network::FireType type) {
        for (const auto &room: this->_playing_rooms) {
            if (room->getId() == room_id) {
                room->addProjectile(client_id, type);
            }
        }
    }




    /**
     * Stop server
     */
    void Server::stopServer()
    {
        this->_server.stop();
        spdlog::info("Shutting down server...");
    }

    /**
     * Getter for _playing_rooms
     * @return {std::vector<Room>}
     */
    std::vector<std::shared_ptr<Room>> Server::getPlayingRooms() { return this->_playing_rooms; }

    /**
     * Getter for _waiting_rooms
     * @return {std::vector<Room>}
     */
    std::vector<std::shared_ptr<Room>> Server::getWaitingRooms() { return this->_waiting_rooms; }

    /**
     * Getter for _waiting_rooms + _playing_rooms
     * @return {std::vector<Room>}
     */
    std::vector<std::shared_ptr<Room>> Server::getAllRooms()
    {
        std::vector<std::shared_ptr<Room>> all_rooms = this->_waiting_rooms;
        all_rooms.insert(all_rooms.end(), this->_playing_rooms.begin(), this->_playing_rooms.end());
        return all_rooms;
    }

    network::NetworkServer &Server::getServer() { return this->_server; }

    /*_________________________________________________________________*/

    /**
     * ServerException custom exception constructor
     * @param msg : exception message to log
     */
    Server::ServerException::ServerException(const char *msg) : _message(msg) {}

    /**
     * Overrides what() method
     * @return char * pointer
     */
    auto Server::ServerException::what() const noexcept -> const char * { return _message.c_str(); }

    /*_________________________________________________________________*/

    /**
     * Called when DataReceive Paquet type is received
     * Switch on PacketType to process Packet independently
     * Deserialize if RawPaquet is sent from a client
     * @param server
     * @param peer
     * @param data
     */
    void handleClientData(Server &server, std::shared_ptr<network::PeerWrapper> &peer, const std::any &data,
        network::PacketType type)
    {
        if (!peer->hasData())
            return;
        switch (type)
        {
        case network::PacketType::InputPacket:
            {
                const auto input_packet = std::any_cast<struct network::InputPacket>(data);
                gameAction(server, peer, input_packet);
                break;
            }
        case network::PacketType::LobbyActionPacket:
            {
                const auto lobby_action_packet = std::any_cast<struct network::LobbyActionPacket>(data);
                lobbyAction(server, peer, lobby_action_packet);
                break;
            }
        case network::PacketType::NoPacket:
        default:
            spdlog::warn("No packet received");
            break;
        }
    }
} // namespace server

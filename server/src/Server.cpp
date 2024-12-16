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
                    spdlog::info("Client connected: {}", static_cast<void *>(event.peer->getPeer().get()));
                    break;
                }
            case network::ServerEventType::ClientDisconnect:
                {
                    spdlog::info("Client disconnected: {}", event.peer->getData<ClientData>().getId());
                    if (event.peer->getData<ClientData>().getRoom() == nullptr)
                        break;
                    this->clientDisconnect(event.peer);
                }
            case network::ServerEventType::DataReceive:
                this->_thread_pool.enqueueTask(
                    [server = this, &peer = event.peer, data = std::move(event.data), dataType = event.packetType]
                    { handleClientData(*server, peer, data, dataType); });
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    /**
     * Disconnect a client from its Room and call sendUpdateRoom method
     * @param peer - client
     */
    void Server::clientDisconnect(std::shared_ptr<network::PeerWrapper> &peer)
    {
        const auto data = peer->getData<ClientData>();
        const uint16_t room_id = data.getRoom()->getId();
        this->leaveClientRoom(peer, room_id);
        if (data.getRoom()->getState() == network::Starting)
        {
            data.getRoom()->setState(network::Waiting);
        }
        data.getRoom()->sendUpdateRoom(*this);
    }

    /**
     * Check room state in order to update server room's vectors
     * If all clients in _waiting_rooms room are ready, move the room to _playing_rooms vector
     * If the current playing level has reached its end, move the room back to _waiting_rooms vector
     */
    void Server::checkRoomState()
    {
        this->_waiting_rooms.erase(std::remove_if(this->_waiting_rooms.begin(), this->_waiting_rooms.end(),
                                                  [this](const std::shared_ptr<Room> &room)
                                                  {
                                                      if (room.get()->getClientsReadiness())
                                                      {
                                                          this->_playing_rooms.push_back(room);
                                                          return true;
                                                      }
                                                      return false;
                                                  }),
                                   this->_waiting_rooms.end());
        // TODO: Add _playing_rooms that finished a level back to _waiting_rooms
        // this->_playing_rooms.erase(
        //     std::remove_if(
        //         this->_playing_rooms.begin(),
        //         this->_playing_rooms.end(),
        //         [this](const std::shared_ptr<Room> &room) {
        //             if (room.get()->getClientsReadiness()) {
        //                 this->_waiting_rooms.push_back(room);
        //                 return true;
        //             }
        //             return false;
        //         }
        //     ),
        // this->_playing_rooms.end()
        // );
    }

    /**
     * Client creates a room
     * @param client
     */
    void Server::createClientRoom(std::shared_ptr<network::PeerWrapper> &client)
    {
        Room room(this->_waiting_rooms.size() + 1);
        auto room_ptr = std::make_shared<Room>(room);
        auto data = client->getData<ClientData>();
        data.setRoom(room_ptr);
        client->setData<ClientData>(std::move(data));
        this->_waiting_rooms.push_back(room_ptr);
        spdlog::info("Client {} created room {}", client->getData<ClientData>().getId(), room.getId());
    }

    /**
     * Assign a Client to a room when a client wants to join an existent room
     * @param client
     * @param room_id
     */
    void Server::assignClientToRoom(std::shared_ptr<network::PeerWrapper> &client, const uint8_t room_id)
    {
        Room room(room_id);
        auto room_ptr = std::make_shared<Room>(room);
        if (std::find(this->_waiting_rooms.begin(), this->_waiting_rooms.end(), room_ptr) != this->_waiting_rooms.end())
        {
            auto data = client->getData<ClientData>();
            data.setRoom(room_ptr);
            client->setData<ClientData>(std::move(data));
            spdlog::info("Client {} joined room {}", client->getData<ClientData>().getId(), room_id);
        }
        else
        {
            throw ServerException("Room doesn't exist");
        }
    }

    /**
     * A Client leaves a room
     * Deletes NetworkPeer from a room's clients vector
     * @param client
     * @param room_id
     */
    void Server::leaveClientRoom(std::shared_ptr<network::PeerWrapper> &client, uint16_t room_id)
    {
        Room room(room_id);
        auto room_ptr = std::make_shared<Room>(room);
        if (std::find(this->_waiting_rooms.begin(), this->_waiting_rooms.end(), room_ptr) != this->_waiting_rooms.end())
        {
            if (std::find(room_ptr.get()->_clients.begin(), room_ptr.get()->_clients.end(), client) !=
                room_ptr.get()->_clients.end())
            {
                auto data = client->getData<ClientData>();
                data.unsetRoom();
                client->setData<ClientData>(std::move(data));
                room_ptr.get()->_clients.erase(
                    std::remove(room_ptr.get()->_clients.begin(), room_ptr.get()->_clients.end(), client),
                    room_ptr.get()->_clients.end());
                spdlog::info("Client {} left room {}", client->getData<ClientData>().getId(), room_ptr.get()->getId());
            }
            else
            {
                const std::string error = fmt::format("Client {} doesn't belong to room {}",
                                                      client->getData<ClientData>().getId(), room_ptr.get()->getId());
                throw ServerException(error.c_str());
            }
        }
        else
        {
            throw ServerException("Room doesn't exist");
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
                auto input_packet = std::any_cast<network::InputPacket>(data);
                break;
            }
        case network::PacketType::LobbyActionPacket:
            {
                auto lobby_action_packet = std::any_cast<network::LobbyActionPacket>(data);
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

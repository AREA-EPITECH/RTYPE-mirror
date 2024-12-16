/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ClientData
*/

#pragma once
#include <string>
#include "Room.hpp"

namespace server {
    // ENetPeer.data
    class ClientData {
        uint32_t _id;
        uint16_t _health = 100;
        std::string _name;
        uint16_t _ship_id = 0;
        bool _is_ready = false;
        // release room
        std::shared_ptr<Room> _room;
    public:
        ClientData(u_int32_t id);
        ~ClientData();

        uint32_t getId() const;
        void setName(const std::string &name);
        std::string getName() const;
        void setShip(uint16_t ship_id);
        uint16_t getShipId() const;
        void setReadyState();
        bool getReadyState() const;
        std::shared_ptr<Room> getRoom() const;
        void setRoom(std::shared_ptr<Room> &room);
        void unsetRoom();
    };
} // namespace server
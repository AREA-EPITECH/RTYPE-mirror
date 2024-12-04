/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ClientData
*/

#pragma once
#include <string>
#include <enet/enet.h>
#include <sys/types.h>
#include <vector>
#include "Room.hpp"

namespace server {
    // ENetPeer.data
    class ClientData {
        uint32_t _id;
        uint16_t _health = 100;
        std::string _name;
        uint16_t _ship_id = 0;
        // release room
        std::shared_ptr<Room> _room;
    public:
        ClientData(const std::shared_ptr<Room> &room, uint32_t id);
        void setName(const std::string &name);
        void setShip(uint16_t ship_id);
        uint32_t getId() const;
    };
} // namespace server
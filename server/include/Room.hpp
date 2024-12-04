/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Room
*/

#pragma once
#include <cstdint>
#include <memory>
#include <enet/enet.h>
#include <vector>

namespace server {
    class Room {
        uint8_t _id;
        bool _is_playing;
    public:
        std::vector<std::shared_ptr<ENetPeer>> _clients;
        explicit Room(uint8_t id);
        bool operator==(const Room& other) const;
        uint32_t getId() const;
    };
} // namespace server
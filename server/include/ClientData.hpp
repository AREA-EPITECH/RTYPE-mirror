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
    struct Ship {
        uint16_t id;
        Pos hitbox;
    };
    class ClientData {
        uint32_t _id = 0;
        std::string _name = "";
        bool _is_ready = false;
        bool _is_alive = true;
        int _score = 0;
        Ship _ship;
        std::shared_ptr<Room> _room = nullptr;
    public:
        ClientData(uint32_t id);
        ~ClientData();

        uint32_t getId() const;
        void setName(const std::string &name);
        std::string getName() const;
        void setShip(uint16_t ship_id);
        uint16_t getShipId() const;
        void setHitbox();
        Pos getHitbox() const;
        void setReadyState();
        bool getReadyState() const;
        void setAlive();
        bool getAlive() const;
        void setScore(int score);
        void resetScore();
        int getScore() const;
        std::shared_ptr<Room> getRoom() const;
        void setRoom(const std::shared_ptr<Room> &room);
        void unsetRoom();
    };
} // namespace server
/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Room
*/

#include "Room.hpp"

namespace server {
    Room::Room(uint8_t id): _id(id), _is_playing(false) {
    }

    bool Room::operator==(const Room& other) const {
        return this->_id == other._id;
    }

    /**
     * Getter client ID
     * @return {uint32_t}
     */
    uint32_t Room::getId() const {
        return this->_id;
    }
} // namespace server
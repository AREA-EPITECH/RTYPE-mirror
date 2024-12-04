/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ClientData
*/

#include "ClientData.hpp"

namespace server {
    ClientData::ClientData(const std::shared_ptr<Room> &room, const uint32_t id): _id(id), _name(), _room(room) {}

    /**
     * setter for client name
     * @param name
     */
    void ClientData::setName(const std::string &name) {
        this->_name = name;
    }

    /**
     * setter for ship ID
     * @param ship_id
     */
    void ClientData::setShip(const uint16_t ship_id) {
        this->_ship_id = ship_id;
    }

    /**
     * Getter client ID
     * @return {uint32_t}
     */
    uint32_t ClientData::getId() const {
        return this->_id;
    }

} // namespace server
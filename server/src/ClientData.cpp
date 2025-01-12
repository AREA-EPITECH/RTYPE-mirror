/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ClientData
*/

#include "ClientData.hpp"

namespace server {
    ClientData::ClientData(u_int32_t id): _id(id) {}

    ClientData::~ClientData() {
        this->_room.reset();
    }
    /**
     * Getter client ID
     * @return {uint32_t}
     */
    uint32_t ClientData::getId() const {
        return this->_id;
    }

    /**
     * Setter for client name
     * @param name
     */
    void ClientData::setName(const std::string &name) {
        this->_name = name;
    }

    /**
     * Getter for Room name
     * @return {string}
     */
    std::string ClientData::getName() const {
        return this->_name;
    }

    /**
     * Setter for ship ID
     * @param ship_id
     */
    void ClientData::setShip(const uint16_t ship_id) {
        this->_ship_id = ship_id;
    }

    /**
     * Getter client ship ID
     * @return {uint16_t}
     */
    uint16_t ClientData::getShipId() const {
        return this->_ship_id;
    }

    /**
     * Setter for client ready state
     */
    void ClientData::setReadyState() {
        this->_ship_id = !this->_ship_id;
    }

    /**
     * Getter client ready state
     * @return {bool}
     */
    bool ClientData::getReadyState() const {
        return this->_is_ready;
    }

    /**
     * Getter room shared pointer
     * @return {std::shared_ptr<Room>}
     */
    std::shared_ptr<Room> ClientData::getRoom() const {
        return this->_room;
    }

    /**
     * Setter room shared pointer
     * @param room - room pointer
     */
    void ClientData::setRoom(const std::shared_ptr<Room> &room) {
        this->_room = room;
    }

    /**
     * Release room shared pointer ownership
     */
    void ClientData::unsetRoom() {
        this->_room.reset();
    }
} // namespace server
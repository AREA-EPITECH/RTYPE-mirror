/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** ClientData
*/

#include "ClientData.hpp"

namespace server {
    ClientData::ClientData(u_int32_t id): _id(id), _ship() {
    }

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
        this->_ship.id = ship_id;
    }

    /**
     * Getter client ship ID
     * @return {uint16_t}
     */
    uint16_t ClientData::getShipId() const {
        return this->_ship.id;
    }

    /**
     * Setter for hitbox of the ship
     */
    void ClientData::setHitbox() {
        switch (this->_ship.id) {
            case 0:
                this->_ship.hitbox = {200, 42};
            break;
            case 1:
                this->_ship.hitbox = {155, 22};
            break;
            case 2:
                this->_ship.hitbox = {150, 37};
            break;
            default:
                break;
        }
    }

    /**
     * Getter client ship hitbox
     * @return {Pos}
     */
    Pos ClientData::getHitbox() const {
        return this->_ship.hitbox;
    }


    /**
     * Setter for client ready state
     */
    void ClientData::setReadyState() {
        this->_is_ready = !this->_is_ready;
    }

    /**
     * Getter client ready state
     * @return {bool}
     */
    bool ClientData::getReadyState() const {
        return this->_is_ready;
    }

    /**
     * Setter for client life
     */
    void ClientData::setAlive() {
        this->_is_alive = !this->_is_alive;
    }

    /**
     * Getter client life
     * @return {bool}
     */
    bool ClientData::getAlive() const {
        return this->_is_alive;
    }

    /**
     * Setter for client life
     */
    void ClientData::setScore(const int score) {
        this->_score += score;
    }

    /**
     * Getter client life
     * @return {bool}
     */
    int ClientData::getScore() const {
        return this->_score;
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
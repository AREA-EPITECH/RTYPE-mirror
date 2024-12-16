/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Utils
*/

#include "Utils.hpp"

namespace utils {
    uint32_t Utils::hash_pointer(const std::shared_ptr<network::PeerWrapper> &client) {
        uintptr_t address = reinterpret_cast<uintptr_t>(client.get());
        address = ~address + (address << 15);
        address = address ^ (address >> 12);
        address = address + (address << 2);
        address = address ^ (address >> 4);
        address = address * 2057;
        address = address ^ (address >> 16);
        return static_cast<uint32_t>(address);
    }
}
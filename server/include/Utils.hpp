/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Utils
*/

#pragma once
#include <cstdint>
#include <memory>
#include <network/PeerWrapper.hpp>

namespace utils {

    class Utils {
        public:
            static uint32_t hash_pointer(const std::shared_ptr<network::PeerWrapper> &client);
            static uint32_t hash_pointer_2(const void *ptr);
    };
}

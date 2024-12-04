/*
** EPITECH PROJECT, 2024
** RTYPE-mirror
** File description:
** main.cpp
*/

#include "network/Client.hpp"
#include "network/event/Event.hpp"
#include "network/packet/Packet.hpp"
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

std::vector<uint8_t> serializeString(const std::string &str) {
    std::vector<uint8_t> buffer;

    // Serialize the string length
    uint32_t length = static_cast<uint32_t>(str.size());
    buffer.resize(sizeof(length));
    std::memcpy(buffer.data(), &length, sizeof(length));

    // Serialize the string content
    buffer.insert(buffer.end(), str.begin(), str.end());

    return buffer;
}

std::string deserializeString(const std::vector<uint8_t> &data) {
    size_t offset = 0;
    // Deserialize the string length
    uint32_t length = 0;
    std::memcpy(&length, data.data() + offset, sizeof(length));
    offset += sizeof(length);

    // Deserialize the string content
    std::string str(reinterpret_cast<const char *>(data.data() + offset), length);
    offset += length;

    return str;
}

int main(int argc, char const *argv[])
{
    network::NetworkClient client;

    if (client.connectToServer("localhost", 12345))
    {
        spdlog::info("Connected to server!");
        int i = 0;
        while (true)
        {
            network::ClientEvent event;
            char input[250];
            while (client.pollEvent(event))
            {
                std::vector<uint8_t> receivedPacket;
                switch (event.type)
                {
                case network::ClientEventType::DataReceive: {
                    if (event.packetType == network::PacketType::RawPacket) {
                        receivedPacket = std::any_cast<std::vector<uint8_t>>(event.data);
                        spdlog::info("Received data: {}", deserializeString(receivedPacket));
                    }
                    break;
                }

                case network::ClientEventType::ServerDisconnect:
                    spdlog::info("Disconnected from server");
                    return 0; // Exit loop on server disconnect
                }
            }
            std::cout << ">> ";
            std::cin.getline(input, 250);
            std::vector<uint8_t> packet = serializeString(input);
            client.sendPacket(packet);
            i++;
        }
    }
    else
    {
        spdlog::error("Failed to connect to server.");
    }
    return 0;
}

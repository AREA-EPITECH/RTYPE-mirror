#include "NetworkWrapper.hpp"
#include "spdlog/spdlog.h"
#include <stdio.h>
#include <iostream>
#include <chrono>

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
            network::InputPacket packet;
            while (client.pollEvent(event))
            {
                switch (event.type)
                {
                case network::ClientEventType::DataReceive:
                    spdlog::info("Received data with id: {}", event.data.header.packetId);
                    break;

                case network::ClientEventType::ServerDisconnect:
                    spdlog::info("Disconnected from server");
                    return 0; // Exit loop on server disconnect
                }
            }
            /*std::cout << ">> ";
            std::cin.getline(input, 250);
            auto now = std::chrono::steady_clock::now();
            packet.header.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
            packet.header.packetId = i;
            packet.data = input;
            client.sendPacket(network::Packet::serializeInputPacket(packet));
            i++;*/
        }
    }
    else
    {
        spdlog::error("Failed to connect to server.");
    }
    return 0;
}

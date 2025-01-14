/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Save_controls
*/

#include "ecs/Systems.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

void updateKeysAndVolumesInJson(const std::string &filePath,
                                const std::unordered_map<std::string, int> &keyBindings,
                                float soundVolume,
                                float musicVolume,
                                int s_width,
                                int s_height) {
    try {
        std::ifstream inputFile(filePath);
        nlohmann::json jsonData;
        inputFile >> jsonData;
        inputFile.close();

        for (const auto &[action, key] : keyBindings) {
            if (jsonData.contains(action)) {
                jsonData[action] = key;
            } else {
                std::cerr << "Action '" << action << "' not found in JSON file!" << std::endl;
            }
        }

        jsonData["Sound"] = static_cast<int>(soundVolume);
        jsonData["Music"] = static_cast<int>(musicVolume);
        jsonData["ResolutionH"] = s_height;
        jsonData["ResolutionW"] = s_width;

        std::ofstream outputFile(filePath);
        outputFile << jsonData.dump(4);
        outputFile.close();

    } catch (const std::exception &e) {
        std::cerr << "Error updating JSON file: " << e.what() << std::endl;
    }
}

void updateSettings(Registry &ecs, const std::string &jsonFilePath, int s_width, int s_height) {
    auto &keys = ecs.get_components<ecs::KeyBindingComponent>();
    auto &sounds = ecs.get_components<ecs::SoundComponent>();
    auto &musics = ecs.get_components<ecs::MusicComponent>();

    std::unordered_map<std::string, int> allKeyBindings;
    float soundVolume = 100.0f;
    float musicVolume = 100.0f;

    for (auto &keyComponent : keys) {
        if (keyComponent.has_value()) {
            const auto &keyBindings = keyComponent.value();
            auto actions = keyBindings.getActions();

            for (const auto &action : actions) {
                int key = keyBindings.getKey(action);
                allKeyBindings[action] = key;
            }
        }
    }

    for (auto &soundComponent : sounds) {
        if (soundComponent.has_value()) {
            soundVolume = soundComponent.value().volume;
            break;
        }
    }

    for (auto &musicComponent : musics) {
        if (musicComponent.has_value()) {
            musicVolume = musicComponent.value().volume;
            break;
        }
    }

    updateKeysAndVolumesInJson(jsonFilePath, allKeyBindings, soundVolume, musicVolume, s_width, s_height);
}
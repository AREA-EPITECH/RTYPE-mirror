#pragma once

// Display Lobby
void update_board_component(Registry &ecs, int screenWidth, int screenHeight);
void display_board (Registry &ecs, int screenWidth, int screenHeight);

void init_lobby_entity (Registry &ecs);

// Display Menu
void display_menu_selectors(Registry &ecs);
void update_menu_selectors(Registry &ecs, int screenWidth, int screenHeight);

void init_menu_entity (Registry &ecs);

// Kill_entity_template

template <typename ComponentType>
void kill_entities_with_component(Registry& ecs) {
    auto& components = ecs.get_components<ComponentType>();
    for (std::size_t i = 0; i < components.size(); ++i) {
        if (components[i].has_value()) {
            ecs.kill_entity(i);
        }
    }
}

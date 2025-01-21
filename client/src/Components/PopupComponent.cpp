/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** PopupComponent
*/

#include <utility>
#include <string>

#include "ecs/Components.hpp"
#include "Registry.hpp"
#include "ecs/Systems.hpp"

namespace ecs
{
    PopupComponent::PopupComponent(Rectangle _boxRect, std::string _title, std::string _message, Color _boxColor, Color _textColor,
                                       WindowFocus _focus,
                                       std::function<int(int screenWidth, int screenHeight)> _dynamicX,
                                       std::function<int(int screenWidth, int screenHeight)> _dynamicY,
                                       std::function<void()> _close_func) :
        boxRect(_boxRect), message(std::move(_message)), title(std::move(_title)), boxColor(_boxColor), textColor(_textColor), isVisible(true),
        dynamicX(std::move(_dynamicX)), dynamicY(std::move(_dynamicY)),
        focus(_focus), close_func(std::move(_close_func))
    {
        spdlog::info("Create Popup");
        if (dynamicX)
            boxRect.x = dynamicX(GetScreenWidth(), GetScreenHeight());
        if (dynamicY)
            boxRect.y = dynamicY(GetScreenWidth(), GetScreenHeight());
    }

    void PopupComponent::draw(WindowFocus _focus)
    {
        if (!isVisible)
            return;

        int previousState = GuiGetState();

        if (focus != _focus)
        {
            GuiDisable();
        }

        if (dynamicX)
            boxRect.x = dynamicX(GetScreenWidth(), GetScreenHeight());
        if (dynamicY)
            boxRect.y = dynamicY(GetScreenWidth(), GetScreenHeight());

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
        GuiPanel(boxRect, title.c_str());

        const int screenHeight = GetScreenHeight();

        Rectangle closeButtonRect = {boxRect.x + boxRect.width - 32, boxRect.y, 32, 32};

        if (GuiButton(closeButtonRect, "X"))
        {
            isVisible = false;
            if (close_func) {
                close_func();
            }
        }

        const int fontSize = 32;

        DrawText(message.c_str(), boxRect.x + (boxRect.width / 2) - (MeasureText(message.c_str(), fontSize) / 2), boxRect.y + (boxRect.height / 2) - (fontSize / 2), fontSize, textColor);

        GuiSetState(previousState);
    }

    void PopupComponent::updateBox(int screenWidth, int screenHeight)
    {
        if (dynamicX)
            boxRect.x = dynamicX(screenWidth, screenHeight);
        if (dynamicY)
            boxRect.y = dynamicY(screenWidth, screenHeight);
    }

    void PopupComponent::createPopup(Registry &ecs, const std::string &title, const std::string &message)
    {
        kill_entities_with_component<ecs::ShowBoxComponent>(ecs);
        kill_entities_with_component<ecs::TextInputComponent>(ecs);
        kill_entities_with_component<ecs::ButtonComponent>(ecs);
        kill_entities_with_component<ecs::TextComponent>(ecs);
        kill_entities_with_component<ecs::CloseLeaderBoard>(ecs);
        kill_entities_with_component<ecs::VesselsComponent>(ecs);
        kill_entities_with_component<ecs::ProjectilesComponent>(ecs);
        kill_entities_with_component<ecs::ControllableComponent>(ecs);
        kill_entities_with_component<ecs::MenuText>(ecs);
        kill_entities_with_component<ecs::ShaderComponent>(ecs);
        kill_entities_with_component<ecs::CameraComponent>(ecs);
        kill_entities_with_component<ecs::LightComponent>(ecs);
        kill_entities_with_component<ecs::BackgroundComponent>(ecs);
        kill_entities_with_component<ecs::DecorElementComponent>(ecs);
        kill_entities_with_component<ecs::HealthBarComponent>(ecs);
        kill_entities_with_component<ecs::ParticleSystemComponent>(ecs);
        kill_entities_with_component<ecs::ImageComponent>(ecs);
        kill_entities_with_component<ecs::FilterComponent>(ecs);
        auto popup = ecs.spawn_entity();
        Rectangle boxRect = {100, 100, 600, 200};
        ecs.add_component<ecs::PopupComponent>(
            popup,
            ecs::PopupComponent(
                boxRect, title, message, DARKGRAY, BLACK, ecs::POPUP_FOCUS,
                [boxRect](int screenWidth, int screenHeight) { return screenWidth / 2 - (boxRect.width / 2); },
                [boxRect](int screenWidth, int screenHeight) { return screenHeight / 2 - (boxRect.height / 2); },
                [&ecs](){ecs.run_event(ecs::ChangeFocusEvent{ecs::MENU_FOCUS});}
            )
        );
        ecs::change_window(ecs, ecs::WindowType::MENU);
        ecs.run_event(ecs::ChangeFocusEvent{ecs::POPUP_FOCUS});
    }
} // namespace ecs

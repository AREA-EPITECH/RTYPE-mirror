//
// Created by lferraro on 12/19/24.
//

#include "ecs/Components.hpp"

namespace ecs {
    SoundComponent::SoundComponent(float initialVolume) : volume(initialVolume) {}


    void SoundComponent::drawVolumeSlider(int x, int y, int width) {
        volume = GuiSliderBar(Rectangle {static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), 20},
                              "Volume", nullptr, &volume, 0, 100);
    }
}

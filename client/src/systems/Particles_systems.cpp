/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Particles_systems
*/

#include "ecs/Systems.hpp"

namespace ecs
{
    /**
     * Create the particles system
     * @param ecs, event
     */
    void particles_system(Registry &ecs, const ParticleSystemEvent &event)
    {
        const client::ParticleSystem particle_system_event{event.position, event.direction, event.infinite, event.lifetime,
            event.max_particles};
        auto entity = ecs.spawn_entity();
        ecs.add_component<ParticleSystemComponent>(entity, {particle_system_event});
    }
}

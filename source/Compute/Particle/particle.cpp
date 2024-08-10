#include <VulkanViewport/Compute/Particle/particle.hpp>
#include <random>
#include <glm/glm.hpp>
namespace VkVP::Particle
{
std::vector<Particle> create_particles(uint32_t particles_per_attractor)
{
  // We mark a few particles as attractors that move along a given path, these
  // will pull in the other particles
  std::vector<glm::vec3> attractors = {
      glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(-5.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -5.0f),
      glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, -8.0f, 0.0f),
  };

  uint32_t numParticles =
      static_cast<uint32_t>(attractors.size()) * particles_per_attractor;

  // Initial particle positions
  std::vector<Particle> particleBuffer(numParticles);

  std::default_random_engine rndEngine((unsigned)time(nullptr));
  std::normal_distribution<float> rndDist(0.0f, 1.0f);

  for (uint32_t i = 0; i < static_cast<uint32_t>(attractors.size()); i++) {
    for (uint32_t j = 0; j < particles_per_attractor; j++) {
      Particle &particle = particleBuffer[i * particles_per_attractor + j];

      // First particle in group as heavy center of gravity
      if (j == 0) {
        particle.pos = glm::vec4(attractors[i] * 1.5f, 90000.0f);
        particle.vel = glm::vec4(glm::vec4(0.0f));
      } else {
        // Position
        glm::vec3 position(attractors[i] + glm::vec3(rndDist(rndEngine),
                                                     rndDist(rndEngine),
                                                     rndDist(rndEngine)) *
                                               0.75f);
        float len = glm::length(glm::normalize(position - attractors[i]));
        position.y *= 2.0f - (len * len);

        // Velocity
        glm::vec3 angular =
            glm::vec3(0.5f, 1.5f, 0.5f) * (((i % 2) == 0) ? 1.0f : -1.0f);
        glm::vec3 velocity = glm::cross((position - attractors[i]), angular) +
                             glm::vec3(rndDist(rndEngine), rndDist(rndEngine),
                                       rndDist(rndEngine) * 0.025f);

        float mass = (rndDist(rndEngine) * 0.5f + 0.5f) * 75.0f;
        particle.pos = glm::vec4(position, mass);
        particle.vel = glm::vec4(velocity, 0.0f);
      }

      // Color gradient offset
      particle.vel.w =
          (float)i * 1.0f / static_cast<uint32_t>(attractors.size());
    }
  }
    return particleBuffer;
}}
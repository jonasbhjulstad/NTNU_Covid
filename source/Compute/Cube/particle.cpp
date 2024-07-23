#include <VulkanViewport/Compute/Cube/particle.hpp>
#include <glm/ext/matrix_transform.inl>
#include <glm/glm.hpp>
#include <random>
namespace VkVP::Cube {
glm::mat3 randomRotation(std::mt19937 &rng) {
  std::uniform_real_distribution<float> dist(0.0f, 2 * M_PI);
  glm::mat4 rot = glm::mat3(1.0f);

  float angle = dist(rng);
  glm::rotate(rot, angle, glm::vec3(0.0f, 0.0f, 1.0f));
  angle = dist(rng);
  glm::rotate(rot, angle, glm::vec3(0.0f, 1.0f, 0.0f));
  angle = dist(rng);
  glm::rotate(rot, angle, glm::vec3(1.0f, 0.0f, 0.0f));
  return glm::mat3(rot);
}

std::vector<Particle> createParticles(uint32_t particles_per_attractor) {
  std::vector<glm::vec3> attractors = {
      glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(-5.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -5.0f),
      glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, -8.0f, 0.0f),
  };

  uint32_t numParticles =
      static_cast<uint32_t>(attractors.size()) * particles_per_attractor;

  // Initial particle positions
  std::vector<Particle> particleComputeBuffer(numParticles);

  std::default_random_engine rndEngine((unsigned)time(nullptr));
  std::normal_distribution<float> rndDist(0.0f, 1.0f);
  std::mt19937 rng(rndEngine());
  for (uint32_t i = 0; i < static_cast<uint32_t>(attractors.size()); i++) {
    for (uint32_t j = 0; j < particles_per_attractor; j++) {
      Particle &particle =
          particleComputeBuffer[i * particles_per_attractor + j];

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
        particle.scale = glm::vec3(1.0f);
        particle.rotation = randomRotation(rng);
      }

      // Color gradient offset
      particle.vel.w =
          (float)i * 1.0f / static_cast<uint32_t>(attractors.size());
    }
  }
  return particleComputeBuffer;
}

} // namespace VkVP::Cube
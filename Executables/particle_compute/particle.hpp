#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <cstdint>
#include <VulkanTools/gltf/Texture.hpp>
struct Particle {
  glm::vec4 pos; // xyz = position, w = mass
  glm::vec4 vel; // xyz = velocity, w = gradient texture position
};

struct ParticleTextures {
  Texture2D particle;
  Texture2D gradient;
};
std::vector<Particle> create_particles(uint32_t particles_per_attractor);


# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/jonas/Documents/Vulkan/examples/imgui/build/imgui-src"
  "C:/Users/jonas/Documents/Vulkan/examples/imgui/build/imgui-build"
  "C:/Users/jonas/Documents/Vulkan/examples/imgui/build/imgui-prefix"
  "C:/Users/jonas/Documents/Vulkan/examples/imgui/build/imgui-prefix/tmp"
  "C:/Users/jonas/Documents/Vulkan/examples/imgui/build/imgui-prefix/src/imgui-stamp"
  "C:/Users/jonas/Documents/Vulkan/examples/imgui/build/imgui-prefix/src"
  "C:/Users/jonas/Documents/Vulkan/examples/imgui/build/imgui-prefix/src/imgui-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/jonas/Documents/Vulkan/examples/imgui/build/imgui-prefix/src/imgui-stamp/${subDir}")
endforeach()

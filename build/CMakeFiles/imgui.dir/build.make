# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\jonas\Documents\Vulkan\examples\imgui

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\jonas\Documents\Vulkan\examples\imgui\build

# Include any dependencies generated for this target.
include CMakeFiles/imgui.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/imgui.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/imgui.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/imgui.dir/flags.make

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui.cpp.obj: CMakeFiles/imgui.dir/flags.make
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui.cpp.obj: CMakeFiles/imgui.dir/includes_CXX.rsp
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui.cpp.obj: _deps/imgui_repo-src/imgui.cpp
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui.cpp.obj: CMakeFiles/imgui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonas\Documents\Vulkan\examples\imgui\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui.cpp.obj -MF CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui.cpp.obj.d -o CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui.cpp.obj -c C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui.cpp

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui.cpp > CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui.cpp.i

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui.cpp -o CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui.cpp.s

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_demo.cpp.obj: CMakeFiles/imgui.dir/flags.make
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_demo.cpp.obj: CMakeFiles/imgui.dir/includes_CXX.rsp
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_demo.cpp.obj: _deps/imgui_repo-src/imgui_demo.cpp
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_demo.cpp.obj: CMakeFiles/imgui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonas\Documents\Vulkan\examples\imgui\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_demo.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_demo.cpp.obj -MF CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_demo.cpp.obj.d -o CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_demo.cpp.obj -c C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui_demo.cpp

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_demo.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui_demo.cpp > CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_demo.cpp.i

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_demo.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui_demo.cpp -o CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_demo.cpp.s

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_draw.cpp.obj: CMakeFiles/imgui.dir/flags.make
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_draw.cpp.obj: CMakeFiles/imgui.dir/includes_CXX.rsp
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_draw.cpp.obj: _deps/imgui_repo-src/imgui_draw.cpp
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_draw.cpp.obj: CMakeFiles/imgui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonas\Documents\Vulkan\examples\imgui\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_draw.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_draw.cpp.obj -MF CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_draw.cpp.obj.d -o CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_draw.cpp.obj -c C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui_draw.cpp

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_draw.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui_draw.cpp > CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_draw.cpp.i

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_draw.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui_draw.cpp -o CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_draw.cpp.s

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_tables.cpp.obj: CMakeFiles/imgui.dir/flags.make
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_tables.cpp.obj: CMakeFiles/imgui.dir/includes_CXX.rsp
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_tables.cpp.obj: _deps/imgui_repo-src/imgui_tables.cpp
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_tables.cpp.obj: CMakeFiles/imgui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonas\Documents\Vulkan\examples\imgui\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_tables.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_tables.cpp.obj -MF CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_tables.cpp.obj.d -o CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_tables.cpp.obj -c C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui_tables.cpp

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_tables.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_tables.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui_tables.cpp > CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_tables.cpp.i

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_tables.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_tables.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui_tables.cpp -o CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_tables.cpp.s

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_widgets.cpp.obj: CMakeFiles/imgui.dir/flags.make
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_widgets.cpp.obj: CMakeFiles/imgui.dir/includes_CXX.rsp
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_widgets.cpp.obj: _deps/imgui_repo-src/imgui_widgets.cpp
CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_widgets.cpp.obj: CMakeFiles/imgui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonas\Documents\Vulkan\examples\imgui\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_widgets.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_widgets.cpp.obj -MF CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_widgets.cpp.obj.d -o CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_widgets.cpp.obj -c C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui_widgets.cpp

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_widgets.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_widgets.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui_widgets.cpp > CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_widgets.cpp.i

CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_widgets.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_widgets.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\imgui_widgets.cpp -o CMakeFiles\imgui.dir\_deps\imgui_repo-src\imgui_widgets.cpp.s

CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_vulkan.cpp.obj: CMakeFiles/imgui.dir/flags.make
CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_vulkan.cpp.obj: CMakeFiles/imgui.dir/includes_CXX.rsp
CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_vulkan.cpp.obj: _deps/imgui_repo-src/backends/imgui_impl_vulkan.cpp
CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_vulkan.cpp.obj: CMakeFiles/imgui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonas\Documents\Vulkan\examples\imgui\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_vulkan.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_vulkan.cpp.obj -MF CMakeFiles\imgui.dir\_deps\imgui_repo-src\backends\imgui_impl_vulkan.cpp.obj.d -o CMakeFiles\imgui.dir\_deps\imgui_repo-src\backends\imgui_impl_vulkan.cpp.obj -c C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\backends\imgui_impl_vulkan.cpp

CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_vulkan.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_vulkan.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\backends\imgui_impl_vulkan.cpp > CMakeFiles\imgui.dir\_deps\imgui_repo-src\backends\imgui_impl_vulkan.cpp.i

CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_vulkan.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_vulkan.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\backends\imgui_impl_vulkan.cpp -o CMakeFiles\imgui.dir\_deps\imgui_repo-src\backends\imgui_impl_vulkan.cpp.s

CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_glfw.cpp.obj: CMakeFiles/imgui.dir/flags.make
CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_glfw.cpp.obj: CMakeFiles/imgui.dir/includes_CXX.rsp
CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_glfw.cpp.obj: _deps/imgui_repo-src/backends/imgui_impl_glfw.cpp
CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_glfw.cpp.obj: CMakeFiles/imgui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\jonas\Documents\Vulkan\examples\imgui\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_glfw.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_glfw.cpp.obj -MF CMakeFiles\imgui.dir\_deps\imgui_repo-src\backends\imgui_impl_glfw.cpp.obj.d -o CMakeFiles\imgui.dir\_deps\imgui_repo-src\backends\imgui_impl_glfw.cpp.obj -c C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\backends\imgui_impl_glfw.cpp

CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_glfw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_glfw.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\backends\imgui_impl_glfw.cpp > CMakeFiles\imgui.dir\_deps\imgui_repo-src\backends\imgui_impl_glfw.cpp.i

CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_glfw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_glfw.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\jonas\Documents\Vulkan\examples\imgui\build\_deps\imgui_repo-src\backends\imgui_impl_glfw.cpp -o CMakeFiles\imgui.dir\_deps\imgui_repo-src\backends\imgui_impl_glfw.cpp.s

# Object files for target imgui
imgui_OBJECTS = \
"CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui.cpp.obj" \
"CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_demo.cpp.obj" \
"CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_draw.cpp.obj" \
"CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_tables.cpp.obj" \
"CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_widgets.cpp.obj" \
"CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_vulkan.cpp.obj" \
"CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_glfw.cpp.obj"

# External object files for target imgui
imgui_EXTERNAL_OBJECTS =

libimgui.a: CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui.cpp.obj
libimgui.a: CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_demo.cpp.obj
libimgui.a: CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_draw.cpp.obj
libimgui.a: CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_tables.cpp.obj
libimgui.a: CMakeFiles/imgui.dir/_deps/imgui_repo-src/imgui_widgets.cpp.obj
libimgui.a: CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_vulkan.cpp.obj
libimgui.a: CMakeFiles/imgui.dir/_deps/imgui_repo-src/backends/imgui_impl_glfw.cpp.obj
libimgui.a: CMakeFiles/imgui.dir/build.make
libimgui.a: CMakeFiles/imgui.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\jonas\Documents\Vulkan\examples\imgui\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX static library libimgui.a"
	$(CMAKE_COMMAND) -P CMakeFiles\imgui.dir\cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\imgui.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/imgui.dir/build: libimgui.a
.PHONY : CMakeFiles/imgui.dir/build

CMakeFiles/imgui.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\imgui.dir\cmake_clean.cmake
.PHONY : CMakeFiles/imgui.dir/clean

CMakeFiles/imgui.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\jonas\Documents\Vulkan\examples\imgui C:\Users\jonas\Documents\Vulkan\examples\imgui C:\Users\jonas\Documents\Vulkan\examples\imgui\build C:\Users\jonas\Documents\Vulkan\examples\imgui\build C:\Users\jonas\Documents\Vulkan\examples\imgui\build\CMakeFiles\imgui.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/imgui.dir/depend


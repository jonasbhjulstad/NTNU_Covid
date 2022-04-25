#ifndef KEY_MAPPING_HPP
#define KEY_MAPPING_HPP
#include <imgui.h>
#include "keycodes.hpp"

void mapKeyboard()
{
	auto& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = KEY_PRIOR;
    io.KeyMap[ImGuiKey_PageDown] = KEY_NEXT;
    io.KeyMap[ImGuiKey_Home] = KEY_HOME;
    //MKp keymap using the keycodes defined in keycodes.hpp
    io.KeyMap[ImGuiKey_Insert] = KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = KEY_A;
    io.KeyMap[ImGuiKey_C] = KEY_C;
    io.KeyMap[ImGuiKey_V] = KEY_V;
    io.KeyMap[ImGuiKey_X] = KEY_X;
    io.KeyMap[ImGuiKey_Y] = KEY_Y;
    io.KeyMap[ImGuiKey_Z] = KEY_Z;

    io.KeyMap[ImGuiKey_A] = KEY_A;
    io.KeyMap[ImGuiKey_C] = KEY_C;
    io.KeyMap[ImGuiKey_V] = KEY_V;
    io.KeyMap[ImGuiKey_X] = KEY_X;
    io.KeyMap[ImGuiKey_Y] = KEY_Y;
    io.KeyMap[ImGuiKey_Z] = KEY_Z;
}

const int getKeyNumberInput(uint32_t key)
{
    switch (key)
    {
        case KEY_0:
            return 0;
        case KEY_1:
            return 1;
        case KEY_2:
            return 2;
        case KEY_3:
            return 3;
        case KEY_4:
            return 4;
        case KEY_5:
            return 5;
        case KEY_6:
            return 6;
        case KEY_7:
            return 7;
        case KEY_8:
            return 8;
        case KEY_9:
            return 9;
        case KEY_NUMPAD0:
            return 0;
        case KEY_NUMPAD1:
            return 1;
        case KEY_NUMPAD2:
            return 2;
        case KEY_NUMPAD3:
            return 3;
        case KEY_NUMPAD4:
            return 4;
        case KEY_NUMPAD5:
            return 5;
        case KEY_NUMPAD6:
            return 6;
        case KEY_NUMPAD7:
            return 7;
        case KEY_NUMPAD8:
            return 8;
        case KEY_NUMPAD9:
            return 9;
        default:
            return -1;
    }
}
#endif
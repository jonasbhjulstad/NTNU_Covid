#ifndef KEY_NUMBER_INPUT_HPP
#define KEY_NUMBER_INPUT_HPP
#include <keycodes.hpp>

int getKeyNumberInput(uint32_t key)
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
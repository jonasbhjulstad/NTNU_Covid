#ifndef MENU_WINDOW_DEFINES_HPP
#define MENU_WINDOW_DEFINES_HPP
#include <string>
#include <map>

enum Menu_Window
{
    MENU_WINDOW_NEW,
    MENU_WINDOW_OPEN,
    MENU_WINDOW_SAVE,
    MENU_WINDOW_NEW_GRAPH,
    MENU_WINDOW_IMPORT,
    MENU_WINDOW_PREFERENCES
};

const std::map<Menu_Window, std::string> Menu_Window_Names = {
    {MENU_WINDOW_NEW, "New"},
    {MENU_WINDOW_OPEN, "Open"},
    {MENU_WINDOW_SAVE, "Save"},
    {MENU_WINDOW_NEW_GRAPH, "New Graph"},
    {MENU_WINDOW_IMPORT, "Import"},
    {MENU_WINDOW_PREFERENCES, "Preferences"}
};

#endif
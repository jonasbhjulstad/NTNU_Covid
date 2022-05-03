#ifndef NV_MENU_WINDOW_DEFINES_HPP
#define NV_MENU_WINDOW_DEFINES_HPP


enum NV_Menu_Window
{
    NV_MENU_WINDOW_NEW,
    NV_MENU_WINDOW_OPEN,
    NV_MENU_WINDOW_SAVE,
    NV_MENU_WINDOW_NEW_GRAPH,
    NV_MENU_WINDOW_IMPORT,
    NV_MENU_WINDOW_PREFERENCES
};

const std::map<NV_Menu_Window, std::string> NV_Menu_Window_Names = {
    {NV_MENU_WINDOW_NEW, "New"},
    {NV_MENU_WINDOW_OPEN, "Open"},
    {NV_MENU_WINDOW_SAVE, "Save"},
    {NV_MENU_WINDOW_NEW_GRAPH, "New Graph"},
    {NV_MENU_WINDOW_IMPORT, "Import"},
    {NV_MENU_WINDOW_PREFERENCES, "Preferences"}
};

#endif
#ifndef MENU_WINDOW_DEFINES_HPP
#define MENU_WINDOW_DEFINES_HPP
#include <string>
#include <map>
#include <set>

enum class Menu_Windows: uint16_t
{
    TOP_MENU_NEW,
    TOP_MENU_FILE,
    TOP_MENU_OPEN,
    TOP_MENU_SAVE,
    TOP_MENU_IMPORT,
    TOP_MENU_PREFERENCES,
};

// const std::unordered_map<Menu_Windows, std::string> Menu_Names = {
//     {Menu_Windows::TOP_MENU_NEW, "New"},
//     {Menu_Windows::TOP_MENU_OPEN, "Open"},
//     {Menu_Windows::TOP_MENU_SAVE, "Save"},
//     {Menu_Windows::TOP_MENU_IMPORT, "Import"},
//     {Menu_Windows::TOP_MENU_PREFERENCES, "Preferences"}
// };

const std::set<Menu_Windows> Top_Menus = {Menu_Windows::TOP_MENU_NEW, Menu_Windows::TOP_MENU_OPEN, Menu_Windows::TOP_MENU_SAVE, Menu_Windows::TOP_MENU_IMPORT, Menu_Windows::TOP_MENU_PREFERENCES};
const std::map<Menu_Windows, std::string> Top_Menu_Names = {
    {Menu_Windows::TOP_MENU_NEW, "New"},
    {Menu_Windows::TOP_MENU_OPEN, "Open"},
    {Menu_Windows::TOP_MENU_SAVE, "Save"},
    {Menu_Windows::TOP_MENU_IMPORT, "Import"},
    {Menu_Windows::TOP_MENU_PREFERENCES, "Preferences"}
};


#endif
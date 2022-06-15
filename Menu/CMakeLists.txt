add_library(NV_Menu STATIC NV_Graph_Designer.cpp NV_Menu.cpp)
target_include_directories(NV_Menu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(NV_Menu PUBLIC igraph imgui)



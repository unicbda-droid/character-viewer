cmake_minimum_required(VERSION 3.16)
project(character LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(VCPKG_DIR "C:/Users/ionco/vcpkg/installed/x64-windows")
set(glfw3_DIR "${VCPKG_DIR}/share/glfw3")
set(imgui_DIR "${VCPKG_DIR}/share/imgui")
find_package(glfw3 QUIET)
find_package(imgui REQUIRED)
set(GLEW_INCLUDE_DIR "${VCPKG_DIR}/include")
set(GLEW_LIBRARY "${VCPKG_DIR}/lib/glew32.lib")

add_executable(character main.cpp imgui_impl_opengl3.cpp)

target_include_directories(character PRIVATE ${GLEW_INCLUDE_DIR})
target_compile_definitions(character PRIVATE IMGUI_IMPL_OPENGL_LOADER_CUSTOM)

target_link_libraries(character PRIVATE engine imgui::imgui)

add_custom_command(TARGET character POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/character/skybox"
        "$<TARGET_FILE_DIR:character>/skybox")

cmake_minimum_required(VERSION 3.16)
project(character LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(VCPKG_DIR "C:/Users/ionco/vcpkg/installed/x64-windows")
set(glfw3_DIR "${VCPKG_DIR}/share/glfw3")
find_package(glfw3 REQUIRED)
set(GLEW_INCLUDE_DIR "${VCPKG_DIR}/include")
set(GLEW_LIBRARY "${VCPKG_DIR}/lib/glew32.lib")

add_executable(character main.cpp)

target_include_directories(character PRIVATE ${GLEW_INCLUDE_DIR})
target_link_libraries(character PRIVATE glfw ${GLEW_LIBRARY} opengl32 winmm)

add_custom_command(TARGET character POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${VCPKG_DIR}/bin/glew32.dll"
        "$<TARGET_FILE_DIR:character>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${VCPKG_DIR}/bin/glfw3.dll"
        "$<TARGET_FILE_DIR:character>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/skybox"
        "$<TARGET_FILE_DIR:character>/skybox")

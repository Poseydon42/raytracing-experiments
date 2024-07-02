cmake_minimum_required(VERSION 3.25)

project(RaytracingExperiments CXX)

find_package(Vulkan REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw)

set(SOURCES
    src/App.cpp
    src/Main.cpp
    src/Vulkan/VulkanDevice.cpp
)

add_executable(RaytracingExperiments ${SOURCES})

target_include_directories(RaytracingExperiments PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(RaytracingExperiments Vulkan::Vulkan glfw)

set_target_properties(RaytracingExperiments PROPERTIES CXX_STANDARD 20)
set_target_properties(RaytracingExperiments PROPERTIES CXX_STANDARD_REQUIRED TRUE)
set_target_properties(RaytracingExperiments PROPERTIES CXX_EXTENSIONS FALSE)

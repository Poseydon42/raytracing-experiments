cmake_minimum_required(VERSION 3.25)

project(RaytracingExperiments CXX)

set(SOURCES
    src/Main.cpp
)

add_executable(RaytracingExperiments ${SOURCES})

set_target_properties(RaytracingExperiments PROPERTIES CXX_STANDARD 20)
set_target_properties(RaytracingExperiments PROPERTIES CXX_STANDARD_REQUIRED TRUE)
set_target_properties(RaytracingExperiments PROPERTIES CXX_EXTENSIONS FALSE)

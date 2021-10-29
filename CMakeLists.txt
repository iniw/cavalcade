# --- global ---

cmake_minimum_required(VERSION 3.21)

# this has to be set before project() is called
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "Build types" FORCE)

project(cavalcade LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# enforce our custom build types

# --- build options ---

include("cmake/CMakeBuildOptions.cmake")

# --- build definitions ---

include("cmake/CMakeBuildDefinitions.cmake")

# --- pre-compiled headers ---

include("cmake/CMakePCH.cmake")

# --- source files ---

include("cmake/CMakeSources.cmake")

# --- target configuration ---

add_library(${CMAKE_PROJECT_NAME} SHARED)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SOURCES})

target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE ${PCH})

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE 
    ${OPTIONS_GENERAL} 
    "$<$<CONFIG:RelWithDebInfo>:${OPTIONS_RELWITHDEBINFO}>" 
    "$<$<CONFIG:Release>:${OPTIONS_RELEASE}>"
    "$<$<CONFIG:Debug>:${OPTIONS_DEBUG}>")

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE 
    "$<$<CONFIG:RelWithDebInfo>:${LINKER_OPTIONS_RELWITHDEBINFO}>" 
    "$<$<CONFIG:Release>:${LINKER_OPTIONS_RELEASE}>")

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE 
    ${DEFINITIONS_GENERAL} 
    "$<$<CONFIG:RelWithDebInfo>:${DEFINITIONS_DEBUG}>" 
    "$<$<CONFIG:Release>:${DEFINITIONS_RELEASE}>"
    "$<$<CONFIG:Debug>:${DEFINITIONS_DEBUG}>")

# --- libraries ---

find_package(fmt CONFIG REQUIRED)
find_package(PolyHook_2 CONFIG REQUIRED)

# link
target_link_libraries(${CMAKE_PROJECT_NAME} fmt::fmt PolyHook_2::PolyHook_2)
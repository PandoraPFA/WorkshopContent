# cmake file for building ExampleContent
#-------------------------------------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "WorkshopContent requires an out-of-source build.")
endif()

# project name
if(NOT WORKSHOP_CONTENT_LIBRARY_NAME STREQUAL "WorkshopPandoraContent")
    set(WORKSHOP_CONTENT_LIBRARY_NAME "WorkshopContent")
endif()
project(${WORKSHOP_CONTENT_LIBRARY_NAME})

# project version
set(${PROJECT_NAME}_VERSION_MAJOR 00)
set(${PROJECT_NAME}_VERSION_MINOR 01)
set(${PROJECT_NAME}_VERSION_PATCH 00)
set(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")

#-------------------------------------------------------------------------------------------------------------------------------------------
# Dependencies
include(PandoraCMakeSettings)

# Prefer local include directory to any paths to installed header files
include_directories(include)

find_package(PandoraSDK 02.03.00 REQUIRED)
find_package(LArContent 02.07.03 REQUIRED)

foreach(package PandoraSDK LArContent)
    if(${package}_FOUND)
        include_directories(${${package}_INCLUDE_DIRS})
        link_libraries(${${package}_LIBRARIES})
        add_definitions (${${package}_DEFINITIONS})
    endif()
endforeach()

if(PANDORA_MONITORING)
    find_package(PandoraMonitoring 02.03.00 REQUIRED)
    include_directories(${PandoraMonitoring_INCLUDE_DIRS})
    link_libraries(${PandoraMonitoring_LIBRARIES})
    add_definitions(${PandoraMonitoring_DEFINITIONS})
    add_definitions("-DMONITORING")

    list(APPEND CMAKE_MODULE_PATH "$ENV{ROOTSYS}/etc/cmake/")
    find_package(ROOT 5.26.00 REQUIRED COMPONENTS Eve Geom RGL EG)
endif()

#-------------------------------------------------------------------------------------------------------------------------------------------
# Low level settings - compiler etc
if (NOT CMAKE_CXX_FLAGS)
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

include(CheckCXXCompilerFlag)
unset(COMPILER_SUPPORTS_CXX_FLAGS CACHE)
CHECK_CXX_COMPILER_FLAG(${CMAKE_CXX_FLAGS} COMPILER_SUPPORTS_CXX_FLAGS)

if(NOT COMPILER_SUPPORTS_CXX_FLAGS)
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} does not support cxx flags ${CMAKE_CXX_FLAGS}")
endif()

if(APPLE)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -Wno-long-long -Wno-sign-compare -Wshadow -fno-strict-aliasing ${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -ansi -pedantic -Wno-long-long -Wno-sign-compare -Wshadow -fno-strict-aliasing ${CMAKE_CXX_FLAGS}")
endif()

#-------------------------------------------------------------------------------------------------------------------------------------------
# Build products

# - Collect sources - not ideal because you have to keep running CMake to pick up changes
file(GLOB_RECURSE WORKSHOP_CONTENT_SRCS RELATIVE ${PROJECT_SOURCE_DIR} "workshopcontent/*.cc")

# - Add library and properties
add_library(${PROJECT_NAME} SHARED ${WORKSHOP_CONTENT_SRCS})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_VERSION} SOVERSION ${${PROJECT_NAME}_SOVERSION})

# - Executable
add_executable(PandoraWorkshop ${PROJECT_SOURCE_DIR}/workshopcontent/Test/PandoraWorkshop.cc)
if(PANDORA_MONITORING)
    include_directories(${ROOT_INCLUDE_DIRS})
    target_link_libraries(PandoraWorkshop ${ROOT_LIBRARIES})
endif()
target_link_libraries(PandoraWorkshop ${PROJECT_NAME})

# - Optional documents
option(ExampleContent_BUILD_DOCS "Build documentation for ${PROJECT_NAME}" OFF)
if(ExampleContent_BUILD_DOCS)
    add_subdirectory(doc)
endif()

#-------------------------------------------------------------------------------------------------------------------------------------------
# Install products

# - library
install(TARGETS ${PROJECT_NAME} DESTINATION lib COMPONENT Runtime)

# - headers
install(DIRECTORY include/ DESTINATION include COMPONENT Development FILES_MATCHING PATTERN "*.h")

# - executable
install(TARGETS PandoraExample DESTINATION bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

#-------------------------------------------------------------------------------------------------------------------------------------------
# display some variables and write them to cache
PANDORA_DISPLAY_STD_VARIABLES()
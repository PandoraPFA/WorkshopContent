# cmake file for building WorkshopContent
#-------------------------------------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "WorkshopContent requires an out-of-source build.")
endif()

# project name
project(WorkshopContent)

# project version
set(${PROJECT_NAME}_VERSION_MAJOR 02)
set(${PROJECT_NAME}_VERSION_MINOR 02)
set(${PROJECT_NAME}_VERSION_PATCH 00)
set(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")

#-------------------------------------------------------------------------------------------------------------------------------------------
# Dependencies
include(PandoraCMakeSettings)

# Prefer local include directory to any paths to installed header files
include_directories(${PROJECT_SOURCE_DIR})

find_package(PandoraSDK 03.01.00 REQUIRED)
find_package(LArContent 03.08.00 REQUIRED)

foreach(package PandoraSDK LArContent)
    if(${package}_FOUND)
        include_directories(${${package}_INCLUDE_DIRS})
        link_libraries(${${package}_LIBRARIES})
        add_definitions (${${package}_DEFINITIONS})
    endif()
endforeach()

if(PANDORA_MONITORING)
    find_package(PandoraMonitoring 03.00.00 REQUIRED)
    include_directories(${PandoraMonitoring_INCLUDE_DIRS})
    link_libraries(${PandoraMonitoring_LIBRARIES})
    add_definitions(${PandoraMonitoring_DEFINITIONS})
    add_definitions("-DMONITORING")

    list(APPEND CMAKE_MODULE_PATH "$ENV{ROOTSYS}/etc/cmake/")
    find_package(ROOT 5.26.00 REQUIRED COMPONENTS Eve Geom RGL EG)
endif()

#-------------------------------------------------------------------------------------------------------------------------------------------
# Low level settings - compiler etc
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -Wno-long-long -Wno-sign-compare -Wshadow -fno-strict-aliasing -std=c++11 ${CMAKE_CXX_FLAGS}")

include(CheckCXXCompilerFlag)
unset(COMPILER_SUPPORTS_CXX_FLAGS CACHE)
CHECK_CXX_COMPILER_FLAG(${CMAKE_CXX_FLAGS} COMPILER_SUPPORTS_CXX_FLAGS)

if(NOT COMPILER_SUPPORTS_CXX_FLAGS)
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} does not support cxx flags ${CMAKE_CXX_FLAGS}")
endif()

#-------------------------------------------------------------------------------------------------------------------------------------------
# Build products

# - Collect sources - not ideal because you have to keep running CMake to pick up changes
file(GLOB_RECURSE WORKSHOP_CONTENT_SRCS RELATIVE ${PROJECT_SOURCE_DIR} "*.cc")
file(GLOB_RECURSE WORKSHOP_CONTENT_TEST_SRCS RELATIVE ${PROJECT_SOURCE_DIR} "Test/*.cc")
list(REMOVE_ITEM WORKSHOP_CONTENT_SRCS ${WORKSHOP_CONTENT_TEST_SRCS})

# - Add library and properties
add_library(${PROJECT_NAME} SHARED ${WORKSHOP_CONTENT_SRCS})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_VERSION} SOVERSION ${${PROJECT_NAME}_SOVERSION})

# - Executable
add_executable(PandoraWorkshop ${PROJECT_SOURCE_DIR}/Test/PandoraWorkshop.cc)
if(PANDORA_MONITORING)
    include_directories(${ROOT_INCLUDE_DIRS})
    target_link_libraries(PandoraWorkshop ${ROOT_LIBRARIES})
endif()
target_link_libraries(PandoraWorkshop ${PROJECT_NAME})

#-------------------------------------------------------------------------------------------------------------------------------------------
# Install products

# - library
install(TARGETS ${PROJECT_NAME} DESTINATION lib COMPONENT Runtime)

# - executable
install(TARGETS PandoraWorkshop DESTINATION bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

#-------------------------------------------------------------------------------------------------------------------------------------------
# display some variables and write them to cache
PANDORA_DISPLAY_STD_VARIABLES()

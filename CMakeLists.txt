# cmake file for building Workshop Package
#-------------------------------------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

#-------------------------------------------------------------------------------------------------------------------------------------------
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -Wno-long-long -Wno-sign-compare -Wshadow -fno-strict-aliasing -std=c++17 ${CMAKE_CXX_FLAGS}")

include(CheckCXXCompilerFlag)
unset(COMPILER_SUPPORTS_CXX_FLAGS CACHE)
CHECK_CXX_COMPILER_FLAG(${CMAKE_CXX_FLAGS} COMPILER_SUPPORTS_CXX_FLAGS)

if(NOT COMPILER_SUPPORTS_CXX_FLAGS)
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} does not support cxx flags ${CMAKE_CXX_FLAGS}")
endif()

#-------------------------------------------------------------------------------------------------------------------------------------------

# Build products
add_subdirectory(workshopcontent)
add_subdirectory(examplecontent)

# install headers
install(DIRECTORY workshopcontent DESTINATION include COMPONENT Development FILES_MATCHING PATTERN "*.h")
install(DIRECTORY examplecontent DESTINATION include COMPONENT Development FILES_MATCHING PATTERN "*.h")

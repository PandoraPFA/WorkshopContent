# cmake file for building Workshop Package
#-------------------------------------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

# Build products
add_subdirectory(workshopcontent)
add_subdirectory(examplecontent)

# install headers
install(DIRECTORY workshopcontent DESTINATION include COMPONENT Development FILES_MATCHING PATTERN "*.h")
install(DIRECTORY examplecontent DESTINATION include COMPONENT Development FILES_MATCHING PATTERN "*.h")

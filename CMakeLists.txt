# cmake file for building Workshop Package
#-------------------------------------------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)

set(WORKSHOP_TOP_LEVEL_DIR ${PROJECT_SOURCE_DIR})

include_directories(${WORKSHOP_TOP_LEVEL_DIR})

# Build products
add_subdirectory(workshopcontent)
add_subdirectory(examplecontent)

# install headers
install(DIRECTORY ${WORKSHOP_TOP_LEVEL_DIR}/workshopcontent DESTINATION ${WORKSHOP_TOP_LEVEL_DIR}/include COMPONENT Development FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${WORKSHOP_TOP_LEVEL_DIR}/examplecontent DESTINATION ${WORKSHOP_TOP_LEVEL_DIR}/include COMPONENT Development FILES_MATCHING PATTERN "*.h")

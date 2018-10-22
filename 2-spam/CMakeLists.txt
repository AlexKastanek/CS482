cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 11)
project (training)

#Set Includes
set(INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDES})

#Set Sources
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME})

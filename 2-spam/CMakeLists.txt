cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 11)
project (spam)

#Set Includes
set(INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDES})

#Set Training Sources
set(TRAINING_SOURCES 
    ${TRAINING_SOURCES} 
    ${PROJECT_SOURCE_DIR}/src/training.cpp
    ${PROJECT_SOURCE_DIR}/src/SmsDatabase.cpp
)

add_executable(training ${TRAINING_SOURCES})
TARGET_LINK_LIBRARIES(training)

#Set Classify Sources
set(CLASSIFY_SOURCES
    ${CLASSIFY_SOURCES}
    ${PROJECT_SOURCE_DIR}/src/classify.cpp
    ${PROJECT_SOURCE_DIR}/src/SmsDatabase.cpp
)

add_executable(classify ${CLASSIFY_SOURCES})
TARGET_LINK_LIBRARIES(classify)

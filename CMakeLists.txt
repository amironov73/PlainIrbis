###########################################################
# PalinIrbis project
# Alexey Mironov, 2020
###########################################################

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# top level project
project(PlainIrbis C)

set(CMAKE_C_STANDARD 90)

if(MSVC)
    add_compile_options(/UTF-8 /Wall)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# headers of irbis and third-party libraries
include_directories(${PROJECT_SOURCE_DIR}/include)

# where artifacts will be gathered
set(ARTIFACTS ${PROJECT_SOURCE_DIR}/artifacts)

# installation settings
install(FILES ${PROJECT_SOURCE_DIR}/include/magna/core.h DESTINATION ${ARTIFACTS})
install(FILES ${PROJECT_SOURCE_DIR}/include/magna/irbis.h DESTINATION ${ARTIFACTS})

# subdirectories
add_subdirectory(libs)
add_subdirectory(apps)
add_subdirectory(tests)

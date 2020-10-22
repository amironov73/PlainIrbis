###########################################################
# PalinIrbis project
# Alexey Mironov, 2020
###########################################################

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

option(WITH_SHARED_LIBRARIES       "Build with shared libraries"        OFF)
option(WITH_TESTS                  "Build with tests"                   ON)
option(WITH_COVERAGE               "Build with code coverage"           OFF)
option(WITH_APPS                   "Build with apps"                    ON)
option(WITH_EXAMPLES               "Build with examples"                ON)
option(WITH_SANITIZERS             "Build with sanitizers"              OFF)
option(WITH_LTO                    "Build with link-time optimizations" OFF)
option(WITH_PEDANTIC               "Build with pedantic checks"         OFF)
option(WITHOUT_STRICT_ALIASING     "Build without strict-aliasing"      OFF)
option(WITH_FUNCTION_LEVEL_LINKING "Build with function-level linking"  OFF)
option(WITH_STATIC_LINKING         "Build with static linking"          OFF)

# top level project
project(PlainIrbis C)

set(CMAKE_C_STANDARD 90)

if(MSVC)

    add_compile_options(/UTF-8 /Wall)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    if (WITH_FUNCTION_LEVEL_LINKING)
        # add_compile_options(/Gy)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Gy")
        # add_link_options(/OPT:REF)
        set (CMAKE_EXE_LINKER_FLAGS /OPT:REF)
    endif()

    if (WITH_STATIC_LINKING)
        # set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MT")
        set (MSVC_RUNTIME_LIBRARY MultiThreaded)
        set (CMAKE_C_FLAGS_RELEASE "/MT")
    endif()

    if (WITH_LTO)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /GL /LTCG")
    endif()

else()
    # gcc or clang

    if (WITH_COVERAGE)
        set (CMAKE_C_FLAGS      "${CMAKE_C_FLAGS} --coverage")
        set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} --coverage")
    endif()

    if (WITH_SANITIZERS)
        set (CMAKE_C_FLAGS    "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
        set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
    endif()

    if (WITH_PEDANTIC)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2")
    endif()

    if (WITHOUT_STRICT_ALIASING)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing")
    endif()

    if (WITH_STATIC_LINKING)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
    endif()

    if (WITH_STATIC_LINKING)
        set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -static")
    endif()

    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unknown-pragmas -pthread")

endif()

# headers of irbis and third-party libraries
include_directories (${PROJECT_SOURCE_DIR}/include)

# where artifacts will be gathered
set (ARTIFACTS ${PROJECT_SOURCE_DIR}/artifacts)

# installation settings
install (FILES ${PROJECT_SOURCE_DIR}/include/magna/core.h  DESTINATION ${ARTIFACTS}/include)
install (FILES ${PROJECT_SOURCE_DIR}/include/magna/irbis.h DESTINATION ${ARTIFACTS}/include)

# subdirectories
add_subdirectory(libs)

if(WITH_APPS)
    add_subdirectory (apps)
endif()

if(WITH_TESTS)
    add_subdirectory (tests)
endif()

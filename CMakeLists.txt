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

# config.h related stuff

INCLUDE (CheckIncludeFiles)
CHECK_INCLUDE_FILES (malloc.h HAVE_MALLOC_H)

INCLUDE (CheckSymbolExists)
CHECK_SYMBOL_EXISTS (alloca "malloc.h" HAVE_ALLOCA)

INCLUDE(CheckTypeSize)
SET(CMAKE_EXTRA_INCLUDE_FILES "stdlib.h;stdint.h")
CHECK_TYPE_SIZE (void*    SIZEOF_VOID_P)
CHECK_TYPE_SIZE (uint16_t SIZEOF_UINT16_T)
CHECK_TYPE_SIZE (uint32_t SIZEOF_UINT32_T)
CHECK_TYPE_SIZE (uint64_t SIZEOF_UINT64_T)
CHECK_TYPE_SIZE (int16_t  SIZEOF_INT16_T)
CHECK_TYPE_SIZE (int32_t  SIZEOF_INT32_T)
CHECK_TYPE_SIZE (int64_t  SIZEOF_INT64_T)
CHECK_TYPE_SIZE (size_t   SIZEOF_SIZE_T)
SET(CMAKE_EXTRA_INCLUDE_FILES)

if(SIZEOF_SIZE_T)
    set(HAVE_SIZE_T 1)
else()
    set(size_t int)
endif()

if(SIZEOF_INT16_T)
    set(HAVE_INT16_T 1)
else()
    set (int16_t short)
endif()

if(SIZEOF_INT32_T)
    set(HAVE_INT32_T 1)
else()
    set (int32_t int)
endif()

if(SIZEOF_INT64_T)
    set(HAVE_INT64_T 1)
else()
    set (int64_t "long long")
endif()

if(SIZEOF_UINT16_T)
    set(HAVE_UINT16_T 1)
else()
    set (uint16_t "unsigned short")
endif()

if(SIZEOF_UINT32_T)
    set(HAVE_UINT32_T 1)
else()
    set (uint32_t "unsigned int")
endif()

if(SIZEOF_UINT64_T)
    set(HAVE_UINT64_T 1)
else()
    set (int64_t "unsigned long long")
endif()

CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/include/magna/config.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/config.h)

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
        if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
            set (CMAKE_C_FLAGS      "${CMAKE_C_FLAGS} --coverage")
            set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} --coverage")
        endif()
    endif()

    if (WITH_SANITIZERS)
        if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
            set (CMAKE_C_FLAGS    "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
            set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
        endif()
    endif()

    if (WITH_PEDANTIC)
        if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
            set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2")
        endif()
    endif()

    if (WITHOUT_STRICT_ALIASING)
        if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
            set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing")
        endif()
    endif()

    if (WITH_STATIC_LINKING)
        if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
            set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
        endif()
    endif()

    if (WITH_STATIC_LINKING)
        if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
            set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -static")
        endif()
    endif()

    if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unknown-pragmas -pthread")
    endif()

endif()

# headers of irbis and third-party libraries
include_directories (${PROJECT_SOURCE_DIR}/include)

# dynamic config.h
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DHAVE_CONFIG_H)

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

###########################################################
# PlainIrbis project
# Alexey Mironov, 2020
###########################################################

# offline safe tests
project(offline CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CppFiles
    src/buffer.cpp
    src/ean.cpp
    src/main.cpp
    src/span.cpp
)

set(HeaderFiles
    include/offline.h
)

add_executable(${PROJECT_NAME}
    ${CppFiles}
    ${HeaderFiles}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE include
    ../../3rdparty
)

target_link_libraries(${PROJECT_NAME} magna irbis)

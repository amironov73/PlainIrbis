###########################################################
# PlainIrbis project
# Alexey Mironov, 2020
###########################################################

# offline safe tests
project(offline C)

set(CFiles
    src/buffer.c
    src/ean.c
    src/main.c
    src/span.c
)

set(HeaderFiles
    include/offline.h
)

add_executable(${PROJECT_NAME}
    ${CFiles}
    ${HeaderFiles}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE include
    ../../3rdparty
)

target_link_libraries(${PROJECT_NAME} magna irbis)

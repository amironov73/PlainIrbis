###########################################################
# PlainIrbis project
# Alexey Mironov, 2020
###########################################################

# offline safe tests
project(offline C)

set(CFiles
    src/array.c
    src/buffer.c
    src/cp1251.c
    src/cp866.c
    src/ean.c
    src/intarray.c
    src/koi8r.c
    src/main.c
    src/path.c
    src/retry.c
    src/span.c
    src/spanarry.c
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

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})

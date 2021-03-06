###########################################################
# PlainIrbis project
# Alexey Mironov, 2020
###########################################################

# offline safe tests
project(offline C)

set(CFiles
    src/array.c
    src/buffer.c
    src/chain.c
    src/chunked.c
    src/connect.c
    src/cp1251.c
    src/cp866.c
    src/ean.c
    src/encoding.c
    src/enumertr.c
    src/field.c
    src/file.c
    src/intarray.c
    src/io.c
    src/koi8r.c
    src/list.c
    src/main.c
    src/memory.c
    src/menu.c
    src/navigatr.c
    src/number.c
    src/path.c
    src/retry.c
    src/span.c
    src/spanarry.c
    src/stream.c
    src/subfield.c
    src/upc.c
    src/utils.c
    src/vector.c
)

# set(CFiles src/span.c src/main.c)

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

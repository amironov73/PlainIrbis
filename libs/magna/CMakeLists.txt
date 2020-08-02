###########################################################
# PlainIrbis project
# Alexey Mironov, 2020
###########################################################

# ars magna core library
project(magna C)

set(HeaderFiles
    ../../include/magna/core.h
)

set(CFiles
    src/array.c
    src/buffer.c
    src/delphi.c
    src/magna.c
)

add_library(${PROJECT_NAME}
    STATIC
    ${HeaderFiles}
    ${CFiles}
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    -DMAGNA_LIBRARY
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})

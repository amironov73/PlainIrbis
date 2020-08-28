###########################################################
# PlainIrbis project
# Alexey Mironov, 2020
###########################################################

# network client library
project(irbis C)

set(HeaderFiles
    ../../include/magna/core.h
    ../../include/magna/irbis.h
)

set(CFiles
    src/connect.c
    src/dll.c
    src/ean.c
    src/error.c
    src/field.c
    src/guard.c
    src/ini.c
    src/menu.c
    src/query.c
    src/record.c
    src/response.c
    src/spec.c
    src/subfield.c
    src/tree.c
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

target_link_libraries(${PROJECT_NAME} magna)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})

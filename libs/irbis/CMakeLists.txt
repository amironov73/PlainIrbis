###########################################################
# PlainIrbis project
# Alexey Mironov, 2020
###########################################################

# network client library
project(irbis C)

set(HeaderFiles
    ../../include/magna/core.h
    ../../include/magna/irbis.h
    ../include/warnpop.h
    ../include/warnpush.h
)

set(CFiles
    src/connect.c
    src/dll.c
    src/ean.c
    src/error.c
    src/field.c
    src/format.c
    src/guard.c
    src/ini.c
    src/isbn.c
    src/iso2709.c
    src/menu.c
    src/query.c
    src/record.c
    src/response.c
    src/search.c
    src/spec.c
    src/subfield.c
    src/tree.c
    src/upc.c
    src/version.c
    src/xrf.c
)

add_library(${PROJECT_NAME}
    STATIC
    ${HeaderFiles}
    ${CFiles}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/../include
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    -DMAGNA_LIBRARY
)

target_link_libraries(${PROJECT_NAME} magna)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})

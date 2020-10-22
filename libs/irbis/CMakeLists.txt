###########################################################
# PlainIrbis project
# Alexey Mironov, 2020
###########################################################

# network client library
project(irbis C)

set(HeaderFiles
    ../../include/magna/core.h
    ../../include/magna/irbis.h
    ../../include/magna/direct.h
    ../../include/magna/fields.h
    ../include/warnpop.h
    ../include/warnpush.h
)

set(CFiles
    src/address.c
    src/alphatab.c
    src/author.c
    src/bookinfo.c
    src/codes.c
    src/collecti.c
    src/connect.c
    src/dbinfo.c
    src/dll.c
    src/ean.c
    src/error.c
    src/exemplar.c
    src/field.c
    src/field203.c
    src/format.c
    src/gbl.c
    src/guard.c
    src/ilf.c
    src/impex.c
    src/ini.c
    src/iri.c
    src/isbn.c
    src/isbninfo.c
    src/iso2709.c
    src/menu.c
    src/mst.c
    src/opt.c
    src/par.c
    src/phantom.c
    src/procinfo.c
    src/query.c
    src/rawrecor.c
    src/reader.c
    src/record.c
    src/registr.c
    src/resource.c
    src/response.c
    src/search.c
    src/servstat.c
    src/spec.c
    src/subfield.c
    src/tabledef.c
    src/term.c
    src/title.c
    src/tree.c
    src/upc.c
    src/uppertab.c
    src/userinfo.c
    src/version.c
    src/visit.c
    src/xrf.c
)

if(WITH_SHARED_LIBRARIES)

    add_library(${PROJECT_NAME}
        SHARED
        ${HeaderFiles}
        ${CFiles}
    )

    target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        -DMAGNA_DYNAMIC_LIBRARY
    )

else()

    add_library(${PROJECT_NAME}
        STATIC
        ${HeaderFiles}
        ${CFiles}
    )

endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/../include
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    -DMAGNA_LIBRARY
)

target_link_libraries(${PROJECT_NAME} magna)

if(MINGW)
    target_link_libraries(${PROJECT_NAME} libws2_32.a)
endif(MINGW)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS}/lib)

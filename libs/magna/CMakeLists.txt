###########################################################
# PlainIrbis project
# Alexey Mironov, 2020
###########################################################

# ars magna core library
project(magna C)

set(HeaderFiles
    ../../include/magna/core.h
    ../include/version.h
    ../include/warnpop.h
    ../include/warnpush.h
)

set(CFiles
    src/array.c
    src/beep.c
    src/buffer.c
    src/chain.c
    src/chunked.c
    src/cp1251.c
    src/cp866.c
    src/date.c
    src/delphi.c
    src/dump.c
    src/encoding.c
    src/enumertr.c
    src/error.c
    src/fastbuf.c
    src/file.c
    src/format.c
    src/handle.c
    src/intarray.c
    src/io.c
    src/keyboard.c
    src/koi8r.c
    src/list.c
    src/logging.c
    src/magna.c
    src/map.c
    src/mapfile.c
    src/memory.c
    src/navigatr.c
    src/number.c
    src/object.c
    src/path.c
    src/process.c
    src/retry.c
    src/screen.c
    src/sleep.c
    src/span.c
    src/spanarry.c
    src/stream.c
    src/string.c
    src/tcp4.c
    src/thread.c
    src/utf7.c
    src/utf8.c
    src/utils.c
    src/variant.c
    src/vector.c
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

if(MINGW)
    target_link_libraries(${PROJECT_NAME} libws2_32.a)
endif(MINGW)

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS}/lib)

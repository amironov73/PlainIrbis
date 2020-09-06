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
    src/chunked.c
    src/date.c
    src/delphi.c
    src/encoding.c
    src/error.c
    src/fastbuf.c
    src/file.c
    src/intarray.c
    src/io.c
    src/keyboard.c
    src/magna.c
    src/memory.c
    src/navigatr.c
    src/path.c
    src/screen.c
    src/span.c
    src/string.c
    src/tcp4.c
    src/thread.c
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

install(TARGETS ${PROJECT_NAME} DESTINATION ${ARTIFACTS})

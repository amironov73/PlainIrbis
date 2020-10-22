#
# Common support library
#

sources = [ 'src/array.c',
    'src/beep.c',
    'src/buffer.c',
    'src/chain.c',
    'src/chunked.c',
    'src/cp1251.c',
    'src/cp866.c',
    'src/date.c',
    'src/delphi.c',
    'src/dump.c',
    'src/encoding.c',
    'src/enumertr.c',
    'src/error.c',
    'src/fastbuf.c',
    'src/file.c',
    'src/format.c',
    'src/handle.c',
    'src/intarray.c',
    'src/io.c',
    'src/keyboard.c',
    'src/koi8r.c',
    'src/list.c',
    'src/logging.c',
    'src/magna.c',
    'src/map.c',
    'src/memory.c',
    'src/navigatr.c',
    'src/number.c',
    'src/object.c',
    'src/path.c',
    'src/process.c',
    'src/retry.c',
    'src/screen.c',
    'src/sleep.c',
    'src/span.c',
    'src/spanarry.c',
    'src/stream.c',
    'src/string.c',
    'src/tcp4.c',
    'src/thread.c',
    'src/utf8.c',
    'src/utils.c',
    'src/vector.c'
    ]

libmagna = static_library('magna',
        sources,
        include_directories: commonInclude
    )

libmagna_dep = declare_dependency(link_with: libmagna)

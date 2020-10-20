#
# Safe tests
#

sources = [ 'src/array.c',
    'src/buffer.c',
    'src/chain.c',
    'src/chunked.c',
    'src/connect.c',
    'src/cp1251.c',
    'src/cp866.c',
    'src/ean.c',
    'src/encoding.c',
    'src/enumertr.c',
    'src/field.c',
    'src/file.c',
    'src/intarray.c',
    'src/io.c',
    'src/koi8r.c',
    'src/list.c',
    'src/main.c',
    'src/memory.c',
    'src/menu.c',
    'src/navigatr.c',
    'src/number.c',
    'src/path.c',
    'src/retry.c',
    'src/span.c',
    'src/spanarry.c',
    'src/stream.c',
    'src/subfield.c',
    'src/upc.c',
    'src/utils.c',
    'src/vector.c'
]

localInclude = commonInclude
localInclude += include_directories ('include')

executable('offline',
        sources,
        include_directories: localInclude,
        dependencies: [ libmagna_dep, libirbis_dep ]
    )

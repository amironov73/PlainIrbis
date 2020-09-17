#
# Network client library
#

sources = [ 'src/connect.c',
    'src/dll.c',
    'src/ean.c',
    'src/error.c',
    'src/field.c',
    'src/format.c',
    'src/guard.c',
    'src/ini.c',
    'src/isbn.c',
    'src/iso2709.c',
    'src/menu.c',
    'src/query.c',
    'src/record.c',
    'src/response.c',
    'src/search.c',
    'src/spec.c',
    'src/subfield.c',
    'src/tree.c',
    'src/upc.c',
    'src/version.c',
    'src/xrf.c'
    ]

libirbis = static_library('irbis',
        sources,
        include_directories: commonInclude
    )

libirbis_dep = declare_dependency(link_with: libirbis)

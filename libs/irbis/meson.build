#
# Network client library
#

sources = [ 'src/address.c',
    'src/alphatab.c',
    'src/author.c',
    'src/bookinfo.c',
    'src/codes.c',
    'src/connect.c',
    'src/dbinfo.c',
    'src/dll.c',
    'src/ean.c',
    'src/error.c',
    'src/exemplar.c',
    'src/field.c',
    'src/format.c',
    'src/gbl.c',
    'src/guard.c',
    'src/ilf.c',
    'src/impex.c',
    'src/ini.c',
    'src/isbn.c',
    'src/iso2709.c',
    'src/menu.c',
    'src/mst.c',
    'src/opt.c',
    'src/par.c',
    'src/procinfo.c',
    'src/query.c',
    'src/rawrecor.c',
    'src/record.c',
    'src/registr.c',
    'src/response.c',
    'src/search.c',
    'src/servstat.c',
    'src/spec.c',
    'src/subfield.c',
    'src/term.c',
    'src/title.c',
    'src/tree.c',
    'src/upc.c',
    'src/uppertab.c',
    'src/userinfo.c',
    'src/version.c',
    'src/visit.c',
    'src/xrf.c'
    ]

libirbis = static_library('irbis',
        sources,
        include_directories: commonInclude
    )

libirbis_dep = declare_dependency(link_with: libirbis)

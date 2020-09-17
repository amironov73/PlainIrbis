#
# Root project
#

project('irbis', 'c',
    version: '0.0.1',
    license: 'MIT',
    meson_version: '>= 0.45.0',
    default_options : ['buildtype=release'])

cc = meson.get_compiler('c') # C compiler for further usage

commonInclude = [ include_directories('include'),
        include_directories('libs/include')
    ]

libirbis_dep = [] # dummy declaration, will be redefined later

subdir('libs')
subdir('tests')
subdir('apps')

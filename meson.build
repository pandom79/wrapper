project('wrapper', 'c', version: '1.0.0', default_options: ['warning_level=3', 'prefix=/usr'] )

add_global_arguments(['-Wno-incompatible-pointer-types'], language: 'c')

math_dep = meson.get_compiler('c').find_library('m', required : true)

libwrapper = library('wrapper', 'string_wrapper.c', 'array_wrapper.c', 'datetime_wrapper.c',
                     version: '1.0.0',
                     soversion: '0', install: true, dependencies : math_dep
                     )

install_headers('wrapper.h', subdir : 'libwrapper')

#executable('wrapper', 'main.c',
#           link_with: libwrapper, install: true
#          )

project('uquake', 'c', default_options : ['buildtype=debugoptimized'])

x11 = dependency('x11')

uquake_sources = ['src/', '']
executable('uquake', 'src/main.c', dependencies : [x11], install : true)


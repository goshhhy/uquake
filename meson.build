project('uquake', 'c', default_options : ['buildtype=debug', 'c_std=gnu99', 'warning_level=3', 'optimization=0'])

x11 = dependency('x11')
xshm = dependency('xext')
alsa = dependency('alsa')

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)
glide3 = cc.find_library('glide3', required : false)

use_networking = get_option('enable_networking')
if use_networking == true
    uquake_net_sources = [
        'src/net/net_dgrm.c',
        'src/net/net_loop.c',
        'src/net/net_main.c',
        'src/net/net_udp.c',
        'src/net/net_bsd.c',
    ]
else
    uquake_net_sources = [
        'src/net/net_loop.c',
        'src/net/net_main.c',
        'src/net/net_none.c'
    ]
endif

target_platform = get_option('target_platform')
if target_platform == 'linux_x11'
    uquake_platform_sources = [
        'src/cd_linux.c',
        'src/sys_linux.c',
        'src/vid_x.c',
        'src/snd_alsa.c',
        'src/snd_dma.c',
        'src/snd_mem.c',
        'src/snd_mix.c'
    ]
else
    error('target platform not supplied or unsupported')
endif

uquake_base_sources = [  'src/cl_demo.c',
                    'src/cl_input.c',
                    'src/cl_main.c',
                    'src/cl_parse.c',
                    'src/cl_tent.c',
                    'src/chase.c',
                    'src/cmd.c',
                    'src/common.c',
                    'src/console.c',
                    'src/crc.c',
                    'src/cvar.c',
                    'src/draw.c',
                    'src/draw/d_edge.c',
                    'src/draw/d_fill.c',
                    'src/draw/d_init.c',
                    'src/draw/d_modech.c',
                    'src/draw/d_part.c',
                    'src/draw/d_polyse.c',
                    'src/draw/d_scan.c',
                    'src/draw/d_sky.c',
                    'src/draw/d_sprite.c',
                    'src/draw/d_surf.c',
                    'src/draw/d_vars.c',
                    'src/draw/d_zpoint.c',
                    'src/host.c',
                    'src/host_cmd.c',
                    'src/keys.c',
                    'src/menu.c',
                    'src/mathlib.c',
                    'src/model.c',
                    'src/pr_cmds.c',
                    'src/pr_edict.c',
                    'src/pr_exec.c',
                    'src/r_fx/r_aclip.c',
                    'src/r_fx/r_alias.c',
                    'src/r_fx/r_bsp.c',
                    'src/r_fx/r_light.c',
                    'src/r_fx/r_draw.c',
                    'src/r_fx/r_efrag.c',
                    'src/r_fx/r_edge.c',
                    'src/r_fx/r_misc.c',
                    'src/r_fx/r_main.c',
                    'src/r_fx/r_sky.c',
                    'src/r_fx/r_sprite.c',
                    'src/r_fx/r_surf.c',
                    'src/r_fx/r_part.c',
                    'src/r_fx/r_vars.c',
                    'src/screen.c',
                    'src/sbar.c',
                    'src/sv_main.c',
                    'src/sv_phys.c',
                    'src/sv_move.c',
                    'src/sv_user.c',
                    'src/zone.c',
                    'src/view.c',
                    'src/wad.c',
                    'src/world.c',
                ]

executable('uquake', [uquake_base_sources, uquake_net_sources, uquake_platform_sources], dependencies : [x11, xshm, m_dep, alsa, glide3], install : true)
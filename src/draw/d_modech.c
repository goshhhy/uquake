// SPDX-License-Identifier: GPL-2.0-or-later
// d_modech.c: called when mode has just changed

#include "../quakedef.h"
#include "d_local.h"

int d_vrectx, d_vrecty, d_vrectright_particle, d_vrectbottom_particle;

int d_y_aspect_shift, d_pix_min, d_pix_max, d_pix_shift;

int d_scantable[MAXHEIGHT];
short *zspantable[MAXHEIGHT];

/*
================
D_Patch
================
*/
void D_Patch( void ) {
}

/*
================
D_ViewChanged
================
*/
void D_ViewChanged( void ) {
    int rowbytes;

    if ( r_dowarp )
        rowbytes = WARP_WIDTH;
    else
        rowbytes = vid.rowbytes;

    scale_for_mip = xscale;
    if ( yscale > xscale )
        scale_for_mip = yscale;

    d_zrowbytes = vid.width * 2;
    d_zwidth = vid.width;

    d_pix_min = r_refdef.vrect.width / 320;
    if ( d_pix_min < 1 )
        d_pix_min = 1;

    d_pix_max = (int)( (float)r_refdef.vrect.width / ( 320.0 / 4.0 ) + 0.5 );
    d_pix_shift = 8 - (int)( (float)r_refdef.vrect.width / 320.0 + 0.5 );
    if ( d_pix_max < 1 )
        d_pix_max = 1;

    if ( pixelAspect > 1.4 )
        d_y_aspect_shift = 1;
    else
        d_y_aspect_shift = 0;

    d_vrectx = r_refdef.vrect.x;
    d_vrecty = r_refdef.vrect.y;
    d_vrectright_particle = r_refdef.vrectright - d_pix_max;
    d_vrectbottom_particle =
        r_refdef.vrectbottom - ( d_pix_max << d_y_aspect_shift );

    {
        int i;

        for ( i = 0; i < vid.height; i++ ) {
            d_scantable[i] = i * rowbytes;
            zspantable[i] = d_pzbuffer + i * d_zwidth;
        }
    }

    D_Patch();
}

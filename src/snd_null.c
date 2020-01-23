// SPDX-License-Identifier: GPL-2.0-or-later
// snd_null.c -- include this instead of all the other snd_* files to have
// no sound code whatsoever

#include "quakedef.h"

cvar_t bgmvolume = {"bgmvolume", "1", true};
cvar_t volume = {"volume", "0.7", true};

void S_Init( void ) {}

void S_AmbientOff( void ) {}

void S_AmbientOn( void ) {}

void S_Shutdown( void ) {}

void S_TouchSound( char *sample ) {}

void S_ClearBuffer( void ) {}

void S_StaticSound( sfx_t *sfx, vec3_t origin, float vol, float attenuation ) {}

void S_StartSound( int entnum, int entchannel, sfx_t *sfx, vec3_t origin,
                   float fvol, float attenuation ) {}

void S_StopSound( int entnum, int entchannel ) {}

sfx_t *S_PrecacheSound( char *sample ) { return NULL; }

void S_ClearPrecache( void ) {}

void S_Update( vec3_t origin, vec3_t v_forward, vec3_t v_right, vec3_t v_up ) {}

void S_StopAllSounds( qboolean clear ) {}

void S_BeginPrecaching( void ) {}

void S_EndPrecaching( void ) {}

void S_ExtraUpdate( void ) {}

void S_LocalSound( char *s ) {}

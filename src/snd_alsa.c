// SPDX-License-Identifier: GPL-2.0-or-later
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <alsa/asoundlib.h>
#include <stdio.h>
#include "quakedef.h"

int audio_fd;
int snd_inited;

static int tryrates[] = {44100, 44100, 22050, 11025, 8000};

static snd_pcm_t *handle;
static snd_pcm_hw_params_t *params;
static snd_pcm_sw_params_t *sparams;

#define SND_MAX_BUF_SIZE 32768

static int bufpos = 0;
static char buf[SND_MAX_BUF_SIZE + 8];
static char stream[4096 + 8];

static void alsa_callback( snd_pcm_sframes_t sframes ) {
    int stride = shm->samplebits / 8, len = sframes * stride, offset = 0;
    int realpos = bufpos;

    if ( len > 4096 ) {
        len = 4096;
        sframes = len / stride;
    }

    printf( "\nsubmit %d bytes\n", len );

    if ( bufpos + len >= SND_MAX_BUF_SIZE ) {
        offset = SND_MAX_BUF_SIZE - bufpos;
        printf( "1 memcpy( stream + %d, buf[%d], %d )\n", 0, bufpos, offset );
        memcpy( &stream[0], &buf[bufpos], offset );
        len -= offset;
        realpos = 0;
    }
    printf( "2 memcpy( stream + %d, buf[%d], %d )\n", offset, realpos, len );
    memcpy( &stream[offset], &buf[realpos], len );

    int err = snd_pcm_writei( handle, stream, sframes );
    if ( err < 0 ) {
        Con_Printf( "Failure writing ALSA sound data: %s\n", snd_strerror( err ) );
    } else {
        printf("wrote %d bytes\n", err * 2);
        bufpos += err * stride;
        if ( bufpos >= SND_MAX_BUF_SIZE )
            bufpos -= SND_MAX_BUF_SIZE;
    }
    printf( "final bufpos %d\n", bufpos);
}

qboolean SNDDMA_Init( void ) {
    int rc;
    int fmt;
    int tmp;
    int i, err;
    char *s;
    int caps;

    snd_inited = 0;

    // open /dev/dsp, confirm capability to mmap, and get size of dma buffer

    err = snd_pcm_open( &handle, "default", SND_PCM_STREAM_PLAYBACK, 0 );
    if ( err < 0 ) {
        Con_Printf( "Failure initialising ALSA: %s\n", snd_strerror( err ) );
        return 0;
    }

    err = snd_pcm_hw_params_malloc( &params );
    if ( err < 0 ) {
        Con_Printf( "Failure allocating ALSA hardware params: %s\n", snd_strerror( err ) );
        return 0;
    }

    err = snd_pcm_hw_params_any( handle, params );
    if ( err < 0 ) {
        Con_Printf( "Failure getting ALSA hardware params: %s\n", snd_strerror( err ) );
        return 0;
    }

    err = snd_pcm_hw_params_set_access( handle, params, SND_PCM_ACCESS_RW_INTERLEAVED );
    if ( err < 0 ) {
        Con_Printf( "Failure setting ALSA access type: %s\n", snd_strerror( err ) );
        return 0;
    }

    shm = &sn;

    // set sample bits & speed

    s = getenv( "QUAKE_SOUND_SAMPLEBITS" );
    if ( s )
        shm->samplebits = atoi( s );
    else if ( ( i = COM_CheckParm( "-sndbits" ) ) != 0 )
        shm->samplebits = atoi( com_argv[i + 1] );
    if ( shm->samplebits != 16 && shm->samplebits != 8 ) {
            shm->samplebits = 16;
    }

    if ( shm->samplebits == 8 ) {
        err = snd_pcm_hw_params_set_format( handle, params, SND_PCM_FORMAT_U8 );
    } else {
        err = snd_pcm_hw_params_set_format( handle, params, SND_PCM_FORMAT_S16_LE );
    }
    if ( err < 0 ) {
        Con_Printf( "Failure setting sample format: %s\n", snd_strerror( err ) );
        return 0;
    }

    s = getenv( "QUAKE_SOUND_SPEED" );
    if ( s )
        tryrates[i] = atoi( s );
    else if ( ( i = COM_CheckParm( "-sndspeed" ) ) != 0 )
        tryrates[i] = atoi( com_argv[i + 1] );

    for ( i = 0; i < sizeof( tryrates ) / 4; i++ ) {
        err = snd_pcm_hw_params_set_rate_near( handle, params, &tryrates[i], 0 );
        if ( err >= 0 ) {
            shm->speed = tryrates[i];
            break;
        }
    }

    if ( err < 0 ) {
        Con_Printf( "Failure setting sample rate: %s\n", snd_strerror( err ) );
        return 0;
    }

    s = getenv( "QUAKE_SOUND_CHANNELS" );
    if ( s )
        shm->channels = atoi( s );
    else if ( ( i = COM_CheckParm( "-sndmono" ) ) != 0 )
        shm->channels = 1;
    else if ( ( i = COM_CheckParm( "-sndstereo" ) ) != 0 )
        shm->channels = 2;
    else
        shm->channels = 2;

    err = snd_pcm_hw_params_set_channels( handle, params, shm->channels );
    if ( err < 0 ) {
        Con_Printf( "Failure setting sound channels: %s\n", snd_strerror( err ) );
        return 0;
    }

    int size = 2048;
    err = snd_pcm_hw_params_set_buffer_size_near( handle, params, &size );
    if ( err < 0 ) {
        Con_Printf( "Failure setting buffer size: %s\n", snd_strerror( err ) );
        return 0;
    }

    err = snd_pcm_hw_params( handle, params );
    if ( err < 0 ) {
        Con_Printf( "Failure setting ALSA params: %s\n", snd_strerror( err ) );
        return 0;
    }

    // set up the callback
    if ((err = snd_pcm_sw_params_malloc( &sparams )) < 0) {
        Con_Printf( "Failure allocating ALSA software params: %s\n", snd_strerror( err ) );
        return 0;
    }

    if ((err = snd_pcm_sw_params_current( handle, sparams )) < 0) {
        Con_Printf( "Failure getting ALSA software params: %s\n", snd_strerror( err ) );
        return 0;
    }

    if ((err = snd_pcm_sw_params_set_avail_min( handle, sparams, 133 )) < 0) {
        Con_Printf( "Failure setting ALSA minimum frames: %s\n", snd_strerror( err ) );
        return 0;
    }

    if ((err = snd_pcm_sw_params_set_start_threshold( handle, sparams, 0 )) < 0) {
        Con_Printf( "Failure setting ALSA start threshold: %s\n", snd_strerror( err ) );
        return 0;
    }

    if ((err = snd_pcm_sw_params( handle, sparams )) < 0) {
        Con_Printf( "Failure setting ALSA software params: %s\n", snd_strerror( err ) );
        return 0;
    }

    if ((err = snd_pcm_prepare( handle )) < 0) {
        Con_Printf( "Failure preparing ALSA callback: %s\n", snd_strerror( err ) );
        return 0;
    }

    shm->samples = SND_MAX_BUF_SIZE / ( shm->samplebits / 8 );
    shm->submission_chunk = 1;

    // memory map the dma buffer

    shm->buffer = &buf;
    shm->samplepos = 0;

    snd_inited = 1;
    return 1;
}

int SNDDMA_GetDMAPos( void ) {
    shm->samplepos = bufpos / (shm->samplebits / 8);
    return shm->samplepos;
}

void SNDDMA_Shutdown( void ) {
    if ( snd_inited ) {
        snd_pcm_close(handle);
        snd_inited = 0;
    }
}

/*
==============
SNDDMA_Submit

Send sound to device if buffer isn't really the dma buffer
===============
*/
void SNDDMA_Submit( void ) {
    int nframes = snd_pcm_avail_update( handle );
    if ( nframes > 0 ) {
        alsa_callback( nframes );
    } else if ( nframes == 0 ) {
        return;
    } else if ( nframes == -EPIPE ) {
        Con_Printf( "buffer underrun\n" );
        snd_pcm_prepare(handle);
    } else {
        Con_Printf( "ALSA error: %s\n", snd_strerror( nframes ) );
        return;
    }
    
}

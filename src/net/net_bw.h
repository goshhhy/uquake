// SPDX-License-Identifier: GPL-2.0-or-later
// net_bw.h

int BW_Init( void );
void BW_Shutdown( void );
void BW_Listen( qboolean state );
int BW_OpenSocket( int port );
int BW_CloseSocket( int socket );
int BW_Connect( int socket, struct qsockaddr *addr );
int BW_CheckNewConnections( void );
int BW_Read( int socket, byte *buf, int len, struct qsockaddr *addr );
int BW_Write( int socket, byte *buf, int len, struct qsockaddr *addr );
int BW_Broadcast( int socket, byte *buf, int len );
char *BW_AddrToString( struct qsockaddr *addr );
int BW_StringToAddr( char *string, struct qsockaddr *addr );
int BW_GetSocketAddr( int socket, struct qsockaddr *addr );
int BW_GetNameFromAddr( struct qsockaddr *addr, char *name );
int BW_GetAddrFromName( char *name, struct qsockaddr *addr );
int BW_AddrCompare( struct qsockaddr *addr1, struct qsockaddr *addr2 );
int BW_GetSocketPort( struct qsockaddr *addr );
int BW_SetSocketPort( struct qsockaddr *addr, int port );

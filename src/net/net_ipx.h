// SPDX-License-Identifier: GPL-2.0-or-later
// net_ipx.h

int IPX_Init( void );
void IPX_Shutdown( void );
void IPX_Listen( qboolean state );
int IPX_OpenSocket( int port );
int IPX_CloseSocket( int socket );
int IPX_Connect( int socket, struct qsockaddr *addr );
int IPX_CheckNewConnections( void );
int IPX_Read( int socket, byte *buf, int len, struct qsockaddr *addr );
int IPX_Write( int socket, byte *buf, int len, struct qsockaddr *addr );
int IPX_Broadcast( int socket, byte *buf, int len );
char *IPX_AddrToString( struct qsockaddr *addr );
int IPX_StringToAddr( char *string, struct qsockaddr *addr );
int IPX_GetSocketAddr( int socket, struct qsockaddr *addr );
int IPX_GetNameFromAddr( struct qsockaddr *addr, char *name );
int IPX_GetAddrFromName( char *name, struct qsockaddr *addr );
int IPX_AddrCompare( struct qsockaddr *addr1, struct qsockaddr *addr2 );
int IPX_GetSocketPort( struct qsockaddr *addr );
int IPX_SetSocketPort( struct qsockaddr *addr, int port );

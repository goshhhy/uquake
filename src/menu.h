// SPDX-License-Identifier: GPL-2.0-or-later

//
// the net drivers should just set the apropriate bits in m_activenet,
// instead of having the menu code look through their internal tables
//
#define MNET_IPX 1
#define MNET_TCP 2

extern int m_activenet;

//
// menus
//
void M_Init( void );
void M_Keydown( int key );
void M_Draw( void );
void M_ToggleMenu_f( void );

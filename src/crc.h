// SPDX-License-Identifier: GPL-2.0-or-later
/* crc.h */

void CRC_Init( unsigned short *crcvalue );
void CRC_ProcessByte( unsigned short *crcvalue, byte data );
unsigned short CRC_Value( unsigned short crcvalue );

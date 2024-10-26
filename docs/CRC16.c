/*++
 *=============================================================
 *
 * Copyright © 2024 Wittern
 *
 * This program is property of Wittern  and must be held in strict
 * confidence and properly safeguarded by the recipient at all times. You
 * may not copy or reproduce, or provide or reveal to any other party,
 * except with prior written authorization of an appropriate officer of
 * Wittern, and any authorized copy or reproduction must include this
 * admonition and copyright notice. You may not reverse engineer, use, copy
 * or modify any program or any copy or modification except as for the
 * express purpose for which Wittern has provided it to the recipient.
 *
 *=============================================================
*--*/
#include "WrapperOs.h"
#include "CRC16.h"

 /* ======================================================================== */
 /*  CRC16_TBL    -- Lookup table used for the CRC-16 code. 									*/
 /*              Using Polynomial: x^16 + x^15 + x^2 + 1 (0xa001)         		*/
 /* ======================================================================== */
uint16_t crc16_tbl[256];

void CRC_Table_Build ( void )
{
    uint16_t crc = 0;
    uint16_t c, i, j;
    for ( i = 0; i < 256; i++ )
    {
        crc16_tbl[i] = 0;
    }
    for ( i = 0; i < 256; i++ )
    {
        crc = 0;
        c = i;
        for ( j = 0; j < 8; j++ )
        {
            if ( ( ( crc ^ c ) & 0x0001 ) != 0 )
            {
                crc = (uint16_t)( ( crc >> 1 ) ^ 0xA001 );
            }
            else
            {
                crc = (uint16_t)( crc >> 1 );
            }
            c >>= 1;
        }
        crc16_tbl[i] = crc;
    }
}


/* ======================================================================== */
/*  CRC16  -- Generates a 16-bit CRC based on a block of 8-bit data.        */
/* ======================================================================== */
uint16_t crc16 ( uint8_t* data, int len )
{
    int i;
    uint16_t crc = 0;
    for ( i = 0; i < len; i++ )
    {
        crc = (uint16_t)( ( crc >> 8 ) ^ crc16_tbl[( (uint8_t)crc ) ^ data[i]] );
    }
    return crc;
}


/* ======================================================================== */
/*  CRC16  -- Builds a 16-bit CRC based on a block of 8-bit data.        */
/* ======================================================================== */
uint16_t G85CRC_16_SUM;

uint16_t G85crc16 ( uint8_t data )
{
    G85CRC_16_SUM = (uint16_t)( ( G85CRC_16_SUM >> 8 ) ^ crc16_tbl[( (uint8_t)G85CRC_16_SUM ) ^ data] );
    return G85CRC_16_SUM;
}


/* ======================================================================== */
/*  CRC16  -- Update CRC.        */
/* ======================================================================== */
extern uint16_t FWcrc16_SUM;

void UpdateFWcrc16_SUM ( uint8_t data )
{
    FWcrc16_SUM = (uint16_t)( ( FWcrc16_SUM >> 8 ) ^ crc16_tbl[( (uint8_t)FWcrc16_SUM ) ^ data] );
}
/*++
 *=============================================================
 *
 * Copyright © 2024 Wittern
 *
 *=============================================================
*--*/

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
#ifndef __CRC16_H_
#define __CRC16_H_

#include <stdint.h>

void CRC_Table_Build ( void );
uint16_t crc16 ( uint8_t* data, int len );
uint16_t G85crc16 ( uint8_t data );
void UpdateFWcrc16_SUM ( uint8_t data );

#endif
/*++
 *=============================================================
 *
 * Copyright © 2024 Wittern
 *
 *=============================================================
*--*/
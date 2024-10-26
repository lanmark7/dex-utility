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
#ifndef __DEX_COMM_THREAD_H_
#define __DEX_COMM_THREAD_H_

#define SOH     0x01       
#define STX     0x02
#define ETX     0x03
#define EOT     0x04
#define ENQ     0x05
#define LF      0x0A
#define CR      0x0D
#define DLE     0x10
#define NAK     0x15
#define SYN     0x16
#define ETB     0x17

 /*
  Disable the -Wunused-variable, in files where this header is included and not used
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

static char CHAR_SET[2] = { "1" };
static char CHAR_CR[2] = { CR };
static char CHAR_LF[2] = { LF };
static char CHAR_MOTOR_MISSING[2] = { "1" };
static char CHAR_MOTOR_PRESENT[2] = { "0" };
static char STAR[2] = { '*' };
static char ZERO[2] = { '0' };
static char DLE0[3] = { DLE, '0' };
static char DLE1[3] = { DLE, '1' };
static char WACK[3] = { DLE, ';' };
static char DLEETB[3] = { DLE, ETB };
static char DLEETX[3] = { DLE, ETX };
static char DLESOH[3] = { DLE, SOH };
static char DLESTX[3] = { DLE, STX };
static char CR_LF[3] = { CR, LF };
static char READ_REVISION_AND_LEVEL[12] = { 'R', '0', '1', 'L', '0', '1', DLE, ETX };
static char SEND_REVISION_AND_LEVEL[22] = { DLE, SOH, '0', '0', '4', '2', '1', '4', '6', '8', '0', '1', '0', '0', 'R', '0', '1', 'L', '0', '1', DLE, ETX };
static char DXS[29] = { 'D', 'X', 'S', '*', 'W', 'T', 'N', '4', '2', '2', '4', '6', '6', '4', '*', 'V', 'A', '*', 'V', '0', '/', '6', '*', '1', CR, LF };
static char ST[13] = { 'S', 'T', '*', '0', '0', '1', '*', '0', '0', '0', '1' };
static char ID1[5] = { "ID1*" };
static char ID4[5] = { "ID4*" };
static char CB1[5] = { "CB1*" };
static char VA1[5] = { "VA1*" };
static char VA2[5] = { "VA2*" };
static char VA3[5] = { "VA3*" };
static char BA1[5] = { "BA1*" };
static char CA1[5] = { "CA1*" };
static char CA2[5] = { "CA2*" };
static char CA3[5] = { "CA3*" };
static char CA4[5] = { "CA4*" };
static char CA8[5] = { "CA8*" };
static char CA9[5] = { "CA9*" };
static char CA10[6] = { "CA10*" };
static char CA15[6] = { "CA15*" };
static char TUBE1[8] = { "TUBE1*" };
static char TUBE2[8] = { "TUBE2*" };
static char CA16[6] = { "CA16*" };
static char DA1[5] = { "DA1*" };
static char DA2[5] = { "DA2*" };
static char DA4[5] = { "DA4*" };
static char DA5[5] = { "DA5*" };
static char DA6[5] = { "DA6*" };
static char DA9[5] = { "DA9*" };
static char DA10[6] = { "DA10*" };
static char DB1[5] = { "DB1*" };
static char DB2[5] = { "DB2*" };
static char DB4[5] = { "DB4*" };
static char DB5[5] = { "DB5*" };
static char DB6[5] = { "DB6*" };
static char DB9[5] = { "DB9*" };
static char DB10[6] = { "DB10*" };
static char TA2[5] = { "TA2*" };
static char PA1[5] = { "PA1*" };
static char PA2[5] = { "PA2*" };
static char PA3[5] = { "PA3*" };
static char PA4[5] = { "PA4*" };
static char PA5[5] = { "PA5*" };
static char PA7[5] = { "PA7*" };
static char PA7CASH[6] = { "CA*0*" };
static char PA7CASHLESS1[6] = { "DA*1*" };
static char PA7CASHLESS2[6] = { "DB*1*" };
static char PA8[5] = { "PA8*" };
static char EA1[5] = { "EA1*" };
static char EA2[5] = { "EA2*" };
static char EA1_EJL[9] = { "EA1*EJL_" };
static char EA2_EJL[9] = { "EA2*EJL_" };
static char EA1_ELA[9] = { "EA1*ELA_" };
static char EA2_ELA[9] = { "EA2*ELA_" };
static char EA1_EGS[9] = { "EA1*EGS*" };
static char EA2_EGS[9] = { "EA2*EGS*" };

static char EA1_EJH[9] = { "EA1*EJH*" };
static char EA2_EJH[9] = { "EA2*EJH*" };
static char EA1_EJJ[9] = { "EA1*EJJ*" };
static char EA2_EJJ[9] = { "EA2*EJJ*" };
static char EA1_EJK[9] = { "EA1*EJK*" };
static char EA2_EJK[9] = { "EA2*EJK*" };
static char EA1_EA[8] = { "EA1*EA*" };
static char EA2_EA[8] = { "EA2*EA*" };
static char EA1_EK[8] = { "EA1*EK*" };
static char EA2_EK[8] = { "EA2*EK*" };
static char EA1_EK2[9] = { "EA1*EK2*" };
static char EA2_EK2[9] = { "EA2*EK2*" };
static char EA1_EN[8] = { "EA1*EN*" };
static char EA2_EN[8] = { "EA2*EN*" };
static char EA1_EC1F[10] = { "EA1*EC1F*" };
static char EA2_EC1F[10] = { "EA2*EC1F*" };

static char EA3[5] = { "EA3*" };
static char EA7[5] = { "EA7*" };
static char SD1[5] = { "SD1*" };
static char G85[5] = { "G85*" };
static char SE[5] = { "SE*" };
static char SE0001[5] = { "0001" };
static char DXE[12] = { 'D', 'X', 'E', '*', '1', '*', '1', CR, LF, DLE, ETX };
static char AUTO[5] = { "AUTO" };
static char UNLOCKED[9] = { "UNLOCKED" };
static char MA5_TMP[10] = { "MA5*TEMP*" };
static char SCALE[] = { "*F*" };

#pragma GCC diagnostic pop


typedef enum
{
    LONG = 00,
    LONGNS,
    ASCII,
    SHORT_BCD,
    SHORTI_BCD,
    CHAR_BCD,
    LAST,
    END_OF_SEGMENT
} DEX_DATA_TYPE;


typedef enum
{
    WAIT_FOR_ENQ = 00,
    FIRST_HAND_SHAKE,
    SECOND_HAND_SHAKE,
    BLOCK_TRANSFER_OUT,
    BLOCK_TRANSFER_IN,
    TRANSFER_COMPLETE,
    EXTERNAL_DEX_PORT_TRANSFER,
    SCALE_DATA_COMMS
} DEX_STATES;

#endif /* __DEX_COMM_THREAD_H_ */
/*++
 *=============================================================
 *
 * Copyright © 2024 Wittern
 *
 *=============================================================
*--*/

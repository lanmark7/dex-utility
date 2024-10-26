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
#ifndef __DEX_ACCOUNTING_H_
#define __DEX_ACCOUNTING_H_
#include "dex_db.h"

extern DEX_DATA_STRUCTURE DexDataStructure;

extern void Init_DEX_Accounting ( void );

extern void Reset_Interval_DEX_Accounting ( void );

extern bool UpdateVendSuccessAccounting ( uint16_t selection, uint16_t price, uint16_t discount );

extern uint32_t HistoricalCashTotal ( void );

extern uint32_t HistoricalvendTotal ( void );

extern uint32_t ResettableCashTotal ( void );

extern uint32_t ResettablevendTotal ( void );

extern uint32_t HistoricalValueVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end );

extern uint32_t HistoricalNumberVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end );

extern uint32_t ResettableValueVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end );

extern uint32_t ResettableNumberVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end );

extern uint32_t HistoricalValueCashlessByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end );

extern uint32_t HistoricalNumberCashlessByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end );

extern uint32_t ResettableValueCashlessByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end );

extern uint32_t ResettableNumberCashlessByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end );

extern uint32_t ResettableNumberCashlessTotal ( void );

extern uint32_t ResettableValueCashlessTotal ( void );

extern uint32_t HistoricalNumberCashlessTotal ( void );

extern uint32_t HistoricalValueCashlessTotal ( void );

extern uint32_t ResettableSecondVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end );

extern uint32_t HistoricalSecondVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end );

extern uint32_t ResettableSecondVendsTotal ( void );

extern uint32_t HistoricalSecondVendsTotal ( void );

extern bool FailedToDropAccounting ( uint16_t selection );

extern void FailedToDropAccountingDataSaveToNVM ( void );

extern bool UpdateVendFailedAccounting ( uint16_t selection );

extern uint32_t ResettableBillTotal ( void );

extern uint32_t HistoricalBillTotal ( void );

extern uint32_t HistoricalValueFreeVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end );

extern uint32_t HistoricalNumberFreeVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end );

extern uint32_t ResettableValueFreeVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end );

extern uint32_t ResettableNumberFreeVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end );

extern uint32_t HistoricalNumberFreeVendsTotal ( void );

extern uint32_t HistoricalValueFreeVendsTotal ( void );

extern uint32_t IntervalValueFreeVendsTotal ( void );

extern uint32_t IntervalNumberFreeVendsTotal ( void );

void GetMACHINE_MODEL_NUMBER ( char* machine_model_number );

void SetMACHINE_MODEL_NUMBER ( char* machine_model_number );

void GetVMC_CONTROL_BOARD_BUILD_STANDARD ( char* control_board_build_standard );

void SetVMC_CONTROL_BOARD_BUILD_STANDARD ( char* control_board_build_standard );

void GetMACHINE_BUILD_STANDARD ( char* machine_build_standard );

void SetMACHINE_BUILD_STANDARD ( char* machine_build_standard );

void GetMACHINE_LOCATION ( char* machine_location );

void SetMACHINE_LOCATION ( char* machine_location );

void GetMACHINE_SERIAL_NUMBER ( char* machine_sn );

void SetMACHINE_SERIAL_NUMBER ( const char* machine_sn );

void GetCONTROL_BOARD_SERIAL_NUMBER ( char* cb_sn );

void SetCONTROL_BOARD_SERIAL_NUMBER ( const char* cb_sn );

void GetVMCMODEL_NUMBER ( char* cb_mn );

void SetVMCMODEL_NUMBER ( char* cb_mn );

uint32_t TotalCashInTube ( void );

void DEXConfigAccounting ();

#endif /* __DEX_ACCOUNTING_H_ */
/*++
 *=============================================================
 *
 * Copyright © 2024 Wittern
 *
 *=============================================================
*--*/

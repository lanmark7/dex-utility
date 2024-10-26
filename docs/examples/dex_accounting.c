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
#include <stdio.h>
#include "dex_db.h"
#include <string.h>
#include "WrapperConfiguration.h"
#include "device.h"
#include "dex_accounting.h"
#include "rtc.h"
#include "version.h"
#include "Logger/data_logger.h"
 /**********************************************************************
 *     Global Variable Definitions
 **********************************************************************/

DEX_DATA_STRUCTURE DexDataStructure;

const char MODEL_NUMBER[13] = { "EVOKE000006W" };
const char FW_VERSION[6] = VMC_VERSION;
const char HW_VERSION[6] = { "0020" };
const char USD_VERSION[6] = { "0020" };
const char SERIAL_NUMBER[17] = { "WTN0000000000000" };

const char INIT_SERIAL_NUMBER[20] = { "0000000000000000" };
const char INIT_MODEL_NUMBER[20] = { "0000000000000000" };
const char INIT_REVISION_NUMBER[3] = { 0, 0 };

const char DEX_DATA_STRUCTURE_SIGNATURE[20] = { "EVOKE00000000000026" };

static bool DEX_Configuration_Update;
extern bool mcl1_Session;
extern bool mcl2_Session;
extern bool sbc_cl_Session;
/**********************************************************************
*     Static Variable Definitions
**********************************************************************/
static char DoorOpenHour = 0;
static char DoorOpenMinute = 0;
static char UpdateDEXStructure;

/**********************************************************************
*     Global Function Definitions
**********************************************************************/
void Init_DEX_Accounting ( void );
void Reset_Interval_DEX_Accounting ( void );
void Reset_ALL_DEX_Accounting ( void );
bool UpdateVendSuccessAccounting ( uint16_t selection, uint16_t price, uint16_t discount );
bool UpdateVendFailedAccounting ( uint16_t selection );
void DoorOpenAccounting ( void );
void DoorCloseAccounting ( void );
void DEXFreeVendAccounting ( uint16_t selection );
const char* GetVersionString ( void );
extern uint8_t dbcGetSelectionDigits ( void );
/**********************************************************************
*     Static Function Definitions
**********************************************************************/


/********************************************************************************
;		Function Definition
;
;********************************************************************************/
extern bool dbcGetPriceForItem ( uint16_t item_number, uint16_t* price );
extern bool dbcGetFreeVendEnabled ( void );
extern uint16_t GetmbvScalingFactor ( void );
extern bool GetmdbToken_Accepted ( void );
extern uint16_t GetmdbToken_Credit ( void );
extern uint16_t logger_fput ( const char* buff );
extern uint32_t GetCash_In_Tubes ( void );
extern uint8_t dbcGetMAXColumns ( void );

void Init_DEX_Accounting ( void )
{
    while ( configurationDexDataStructureBase + sizeof ( DexDataStructure ) > CONFIGURATION_END )
    {
        //Configuration Size missmatch - make it obvious and lock up system
    }
    // Restore DEX structure from Configuration
    Configuration_ReadData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
    memset ( &DexDataStructure.DexCoinTubeField, 0x00, sizeof ( DexDataStructure.DexCoinTubeField ) );
    // Compare structure signatures and if not the same wipe out the DEX accounting data
    if ( strcmp ( DEX_DATA_STRUCTURE_SIGNATURE, DexDataStructure.DEX_Data_Signature.DexDataStructureSignature ) != 0 )
    {
        Reset_ALL_DEX_Accounting ();
    }
}


void UpdateDEXAccountingStructure ( void )
{
    if ( UpdateDEXStructure )
    {
        UpdateDEXStructure = false;
        Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
    }
}


uint16_t GetSelectionOffset ( uint16_t selection )
{
    uint16_t selection_offset = 0;
    uint8_t row, col, offset;
    if ( dbcGetSelectionDigits () == 2 )
    {
        if (selection < 10)
        {
            return 0xffff;  //-1
        }
        selection_offset = selection - 10;
    }
    // Convert offset 3 digit and 10col
    else if ( ( dbcGetSelectionDigits () == 3 ) && ( dbcGetMAXColumns () <= 10 ) )
    {
        if (selection < 100)
        {
            return 0xffff;  //-1
        }
        row = ( ( selection / 10 ) - 9 );
        offset = ( ( row - 1 ) * 10 );
        col = selection % 10;
        selection_offset = offset + col;
    }
    else
    {
        if (selection < 100)
        {
            return 0xffff;  //-1
        }
        row = ( selection / 100 );
        offset = ( ( row - 1 ) * 12 );
        col = selection - ( row * 100 );
        if ( col > 11 )
        {
            return 0xffff;  //-1
        }
        selection_offset = offset + col;
    }
    return selection_offset;
}


void Reset_Interval_DEX_Accounting ( void )
{
    memset ( &DexDataStructure.DexResetableFields, 0x00, sizeof ( DexDataStructure.DexResetableFields ) );
    memset ( &DexDataStructure.Dex_PA_ResetableField, 0x00, sizeof ( DexDataStructure.Dex_PA_ResetableField ) );
    memset ( &DexDataStructure.Dex_EA_ProductDispenseResetableField, 0x00, sizeof ( DexDataStructure.Dex_EA_ProductDispenseResetableField ) );
    memset ( &DexDataStructure.Dex_EA_ResetableField, 0x00, sizeof ( DexDataStructure.Dex_EA_ResetableField ) );
    DexDataStructure.Dex_EA_UnResetableField.EA301_NUMBER_OF_READS_WITH_RESET_SINCE_INITIALIZATION++;
    DexDataStructure.Dex_EA_UnResetableField.EA3010_NUMBER_OF_RESETS_SINCE_INITIALIZATION++;
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}


void Reset_ALL_DEX_Accounting ( void )
{
    memset ( &DexDataStructure.DexResetableFields, 0x00, sizeof ( DexDataStructure.DexResetableFields ) );
    memset ( &DexDataStructure.Dex_PA_ResetableField, 0x00, sizeof ( DexDataStructure.Dex_PA_ResetableField ) );
    memset ( &DexDataStructure.Dex_EA_ProductDispenseResetableField, 0x00, sizeof ( DexDataStructure.Dex_EA_ProductDispenseResetableField ) );
    memset ( &DexDataStructure.Dex_EA_ResetableField, 0x00, sizeof ( DexDataStructure.Dex_EA_ResetableField ) );
    memset ( &DexDataStructure.DexUnResetableFields, 0x00, sizeof ( DexDataStructure.DexUnResetableFields ) );
    memset ( &DexDataStructure.DexDeviceIDFields, 0x00, sizeof ( DexDataStructure.DexDeviceIDFields ) );
    memcpy ( DexDataStructure.DexDeviceIDFields.ID102_MACHINE_MODEL_NUMBER, MODEL_NUMBER, MIN( sizeof DexDataStructure.DexDeviceIDFields.ID102_MACHINE_MODEL_NUMBER, sizeof(MODEL_NUMBER) ) );
    memcpy ( DexDataStructure.DexDeviceIDFields.CB103_VMC_CONTROL_BOARD_BUILD_STANDARD, FW_VERSION, MIN( sizeof DexDataStructure.DexDeviceIDFields.CB103_VMC_CONTROL_BOARD_BUILD_STANDARD, sizeof(FW_VERSION) ) );
    memcpy ( DexDataStructure.DexDeviceIDFields.ID103_MACHINE_BUILD_STANDARD, FW_VERSION, sizeof DexDataStructure.DexDeviceIDFields.ID103_MACHINE_BUILD_STANDARD );
    memset ( DexDataStructure.DexDeviceIDFields.ID104_MACHINE_LOCATION, 0x00, sizeof DexDataStructure.DexDeviceIDFields.ID104_MACHINE_LOCATION );
    Configuration_ReadData ( configuration__MachineSerialNumber, (uint8_t*)DexDataStructure.DexDeviceIDFields.ID101_MACHINE_SERIAL_NUMBER, sizeof ( DexDataStructure.DexDeviceIDFields.ID101_MACHINE_SERIAL_NUMBER ) );
    DexDataStructure.DexDeviceIDFields.ID101_MACHINE_SERIAL_NUMBER[20] = 0; // null end of serial number
    if ( DexDataStructure.DexDeviceIDFields.ID101_MACHINE_SERIAL_NUMBER[0] > 0x7f )
    {
        memcpy ( DexDataStructure.DexDeviceIDFields.ID101_MACHINE_SERIAL_NUMBER, SERIAL_NUMBER, MIN( sizeof DexDataStructure.DexDeviceIDFields.ID101_MACHINE_SERIAL_NUMBER, sizeof(SERIAL_NUMBER) ) );
    }
    Configuration_ReadData ( Configuration_ControlBoardSerialNumber, (uint8_t*)DexDataStructure.DexDeviceIDFields.CB101_VMC_CONTROL_BOARD_SERIAL_NUMBER, sizeof ( DexDataStructure.DexDeviceIDFields.CB101_VMC_CONTROL_BOARD_SERIAL_NUMBER ) );
    DexDataStructure.DexDeviceIDFields.CB101_VMC_CONTROL_BOARD_SERIAL_NUMBER[20] = 0; // null end of serial number
    if ( DexDataStructure.DexDeviceIDFields.CB101_VMC_CONTROL_BOARD_SERIAL_NUMBER[0] > 0x7f )
    {
        memcpy ( DexDataStructure.DexDeviceIDFields.CB101_VMC_CONTROL_BOARD_SERIAL_NUMBER, SERIAL_NUMBER, MIN( sizeof DexDataStructure.DexDeviceIDFields.CB101_VMC_CONTROL_BOARD_SERIAL_NUMBER, sizeof(SERIAL_NUMBER) ) );
    }
    memcpy ( DexDataStructure.DexDeviceIDFields.CB102_VMC_CONTROL_BOARD_MODEL_NUMBER, MODEL_NUMBER, MIN( sizeof DexDataStructure.DexDeviceIDFields.CB102_VMC_CONTROL_BOARD_MODEL_NUMBER, sizeof(MODEL_NUMBER) ) );
    memset ( &DexDataStructure.Dex_PA_UnResetableField, 0x00, sizeof ( DexDataStructure.Dex_PA_UnResetableField ) );
    memset ( &DexDataStructure.Dex_EA_ProductDispenseUnResetableField, 0x00, sizeof ( DexDataStructure.Dex_EA_ProductDispenseUnResetableField ) );
    memset ( &DexDataStructure.Dex_EA_UnResetableField, 0x00, sizeof ( DexDataStructure.Dex_EA_UnResetableField ) );
    memcpy ( DexDataStructure.DEX_Data_Signature.DexDataStructureSignature, DEX_DATA_STRUCTURE_SIGNATURE, sizeof DexDataStructure.DEX_Data_Signature.DexDataStructureSignature );
    // Send updated information to the Configuration
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}


bool UpdateVendSuccessAccounting ( uint16_t selection, uint16_t price, uint16_t discount )
{
    uint16_t token_credit;
    uint16_t selection_offset;
    uint8_t row, col, offset;
    RTC rtc_time = { 0 };
    token_credit = GetmdbToken_Credit ();
    if ( dbcGetSelectionDigits () == 2 )
    {
        selection_offset = selection - 10;
    }
    // Convert offset 3 digit and 10col
    else if ( ( dbcGetSelectionDigits () == 3 ) && ( dbcGetMAXColumns () <= 10 ) )
    {
        row = ( ( selection / 10 ) - 9 );
        offset = ( ( row - 1 ) * 10 );
        col = selection % 10;
        selection_offset = offset + col;
    }
    else
    {
        row = ( selection / 100 );
        offset = ( ( row - 1 ) * 12 );
        col = selection - ( row * 100 );
        selection_offset = offset + col;
    }
    if ( GetmdbToken_Accepted () )
    {
        DexDataStructure.DexUnResetableFields.TA201_VALUE_OF_TOKEN_COUPON_SALES_SINCE_INITIALIZATION += price;
        DexDataStructure.DexUnResetableFields.TA202_NUMBER_OF_TOKEN_COUPON_SALES_SINCE_INITIALIZATION += 1;
        DexDataStructure.DexResetableFields.TA203_VALUE_OF_TOKEN_COUPON_SALES_SINCE_LAST_RESET += price;
        DexDataStructure.DexResetableFields.TA204_NUMBER_OF_TOKEN_COUPON_SALES_SINCE_LAST_RESET += 1;
    }
    else if ( token_credit != 0 )
    {
        if ( token_credit < price )
        {
            DexDataStructure.DexResetableFields.CA203_VALUE_OF_CASH_SALES_SINCE_LAST_RESET += ( price - token_credit );
            DexDataStructure.DexResetableFields.CA204_NUMBER_OF_CASH_VENDS_SINCE_LAST_RESET += 1;
            DexDataStructure.DexUnResetableFields.CA201_VALUE_OF_CASH_SALES_SINCE_INITIALIZATION += ( price - token_credit );
            DexDataStructure.DexUnResetableFields.CA202_NUMBER_OF_CASH_VENDS_SINCE_INITIALIZATION += 1;
            DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA705_NUMBER_OF_CASH_SALES_SINCE_INITIALISATION += 1;
            DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA706_VALUE_OF_CASH_SALES_SINCE_INITIALISATION += ( price - token_credit );
            DexDataStructure.Dex_PA_ResetableField[selection_offset].PA707_NUMER_OF_CASH_SALES_SINCE_LAST_RESET += 1;
            DexDataStructure.Dex_PA_ResetableField[selection_offset].PA708_VALUE_OF_CASH_SALES_SINCE_LAST_RESET += ( price - token_credit );
        }
        else
        {
            token_credit = price;
        }
        DexDataStructure.DexUnResetableFields.TA205_VALUE_OF_VALUE_TOKEN_COUPON_SALES_SINCE_INITIALIZATION += token_credit;
        DexDataStructure.DexUnResetableFields.TA206_NUMBER_OF_VALUE_TOKEN_COUPON_SALES_SINCE_INITIALIZATION += 1;
        DexDataStructure.DexResetableFields.TA207_VALUE_OF_VALUE_TOKEN_COUPON_SALES_SINCE_LAST_RESET += token_credit;
        DexDataStructure.DexResetableFields.TA208_NUMBER_OF_VALUE_TOKEN_COUPON_SALES_SINCE_LAST_RESET += 1;
    }
    else if ( !mcl1_Session && !mcl2_Session && !sbc_cl_Session )
    {
        DexDataStructure.DexResetableFields.CA203_VALUE_OF_CASH_SALES_SINCE_LAST_RESET += price;
        DexDataStructure.DexResetableFields.CA204_NUMBER_OF_CASH_VENDS_SINCE_LAST_RESET += 1;
        DexDataStructure.DexUnResetableFields.CA201_VALUE_OF_CASH_SALES_SINCE_INITIALIZATION += price;
        DexDataStructure.DexUnResetableFields.CA202_NUMBER_OF_CASH_VENDS_SINCE_INITIALIZATION += 1;
        DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA705_NUMBER_OF_CASH_SALES_SINCE_INITIALISATION += 1;
        DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA706_VALUE_OF_CASH_SALES_SINCE_INITIALISATION += price;
        DexDataStructure.Dex_PA_ResetableField[selection_offset].PA707_NUMER_OF_CASH_SALES_SINCE_LAST_RESET += 1;
        DexDataStructure.Dex_PA_ResetableField[selection_offset].PA708_VALUE_OF_CASH_SALES_SINCE_LAST_RESET += price;
    }
    else if ( mcl1_Session )
    {
        DexDataStructure.DexUnResetableFields.DA201_VALUE_OF_CASHLESS_1_SALES_SINCE_INITIALIZATION += price;
        DexDataStructure.DexUnResetableFields.DA202_NUMBER_OF_CASHLESS_1_SALES_SINCE_INITIALIZATION += 1;
        DexDataStructure.DexResetableFields.DA203_VALUE_OF_CASHLESS_1_SALES_SINCE_LAST_RESET += price;
        DexDataStructure.DexResetableFields.DA204_NUMBER_OF_CASHLESS_1_SALES_SINCE_LAST_RESET += 1;
        DexDataStructure.DexResetableFields.DA501_VALUE_OF_CASHLESS_1_DISCOUNTS_SINCE_LAST_RESET += discount;
        DexDataStructure.DexUnResetableFields.DA503_VALUE_OF_CASHLESS_1_DISCOUNTS_SINCE_INITIALISATION += discount;
        DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA705_NUMBER_OF_CASHLESS_SALES_SINCE_INITIALISATION += 1;
        DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA706_VALUE_OF_CASHLESS_SALES_SINCE_INITIALISATION += price;
        DexDataStructure.Dex_PA_ResetableField[selection_offset].PA707_NUMER_OF_CASHLESS_SALES_SINCE_LAST_RESET += 1;
        DexDataStructure.Dex_PA_ResetableField[selection_offset].PA708_VALUE_OF_CASHLESS_SALES_SINCE_LAST_RESET += price;
        if ( discount != 0 )
        {
            DexDataStructure.DexUnResetableFields.DA504_NUMBER_OF_DISCOUNT_CASHLESS_1_VENDS_SINCE_INITIALISATION += 1;
            DexDataStructure.DexResetableFields.DA502_NUMBER_OF_DISCOUNT_CASHLESS_1_VENDS_SINCE_LAST_RESET += 1;
        }
        logger_fput ( CASH_LESS_1_TXT );
    }
    else if ( ( mcl2_Session ) || ( sbc_cl_Session ) )
    {
        DexDataStructure.DexUnResetableFields.DB201_VALUE_OF_CASHLESS_2_SALES_SINCE_INITIALIZATION += price;
        DexDataStructure.DexUnResetableFields.DB202_NUMBER_OF_CASHLESS_2_SALES_SINCE_INITIALIZATION += 1;
        DexDataStructure.DexResetableFields.DB203_VALUE_OF_CASHLESS_2_SALES_SINCE_LAST_RESET += price;
        DexDataStructure.DexResetableFields.DB204_NUMBER_OF_CASHLESS_2_SALES_SINCE_LAST_RESET += 1;
        DexDataStructure.DexResetableFields.DB501_VALUE_OF_CASHLESS_2_DISCOUNTS_SINCE_LAST_RESET += discount;
        DexDataStructure.DexUnResetableFields.DB503_VALUE_OF_CASHLESS_2_DISCOUNTS_SINCE_INITIALISATION += discount;
        DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA705_NUMBER_OF_CASHLESS_SALES_SINCE_INITIALISATION += 1;
        DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA706_VALUE_OF_CASHLESS_SALES_SINCE_INITIALISATION += price;
        DexDataStructure.Dex_PA_ResetableField[selection_offset].PA707_NUMER_OF_CASHLESS_SALES_SINCE_LAST_RESET += 1;
        DexDataStructure.Dex_PA_ResetableField[selection_offset].PA708_VALUE_OF_CASHLESS_SALES_SINCE_LAST_RESET += price;
        if ( discount != 0 )
        {
            DexDataStructure.DexUnResetableFields.DB504_NUMBER_OF_DISCOUNT_CASHLESS_2_VENDS_SINCE_INITIALISATION += 1;
            DexDataStructure.DexResetableFields.DB502_NUMBER_OF_DISCOUNT_CASHLESS_2_VENDS_SINCE_LAST_RESET += 1;
        }
        logger_fput ( CASH_LESS_2_TXT );
    }
    DexDataStructure.DexResetableFields.VA103_VALUE_OF_ALL_PAID_VEND_SALES_SINCE_LAST_RESET += price;
    DexDataStructure.DexResetableFields.VA104_NUMBER_OF_ALL_PAID_VEND_SALES_SINCE_LAST_RESET += 1;
    DexDataStructure.DexResetableFields.VA107_VALUE_OF_ALL_DISCOUNTS_SINCE_LAST_RESET += discount;
    DexDataStructure.DexUnResetableFields.VA101_VALUE_OF_ALL_PAID_VEND_SALES_SINCE_INITIALIZATION += price;
    DexDataStructure.DexUnResetableFields.VA102_NUMBER_OF_ALL_PAID_VEND_SALES_SINCE_INITIALIZATION += 1;
    DexDataStructure.DexUnResetableFields.VA105_VALUE_OF_ALL_DISCOUNTS_SINCE_SINCE_INITIALIZATION += discount;
    DexDataStructure.Dex_PA_ResetableField[selection_offset].PA208_VALUE_OF_DISCOUNTS_SINCE_LAST_RESET += discount;
    DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA206_VALUE_OF_DISCOUNTS_SINCE_INITIALIZATION += discount;
    if ( discount != 0 )
    {
        DexDataStructure.DexUnResetableFields.VA106_NUMBER_OF_ALL_DISCOUNTED_PAID_VENDS_SINCE_INITIALIZATION += 1;
        DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA205_NUMBER_OF_DISCOUNTS_SINCE_INITIALIZATION += 1;
        DexDataStructure.Dex_PA_ResetableField[selection_offset].PA207_NUMBER_OF_DISCOUNTS_SINCE_LAST_RESET += 1;
    }
    DexDataStructure.Dex_PA_ResetableField[selection_offset].PA203_NUMBER_OF_PAID_PRODUCTS_VENDED_SINCE_LAST_RESET += 1;
    DexDataStructure.Dex_PA_ResetableField[selection_offset].PA204_VALUE_OF_PAID_PRODUCTS_VENDED_SINCE_LAST_RESET += price;
    DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA201_NUMBER_OF_PAID_PRODUCTS_VENDED_SINCE_INITIALIZATION += 1;
    DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA202_VALUE_OF_PAID_PRODUCTS_VENDED_SINCE_INITIALIZATION += price;
    rtc_gettime ( &rtc_time );
    DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA501_DATEYEAR_OF_MOST_RECENT_SALE = rtc_time.year;
    DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA501_DATEMONTH_OF_MOST_RECENT_SALE = rtc_time.month;
    DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA501_DATEDAY_OF_MOST_RECENT_SALE = rtc_time.mday;
    DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA502_TIMEHOUR_OF_MOST_RECENT_SALE = rtc_time.hour;
    DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA502_TIMEMINUTE_OF_MOST_RECENT_SALE = rtc_time.min;

    UpdateDEXStructure = true;
    return 0;
}

static bool failedToDropValueChanged;


bool FailedToDropAccounting ( uint16_t selection )
{
    RTC rtc_time = { 0 };
    uint16_t selection_offset;
    uint8_t row, col, offset;
    if ( dbcGetSelectionDigits () == 2 )
    {
        selection_offset = selection - 10;
    }
    // Convert offset 3 digit and 10col
    else if ( ( dbcGetSelectionDigits () == 3 ) && ( dbcGetMAXColumns () <= 10 ) )
    {
        row = ( ( selection / 10 ) - 9 );
        offset = ( ( row - 1 ) * 10 );
        col = selection % 10;
        selection_offset = offset + col;
    }
    else
    {
        row = ( selection / 100 );
        offset = ( ( row - 1 ) * 12 );
        col = selection - ( row * 100 );
        selection_offset = offset + col;
    }
    if ( selection_offset >= 0 && selection_offset < MAX_MOTOR_COUNT )
    {
        DexDataStructure.Dex_EA_ProductDispenseResetableField[selection_offset].EA202_EJL_NUMBER_OF_EVENTS_SINCE_LAST_RESET++;
        DexDataStructure.Dex_EA_ProductDispenseUnResetableField[selection_offset].EA203_EJL_NUMBER_OF_EVENTS_SINCE_INITIALIZATION++;
        rtc_gettime ( &rtc_time );
        DexDataStructure.Dex_EA_ProductDispenseUnResetableField[selection_offset].EA102_EJL_DATEYEAR_OF_EVENT = rtc_time.year;
        DexDataStructure.Dex_EA_ProductDispenseUnResetableField[selection_offset].EA102_EJL_DATEMONTH_OF_EVENT = rtc_time.month;
        DexDataStructure.Dex_EA_ProductDispenseUnResetableField[selection_offset].EA102_EJL_DATEDAY_OF_EVENT = rtc_time.mday;
        DexDataStructure.Dex_EA_ProductDispenseUnResetableField[selection_offset].EA103_EJL_TIMEHOUR_OF_EVENT = rtc_time.hour;
        DexDataStructure.Dex_EA_ProductDispenseUnResetableField[selection_offset].EA103_EJL_TIMEMINUTE_OF_EVENT = rtc_time.min;
        DexDataStructure.DexResetableFields.EA202_EJL_TOTAL_NUMBER_OF_EVENTS_SINCE_LAST_RESET++;
        DexDataStructure.DexUnResetableFields.EA202_EJL_TOTAL_NUMBER_OF_EVENTS_SINCE_INITIALIZATION++;
        failedToDropValueChanged = true;
    }
    return 0;
}

void FailedToDropAccountingDataSaveToNVM ( void )
{
    if ( failedToDropValueChanged == true )
    {
        failedToDropValueChanged = false;
        UpdateDEXStructure = true;
    }
}


bool UpdateVendFailedAccounting ( uint16_t selection )
{
    RTC rtc_time = { 0 };
    uint16_t selection_offset;
    uint8_t row, col, offset;
    if ( dbcGetSelectionDigits () == 2 )
    {
        selection_offset = selection - 10;
    }
    // Convert offset 3 digit and 10col
    else if ( ( dbcGetSelectionDigits () == 3 ) && ( dbcGetMAXColumns () <= 10 ) )
    {
        row = ( ( selection / 10 ) - 9 );
        offset = ( ( row - 1 ) * 10 );
        col = selection % 10;
        selection_offset = offset + col;
    }
    else
    {
        row = ( selection / 100 );
        offset = ( ( row - 1 ) * 12 );
        col = selection - ( row * 100 );
        selection_offset = offset + col;
    }

    if ( selection_offset >= 0 && selection_offset < MAX_MOTOR_COUNT )
    {
        DexDataStructure.Dex_EA_ProductDispenseResetableField[selection_offset].EA202_ELA_NUMBER_OF_EVENTS_SINCE_LAST_RESET++;
        DexDataStructure.Dex_EA_ProductDispenseUnResetableField[selection_offset].EA203_ELA_NUMBER_OF_EVENTS_SINCE_INITIALIZATION++;
        rtc_gettime ( &rtc_time );
        DexDataStructure.Dex_EA_ProductDispenseUnResetableField[selection_offset].EA102_ELA_DATEYEAR_OF_EVENT = rtc_time.year;
        DexDataStructure.Dex_EA_ProductDispenseUnResetableField[selection_offset].EA102_ELA_DATEMONTH_OF_EVENT = rtc_time.month;
        DexDataStructure.Dex_EA_ProductDispenseUnResetableField[selection_offset].EA102_ELA_DATEDAY_OF_EVENT = rtc_time.mday;
        DexDataStructure.Dex_EA_ProductDispenseUnResetableField[selection_offset].EA103_ELA_TIMEHOUR_OF_EVENT = rtc_time.hour;
        DexDataStructure.Dex_EA_ProductDispenseUnResetableField[selection_offset].EA103_ELA_TIMEMINUTE_OF_EVENT = rtc_time.min;
        DexDataStructure.DexResetableFields.EA202_ELA_TOTAL_NUMBER_OF_EVENTS_SINCE_LAST_RESET++;
        DexDataStructure.DexUnResetableFields.EA202_ELA_TOTAL_NUMBER_OF_EVENTS_SINCE_INITIALIZATION++;
        UpdateDEXStructure = true;
    }
    return 0;
}



void CashOverpayAccounting ( uint16_t overpay )
{
    DexDataStructure.DexUnResetableFields.CA802_VALUE_OF_CASH_OVERPAY_SINCE_INITIALIZATION += overpay;
    DexDataStructure.DexResetableFields.CA801_VALUE_OF_CASH_OVERPAY_SINCE_LAST_RESET += overpay;
    UpdateDEXStructure = true;
}


void DoorOpenAccounting ( void )
{
    RTC rtc_time = { 0 };
    rtc_gettime ( &rtc_time );
    DoorOpenHour = rtc_time.hour;
    DoorOpenMinute = rtc_time.min;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EGS_DATEYEAR_OF_EVENT = rtc_time.year;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EGS_DATEMONTH_OF_EVENT = rtc_time.month;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EGS_DATEDAY_OF_EVENT = rtc_time.mday;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EGS_TIMEHOUR_OF_EVENT = rtc_time.hour;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EGS_TIMEMINUTE_OF_EVENT = rtc_time.min;
    DexDataStructure.Dex_EA_UnResetableField.EA203_EGS_NUMBER_OF_EVENTS_SINCE_INITIALIZATION = DexDataStructure.Dex_EA_UnResetableField.EA203_EGS_NUMBER_OF_EVENTS_SINCE_INITIALIZATION + 1;
    DexDataStructure.Dex_EA_ResetableField.EA202_EGS_NUMBER_OF_EVENTS_SINCE_LAST_RESET = DexDataStructure.Dex_EA_ResetableField.EA202_EGS_NUMBER_OF_EVENTS_SINCE_LAST_RESET + 1;
    UpdateDEXStructure = true;
}



void DoorCloseAccounting ( void )
{
    uint16_t  dooropen_min_of_day, doorclose_min_of_day;
    RTC rtc_time = { 0 };
    rtc_gettime ( &rtc_time );
    dooropen_min_of_day = DoorOpenMinute + ( DoorOpenHour * 60 );
    doorclose_min_of_day = rtc_time.min + ( rtc_time.hour * 60 );
    if ( ( dooropen_min_of_day != 0 ) && ( doorclose_min_of_day > dooropen_min_of_day ) )
    {
        DexDataStructure.Dex_EA_UnResetableField.EA206_EGS_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES = doorclose_min_of_day - dooropen_min_of_day;
        UpdateDEXStructure = true;
    }
}


void TemperatureSensorFailAccounting ( void )
{
    RTC rtc_time = { 0 };
    rtc_gettime ( &rtc_time );
    DexDataStructure.Dex_EA_UnResetableField.EA102_EJK_DATEYEAR_OF_EVENT = rtc_time.year;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EJK_DATEMONTH_OF_EVENT = rtc_time.month;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EJK_DATEDAY_OF_EVENT = rtc_time.mday;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EJK_TIMEHOUR_OF_EVENT = rtc_time.hour;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EJK_TIMEMINUTE_OF_EVENT = rtc_time.min;
    DexDataStructure.Dex_EA_UnResetableField.EA203_EJK_NUMBER_OF_EVENTS_SINCE_INITIALIZATION = DexDataStructure.Dex_EA_UnResetableField.EA203_EJK_NUMBER_OF_EVENTS_SINCE_INITIALIZATION + 1;
    DexDataStructure.Dex_EA_ResetableField.EA202_EJK_NUMBER_OF_EVENTS_SINCE_LAST_RESET = DexDataStructure.Dex_EA_ResetableField.EA202_EJK_NUMBER_OF_EVENTS_SINCE_LAST_RESET + 1;
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}


void HealthFailAccounting ( void )
{
    RTC rtc_time = { 0 };
    rtc_gettime ( &rtc_time );
    DexDataStructure.Dex_EA_UnResetableField.EA102_EJH_DATEYEAR_OF_EVENT = rtc_time.year;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EJH_DATEMONTH_OF_EVENT = rtc_time.month;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EJH_DATEDAY_OF_EVENT = rtc_time.mday;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EJH_TIMEHOUR_OF_EVENT = rtc_time.hour;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EJH_TIMEMINUTE_OF_EVENT = rtc_time.min;
    DexDataStructure.Dex_EA_UnResetableField.EA203_EJH_NUMBER_OF_EVENTS_SINCE_INITIALIZATION = DexDataStructure.Dex_EA_UnResetableField.EA203_EJH_NUMBER_OF_EVENTS_SINCE_INITIALIZATION + 1;
    DexDataStructure.Dex_EA_ResetableField.EA202_EJH_NUMBER_OF_EVENTS_SINCE_LAST_RESET = DexDataStructure.Dex_EA_ResetableField.EA202_EJH_NUMBER_OF_EVENTS_SINCE_LAST_RESET + 1;
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}


void PullDownFailAccounting ( void )
{
    RTC rtc_time = { 0 };
    rtc_gettime ( &rtc_time );
    DexDataStructure.Dex_EA_UnResetableField.EA102_EJJ_DATEYEAR_OF_EVENT = rtc_time.year;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EJJ_DATEMONTH_OF_EVENT = rtc_time.month;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EJJ_DATEDAY_OF_EVENT = rtc_time.mday;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EJJ_TIMEHOUR_OF_EVENT = rtc_time.hour;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EJJ_TIMEMINUTE_OF_EVENT = rtc_time.min;
    DexDataStructure.Dex_EA_UnResetableField.EA203_EJJ_NUMBER_OF_EVENTS_SINCE_INITIALIZATION = DexDataStructure.Dex_EA_UnResetableField.EA203_EJJ_NUMBER_OF_EVENTS_SINCE_INITIALIZATION + 1;
    DexDataStructure.Dex_EA_ResetableField.EA202_EJJ_NUMBER_OF_EVENTS_SINCE_LAST_RESET = DexDataStructure.Dex_EA_ResetableField.EA202_EJJ_NUMBER_OF_EVENTS_SINCE_LAST_RESET + 1;
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}


void BillValidatorFailAccounting ( void )
{
    RTC rtc_time = { 0 };
    rtc_gettime ( &rtc_time );
    DexDataStructure.Dex_EA_UnResetableField.EA102_EN_DATEYEAR_OF_EVENT = rtc_time.year;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EN_DATEMONTH_OF_EVENT = rtc_time.month;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EN_DATEDAY_OF_EVENT = rtc_time.mday;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EN_TIMEHOUR_OF_EVENT = rtc_time.hour;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EN_TIMEMINUTE_OF_EVENT = rtc_time.min;
    DexDataStructure.Dex_EA_UnResetableField.EA203_EN_NUMBER_OF_EVENTS_SINCE_INITIALIZATION = DexDataStructure.Dex_EA_UnResetableField.EA203_EN_NUMBER_OF_EVENTS_SINCE_INITIALIZATION + 1;
    DexDataStructure.Dex_EA_ResetableField.EA202_EN_NUMBER_OF_EVENTS_SINCE_LAST_RESET = DexDataStructure.Dex_EA_ResetableField.EA202_EN_NUMBER_OF_EVENTS_SINCE_LAST_RESET + 1;
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}


void CoinMechFailAccounting ( void )
{
    RTC rtc_time = { 0 };
    rtc_gettime ( &rtc_time );
    DexDataStructure.Dex_EA_UnResetableField.EA102_EA_DATEYEAR_OF_EVENT = rtc_time.year;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EA_DATEMONTH_OF_EVENT = rtc_time.month;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EA_DATEDAY_OF_EVENT = rtc_time.mday;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EA_TIMEHOUR_OF_EVENT = rtc_time.hour;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EA_TIMEMINUTE_OF_EVENT = rtc_time.min;
    DexDataStructure.Dex_EA_UnResetableField.EA203_EA_NUMBER_OF_EVENTS_SINCE_INITIALIZATION = DexDataStructure.Dex_EA_UnResetableField.EA203_EA_NUMBER_OF_EVENTS_SINCE_INITIALIZATION + 1;
    DexDataStructure.Dex_EA_ResetableField.EA202_EA_NUMBER_OF_EVENTS_SINCE_LAST_RESET = DexDataStructure.Dex_EA_ResetableField.EA202_EA_NUMBER_OF_EVENTS_SINCE_LAST_RESET + 1;
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}



void Cashless1FailAccounting ( void )
{
    RTC rtc_time = { 0 };
    rtc_gettime ( &rtc_time );
    DexDataStructure.Dex_EA_UnResetableField.EA102_EK_DATEYEAR_OF_EVENT = rtc_time.year;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EK_DATEMONTH_OF_EVENT = rtc_time.month;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EK_DATEDAY_OF_EVENT = rtc_time.mday;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EK_TIMEHOUR_OF_EVENT = rtc_time.hour;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EK_TIMEMINUTE_OF_EVENT = rtc_time.min;
    DexDataStructure.Dex_EA_UnResetableField.EA203_EK_NUMBER_OF_EVENTS_SINCE_INITIALIZATION = DexDataStructure.Dex_EA_UnResetableField.EA203_EK_NUMBER_OF_EVENTS_SINCE_INITIALIZATION + 1;
    DexDataStructure.Dex_EA_ResetableField.EA202_EK_NUMBER_OF_EVENTS_SINCE_LAST_RESET = DexDataStructure.Dex_EA_ResetableField.EA202_EK_NUMBER_OF_EVENTS_SINCE_LAST_RESET + 1;
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}


void Cashless2FailAccounting ( void )
{
    RTC rtc_time = { 0 };
    rtc_gettime ( &rtc_time );
    DexDataStructure.Dex_EA_UnResetableField.EA102_EK2_DATEYEAR_OF_EVENT = rtc_time.year;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EK2_DATEMONTH_OF_EVENT = rtc_time.month;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EK2_DATEDAY_OF_EVENT = rtc_time.mday;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EK2_TIMEHOUR_OF_EVENT = rtc_time.hour;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EK2_TIMEMINUTE_OF_EVENT = rtc_time.min;
    DexDataStructure.Dex_EA_UnResetableField.EA203_EK2_NUMBER_OF_EVENTS_SINCE_INITIALIZATION = DexDataStructure.Dex_EA_UnResetableField.EA203_EK2_NUMBER_OF_EVENTS_SINCE_INITIALIZATION + 1;
    DexDataStructure.Dex_EA_ResetableField.EA202_EK2_NUMBER_OF_EVENTS_SINCE_LAST_RESET = DexDataStructure.Dex_EA_ResetableField.EA202_EK2_NUMBER_OF_EVENTS_SINCE_LAST_RESET + 1;
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}



void KeyboardFailAccounting ( void )
{
    RTC rtc_time = { 0 };
    rtc_gettime ( &rtc_time );
    DexDataStructure.Dex_EA_UnResetableField.EA102_EC1F_DATEYEAR_OF_EVENT = rtc_time.year;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EC1F_DATEMONTH_OF_EVENT = rtc_time.month;
    DexDataStructure.Dex_EA_UnResetableField.EA102_EC1F_DATEDAY_OF_EVENT = rtc_time.mday;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EC1F_TIMEHOUR_OF_EVENT = rtc_time.hour;
    DexDataStructure.Dex_EA_UnResetableField.EA103_EC1F_TIMEMINUTE_OF_EVENT = rtc_time.min;
    DexDataStructure.Dex_EA_UnResetableField.EA203_EC1F_NUMBER_OF_EVENTS_SINCE_INITIALIZATION = DexDataStructure.Dex_EA_UnResetableField.EA203_EC1F_NUMBER_OF_EVENTS_SINCE_INITIALIZATION + 1;
    DexDataStructure.Dex_EA_ResetableField.EA202_EC1F_NUMBER_OF_EVENTS_SINCE_LAST_RESET = DexDataStructure.Dex_EA_ResetableField.EA202_EC1F_NUMBER_OF_EVENTS_SINCE_LAST_RESET + 1;
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}


void PowerOutageAccounting ( void )
{
    DexDataStructure.Dex_EA_UnResetableField.EA702_NUMBER_OF_POWER_OUTAGES_SINCE_INITIALIZATION++;
    DexDataStructure.Dex_EA_ResetableField.EA701_NUMBER_OF_POWER_OUTAGES_SINCE_LAST_RESET++;
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}



void DEXReadAccounting ( void )
{
    RTC rtc_time = { 0 };
    rtc_gettime ( &rtc_time );
    DexDataStructure.Dex_EA_UnResetableField.EA302_DATEYEAR_OF_THIS_READOUT = rtc_time.year;
    DexDataStructure.Dex_EA_UnResetableField.EA302_DATEMONTH_OF_THIS_READOUT = rtc_time.month;
    DexDataStructure.Dex_EA_UnResetableField.EA302_DATEDAY_OF_THIS_READOUT = rtc_time.mday;
    DexDataStructure.Dex_EA_UnResetableField.EA303_TIMEHOUR_OF_THIS_READOUT = rtc_time.hour;
    DexDataStructure.Dex_EA_UnResetableField.EA303_TIMEMINUTE_OF_THIS_READOUT = rtc_time.min;
    DexDataStructure.Dex_EA_UnResetableField.EA309_NUMBER_OF_READS_WITH_OR_WITHOUT_RESET_SINCE_INITIALIZATION++;
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}



void DEXReadTimeAccounting ( void )
{
    RTC rtc_time = { 0 };
    rtc_gettime ( &rtc_time );
    DexDataStructure.Dex_EA_UnResetableField.EA305_DATEYEAR_OF_LAST_READ_OUT = DexDataStructure.Dex_EA_UnResetableField.EA302_DATEYEAR_OF_THIS_READOUT;
    DexDataStructure.Dex_EA_UnResetableField.EA305_DATEMONTH_OF_LAST_READ_OUT = DexDataStructure.Dex_EA_UnResetableField.EA302_DATEMONTH_OF_THIS_READOUT;
    DexDataStructure.Dex_EA_UnResetableField.EA305_DATEDAY_OF_LAST_READ_OUT = DexDataStructure.Dex_EA_UnResetableField.EA302_DATEDAY_OF_THIS_READOUT;
    DexDataStructure.Dex_EA_UnResetableField.EA306_TIMEHOUR_OF_LAST_READ_OUT = DexDataStructure.Dex_EA_UnResetableField.EA303_TIMEHOUR_OF_THIS_READOUT;
    DexDataStructure.Dex_EA_UnResetableField.EA306_TIMEMINUTE_OF_LAST_READ_OUT = DexDataStructure.Dex_EA_UnResetableField.EA303_TIMEMINUTE_OF_THIS_READOUT;
    DexDataStructure.Dex_EA_UnResetableField.EA302_DATEYEAR_OF_THIS_READOUT = rtc_time.year;
    DexDataStructure.Dex_EA_UnResetableField.EA302_DATEMONTH_OF_THIS_READOUT = rtc_time.month;
    DexDataStructure.Dex_EA_UnResetableField.EA302_DATEDAY_OF_THIS_READOUT = rtc_time.mday;
    DexDataStructure.Dex_EA_UnResetableField.EA303_TIMEHOUR_OF_THIS_READOUT = rtc_time.hour;
    DexDataStructure.Dex_EA_UnResetableField.EA303_TIMEMINUTE_OF_THIS_READOUT = rtc_time.min;
}



void DEXTestVendAccounting ( uint16_t selection )
{
    uint16_t price;
    uint16_t selection_offset;
    uint8_t row, col, offset;
    if ( dbcGetSelectionDigits () == 2 )
    {
        selection_offset = selection - 10;
    }
    // Convert offset 3 digit and 10col
    else if ( ( dbcGetSelectionDigits () == 3 ) && ( dbcGetMAXColumns () <= 10 ) )
    {
        row = ( ( selection / 10 ) - 9 );
        offset = ( ( row - 1 ) * 10 );
        col = selection % 10;
        selection_offset = offset + col;
    }
    else
    {
        row = ( selection / 100 );
        offset = ( ( row - 1 ) * 12 );
        col = selection - ( row * 100 );
        selection_offset = offset + col;
    }
    logger_fput ( TEST_VEND_TXT );
    dbcGetPriceForItem ( selection, &price );
    DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA301_NUMBER_OF_TEST_VENDS_SINCE_INITIALIZATION++;
    DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA302_VALUE_OF_TEST_VENDS_SINCE_INITIALIZATION = DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA302_VALUE_OF_TEST_VENDS_SINCE_INITIALIZATION + price;
    DexDataStructure.Dex_PA_ResetableField[selection_offset].PA303_NUMBER_OF_TEST_VENDS_SINCE_LAST_RESET++;
    DexDataStructure.Dex_PA_ResetableField[selection_offset].PA304_VALUE_OF_TEST_VENDS_SINCE_LAST_RESET = DexDataStructure.Dex_PA_ResetableField[selection_offset].PA304_VALUE_OF_TEST_VENDS_SINCE_LAST_RESET + price;
    DexDataStructure.DexResetableFields.VA203_TEST_VEND_VALUE_OF_SALES_SINCE_LAST_RESET += price;
    DexDataStructure.DexResetableFields.VA204_NUMBER_OF_TEST_VENDS_SINCE_LAST_RESET += 1;
    DexDataStructure.DexUnResetableFields.VA201_TEST_VEND_VALUE_OF_SALES_SINCE_INITIALIZATION += price;
    DexDataStructure.DexUnResetableFields.VA202_NUMBER_OF_TEST_VENDS_SINCE_INITIALIZATION += 1;
    UpdateDEXStructure = true;
}


void DEXFreeVendAccounting ( uint16_t selection )
{
    uint16_t price;
    uint16_t selection_offset;
    uint8_t row, col, offset;
    if ( dbcGetSelectionDigits () == 2 )
    {
        selection_offset = selection - 10;
    }
    // Convert offset 3 digit and 10col
    else if ( ( dbcGetSelectionDigits () == 3 ) && ( dbcGetMAXColumns () <= 10 ) )
    {
        row = ( ( selection / 10 ) - 9 );
        offset = ( ( row - 1 ) * 10 );
        col = selection % 10;
        selection_offset = offset + col;
    }
    else
    {
        row = ( selection / 100 );
        offset = ( ( row - 1 ) * 12 );
        col = selection - ( row * 100 );
        selection_offset = offset + col;
    }
    logger_fput ( FREE_VEND_TXT );
    dbcGetPriceForItem ( selection, &price );
    DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA401_NUMBER_OF_FREE_VENDS_SINCE_INITIALIZATION++;
    DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA402_VALUE_OF_FREE_VENDS_SINCE_INITIALIZATION = DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA402_VALUE_OF_FREE_VENDS_SINCE_INITIALIZATION + price;
    DexDataStructure.Dex_PA_ResetableField[selection_offset].PA403_NUMBER_OF_FREE_VENDS_SINCE_LAST_RESET++;
    DexDataStructure.Dex_PA_ResetableField[selection_offset].PA404_VALUE_OF_FREE_VENDS_SINCE_LAST_RESET = DexDataStructure.Dex_PA_ResetableField[selection_offset].PA404_VALUE_OF_FREE_VENDS_SINCE_LAST_RESET + price;
    DexDataStructure.DexResetableFields.VA303_FREE_VEND_VALUE_OF_SALES_SINCE_LAST_RESET += price;
    DexDataStructure.DexResetableFields.VA304_NUMBER_OF_FREE_VEND_SALES_SINCE_LAST_RESET += 1;
    DexDataStructure.DexUnResetableFields.VA301_FREE_VEND_VALUE_OF_SALES_SINCE_INITIALIZATION += price;
    DexDataStructure.DexUnResetableFields.VA302_NUMBER_OF_FREE_VEND_SALES_SINCE_INITIALIZATION += 1;
    UpdateDEXStructure = true;
}


void DEXConfigAccounting ()
{
    if ( DEX_Configuration_Update )
    {
        DEX_Configuration_Update = false;
        Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
    }
}


void DEXCashToTubesAccounting ( uint16_t value )
{
    if(GetOperate_State() == SERVICE_MODE)
    { 
        DexDataStructure.DexResetableFields.CA1001_VALUE_OF_CASH_FILLED_SINCE_LAST_RESET += value; 			
        DexDataStructure.DexUnResetableFields.CA1002_VALUE_OF_CASH_FILLED_SINCE_INITIALIZATION += value;
    }
    LoggerAddEntry ( COIN_ACCEPTED_COIN_TUBE_TXT, value );
    DexDataStructure.DexResetableFields.CA303_VALUE_OF_CASH_TO_TUBES_SINCE_LAST_RESET += value;
    DexDataStructure.DexResetableFields.CA301_VALUE_OF_CASH_IN_SINCE_LAST_RESET += value;
    DexDataStructure.DexUnResetableFields.CA307_VALUE_OF_CASH_TO_TUBES_SINCE_INITIALIZATION += value;
    DexDataStructure.DexUnResetableFields.CA305_VALUE_OF_CASH_IN_SINCE_INITIALIZATION += value;
    DexDataStructure.DexResetableFields.COINS_ACCEPTED_SINCE_LAST_RESET += 1;
    DEX_Configuration_Update = true;
}



void CoinRejectedAccounting ( void )
{
    DexDataStructure.DexResetableFields.COINS_REJECTED_SINCE_LAST_RESET += 1;
    DEX_Configuration_Update = true;
}



void BillRejectedAccounting ( void )
{
    DexDataStructure.DexResetableFields.BILLS_REJECTED_SINCE_LAST_RESET += 1;
    DEX_Configuration_Update = true;
}



void DEXCashToBoxAccounting ( uint16_t value )
{
    if(GetOperate_State() == SERVICE_MODE)
    { 
        DexDataStructure.DexResetableFields.CA1001_VALUE_OF_CASH_FILLED_SINCE_LAST_RESET += value; 			
        DexDataStructure.DexUnResetableFields.CA1002_VALUE_OF_CASH_FILLED_SINCE_INITIALIZATION += value;
    }
    LoggerAddEntry ( COIN_ACCEPTED_COIN_BOX_TXT, value );
    DexDataStructure.DexResetableFields.CA302_VALUE_OF_CASH_TO_CASH_BOX_SINCE_LAST_RESET += value;
    DexDataStructure.DexResetableFields.CA301_VALUE_OF_CASH_IN_SINCE_LAST_RESET += value;
    DexDataStructure.DexUnResetableFields.CA305_VALUE_OF_CASH_IN_SINCE_INITIALIZATION += value;
    DexDataStructure.DexUnResetableFields.CA306_VALUE_OF_CASH_TO_BOX_SINCE_INITIALIZATION += value;
    DexDataStructure.DexResetableFields.COINS_ACCEPTED_SINCE_LAST_RESET += 1;
    DEX_Configuration_Update = true;
}


void DEXDispensedAccounting ( uint16_t value )
{
    //logger_fput(COIN_RETURN_TXT); 
    LoggerAddEntry ( COIN_RETURN_TXT, value );
    DexDataStructure.DexResetableFields.CA401_VALUE_OF_CASH_DISPENSED_SINCE_LAST_RESET += value;
    DexDataStructure.DexUnResetableFields.CA403_VALUE_OF_CASH_DISPENSED_SINCE_INITIALIZATION += value;
    DEX_Configuration_Update = true;
}


void DEXManualDispensedAccounting ( uint16_t value )
{
    logger_fput ( COIN_RETURN_MANUAL_TXT );
    DexDataStructure.DexResetableFields.CA402_VALUE_OF_MANUAL_CASH_DISPENSED_SINCE_LAST_RESET += value;
    DexDataStructure.DexUnResetableFields.CA404_VALUE_OF_MANUAL_CASH_DISPENSED_SINCE_INITIALIZATION += value;
    DEXDispensedAccounting ( value );
}



void DEXBillsToStackerAccounting ( uint16_t value )
{
    //CA10 stuff
    if(GetOperate_State() == SERVICE_MODE)
    {
        DexDataStructure.DexResetableFields.CA1003_VALUE_OF_BILLS_FILLED_SINCE_LAST_RESET += value;    
        DexDataStructure.DexUnResetableFields.CA1004_VALUE_OF_BILLS_FILLED_SINCE_INITIALISATION += value/GetmbvScalingFactor();
    }
    LoggerAddEntry ( BILL_ACCEPTED_BVS_TXT, value );
    DexDataStructure.DexResetableFields.CA304_VALUE_OF_BILLS_IN_SINCE_LAST_RESET += value / GetmbvScalingFactor ();
    DexDataStructure.DexResetableFields.CA309_VALUE_OF_BILLS_IN_SINCE_LAST_RESET += value;
    DexDataStructure.DexUnResetableFields.CA308_VALUE_OF_BILLS_IN_SINCE_INITIALIZATION += value / GetmbvScalingFactor ();
    DexDataStructure.DexUnResetableFields.CA310_VALUE_OF_BILLS_IN_SINCE_INITIALIZATION += value;
    DexDataStructure.DexResetableFields.BILLS_ACCEPTED_SINCE_LAST_RESET += 1;
    DEX_Configuration_Update = true;
}



uint8_t GetCoinAcceptanceRate ( void )
{
    static float a, b;
    static uint8_t rate;
    static double c;

    if ( DexDataStructure.DexResetableFields.COINS_REJECTED_SINCE_LAST_RESET == 0 )
    {
        rate = 100;
    }
    else
    {
        a = DexDataStructure.DexResetableFields.COINS_ACCEPTED_SINCE_LAST_RESET;
        b = DexDataStructure.DexResetableFields.COINS_ACCEPTED_SINCE_LAST_RESET + DexDataStructure.DexResetableFields.COINS_REJECTED_SINCE_LAST_RESET;
        c = a / b;
        c = c * 100;
        rate = c;
    }
    return rate;
}



uint8_t GetBillAcceptanceRate ( void )
{
    float a, b;
    uint8_t rate;
    double c;
    if ( DexDataStructure.DexResetableFields.BILLS_REJECTED_SINCE_LAST_RESET == 0 )
    {
        rate = 100;
    }
    else
    {
        a = DexDataStructure.DexResetableFields.BILLS_ACCEPTED_SINCE_LAST_RESET;
        b = DexDataStructure.DexResetableFields.BILLS_ACCEPTED_SINCE_LAST_RESET + DexDataStructure.DexResetableFields.BILLS_REJECTED_SINCE_LAST_RESET;
        c = a / b;
        c = c * 100;
        rate = c;
    }
    return rate;
}



void DEXBillsToRecyclerAccounting ( uint16_t value )
{
    //logger_fput(BILL_ACCEPTED_BVR_TXT); 
    LoggerAddEntry ( BILL_ACCEPTED_BVR_TXT, value );
    DexDataStructure.DexResetableFields.CA311_VALUE_OF_BILLS_TO_RECYCLER_SINCE_LAST_RESET += value;
    DexDataStructure.DexUnResetableFields.CA310_VALUE_OF_BILLS_IN_SINCE_INITIALIZATION += value;
    DEX_Configuration_Update = true;
}


void DEXBillsTransferedToRecyclerAccounting ( uint16_t value )
{
    DexDataStructure.DexResetableFields.CA407_VALUE_OF_BILLS_TRANSFERRED_TO_RECYCLER_SINCE_LAST_RESET += value;
    DexDataStructure.DexUnResetableFields.CA410_VALUE_OF_BILLS_TRANSFERRED_TO_RECYCLER_SINCE_INITIATLISATION += value;
    DEX_Configuration_Update = true;
}


void DEXBillsDispensedAccounting ( uint16_t value )
{
    DexDataStructure.DexResetableFields.CA405_VALUE_OF_BILLS_DISPENSED_SINCE_LAST_RESET += value;
    DexDataStructure.DexUnResetableFields.CA408_VALUE_OF_BILLS_DISPENSED_SINCE_INITIALISATION += value;
    DEX_Configuration_Update = true;
}


void DEXBillsManuallyDispensedAccounting ( uint16_t value )
{
    DexDataStructure.DexResetableFields.CA406_VALUE_OF_BILLS_MANUALLY_DISPENSED_SINCE_LAST_RESET += value;
    DexDataStructure.DexUnResetableFields.CA404_VALUE_OF_MANUAL_CASH_DISPENSED_SINCE_INITIALIZATION += value;
    DEX_Configuration_Update = true;
}


uint32_t HistoricalCashTotal ( void )
{
    return DexDataStructure.DexUnResetableFields.CA201_VALUE_OF_CASH_SALES_SINCE_INITIALIZATION;
}


uint32_t HistoricalvendTotal ( void )
{
    return DexDataStructure.DexUnResetableFields.VA102_NUMBER_OF_ALL_PAID_VEND_SALES_SINCE_INITIALIZATION;
}



uint32_t ResettableCashTotal ( void )
{
    return DexDataStructure.DexResetableFields.CA203_VALUE_OF_CASH_SALES_SINCE_LAST_RESET;
}


uint32_t ResettablevendTotal ( void )
{
    return  DexDataStructure.DexResetableFields.CA204_NUMBER_OF_CASH_VENDS_SINCE_LAST_RESET;
}



uint32_t ResettableBillTotal ( void )
{
    return  DexDataStructure.DexResetableFields.CA309_VALUE_OF_BILLS_IN_SINCE_LAST_RESET;
}



uint32_t HistoricalBillTotal ( void )
{
    return  DexDataStructure.DexUnResetableFields.CA310_VALUE_OF_BILLS_IN_SINCE_INITIALIZATION;
}


uint32_t HistoricalValueFreeVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end )
{
    uint16_t selection_offset;
    uint32_t total = 0;
    while ( selection_offset_start <= selection_offset_end )
    {
        selection_offset = GetSelectionOffset ( selection_offset_start );
        if ( selection_offset != 0xffff )
        {
            total = total + DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA402_VALUE_OF_FREE_VENDS_SINCE_INITIALIZATION;
        }
        selection_offset_start++;
    }
    return total;
}


uint32_t HistoricalNumberFreeVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end )
{
    uint32_t total = 0;
    uint16_t selection_offset;
    while ( selection_offset_start <= selection_offset_end )
    {
        selection_offset = GetSelectionOffset ( selection_offset_start );
        if ( selection_offset != 0xffff )
        {
            total = total + DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA401_NUMBER_OF_FREE_VENDS_SINCE_INITIALIZATION;
        }
        selection_offset_start++;
    }
    return total;
}


uint32_t ResettableValueFreeVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end )
{
    uint32_t total = 0;
    uint16_t selection_offset;
    while ( selection_offset_start <= selection_offset_end )
    {
        selection_offset = GetSelectionOffset ( selection_offset_start );
        if ( selection_offset != 0xffff )
        {
            total = total + DexDataStructure.Dex_PA_ResetableField[selection_offset].PA404_VALUE_OF_FREE_VENDS_SINCE_LAST_RESET;
        }
        selection_offset_start++;
    }
    return total;
}


uint32_t ResettableNumberFreeVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end )
{
    uint32_t total = 0;
    uint16_t selection_offset;
    while ( selection_offset_start <= selection_offset_end )
    {
        selection_offset = GetSelectionOffset ( selection_offset_start );
        if ( selection_offset != 0xffff )
        {
            total = total + DexDataStructure.Dex_PA_ResetableField[selection_offset].PA403_NUMBER_OF_FREE_VENDS_SINCE_LAST_RESET;
        }
        selection_offset_start++;
    }
    return total;
}



uint32_t HistoricalNumberFreeVendsTotal ( void )
{
    return  DexDataStructure.DexUnResetableFields.VA302_NUMBER_OF_FREE_VEND_SALES_SINCE_INITIALIZATION;
}


uint32_t HistoricalValueFreeVendsTotal ( void )
{
    return  DexDataStructure.DexUnResetableFields.VA301_FREE_VEND_VALUE_OF_SALES_SINCE_INITIALIZATION;
}


uint32_t IntervalValueFreeVendsTotal ( void )
{
    return  DexDataStructure.DexResetableFields.VA303_FREE_VEND_VALUE_OF_SALES_SINCE_LAST_RESET;
}



uint32_t IntervalNumberFreeVendsTotal ( void )
{
    return  DexDataStructure.DexResetableFields.VA304_NUMBER_OF_FREE_VEND_SALES_SINCE_LAST_RESET;
}




uint32_t HistoricalValueVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end )
{
    uint32_t total = 0;
    uint16_t selection_offset;
    while ( selection_offset_start <= selection_offset_end )
    {
        selection_offset = GetSelectionOffset ( selection_offset_start );
        if ( selection_offset != 0xffff )
        {
            total = total + DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA202_VALUE_OF_PAID_PRODUCTS_VENDED_SINCE_INITIALIZATION;
        }
        selection_offset_start++;
    }
    return total;
}

uint32_t HistoricalNumberVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end )
{
    uint32_t total = 0;
    uint16_t selection_offset;
    while ( selection_offset_start <= selection_offset_end )
    {
        selection_offset = GetSelectionOffset ( selection_offset_start );
        if ( selection_offset != 0xffff )
        {
            total = total + DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA201_NUMBER_OF_PAID_PRODUCTS_VENDED_SINCE_INITIALIZATION;
        }
        selection_offset_start++;
    }
    return total;
}

uint32_t ResettableValueVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end )
{
    uint32_t total = 0;
    uint16_t selection_offset;
    while ( selection_offset_start <= selection_offset_end )
    {
        selection_offset = GetSelectionOffset ( selection_offset_start );
        if ( selection_offset != 0xffff )
        {
            total = total + DexDataStructure.Dex_PA_ResetableField[selection_offset].PA204_VALUE_OF_PAID_PRODUCTS_VENDED_SINCE_LAST_RESET;
        }
        selection_offset_start++;
    }
    return total;
}

uint32_t ResettableNumberVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end )
{
    uint32_t total = 0;
    uint16_t selection_offset;
    while ( selection_offset_start <= selection_offset_end )
    {
        selection_offset = GetSelectionOffset ( selection_offset_start );
        if ( selection_offset != 0xffff )
        {
            total = total + DexDataStructure.Dex_PA_ResetableField[selection_offset].PA203_NUMBER_OF_PAID_PRODUCTS_VENDED_SINCE_LAST_RESET;
        }
        selection_offset_start++;
    }
    return total;
}





uint32_t HistoricalValueCashlessByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end )
{
    uint32_t total = 0;
    uint16_t selection_offset;
    while ( selection_offset_start <= selection_offset_end )
    {
        selection_offset = GetSelectionOffset ( selection_offset_start );
        if ( selection_offset != 0xffff )
        {
            total = total + DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA706_VALUE_OF_CASHLESS_SALES_SINCE_INITIALISATION;
        }
        selection_offset_start++;
    }
    return total;
}



uint32_t HistoricalNumberCashlessByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end )
{
    uint32_t total = 0;
    uint16_t selection_offset;
    while ( selection_offset_start <= selection_offset_end )
    {
        selection_offset = GetSelectionOffset ( selection_offset_start );
        if ( selection_offset != 0xffff )
        {
            total = total + DexDataStructure.Dex_PA_UnResetableField[selection_offset].PA705_NUMBER_OF_CASHLESS_SALES_SINCE_INITIALISATION;
        }
        selection_offset_start++;
    }
    return total;
}



uint32_t ResettableValueCashlessByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end )
{
    uint32_t total = 0;
    uint16_t selection_offset;
    while ( selection_offset_start <= selection_offset_end )
    {
        selection_offset = GetSelectionOffset ( selection_offset_start );
        if ( selection_offset != 0xffff )
        {
            total = total + DexDataStructure.Dex_PA_ResetableField[selection_offset].PA708_VALUE_OF_CASHLESS_SALES_SINCE_LAST_RESET;
        }
        selection_offset_start++;
    }
    return total;
}



uint32_t ResettableNumberCashlessByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end )
{
    uint32_t total = 0;
    uint16_t selection_offset;
    while ( selection_offset_start <= selection_offset_end )
    {
        selection_offset = GetSelectionOffset ( selection_offset_start );
        if ( selection_offset != 0xffff )
        {
            total = total + DexDataStructure.Dex_PA_ResetableField[selection_offset].PA707_NUMER_OF_CASHLESS_SALES_SINCE_LAST_RESET;
        }
        selection_offset_start++;
    }
    return total;
}





uint32_t ResettableNumberCashlessTotal ( void )
{
    return DexDataStructure.DexResetableFields.DA204_NUMBER_OF_CASHLESS_1_SALES_SINCE_LAST_RESET + DexDataStructure.DexResetableFields.DB204_NUMBER_OF_CASHLESS_2_SALES_SINCE_LAST_RESET;
}

uint32_t ResettableValueCashlessTotal ( void )
{
    return DexDataStructure.DexResetableFields.DA203_VALUE_OF_CASHLESS_1_SALES_SINCE_LAST_RESET + DexDataStructure.DexResetableFields.DB203_VALUE_OF_CASHLESS_2_SALES_SINCE_LAST_RESET;
}



uint32_t HistoricalNumberCashlessTotal ( void )
{
    return DexDataStructure.DexUnResetableFields.DA202_NUMBER_OF_CASHLESS_1_SALES_SINCE_INITIALIZATION + DexDataStructure.DexUnResetableFields.DB202_NUMBER_OF_CASHLESS_2_SALES_SINCE_INITIALIZATION;
}


uint32_t HistoricalValueCashlessTotal ( void )
{
    return DexDataStructure.DexUnResetableFields.DA201_VALUE_OF_CASHLESS_1_SALES_SINCE_INITIALIZATION + DexDataStructure.DexUnResetableFields.DB201_VALUE_OF_CASHLESS_2_SALES_SINCE_INITIALIZATION;
}


uint32_t ResettableSecondVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end )
{
    uint32_t total = 0;
    uint16_t selection_offset;
    while ( selection_offset_start <= selection_offset_end )
    {
        selection_offset = GetSelectionOffset ( selection_offset_start );
        if ( selection_offset != 0xffff )
        {
            total = total + DexDataStructure.Dex_EA_ProductDispenseResetableField[selection_offset].EA202_EJL_NUMBER_OF_EVENTS_SINCE_LAST_RESET;
        }
        selection_offset_start++;
    }
    return total;
}


uint32_t HistoricalSecondVendsByRange ( uint16_t selection_offset_start, uint16_t selection_offset_end )
{
    uint32_t total = 0;
    uint16_t selection_offset;
    while ( selection_offset_start <= selection_offset_end )
    {
        selection_offset = GetSelectionOffset ( selection_offset_start );
        if ( selection_offset != 0xffff )
        {
            total = total + DexDataStructure.Dex_EA_ProductDispenseUnResetableField[selection_offset].EA203_EJL_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
        }
        selection_offset_start++;
    }
    return total;
}


uint32_t ResettableSecondVendsTotal ( void )
{
    return  DexDataStructure.DexResetableFields.EA202_EJL_TOTAL_NUMBER_OF_EVENTS_SINCE_LAST_RESET;
}


uint32_t HistoricalSecondVendsTotal ( void )
{
    return DexDataStructure.DexUnResetableFields.EA202_EJL_TOTAL_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
}


void GetMACHINE_MODEL_NUMBER ( char* machine_model_number )
{
    memcpy ( machine_model_number, DexDataStructure.DexDeviceIDFields.ID102_MACHINE_MODEL_NUMBER, sizeof DexDataStructure.DexDeviceIDFields.ID102_MACHINE_MODEL_NUMBER );
}


void SetMACHINE_MODEL_NUMBER ( char* machine_model_number )
{
    memcpy ( DexDataStructure.DexDeviceIDFields.ID102_MACHINE_MODEL_NUMBER, machine_model_number, sizeof DexDataStructure.DexDeviceIDFields.ID102_MACHINE_MODEL_NUMBER );
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}


void GetVMC_CONTROL_BOARD_BUILD_STANDARD ( char* control_board_build_standard )
{
    memcpy ( control_board_build_standard, DexDataStructure.DexDeviceIDFields.CB103_VMC_CONTROL_BOARD_BUILD_STANDARD, sizeof DexDataStructure.DexDeviceIDFields.CB103_VMC_CONTROL_BOARD_BUILD_STANDARD );
}


void SetVMC_CONTROL_BOARD_BUILD_STANDARD ( char* control_board_build_standard )
{
    memcpy ( DexDataStructure.DexDeviceIDFields.CB103_VMC_CONTROL_BOARD_BUILD_STANDARD, control_board_build_standard, sizeof DexDataStructure.DexDeviceIDFields.CB103_VMC_CONTROL_BOARD_BUILD_STANDARD );
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}


void GetMACHINE_BUILD_STANDARD ( char* machine_build_standard )
{
    memcpy ( machine_build_standard, DexDataStructure.DexDeviceIDFields.ID103_MACHINE_BUILD_STANDARD, sizeof DexDataStructure.DexDeviceIDFields.ID103_MACHINE_BUILD_STANDARD );
}


void SetMACHINE_BUILD_STANDARD ( char* machine_build_standard )
{
    memcpy ( DexDataStructure.DexDeviceIDFields.ID103_MACHINE_BUILD_STANDARD, machine_build_standard, sizeof DexDataStructure.DexDeviceIDFields.ID103_MACHINE_BUILD_STANDARD );
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}



void GetMACHINE_LOCATION ( char* machine_location )
{
    memcpy ( machine_location, DexDataStructure.DexDeviceIDFields.ID104_MACHINE_LOCATION, sizeof DexDataStructure.DexDeviceIDFields.ID104_MACHINE_LOCATION );
}


void SetMACHINE_LOCATION ( char* machine_location )
{
    memcpy ( DexDataStructure.DexDeviceIDFields.ID104_MACHINE_LOCATION, machine_location, sizeof DexDataStructure.DexDeviceIDFields.ID104_MACHINE_LOCATION );
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}


void GetMACHINE_ASSET_NUMBER ( char* asset_number )
{
    memcpy ( asset_number, DexDataStructure.DexDeviceIDFields.ID106_MACHINE_ASSET_NUMBER, sizeof DexDataStructure.DexDeviceIDFields.ID106_MACHINE_ASSET_NUMBER );
}


void SetMACHINE_ASSET_NUMBER ( char* asset_number )
{
    memcpy ( DexDataStructure.DexDeviceIDFields.ID106_MACHINE_ASSET_NUMBER, asset_number, sizeof DexDataStructure.DexDeviceIDFields.ID106_MACHINE_ASSET_NUMBER );
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}


void GetMACHINE_SERIAL_NUMBER ( char* machine_sn )
{
    memcpy ( machine_sn, DexDataStructure.DexDeviceIDFields.ID101_MACHINE_SERIAL_NUMBER, sizeof DexDataStructure.DexDeviceIDFields.ID101_MACHINE_SERIAL_NUMBER );
}



void SetMACHINE_SERIAL_NUMBER ( const char* machine_sn )
{
    memcpy ( DexDataStructure.DexDeviceIDFields.ID101_MACHINE_SERIAL_NUMBER, machine_sn, sizeof DexDataStructure.DexDeviceIDFields.ID101_MACHINE_SERIAL_NUMBER );
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
    Configuration_Write_Data ( configuration__MachineSerialNumber, (uint8_t*)machine_sn, 20 );
}



void InitIdentification ( void )
{
    memcpy ( DexDataStructure.DexDeviceIDFields.CA101_CHANGER_SERIAL_NUMBER, INIT_SERIAL_NUMBER, 15 );
    memcpy ( DexDataStructure.DexDeviceIDFields.CA102_COIN_MECHANISM_MODEL_NUMBER, INIT_MODEL_NUMBER, 12 );
    memcpy ( DexDataStructure.DexDeviceIDFields.CA103_COIN_MECHANISM_SOFTWARE_REVISION, INIT_REVISION_NUMBER, 2 );
    memcpy ( DexDataStructure.DexDeviceIDFields.BA101_BILL_VALIDATOR_SERIAL_NUMBER, INIT_SERIAL_NUMBER, 15 );
    memcpy ( DexDataStructure.DexDeviceIDFields.BA102_BILL_VALIDATOR_MODEL_NUMBER, INIT_MODEL_NUMBER, 12 );
    memcpy ( DexDataStructure.DexDeviceIDFields.BA103_BILL_VALIDATOR_SOFTWARE_REVISION, INIT_REVISION_NUMBER, 2 );
    memcpy ( DexDataStructure.DexDeviceIDFields.DA101_CASHLESS_1_SERIAL_NUMBER, INIT_SERIAL_NUMBER, 15 );
    memcpy ( DexDataStructure.DexDeviceIDFields.DA102_CASHLESS_1_MODEL_NUMBER, INIT_MODEL_NUMBER, 12 );
    memcpy ( DexDataStructure.DexDeviceIDFields.DA103_CASHLESS_1_SOFTWARE_REVISION, INIT_REVISION_NUMBER, 2 );
    memcpy ( DexDataStructure.DexDeviceIDFields.DB101_CASHLESS_2_SERIAL_NUMBER, INIT_SERIAL_NUMBER, 15 );
    memcpy ( DexDataStructure.DexDeviceIDFields.DB102_CASHLESS_2_MODEL_NUMBER, INIT_MODEL_NUMBER, 12 );
    memcpy ( DexDataStructure.DexDeviceIDFields.DB103_CASHLESS_2_SOFTWARE_REVISION, INIT_REVISION_NUMBER, 2 );
}



void SetMechIdentification ( uint8_t* mechidentification )
{
    memcpy ( DexDataStructure.DexDeviceIDFields.CA101_CHANGER_SERIAL_NUMBER, mechidentification, 15 );
    memcpy ( DexDataStructure.DexDeviceIDFields.CA102_COIN_MECHANISM_MODEL_NUMBER, mechidentification + 15, 12 );
    memcpy ( DexDataStructure.DexDeviceIDFields.CA103_COIN_MECHANISM_SOFTWARE_REVISION, mechidentification + 27, 2 );
}


void SetBVIdentification ( uint8_t* bvidentification )
{
    memcpy ( DexDataStructure.DexDeviceIDFields.BA101_BILL_VALIDATOR_SERIAL_NUMBER, bvidentification, 15 );
    memcpy ( DexDataStructure.DexDeviceIDFields.BA102_BILL_VALIDATOR_MODEL_NUMBER, bvidentification + 15, 12 );
    memcpy ( DexDataStructure.DexDeviceIDFields.BA103_BILL_VALIDATOR_SOFTWARE_REVISION, bvidentification + 27, 2 );
}


void SetCL1Identification ( uint8_t* cl1identification )
{
    memcpy ( DexDataStructure.DexDeviceIDFields.DA101_CASHLESS_1_SERIAL_NUMBER, cl1identification, 15 );
    memcpy ( DexDataStructure.DexDeviceIDFields.DA102_CASHLESS_1_MODEL_NUMBER, cl1identification + 15, 12 );
    memcpy ( DexDataStructure.DexDeviceIDFields.DA103_CASHLESS_1_SOFTWARE_REVISION, cl1identification + 27, 2 );
}


void SetCL2Identification ( uint8_t* cl2identification )
{
    memcpy ( DexDataStructure.DexDeviceIDFields.DB101_CASHLESS_2_SERIAL_NUMBER, cl2identification, 15 );
    memcpy ( DexDataStructure.DexDeviceIDFields.DB102_CASHLESS_2_MODEL_NUMBER, cl2identification + 15, 12 );
    memcpy ( DexDataStructure.DexDeviceIDFields.DB103_CASHLESS_2_SOFTWARE_REVISION, cl2identification + 27, 2 );
}


void GetCONTROL_BOARD_SERIAL_NUMBER ( char* cb_sn )
{
    memcpy ( cb_sn, DexDataStructure.DexDeviceIDFields.CB101_VMC_CONTROL_BOARD_SERIAL_NUMBER, sizeof DexDataStructure.DexDeviceIDFields.CB101_VMC_CONTROL_BOARD_SERIAL_NUMBER );
}


void SetCONTROL_BOARD_SERIAL_NUMBER ( const char* cb_sn )
{
    memcpy ( DexDataStructure.DexDeviceIDFields.CB101_VMC_CONTROL_BOARD_SERIAL_NUMBER, cb_sn, sizeof DexDataStructure.DexDeviceIDFields.CB101_VMC_CONTROL_BOARD_SERIAL_NUMBER );
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
    Configuration_Write_Data ( Configuration_ControlBoardSerialNumber, (uint8_t*)cb_sn, 20 );
}



void GetVMCMODEL_NUMBER ( char* cb_mn )
{
    memcpy ( cb_mn, DexDataStructure.DexDeviceIDFields.CB102_VMC_CONTROL_BOARD_MODEL_NUMBER, sizeof DexDataStructure.DexDeviceIDFields.CB102_VMC_CONTROL_BOARD_MODEL_NUMBER );
}


void SetVMCMODEL_NUMBER ( char* cb_mn )
{
    memcpy ( DexDataStructure.DexDeviceIDFields.CB102_VMC_CONTROL_BOARD_MODEL_NUMBER, cb_mn, sizeof DexDataStructure.DexDeviceIDFields.CB102_VMC_CONTROL_BOARD_MODEL_NUMBER );
    Configuration_WriteData ( configurationDexDataStructureBase, (uint8_t*)&DexDataStructure, sizeof ( DexDataStructure ) );
}

const char* GetVersionString ( void )
{
    return FW_VERSION;
}


uint32_t TotalCashInTube ( void )
{
    return  GetCash_In_Tubes ();
}
/*++
 *=============================================================
 *
 * Copyright © 2024 Wittern
 *
 *=============================================================
*--*/

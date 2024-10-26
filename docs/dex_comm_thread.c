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
#include <string.h>
#include <stdlib.h>

#include "WrapperOs.h"
#include "CRC16.h"

#include "dex_comm_thread.h"
#include "dex_accounting.h"
#include "dex_db.h"
#include "inter_controller_comm_thread.h"
#include "operations.h"
#include "Logger/data_logger.h"
#include "refrigeration_thread.h"
#include "rtc.h"
#include "WrapperDex.h"

 /*
 Differences between tiny, short and full DEX Files defined below;

 Tiny has;
 CB101_VMC_CONTROL_BOARD_SERIAL_NUMBER
 CB102_VMC_CONTROL_BOARD_MODEL_NUMBER
 CB103_VMC_CONTROL_BOARD_BUILD_STANDARD
 VA101_VALUE_OF_ALL_PAID_VEND_SALES_SINCE_INITIALIZATION
 VA102_NUMBER_OF_ALL_PAID_VEND_SALES_SINCE_INITIALIZATION
 VA301_FREE_VEND_VALUE_OF_SALES_SINCE_INITIALIZATION
 VA302_NUMBER_OF_FREE_VEND_SALES_SINCE_INITIALIZATION
 CA101_CHANGER_SERIAL_NUMBER
 CA102_COIN_MECHANISM_MODEL_NUMBER
 CA103_COIN_MECHANISM_SOFTWARE_REVISION
 CA201_VALUE_OF_CASH_SALES_SINCE_INITIALIZATION
 CA202_NUMBER_OF_CASH_VENDS_SINCE_INITIALIZATION
 CA301_VALUE_OF_CASH_IN_SINCE_LAST_RESET
 CA302_VALUE_OF_CASH_TO_CASH_BOX_SINCE_LAST_RESET
 CA303_VALUE_OF_CASH_TO_TUBES_SINCE_LAST_RESET
 CA304_VALUE_OF_BILLS_IN_SINCE_LAST_RESET
 CA305_VALUE_OF_CASH_IN_SINCE_INITIALIZATION
 CA306_VALUE_OF_CASH_TO_BOX_SINCE_INITIALIZATION
 CA307_VALUE_OF_CASH_TO_TUBES_SINCE_INITIALIZATION
 CA308_VALUE_OF_BILLS_IN_SINCE_INITIALIZATION
 .CA309_VALUE_OF_BILLS_IN_SINCE_LAST_RESET
 CA310_VALUE_OF_BILLS_IN_SINCE_INITIALIZATION
 CA401_VALUE_OF_CASH_DISPENSED_SINCE_LAST_RESET
 CA402_VALUE_OF_MANUAL_CASH_DISPENSED_SINCE_LAST_RESET
 .CA403_VALUE_OF_CASH_DISPENSED_SINCE_INITIALIZATION
 CA15TUBE101_VALUE_OF_TUBE_CONTENTS
 CA15TUBE101_VALUE_OF_TUBE_CONTENTS
 BA101_BILL_VALIDATOR_SERIAL_NUMBER
 BA102_BILL_VALIDATOR_MODEL_NUMBER
 BA103_BILL_VALIDATOR_SOFTWARE_REVISION
 DA101_CASHLESS_1_SERIAL_NUMBER
 DA102_CASHLESS_1_MODEL_NUMBER
 DA103_CASHLESS_1_SOFTWARE_REVISION
 DA201_VALUE_OF_CASHLESS_1_SALES_SINCE_INITIALIZATION
 DA202_NUMBER_OF_CASHLESS_1_SALES_SINCE_INITIALIZATION
 DB101_CASHLESS_2_SERIAL_NUMBER
 DB102_CASHLESS_2_MODEL_NUMBER
 DB103_CASHLESS_2_SOFTWARE_REVISION
 DB201_VALUE_OF_CASHLESS_2_SALES_SINCE_INITIALIZATION
 DB202_NUMBER_OF_CASHLESS_2_SALES_SINCE_INITIALIZATION
 TA201_VALUE_OF_TOKEN_COUPON_SALES_SINCE_INITIALIZATION
 TA202_NUMBER_OF_TOKEN_COUPON_SALES_SINCE_INITIALIZATION
 PA1 = Selection and Price
 PA201_NUMBER_OF_PAID_PRODUCTS_VENDED_SINCE_INITIALIZATION
 PA202_VALUE_OF_PAID_PRODUCTS_VENDED_SINCE_INITIALIZATION

 EA101_EGS -  Door open
 EA102_EGS_DATEYEAR_OF_EVENT
 EA102_EGS_DATEMONTH_OF_EVENT
 EA102_EGS_DATEDAY_OF_EVENT
 EA103_EGS_TIMEHOUR_OF_EVENT
 EA103_EGS_TIMEMINUTE_OF_EVENT
 EA202_EGS_NUMBER_OF_EVENTS_SINCE_LAST_RESET
 EA203_EGS_NUMBER_OF_EVENTS_SINCE_INITIALIZATION
 Current Door current state
 EA206_EGS_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES

 EA101_EJH -  Health rules have been violated for the temperature controlled  (if happened in the life of the machine)
 EA202_EJH_NUMBER_OF_EVENTS_SINCE_LAST_RESET
 EA102_EJH_DATEYEAR_OF_EVENT
 EA102_EJH_DATEMONTH_OF_EVENT
 EA102_EJH_DATEDAY_OF_EVENT
 EA103_EJH_TIMEHOUR_OF_EVENT
 EA103_EJH_TIMEMINUTE_OF_EVENT
 EA202_EJH_NUMBER_OF_EVENTS_SINCE_LAST_RESET
 EA203_EJH_NUMBER_OF_EVENTS_SINCE_INITIALIZATION);
 Current health alert (active) status
 EA206_EJH_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES)

 EA101_EJJ -  cooling unit does not cool down to the pre-set temperature (if happened in the life of the machine)
 EA102_EJJ_DATEYEAR_OF_EVENT
 EA102_EJJ_DATEMONTH_OF_EVENT
 EA102_EJJ_DATEDAY_OF_EVENT
 EA103_EJJ_TIMEHOUR_OF_EVENT
 EA103_EJJ_TIMEMINUTE_OF_EVENT
 EA202_EJJ_NUMBER_OF_EVENTS_SINCE_LAST_RESET
 EA203_EJJ_NUMBER_OF_EVENTS_SINCE_INITIALIZATION);
  if system not cooling down to pre-set temperature(active) in 12 hours send active
 EA206_EJJ_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES

 EA101_EJK - Temperature sensor is defective (if happened in the life of the machine)
 Dex_EA_UnResetableField.EA102_EJK_DATEYEAR_OF_EVENT
 .EA102_EJK_DATEMONTH_OF_EVENT
 EA102_EJK_DATEDAY_OF_EVENT
 EA103_EJK_TIMEHOUR_OF_EVENT
 EA103_EJK_TIMEMINUTE_OF_EVENT
 EA202_EJK_NUMBER_OF_EVENTS_SINCE_LAST_RESET
 EA203_EJK_NUMBER_OF_EVENTS_SINCE_INITIALIZATION
 if temperature sensor bad send active
 EA206_EJK_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES

 EA101_EA - General non-specific Coin Mechanism fault (if happened in the life of the machine)
 EA102_EA_DATEYEAR_OF_EVENT
 EA102_EA_DATEMONTH_OF_EVENT
 EA102_EA_DATEDAY_OF_EVENT
 EA103_EA_TIMEHOUR_OF_EVENT
 EA103_EA_TIMEMINUTE_OF_EVENT
 EA202_EA_NUMBER_OF_EVENTS_SINCE_LAST_RESETEA203_EA_NUMBER_OF_EVENTS_SINCE_INITIALIZATION);
 Coin Mechanism fault present send active
 EA206_EA_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES

 EA101_EK - General non-specific Cashless 1 Mechanism fault (if happened in the life of the machine)
 EA102_EK_DATEYEAR_OF_EVENT
 EA102_EK_DATEMONTH_OF_EVENT
 EA102_EK_DATEDAY_OF_EVENT
 EA103_EK_TIMEHOUR_OF_EVENT
 EA103_EK_TIMEMINUTE_OF_EVENT
 EA202_EK_NUMBER_OF_EVENTS_SINCE_LAST_RESET
 EA203_EK_NUMBER_OF_EVENTS_SINCE_INITIALIZATION
 if Cashless Mechanism fault send active
 EA206_EK_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES

 EA101_EK2 - General non-specific Cashless 2 Mechanism fault (if happened in the life of the machine)
 EA102_EK2_DATEYEAR_OF_EVENT
 .EA102_EK2_DATEMONTH_OF_EVENT
 EA102_EK2_DATEDAY_OF_EVENT
 EA103_EK2_TIMEHOUR_OF_EVENT
 EA103_EK2_TIMEMINUTE_OF_EVENT
 EA202_EK2_NUMBER_OF_EVENTS_SINCE_LAST_RESET
 EA203_EK2_NUMBER_OF_EVENTS_SINCE_INITIALIZATION
 if Cashless2 Mechanism fault send active
 EA206_EK2_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES

 EA101_EN - General non-specific Bill Validator fault (if happened in the life of the machine)
 EA102_EN_DATEYEAR_OF_EVENT
 EA102_EN_DATEMONTH_OF_EVENT
 EA102_EN_DATEDAY_OF_EVENT
 EA103_EN_TIMEHOUR_OF_EVENT
 EA103_EN_TIMEMINUTE_OF_EVENT
 EA202_EN_NUMBER_OF_EVENTS_SINCE_LAST_RESET
 EA203_EN_NUMBER_OF_EVENTS_SINCE_INITIALIZATION
 if Bill Mechanism fault send active
 EA206_EN_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES


 EA101_EC1F - A malfunction has been detected in the selection board  (if happened in the life of the machine)
 EA102_EC1F_DATEYEAR_OF_EVENT
 EA102_EC1F_DATEMONTH_OF_EVENT
 EA102_EC1F_DATEDAY_OF_EVENT
 EA103_EC1F_TIMEHOUR_OF_EVENT
 EA103_EC1F_TIMEMINUTE_OF_EVENT
 EA202_EC1F_NUMBER_OF_EVENTS_SINCE_LAST_RESET
 EA203_EC1F_NUMBER_OF_EVENTS_SINCE_INITIALIZATION
 if Keypad fault send active
 EA206_EC1F_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES

 EA301_NUMBER_OF_READS_WITH_RESET_SINCE_INITIALIZATION
 EA302_DATEYEAR_OF_THIS_READOUT
 EA302_DATEMONTH_OF_THIS_READOUT
 EA302_DATEDAY_OF_THIS_READOUT
 EA303_TIMEHOUR_OF_THIS_READOUT
 EA303_TIMEMINUTE_OF_THIS_READOUT
 EA304_TIME_TERMINAL_INTERROGATOR_IDENTIFICATION
 EA305_DATEYEAR_OF_LAST_READ_OUT
 EA305_DATEMONTH_OF_LAST_READ_OUT
 EA305_DATEDAY_OF_LAST_READ_OUT
 EA306_TIMEHOUR_OF_LAST_READ_OUT
 EA306_TIMEMINUTE_OF_LAST_READ_OUT
 EA307_LAST_TERMINAL_INTERROGATOR_IDENTIFICATION
 EA309_NUMBER_OF_READS_WITH_OR_WITHOUT_RESET_SINCE_INITIALIZATION
 EA3010_NUMBER_OF_RESETS_SINCE_INITIALIZATION
 EA701_NUMBER_OF_POWER_OUTAGES_SINCE_LAST_RESET
 EA702_NUMBER_OF_POWER_OUTAGES_SINCE_INITIALIZATION

 Short has tiny plus;
 ID401_DECIMAL_POINT_POSITION
 ID402_CURRENCY_NUMERIC_CODE
 ID403_CURRENCY_ALPHABETIC_CODE
 VA103_VALUE_OF_ALL_PAID_VEND_SALES_SINCE_LAST_RESET
 VA104_NUMBER_OF_ALL_PAID_VEND_SALES_SINCE_LAST_RESET
 VA105_VALUE_OF_ALL_DISCOUNTS_SINCE_SINCE_INITIALIZATION
 VA106_NUMBER_OF_ALL_DISCOUNTED_PAID_VENDS_SINCE_INITIALIZATION
 VA107_VALUE_OF_ALL_DISCOUNTS_SINCE_LAST_RESET
 VA108_NUMBER_OF_ALL_DISCOUNTED_PAID_VENDS_SINCE_LAST_RESET
 VA201_TEST_VEND_VALUE_OF_SALES_SINCE_INITIALIZATION
 VA202_NUMBER_OF_TEST_VENDS_SINCE_INITIALIZATION
 VA203_TEST_VEND_VALUE_OF_SALES_SINCE_LAST_RESET
 VA204_NUMBER_OF_TEST_VENDS_SINCE_LAST_RESET
 VA303_FREE_VEND_VALUE_OF_SALES_SINCE_LAST_RESET);
 VA304_NUMBER_OF_FREE_VEND_SALES_SINCE_LAST_RESET);
 CA203_VALUE_OF_CASH_SALES_SINCE_LAST_RESET
 CA204_NUMBER_OF_CASH_VENDS_SINCE_LAST_RESET
 CA311_VALUE_OF_BILLS_TO_RECYCLER_SINCE_LAST_RESET
 CA312_VALUE_OF_BILLS_TO_RECYCLER_SINCE_INITIALIZATION
 CA404_VALUE_OF_MANUAL_CASH_DISPENSED_SINCE_INITIALIZATION
 CA405_VALUE_OF_BILLS_DISPENSED_SINCE_LAST_RESET
 CA406_VALUE_OF_BILLS_MANUALLY_DISPENSED_SINCE_LAST_RESET
 CA407_VALUE_OF_BILLS_TRANSFERRED_TO_RECYCLER_SINCE_LAST_RESET
 CA408_VALUE_OF_BILLS_DISPENSED_SINCE_INITIALISATION
 CA409_VALUE_OF_BILLS_MANUALLY_DISPENSED_SINCE_INITIATLISATION
 CA801_VALUE_OF_CASH_OVERPAY_SINCE_LAST_RESET
 CA802_VALUE_OF_CASH_OVERPAY_SINCE_INITIALIZATION
 CA901_VALUE_OF_PAY_VENDS_EXACT_CHANGE_SINCE_LAST_RESET
 CA902_VALUE_OF_PAY_VENDS_EXACT_CHANGE_SINCE_INITIALIZATION
 CA1001_VALUE_OF_CASH_FILLED_SINCE_LAST_RESET
 CA1002_VALUE_OF_CASH_FILLED_SINCE_INITIALIZATION
 CA1003_VALUE_OF_BILLS_FILLED_SINCE_LAST_RESET
 CA1004_VALUE_OF_BILLS_FILLED_SINCE_INITIALISATION
 CA15TUBE103_NUMBER_OF_COIN_TYPE_0 through 15
 .CA1601_VALUE_CREDITED_FROM_MACHINE_SINCE_LAST_RESET
 CA1602_VALUE_CREDITED_FROM_MACHINE_SINCE_INITIALIZATION
 DA203_VALUE_OF_CASHLESS_1_SALES_SINCE_LAST_RESET
 DA204_NUMBER_OF_CASHLESS_1_SALES_SINCE_LAST_RESET
 DA401_VALUE_OF_CREDIT_TO_CASHLESS_1_SINCE_INITIALIZATION
 DA402_VALUE_OF_CREDIT_TO_CASHLESS_1_SINCE_LAST_RESET
 DA501_VALUE_OF_CASHLESS_1_DISCOUNTS_SINCE_LAST_RESET
 DA502_NUMBER_OF_DISCOUNT_CASHLESS_1_VENDS_SINCE_LAST_RESET
 DA503_VALUE_OF_CASHLESS_1_DISCOUNTS_SINCE_INITIALISATION
 DA504_NUMBER_OF_DISCOUNT_CASHLESS_1_VENDS_SINCE_INITIALISATION
 DA505_VALUE_OF_CASHLESS_1_SURCHARGES_SINCE_LAST_RESET
 DA506_NUMBER_OF_SURCHARGE_CASHLESS_1_VENDS_SINCE_LAST_RESET
 DA507_VALUE_OF_CASHLESS_1_SURCHARGES_SINCE_INITIALISATION
 DA508_NUMBER_OF_SURCHARGE_CASHLESS_1_VENDS_SINCE_INITIALISATION
 DA601_REVALUATION_INCENTIVE_ON_CASHLESS_1_SINCE_INITIALISATION
 DA602_REVALUATION_INCENTIVE_ON_CASHLESS_1_SINCE_LAST_RESET
 DA901_VALUE_OF_CASHLESS_1_OVERPAY_SINCE_LAST_RESET
 DA902_VALUE_OF_CASHLESS_1_OVERPAY_SINCE_INITIALISATION
 DA1001_CASHLESS_1_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_INITIALISATION
 DA1002_CASHLESS_1_VALUE_OF_MIXED_PAYMENT_CASHLESS_AMOUNT_SINCE_INITIALISATION
 DA1003_CASHLESS_1_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_LAST_RESET
 DA1004_CASHLESS_1_VALUE_OF_MIXED_PAYMENT_CASHLESS_AMOUNT_SINCE_LAST_RESET
 DB203_VALUE_OF_CASHLESS_2_SALES_SINCE_LAST_RESET
 DB204_NUMBER_OF_CASHLESS_2_SALES_SINCE_LAST_RESET
 DB401_VALUE_OF_CREDIT_TO_CASHLESS_2_SINCE_INITIALIZATION
 DB402_VALUE_OF_CREDIT_TO_CASHLESS_2_SINCE_LAST_RESET
 DB501_VALUE_OF_CASHLESS_2_DISCOUNTS_SINCE_LAST_RESET
 DB502_NUMBER_OF_DISCOUNT_CASHLESS_2_VENDS_SINCE_LAST_RESET
 DB503_VALUE_OF_CASHLESS_2_DISCOUNTS_SINCE_INITIALISATION
 DB504_NUMBER_OF_DISCOUNT_CASHLESS_2_VENDS_SINCE_INITIALISATION
 DB505_VALUE_OF_CASHLESS_2_SURCHARGES_SINCE_LAST_RESET
 DB506_NUMBER_OF_SURCHARGE_CASHLESS_2_VENDS_SINCE_LAST_RESET
 DB507_VALUE_OF_CASHLESS_2_SURCHARGES_SINCE_INITIALISATION
 DB508_NUMBER_OF_SURCHARGE_CASHLESS_2_VENDS_SINCE_INITIALISATION
 DB601_REVALUATION_INCENTIVE_ON_CASHLESS_2_SINCE_INITIALISATION
 DB602_REVALUATION_INCENTIVE_ON_CASHLESS_2_SINCE_LAST_RESET
 DB901_VALUE_OF_CASHLESS_2_OVERPAY_SINCE_LAST_RESET
 DB902_VALUE_OF_CASHLESS_2_OVERPAY_SINCE_INITIALISATION
 DB1001_CASHLESS_2_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_INITIALISATION
 DB1002_CASHLESS_2_VALUE_OF_MIXED_PAYMENT_CASHLESS_AMOUNT_SINCE_INITIALISATION
 DB1003_CASHLESS_2_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_LAST_RESET
 DB1004_CASHLESS_2_VALUE_OF_MIXED_PAYMENT_CASHLESS_AMOUNT_SINCE_LAST_RESET
 TA203_VALUE_OF_TOKEN_COUPON_SALES_SINCE_LAST_RESET
 TA204_NUMBER_OF_TOKEN_COUPON_SALES_SINCE_LAST_RESET
 TA205_VALUE_OF_VALUE_TOKEN_COUPON_SALES_SINCE_INITIALIZATION
 TA206_NUMBER_OF_VALUE_TOKEN_COUPON_SALES_SINCE_INITIALIZATION
 TA207_VALUE_OF_VALUE_TOKEN_COUPON_SALES_SINCE_LAST_RESET
 TA208_NUMBER_OF_VALUE_TOKEN_COUPON_SALES_SINCE_LAST_RESET);
 PA1 Motor present or not
 PA301_NUMBER_OF_TEST_VENDS_SINCE_INITIALIZATION
 PA302_VALUE_OF_TEST_VENDS_SINCE_INITIALIZATION
 PA401_NUMBER_OF_FREE_VENDS_SINCE_INITIALIZATION
 PA501_DATEYEAR_OF_MOST_RECENT_SALE
 PA501_DATEMONTH_OF_MOST_RECENT_SALE
 PA501_DATEDAY_OF_MOST_RECENT_SALE
 PA502_TIMEHOUR_OF_MOST_RECENT_SALE
 PA502_TIMEMINUTE_OF_MOST_RECENT_SALE
 PA701_PRODUCT_NUMBER
 PA702_CASH_DEVICE
 PA703_PRICE LIST NUMBER
 PA704_APPLIED PRICE
 PA705_NUMBER_OF_CASH_SALES_SINCE_INITIALISATION);
 PA706_VALUE_OF_CASH_SALES_SINCE_INITIALISATION);
 PA701_PRODUCT NUMBER
 PA702_CASHLESS_DEVICE
 PA703_PRICE LIST NUMBER
 PA704_APPLIED PRICE
 PA705_NUMBER_OF_CASHLESS_SALES_SINCE_INITIALISATION
 PA706_VALUE_OF_CASHLESS_SALES_SINCE_INITIALISATION
 PA801_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_INITIALISATION
 PA802_VALUE_OF_MIXED_PAYMENT_CASH_AMOUNT_SINCE_INITIALISATION



 Full has short plus;
 PA203_NUMBER_OF_PAID_PRODUCTS_VENDED_SINCE_LAST_RESET
 PA204_VALUE_OF_PAID_PRODUCTS_VENDED_SINCE_LAST_RESET
 PA205_NUMBER_OF_DISCOUNTS_SINCE_INITIALIZATION
 PA206_VALUE_OF_DISCOUNTS_SINCE_INITIALIZATION
 PA207_NUMBER_OF_DISCOUNTS_SINCE_LAST_RESET
 PA208_VALUE_OF_DISCOUNTS_SINCE_LAST_RESET
 PA209_NUMBER_OF_SURCHARGED_PAID_SINCE_INITIATLIZATION
 PA2010_VALUE_OF_SURCHARGES_PAID_SINCE_INITIALIZATION
 PA2011_NUMBER_OF_SURCHARGED_PAID_SINCE_LAST_RESET
 PA2012_VALUE_OF_SURCHARGES_PAID_SINCE_LAST_RESET
 PA303_NUMBER_OF_TEST_VENDS_SINCE_LAST_RESET
 PA304_VALUE_OF_TEST_VENDS_SINCE_LAST_RESET)
 PA402_VALUE_OF_FREE_VENDS_SINCE_INITIALIZATION
 PA403_NUMBER_OF_FREE_VENDS_SINCE_LAST_RESET
 PA404_VALUE_OF_FREE_VENDS_SINCE_LAST_RESET
 PA503_NUMBER_OF_TIMES_SOLD_OUT_PRODUCT_SELECTED
 PA707_NUMER_OF_CASH_SALES_SINCE_LAST_RESET
 PA708_VALUE_OF_CASH_SALES_SINCE_LAST_RESET
 PA707_NUMER_OF_CASHLESS_SALES_SINCE_LAST_RESET
 PA708_VALUE_OF_CASHLESS_SALES_SINCE_LAST_RESET
 PA803_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_LAST_RESET
 PA804_VALUE_OF_MIXED_PAYMENT_CASH_AMOUNT_SINCE_LAST_RESET
 EA1_EJL  Failed to drop by selection
 EA102_EJL_DATEYEAR_OF_EVENT
 EA102_EJL_DATEMONTH_OF_EVENT
 EA102_EJL_DATEDAY_OF_EVENT
 EA103_EJL_TIMEHOUR_OF_EVENT
 EA103_EJL_TIMEMINUTE_OF_EVENT
 EA202_EJL_NUMBER_OF_EVENTS_SINCE_LAST_RESET
 EA203_EJL_NUMBER_OF_EVENTS_SINCE_INITIALIZATION
 EA205_EJL_CURRENT_EVENT_ACTIVITY
 EA206_EJL_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES
 EA1_ELA failed vend by selection
 EA102_ELA_DATEYEAR_OF_EVENT
 EA102_ELA_DATEMONTH_OF_EVENT
 EA102_ELA_DATEDAY_OF_EVENT
 EA103_ELA_TIMEHOUR_OF_EVENT
 EA103_ELA_TIMEMINUTE_OF_EVENT
 EA202_ELA_NUMBER_OF_EVENTS_SINCE_LAST_RESET
 EA203_ELA_NUMBER_OF_EVENTS_SINCE_INITIALIZATION
 EA205_ELA_CURRENT_EVENT_ACTIVITY
 EA206_ELA_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES

 */
 // Scale structure
#define SERIAL_NUMBER_SIZE 20
typedef PACK ( struct
{
    char scaleSerialNumber[SERIAL_NUMBER_SIZE];
    char scaleRevision;
    uint32_t scaleCurrentWeight;
    uint32_t scaleChangeWeight;
} )locker_scale_data_t;

static locker_scale_data_t lockerScaleData[120];

static uint8_t DLE_State;
#define RX_Buffsize 2700
static bool  RX_BlockReceiveIP;
static bool DEXNonCommTransferIP;
static bool DEXNonCommTransferBlockComplete;
static uint8_t RX_BlockPointer = 0;
static uint8_t RX_Block[RX_Buffsize];
static uint8_t RX_Buff[RX_Buffsize];
static uint8_t TX_Buff[512];
static uint8_t DEX_State;
static uint8_t ExternalDEXPortTransferSize;
static uint8_t MasterCommID[12];
static char SendReceive;
static uint16_t CRC_16_SUM;
extern uint16_t G85CRC_16_SUM;
extern uint16_t crc16_tbl[256];
extern DEX_DATA_STRUCTURE DexDataStructure;
static bool ACKType = false;
static uint16_t BlockCount;
static uint32_t SegmentCount;
static uint16_t SelectionOffset;
extern char GetDecimalPlaces ( void );
extern uint16_t GetCountryCode ( void );
extern bool dbcGetPriceForItem ( uint16_t item_number, uint16_t* price );
extern void DEXReadAccounting ( void );
extern void DEXReadTimeAccounting ( void );
extern uint16_t dbcGetServicePassword ( void );
extern dex_type_t dbcGetDexStatus ( void );
bool Char_Received = false;
static bool FullDEX = false;
static bool TinyDEX = true;
extern void getRefrigerationMode ( char* refermode );
extern bool dbcGetItemDefinition ( uint16_t item_number, uint8_t* itemdefinition );
extern void RefrigerationLogMode ( bool state );
extern bool dbcGetProductinfo ( uint16_t item_number, uint8_t* prodcuctifo );
extern bool dbcUpdateProductinfo ( uint16_t item_number, uint8_t* prodcuctifo );
extern bool dbcUpdateItemDefinition ( uint16_t item_number, uint8_t* itemdefinition );
extern void dbcSavePriceForRange ( uint16_t slection_start, uint16_t slection_end, uint16_t price );
extern bool dbcGetDEXResetOnReadEnabled ( void );
extern void sendOperateThread ( Operate_State_T type, uint8_t* message, uint8_t  size );
extern uint32_t GetCash_In_Tubes ( void );
extern uint8_t dbcGetSelectionDigits ( void );
extern uint8_t mccGetTubeLevel ( uint8_t tube );
extern bool GetDoor ( void );
extern void dex_comm_thread_run ( void );
extern void clrSBC_DB_Transf_flag ( void );
extern bool dbcGetHealthAlert ( void );
extern bool dbcGetPullDownExceeded ( void );
extern bool TemperatureSensorFailStatus ( void );
extern bool dbcGetCoinMechFailure ( void );
extern bool dbcGetCashless1Failure ( void );
extern bool dbcGetCashless2Failure ( void );
extern bool dbcGetKeyboardFailure ( void );
extern bool dbcGetBillValidatorFailure ( void );
extern void SetMACHINE_ASSET_NUMBER ( char* asset_number );
extern void SetMACHINE_LOCATION ( char* machine_location );
extern const char FW_VERSION[6];
extern void SBC_DEXConfigDataType ( void );
extern void dbcSaveFlexConnect_UnlockKey ( uint64_t unlockkey );
extern bool FlexConnectUnlocked ( void );
extern uint32_t GetmdbTotal_Credit ( void );
extern uint8_t dbcGetMAXColumns ( void );
extern void UpdateDEXAccountingStructure ( void );
extern bool  dbcGetScaleEnable ( void );
extern int8_t GetTemperatureMAX ( void );
extern int8_t GetTemperature ( uint8_t sensor );
bool transmit; // This is used to prevent reading extra characters in our G85 CRC creation

void dex_comm_thread ( void const* argument );
osThreadId tid_dex_comm_thread;
osThreadDef ( dex_comm_thread, osPriorityBelowNormal, 1, 0 ); // thread object

const size_t TX_Buff_SZ = (sizeof( TX_Buff ) - 1);


int GetWeightForSerialNumber ( char* serialnumber )
{
    //returns uint32_t weight or NULL if no weight found
    uint8_t counter;
    for ( counter = 0; counter < 120; counter++ )
    {
        if ( strstr ( lockerScaleData[counter].scaleSerialNumber, serialnumber ) != NULL )
        {
            return lockerScaleData[counter].scaleCurrentWeight;
        }
    }
    return 0;
}



uint32_t GetWeightChangeSerialNumber ( char* serialnumber )
{
    uint8_t counter;
    uint32_t largest_change = 0;
    for ( counter = 0; counter < 120; counter++ )
    {
        if ( lockerScaleData[counter].scaleChangeWeight > largest_change )
        {
            largest_change = lockerScaleData[counter].scaleChangeWeight;
            memset ( serialnumber, 0, SERIAL_NUMBER_SIZE );
            memcpy ( serialnumber, lockerScaleData[counter].scaleSerialNumber, MIN(strlen( lockerScaleData[counter].scaleSerialNumber ), (SERIAL_NUMBER_SIZE - 1)) );
        }
    }
    return largest_change;
}



uint8_t GetMaxWeightChangeOffset ( int32_t weightchange )
{
    uint8_t counter, largest_change = 0;
    weightchange = 0;
    for ( counter = 0; counter < 120; counter++ )
    {
        if ( abs ( lockerScaleData[counter].scaleCurrentWeight ) > abs ( weightchange ) )
        {
            largest_change = counter;
            weightchange = lockerScaleData[counter].scaleCurrentWeight;
        }
    }
    return largest_change;
}


void GetSerialNumberByOffset ( uint16_t offset, char* serialnumber )
{
    if ( offset < 120 )
    {
        memset ( serialnumber, 0, SERIAL_NUMBER_SIZE );
        strcpy ( serialnumber, lockerScaleData[offset].scaleSerialNumber );
    }
}



uint32_t dec2bcd ( uint16_t dec )
{
    uint32_t result = 0;
    int shift = 0;
    while ( dec )
    {
        result += ( dec % 10 ) << shift;
        dec = dec / 10;
        shift += 4;
    }
    return result;
}


char nibble2bcd ( uint8_t nibble )
{
    if ( nibble <= 0x09 )
    {
        return nibble + 0x30;
    }
    else if ( nibble <= 0x0F )
    {
        return nibble + 0x37;
    }
    return 0x00;
}



void ParseData ( char* datatoparse )
{
    uint8_t counter;
    uint32_t PTRoffsetSave;
    static int16_t pointer = 0;
    static uint16_t count;
    static char* ptr;
    static char* startptr;
    static char* endptr;
    static char* bptr;
    static char sn[31];
    static uint16_t item_number, length;
    static uint16_t price;
    static uint64_t mul, flexConnect_unlockKey;
    uint8_t itemdefinition[20];
    // Parse the Refer log mode turn on. MC5*DIA*RFG*1
    ptr = strstr ( datatoparse, "MC5*DIA*RFG*1" );
    if ( ptr != NULL )
    {
        RefrigerationLogMode ( true );
        return;
    }
    ptr = strstr ( datatoparse, "MC5*DIA*RFG*0" );
    // Parse the Refer log mode turn off. MC5*DIA*RFG*0
    if ( ptr != NULL )
    {
        RefrigerationLogMode ( false );
        return;
    }
    // Parse the IC1******"Asset Number"
    ptr = strstr ( datatoparse, "IC1******" );
    if ( ptr != NULL )
    {
        startptr = ptr;
        ptr = strstr ( startptr + 10, "*" );
        // Pointing to Serial Number
        if ( ptr == NULL )
        {
            ptr = strstr ( startptr + 10, CR_LF );
            if ( ( ptr == NULL ) || ( ( ptr - startptr ) < 2 ) )
            {
                return;
            }
        }
        ptr = startptr + 9;
        memset ( sn, 0x00, 20 );
        pointer = 0;
        while ( ( ptr != NULL && ptr[pointer] != '\0' ) && ( ptr[pointer] != '*' ) && ( ptr[pointer] != CR ) && ( pointer <= 20 ) )
        {
            sn[pointer] = ptr[pointer];
            pointer++;
        }
        SetMACHINE_ASSET_NUMBER ( sn );
        SBC_DEXConfigDataType ();
        return;
    }
    // Parse the IC1******"FlexConnect code"
    ptr = strstr ( datatoparse, "IC1*****" );
    if ( ptr != NULL )
    {
        startptr = ptr;
        ptr = strstr ( startptr + 9, "*" );
        // Pointing to flex connect code
        if ( ptr == NULL )
        {
            ptr = strstr ( startptr + 9, CR_LF );
            if ( ( ptr == NULL ) || ( ( ptr - startptr ) < 2 ) )
            {
                return;
            }
        }
        ptr--;
        pointer = 0;
        count = ( ptr - ( startptr + 9 ) ) + 1;
        flexConnect_unlockKey = 0;
        mul = 1;
        while ( ( ptr != NULL && ptr[pointer] != '\0' ) && ( ptr[pointer] != '*' ) && ( ptr[pointer] != CR ) && ( count + pointer >= 0 ) )
        {
            flexConnect_unlockKey = flexConnect_unlockKey + ( ptr[pointer] - 0x30 ) * mul;
            pointer--;
            mul = mul * 10;
        }
        dbcSaveFlexConnect_UnlockKey ( flexConnect_unlockKey );
        return;
    }
    // Parse the IC1****"Machine Location"
    ptr = strstr ( datatoparse, "IC1****" );
    if ( ptr != NULL )
    {
        startptr = ptr;
        ptr = strstr ( startptr + 8, "*" );
        // Pointing to Serial Number
        if ( ptr == NULL )
        {
            ptr = strstr ( startptr + 8, CR_LF );
            if ( ( ptr == NULL ) || ( ( ptr - startptr ) < 2 ) )
            {
                return;
            }
        }
        ptr = startptr + 7;
        memset ( sn, 0x0, 30 );
        pointer = 0;
        while ( ( ptr != NULL && ptr[pointer] != '\0' ) && ( ptr[pointer] != '*' ) && ( ptr[pointer] != CR ) && ( pointer <= 30 ) )
        {
            sn[pointer] = ptr[pointer];
            pointer++;
        }
        SetMACHINE_LOCATION ( sn );
        SBC_DEXConfigDataType ();
        return;
    }
    // Parse the IC1*"SERIAL NUMBER"
    ptr = strstr ( datatoparse, "IC1*" );
    if ( ptr != NULL )
    {
        startptr = ptr;
        ptr = strstr ( startptr + 5, "*" );
        // Pointing to Serial Number
        if ( ptr == NULL )
        {
            ptr = strstr ( startptr + 5, CR_LF );
            if ( ( ptr == NULL ) || ( ( ptr - startptr ) < 2 ) )
            {
                return;
            }
        }
        ptr = startptr + 4;
        memset ( sn, 0x30, 20 );
        pointer = 0;
        while ( ( ptr != NULL && ptr[pointer] != '\0' ) && ( ptr[pointer] != '*' ) && ( ptr[pointer] != CR ) && ( pointer <= 20 ) )
        {
            sn[pointer] = ptr[pointer];
            pointer++;
        }
        SetMACHINE_SERIAL_NUMBER ( sn );
        SetCONTROL_BOARD_SERIAL_NUMBER ( sn );
        return;
    }
    PTRoffsetSave = 0;
    // Get selection data = May be more than one set.
    // If door open "STOP" dont accept selection data
    if ( GetDoor () )
    {
        return;
    }
    for ( counter = 0; counter < 20; counter++ )
    {
        // Parse the PC1*020*1000*Selection 20****0*0*********** record
        ptr = strstr ( datatoparse + PTRoffsetSave, "PC1*" );
        if ( ptr == NULL )
        {
            return;
        }
        PTRoffsetSave = ( ptr + 5 ) - datatoparse;
        startptr = ptr;
        ptr = strstr ( ptr + 4, "*" );
        // Pointing to selection
        if ( ( ptr == NULL ) || ( ( ptr - startptr ) < 2 ) )
        {
            return;
        }
        // if 2 digit selection
        if ( dbcGetSelectionDigits () == 2 )
        {
            item_number = ( ( ( ptr[-1] ) - 0x30 ) + ( ( ptr[-2] ) - 0x30 ) * 10 );
            if ( item_number > 99 )
            {
                return;
            }
        }
        // if 3 digit selection
        else if ( ( ptr[-3] > '0' ) && ( ptr[-3] <= '9' ) && ( dbcGetSelectionDigits () == 3 ) )
        {
            item_number = ( ( ( ptr[-1] ) - 0x30 ) + ( ( ( ptr[-2] ) - 0x30 ) * 10 ) + ( ( ptr[-3] ) - 0x30 ) * 100 );
            if ( item_number > 999 )
            {
                return;
            }

        }
        // if 4 digit selection
        else if ( ( ptr[-4] > '0' ) && ( ptr[-4] <= '9' ) && ( dbcGetSelectionDigits () == 4 ) )
        {
            item_number = ( ( ( ptr[-1] ) - 0x30 ) + ( ( ( ptr[-2] ) - 0x30 ) * 10 ) + ( ( ( ptr[-3] ) - 0x30 ) * 100 ) + ( ( ptr[-4] ) - 0x30 ) * 1000 );
            if ( item_number > 1412 )
            {
                return;
            }
        }
        else
        {
            return;
        }
        startptr = ptr;
        endptr = strstr ( ptr + 1, "PC1*" );
        bptr = strstr ( ptr + 1, "*" );
        if ( endptr == NULL )
        {
            endptr = bptr;
        }
        if ( ( endptr != NULL ) && ( bptr != NULL ) )
        {
            if ( endptr > bptr )
            {
                endptr = bptr;
            }
        }
        if ( endptr == NULL )
        {
            endptr = strstr ( ptr + 1, CHAR_CR );
        }
        if ( endptr == NULL )
        {
            endptr = strstr ( ptr + 1, CHAR_LF );
        }
        ptr = endptr;
        // Pointing to Price 
        if ( ( ptr == NULL ) || ( ( ptr - startptr ) < 2 ) )
        {
            return;
        }
        endptr = ptr - 1;
        mul = 1;
        price = 0;
        while ( startptr < endptr )
        {
            price = price + ( endptr[0] - 0x30 ) * mul;
            endptr--;
            mul = mul * 10;
        }
        dbcSavePriceForRange ( item_number, item_number, price );
        startptr = ptr;
        ptr = strstr ( ptr + 1, "*" );
        if ( ptr == NULL )
        {
            return;
        }
        length = ( ptr - startptr ) - 1;
        endptr = strstr ( ptr + 1, "*" );
        if ( endptr == NULL )
        {
            endptr = strstr ( ptr + 1, CHAR_CR );
        }
        if ( endptr == NULL )
        {
            endptr = strstr ( ptr + 1, CHAR_LF );
        }
        if ( endptr == NULL )
        {
            endptr = strstr ( ptr + 1, "PC1*" );
        }
        ptr = endptr;
        // Pointing to item definition 
        if ( ( ptr == NULL ) || ( length < 2 ) || ( length >= 20 ) )
        {
        }
        else
        {
            memset ( itemdefinition, 0, 20 );
            memcpy ( itemdefinition, startptr + 1, length );
            dbcUpdateItemDefinition ( item_number, itemdefinition );
        }
    }
}


bool TX_DEXmemcpy ( uint8_t datatype, const uint32_t* dataaddress )
{
    char converted_data[10];
    uint8_t tx_length, G85_ptr;
    static uint32_t longdata;
    static uint32_t loopcount;
    dex_type_t dex_status;
    uint8_t* dataaddress_byte = (uint8_t*)dataaddress;

    dex_status = dbcGetDexStatus ();

    if ( dex_status == FULL_DEX_EN )
    {
        FullDEX = true;
        TinyDEX = false;
    }
    else if ( dex_status == TINY_DEX_EN )
    {
        TinyDEX = true;
        FullDEX = false;
    }
    else
    {
        TinyDEX = false;
        FullDEX = false;
    }

    switch ( datatype )
    {
        case END_OF_SEGMENT:
            SegmentCount++;
            if ( ( TX_Buff[strlen ( (char*)TX_Buff ) - 1] ) == '*' )
            {
                TX_Buff[strlen ( (char*)TX_Buff ) - 1] = 0;
            }
            // fall through
        case ASCII:
            strncat ( (char*)TX_Buff, (char*)dataaddress, TX_Buff_SZ );
            break;
        case LAST:
            strncat ( (char*)TX_Buff, (char*)dataaddress, TX_Buff_SZ );
            send_dex_data ( TX_Buff, strlen ( (char*)TX_Buff ), 0x01, osWaitForever );
            tx_length = strlen ( (char*)TX_Buff );
            TX_Buff[tx_length - 2] = TX_Buff[tx_length - 1];
            CRC_16_SUM = crc16 ( TX_Buff + 2, tx_length - 3 );
            TX_Buff[0] = (uint8_t)( CRC_16_SUM & 0x00ff );
            TX_Buff[1] = (uint8_t)( CRC_16_SUM >> 8 );
            send_dex_data ( TX_Buff, 2, 0x01, osWaitForever );
            receive_dex_data ( &RX_Buff, 2, 0x01, 1000 );
            if ( ACKType )
            {
                if ( memcmp ( RX_Buff, DLE1, 2 ) == 0 )
                {
                    return true;
                }
            }
            else
            {
                ACKType = false;
                if ( memcmp ( RX_Buff, DLE0, 2 ) == 0 )
                {
                    return true;
                }
            }
            break;
        case LONGNS:
            longdata = *dataaddress;
            ltoa ( longdata, converted_data, 10 );
            if ( converted_data[0] == 0 )
            {
                converted_data[0] = '0';
                converted_data[1] = '0';
                converted_data[2] = 0;
            }
            if ( converted_data[1] == 0 )
            {
                converted_data[1] = converted_data[0];
                converted_data[0] = '0';
                converted_data[2] = 0;
            }
            strncat ( (char*)TX_Buff, converted_data, TX_Buff_SZ );
            break;
        case LONG:
            longdata = *dataaddress;
            ltoa ( longdata, converted_data, 10 );
            strncat ( (char*)TX_Buff, converted_data, TX_Buff_SZ );
            strncat ( (char*)TX_Buff, STAR, TX_Buff_SZ );
            break;
        case SHORT_BCD:    // 2 bytes of BCD(int (short) to)  ASCII
            converted_data[0] = nibble2bcd ( dataaddress_byte[0] & 0x0F );
            converted_data[1] = nibble2bcd ( dataaddress_byte[0] >> 4 );
            converted_data[2] = nibble2bcd ( dataaddress_byte[1] & 0x0F );
            converted_data[3] = nibble2bcd ( dataaddress_byte[1] >> 4 );
            converted_data[4] = 0;
            strncat ( (char*)TX_Buff, converted_data, TX_Buff_SZ );
            break;
        case SHORTI_BCD:    // 2 bytes of BCD(int (short) to) inverted ASCII
            converted_data[1] = nibble2bcd ( dataaddress_byte[0] & 0x0F );
            converted_data[0] = nibble2bcd ( dataaddress_byte[0] >> 4 );
            converted_data[3] = nibble2bcd ( dataaddress_byte[1] & 0x0F );
            converted_data[2] = nibble2bcd ( dataaddress_byte[1] >> 4 );
            converted_data[4] = 0;
            strncat ( (char*)TX_Buff, converted_data, TX_Buff_SZ );
            break;
        case CHAR_BCD:    // 1 Byte (char) of BCD to Ascii
            converted_data[0] = nibble2bcd ( dataaddress_byte[0] & 0x0F );
            converted_data[1] = nibble2bcd ( dataaddress_byte[0] >> 4 );
            converted_data[2] = 0;
            strncat ( (char*)TX_Buff, converted_data, TX_Buff_SZ );
            break;
        default:
            break;
    }
    tx_length = strlen ( (char*)TX_Buff );
    if (datatype == END_OF_SEGMENT)
    {
        // Update G85
        //controls errant character reading, was reading dxs line before, this skips it.
        G85_ptr = 2;
        if(!transmit)
        {
            G85_ptr = 28;
        }
        for( ; G85_ptr < tx_length; G85_ptr++ )
        {
            
            (void) G85crc16 ( TX_Buff[G85_ptr] );
        }

        transmit = true;
        if ( DEXNonCommTransferIP )
        {
            TX_Buff[tx_length] = 0;
            loopcount = 100;
            DEXNonCommTransferBlockComplete = true;
            while ( ( loopcount > 0 ) && ( DEXNonCommTransferBlockComplete == true ) )
            {
                loopcount--;
                osDelay ( 10 );
            }
            return true;
        }
        strncat ( (char*)TX_Buff, DLEETB, TX_Buff_SZ );
        tx_length += 2;
        send_dex_data ( TX_Buff, tx_length, 0x01, osWaitForever );
        TX_Buff[tx_length - 2] = TX_Buff[tx_length - 1];
        CRC_16_SUM = crc16 ( TX_Buff + 2, tx_length - 3 );
        TX_Buff[0] = (uint8_t)( CRC_16_SUM & 0x00ff );
        TX_Buff[1] = (uint8_t)( CRC_16_SUM >> 8 );
        send_dex_data ( TX_Buff, 2, 0x01, osWaitForever );
        receive_dex_data ( &RX_Buff, 2, 0x01, 1000 );
        if ( ACKType )
        {
            ACKType = false;
            if ( memcmp ( RX_Buff, DLE1, 2 ) == 0 )
            {
                osDelay ( 50 );
                BlockCount++;
                memset ( (char*)TX_Buff, 0x00, sizeof ( TX_Buff ) );
                strncat ( (char*)TX_Buff, DLESTX, TX_Buff_SZ );
                return true;
            }
        }
        else
        {
            ACKType = true;
            if ( memcmp ( RX_Buff, DLE0, 2 ) == 0 )
            {
                osDelay ( 50 );
                BlockCount++;
                memset ( (char*)TX_Buff, 0x00, sizeof ( TX_Buff ) );
                strncat ( (char*)TX_Buff, DLESTX, TX_Buff_SZ );
                return true;
            }
        }
    }
    return false;
}


/*----------------------------------------------------------------------------
 *      dex comm thread
 *---------------------------------------------------------------------------*/

int Init_dex_comm_thread ( void )
{
    tid_dex_comm_thread = osThreadCreate ( osThread ( dex_comm_thread ), NULL );
    if ( !tid_dex_comm_thread ) return( -1 );
    return( 0 );
}

//CMSIS-RTOS Thread - UART command thread 
void dex_comm_thread ( const void* args )
{
    char* sptr;
    char* eptr;
    char* bptr;
    uint8_t counter;
    uint32_t weight_save;
    static uint16_t receive_retries;
    static uint32_t price;
    static uint32_t selection;
    static uint8_t row, col;
    int8_t temp;
    RTC rtc_time = { 0 };
    char currentTemp[4];
    uint32_t password1 = dbcGetServicePassword ();
    uint32_t password2 = dbcGetServicePassword ();
    uint8_t product_identification[20] = { "NoProductIdentifier" };
    bool selection_status;

    DEX_State = init_dex_comm ();

    while ( 1 )
    {
        dex_comm_thread_run ();
        switch ( DEX_State )
        {
            case WAIT_FOR_ENQ:
                UpdateDEXAccountingStructure ();
                BlockCount = 0;
                SegmentCount = 1;
                CRC_16_SUM = 0;
                SendReceive = 0;
                SelectionOffset = 0;
                memset ( &RX_Buff, 0x00, sizeof ( RX_Buff ) );
                memset ( &TX_Buff, 0x00, sizeof ( TX_Buff ) );
                receive_dex_data ( &RX_Buff[0], 1, 0x01, 1000 );
                if ( RX_Buff[0] == ENQ )
                {
                    if ( ( GetOperate_State () == IDLE_STATE ) && ( GetmdbTotal_Credit () == 0 ) )
                    {
                    }
                    else
                    {
                        if ( ( SBCGetCashLessVend () ) && ( GetOperate_State () == PROCESS_VEND ) )
                        {
                            //Allow dexing in FlexConnect mode
                        }
                        else
                        {
                            break;
                        }
                    }
                    DEXNonCommTransferIP = false;
                    send_dex_data ( DLE0, 2, 0x01, osWaitForever );
                    DEX_State = FIRST_HAND_SHAKE;
                    receive_dex_data ( &RX_Buff, 23, 0x01, 1000 );
                }
                break;
            case FIRST_HAND_SHAKE:
                // Decode first hand shake
                if ( memcmp ( RX_Buff, DLESOH, 2 ) != 0 )
                {   // not the hand shake so get out
                    DEX_State = WAIT_FOR_ENQ;
                    sendOperateThread ( IDLE_STATE, 0, 0 );
                    break;
                }
                // Check read revision and DLE,ETX
                if ( memcmp ( RX_Buff + 13, READ_REVISION_AND_LEVEL, 8 ) != 0 )
                {   // not the hand shake so get out
                    DEX_State = WAIT_FOR_ENQ;
                    sendOperateThread ( IDLE_STATE, 0, 0 );
                    break;
                }
                // Check CRC
                // with bypass of ETX
                RX_Buff[19] = RX_Buff[20];
                CRC_16_SUM = crc16 ( RX_Buff + 2, 18 );
                if ( ( (uint8_t)( CRC_16_SUM & 0x00ff ) == RX_Buff[21] ) &&
                     ( (uint8_t)( CRC_16_SUM >> 8 ) == RX_Buff[22] ) )
                {
                    // Save Comm ID
                    memcpy ( MasterCommID, RX_Buff + 2, 10 );
                    //  Audit or Config upload "R" = Audit - "S" = Configuration Upload
                    SendReceive = RX_Buff[12];
                    send_dex_data ( DLE1, 2, 0x01, osWaitForever );
                    //  Receive EOT
                    receive_dex_data ( &RX_Buff, 1, 0x01, 1000 );
                    // EOT Received??
                    if ( RX_Buff[0] == EOT )
                    {
                        DEX_State = SECOND_HAND_SHAKE;
                        break;
                    }
                }
                // There was a failure of the first handshake so NAK and return to 
                TX_Buff[0] = NAK;
                send_dex_data ( &TX_Buff[0], 1, 0x01, osWaitForever );
                DEX_State = WAIT_FOR_ENQ;
                sendOperateThread ( IDLE_STATE, 0, 0 );
                break;
            case SECOND_HAND_SHAKE:
                TX_Buff[0] = ENQ;

                send_dex_data ( &TX_Buff[0], 1, 0x01, osWaitForever );
                //  Receive DLE0 = ACK0
                receive_dex_data ( &RX_Buff, 2, 0x01, 1000 );
                if ( memcmp ( RX_Buff, DLE0, 2 ) == 0 )
                {
                    osDelay ( 100 );
                    memcpy ( TX_Buff, SEND_REVISION_AND_LEVEL, 22 );
                    send_dex_data ( TX_Buff, 22, 0x01, osWaitForever );
                    TX_Buff[20] = TX_Buff[21];
                    CRC_16_SUM = crc16 ( TX_Buff + 2, 19 );
                    TX_Buff[0] = (uint8_t)( CRC_16_SUM & 0x00ff );
                    TX_Buff[1] = (uint8_t)( CRC_16_SUM >> 8 );
                    send_dex_data ( TX_Buff, 2, 0x01, osWaitForever );
                    //  Receive DLE1 = ACK1
                    receive_dex_data ( &RX_Buff, 2, 0x01, 1000 );
                    if ( memcmp ( RX_Buff, DLE1, 2 ) == 0 )
                    {
                        TX_Buff[0] = EOT;
                        send_dex_data ( &TX_Buff[0], 1, 0x01, osWaitForever );
                        //This is where we split Audit or Config upload "R" = Audit - "S" = Configuration Upload based on first handshake
                        if ( SendReceive == 'S' )
                        {   // Config
                            //  Receive ENQ
                            receive_dex_data ( &RX_Buff, 1, 0x01, 1000 );
                            if ( RX_Buff[0] == ENQ )
                            {
                                osDelay ( 5 );
                                send_dex_data ( DLE0, 2, 0x01, osWaitForever );
                                DEX_State = BLOCK_TRANSFER_IN;
                                DLE_State = 0;
                                break;
                            }
                        }
                        else
                        {   // Audit  
                            osDelay ( 100 );
                            TX_Buff[0] = ENQ;
                            send_dex_data ( &TX_Buff[0], 1, 0x01, osWaitForever );
                            //  Receive DLE0 = ACK0
                            receive_dex_data ( &RX_Buff, 2, 0x01, 1000 );
                            if ( memcmp ( RX_Buff, DLE0, 2 ) == 0 )
                            {
                                osDelay ( 100 );
                                ACKType = true;
                                // Initialize block transfer
                                BlockCount = 0;
                                SelectionOffset = 0;
                                memset ( (char*)TX_Buff, 0x00, sizeof ( TX_Buff ) );
                                strncat ( (char*)TX_Buff, DLESTX, TX_Buff_SZ );
                                DEX_State = BLOCK_TRANSFER_OUT;
                                break;
                            }
                        }
                    }
                }
                // There was a failure of the second handshake so NAK and return to 
                TX_Buff[0] = NAK;
                send_dex_data ( &TX_Buff[0], 1, 0x01, osWaitForever );
                DEX_State = WAIT_FOR_ENQ;
                sendOperateThread ( IDLE_STATE, 0, 0 );
                break;
            case BLOCK_TRANSFER_IN:
                RX_BlockReceiveIP = true;
                RX_BlockPointer = 0;
                receive_retries = 0;
                while ( RX_BlockReceiveIP )
                {
                    dex_comm_thread_run ();
                    Char_Received = false;
                    receive_dex_data ( &RX_Buff, 1, 0x01, 1000 );
                    RX_Block[RX_BlockPointer] = RX_Buff[0];
                    RX_BlockPointer++;
                    if ( ( RX_Buff[0] == ETB ) || ( RX_Buff[0] == ETX ) )
                    {
                        RX_BlockReceiveIP = false;
                    }
                    if ( Char_Received == false )
                    {
                        receive_retries++;
                        if ( receive_retries > 20 )
                        {
                            DEX_State = WAIT_FOR_ENQ;
                            sendOperateThread ( IDLE_STATE, 0, 0 );
                            break;
                        }
                    }
                }
                // Get CRC  
                receive_dex_data ( &RX_Buff, 2, 0x01, 1000 );
                RX_Block[RX_BlockPointer - 2] = RX_Block[RX_BlockPointer - 1];
                // check CRC
                CRC_16_SUM = crc16 ( RX_Block + 2, RX_BlockPointer - 3 );
                if ( ( (uint8_t)( CRC_16_SUM & 0x00ff ) == RX_Buff[0] ) &&
                     ( (uint8_t)( CRC_16_SUM >> 8 ) == RX_Buff[1] ) )
                {
                    ParseData ( (char*)RX_Block );
                    if ( DLE_State == 0 )
                    {
                        DLE_State = 1;
                        send_dex_data ( DLE1, 2, 0x01, osWaitForever );
                    }
                    else
                    {
                        DLE_State = 0;
                        send_dex_data ( DLE0, 2, 0x01, osWaitForever );
                    }
                    // Done??
                    if ( RX_Block[RX_BlockPointer - 1] == ETX )
                    {
                        DEX_State = TRANSFER_COMPLETE;
                        SBCSendMsg ( WRITE_DB_REC_TRAN );
                        logger_fput ( DEX_WRITE_TXT );
                    }
                    memset ( &RX_Block, 0x00, sizeof ( RX_Block ) );
                    break;
                }

                // There was a failure of the block so NAK and return to 
                TX_Buff[0] = NAK;
                send_dex_data ( &TX_Buff[0], 1, 0x01, osWaitForever );
                //USARTdrv->Send(&TX_Buff[0],1);
                //osSignalWait(0x01, osWaitForever);
                break;
            case BLOCK_TRANSFER_OUT:
                DEXReadTimeAccounting ();
                TX_DEXmemcpy ( ASCII, (uint32_t*)DXS );
                transmit = false;
                G85CRC_16_SUM = 0;
                //  Get the system settings for future use
                memcpy ( DexDataStructure.DexDeviceIDFields.CB103_VMC_CONTROL_BOARD_BUILD_STANDARD, FW_VERSION, MIN( sizeof DexDataStructure.DexDeviceIDFields.CB103_VMC_CONTROL_BOARD_BUILD_STANDARD, sizeof(FW_VERSION) ) );
                memcpy ( DexDataStructure.DexDeviceIDFields.ID103_MACHINE_BUILD_STANDARD, FW_VERSION, sizeof DexDataStructure.DexDeviceIDFields.ID103_MACHINE_BUILD_STANDARD );
                TX_DEXmemcpy ( ASCII, (uint32_t*)ST );
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)ID1 );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.ID101_MACHINE_SERIAL_NUMBER );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.ID102_MACHINE_MODEL_NUMBER );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.ID103_MACHINE_BUILD_STANDARD );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.ID104_MACHINE_LOCATION );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                if ( FlexConnectUnlocked () )
                {
                    TX_DEXmemcpy ( ASCII, (uint32_t*)UNLOCKED );
                }
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.ID106_MACHINE_ASSET_NUMBER );
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                if ( !TinyDEX )
                {
                    TX_DEXmemcpy ( ASCII, (uint32_t*)ID4 );
                    DexDataStructure.DexDeviceIDFields.ID401_DECIMAL_POINT_POSITION[0] = GetDecimalPlaces ();
                    DexDataStructure.DexDeviceIDFields.ID402_CURRENCY_NUMERIC_CODE[0] = ( GetCountryCode () & 0x00ff );
                    DexDataStructure.DexDeviceIDFields.ID402_CURRENCY_NUMERIC_CODE[1] = ( GetCountryCode () >> 8 );
                    DexDataStructure.DexDeviceIDFields.ID403_CURRENCY_ALPHABETIC_CODE[0] = ( GetCountryCode () & 0x00ff );
                    DexDataStructure.DexDeviceIDFields.ID403_CURRENCY_ALPHABETIC_CODE[1] = ( GetCountryCode () >> 8 );
                    TX_DEXmemcpy ( CHAR_BCD, (uint32_t*)&DexDataStructure.DexDeviceIDFields.ID401_DECIMAL_POINT_POSITION );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    TX_DEXmemcpy ( SHORTI_BCD, (uint32_t*)&DexDataStructure.DexDeviceIDFields.ID402_CURRENCY_NUMERIC_CODE );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    TX_DEXmemcpy ( SHORTI_BCD, (uint32_t*)&DexDataStructure.DexDeviceIDFields.ID403_CURRENCY_ALPHABETIC_CODE );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                }
                TX_DEXmemcpy ( ASCII, (uint32_t*)CB1 );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.CB101_VMC_CONTROL_BOARD_SERIAL_NUMBER );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.CB102_VMC_CONTROL_BOARD_MODEL_NUMBER );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.CB103_VMC_CONTROL_BOARD_BUILD_STANDARD );
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)VA1 );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.VA101_VALUE_OF_ALL_PAID_VEND_SALES_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.VA102_NUMBER_OF_ALL_PAID_VEND_SALES_SINCE_INITIALIZATION );
                if ( !TinyDEX )
                {
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.VA103_VALUE_OF_ALL_PAID_VEND_SALES_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.VA104_NUMBER_OF_ALL_PAID_VEND_SALES_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.VA105_VALUE_OF_ALL_DISCOUNTS_SINCE_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.VA106_NUMBER_OF_ALL_DISCOUNTED_PAID_VENDS_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.VA107_VALUE_OF_ALL_DISCOUNTS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.VA108_NUMBER_OF_ALL_DISCOUNTED_PAID_VENDS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)VA2 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.VA201_TEST_VEND_VALUE_OF_SALES_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.VA202_NUMBER_OF_TEST_VENDS_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.VA203_TEST_VEND_VALUE_OF_SALES_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.VA204_NUMBER_OF_TEST_VENDS_SINCE_LAST_RESET );
                }
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)VA3 );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.VA301_FREE_VEND_VALUE_OF_SALES_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.VA302_NUMBER_OF_FREE_VEND_SALES_SINCE_INITIALIZATION );
                if ( !TinyDEX )
                {
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.VA303_FREE_VEND_VALUE_OF_SALES_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.VA304_NUMBER_OF_FREE_VEND_SALES_SINCE_LAST_RESET );
                }
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)CA1 );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.CA101_CHANGER_SERIAL_NUMBER );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.CA102_COIN_MECHANISM_MODEL_NUMBER );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( SHORT_BCD, (uint32_t*)&DexDataStructure.DexDeviceIDFields.CA103_COIN_MECHANISM_SOFTWARE_REVISION );
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)CA2 );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA201_VALUE_OF_CASH_SALES_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA202_NUMBER_OF_CASH_VENDS_SINCE_INITIALIZATION );
                if ( !TinyDEX )
                {
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA203_VALUE_OF_CASH_SALES_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA204_NUMBER_OF_CASH_VENDS_SINCE_LAST_RESET );
                }
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)CA3 );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA301_VALUE_OF_CASH_IN_SINCE_LAST_RESET );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA302_VALUE_OF_CASH_TO_CASH_BOX_SINCE_LAST_RESET );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA303_VALUE_OF_CASH_TO_TUBES_SINCE_LAST_RESET );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA304_VALUE_OF_BILLS_IN_SINCE_LAST_RESET );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA305_VALUE_OF_CASH_IN_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA306_VALUE_OF_CASH_TO_BOX_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA307_VALUE_OF_CASH_TO_TUBES_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA308_VALUE_OF_BILLS_IN_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA309_VALUE_OF_BILLS_IN_SINCE_LAST_RESET );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA310_VALUE_OF_BILLS_IN_SINCE_INITIALIZATION );
                if ( !TinyDEX )
                {
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA311_VALUE_OF_BILLS_TO_RECYCLER_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA312_VALUE_OF_BILLS_TO_RECYCLER_SINCE_INITIALIZATION );
                }
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)CA4 );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA401_VALUE_OF_CASH_DISPENSED_SINCE_LAST_RESET );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA402_VALUE_OF_MANUAL_CASH_DISPENSED_SINCE_LAST_RESET );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA403_VALUE_OF_CASH_DISPENSED_SINCE_INITIALIZATION );
                if ( !TinyDEX )
                {
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA404_VALUE_OF_MANUAL_CASH_DISPENSED_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA405_VALUE_OF_BILLS_DISPENSED_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA406_VALUE_OF_BILLS_MANUALLY_DISPENSED_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA407_VALUE_OF_BILLS_TRANSFERRED_TO_RECYCLER_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA408_VALUE_OF_BILLS_DISPENSED_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA409_VALUE_OF_BILLS_MANUALLY_DISPENSED_SINCE_INITIATLISATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA410_VALUE_OF_BILLS_TRANSFERRED_TO_RECYCLER_SINCE_INITIATLISATION );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)CA8 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA801_VALUE_OF_CASH_OVERPAY_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA802_VALUE_OF_CASH_OVERPAY_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)CA9 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA901_VALUE_OF_PAY_VENDS_EXACT_CHANGE_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA902_VALUE_OF_PAY_VENDS_EXACT_CHANGE_SINCE_INITIALIZATION );
                }
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)CA10 );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA1001_VALUE_OF_CASH_FILLED_SINCE_LAST_RESET );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA1002_VALUE_OF_CASH_FILLED_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA1003_VALUE_OF_BILLS_FILLED_SINCE_LAST_RESET );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA1004_VALUE_OF_BILLS_FILLED_SINCE_INITIALISATION );
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );

                TX_DEXmemcpy ( ASCII, (uint32_t*)CA15 );
                DexDataStructure.DexCoinTubeField.CA15TUBE101_VALUE_OF_TUBE_CONTENTS = GetCash_In_Tubes ();
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE101_VALUE_OF_TUBE_CONTENTS );
                if ( !TinyDEX )
                {
                    TX_DEXmemcpy ( ASCII, (uint32_t*)TUBE1 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE103_NUMBER_OF_COIN_TYPE_0 = mccGetTubeLevel ( 0 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE103_NUMBER_OF_COIN_TYPE_0 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE104_NUMBER_OF_COIN_TYPE_1 = mccGetTubeLevel ( 1 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE104_NUMBER_OF_COIN_TYPE_1 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE105_NUMBER_OF_COIN_TYPE_2 = mccGetTubeLevel ( 2 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE105_NUMBER_OF_COIN_TYPE_2 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE106_NUMBER_OF_COIN_TYPE_3 = mccGetTubeLevel ( 3 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE106_NUMBER_OF_COIN_TYPE_3 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE107_NUMBER_OF_COIN_TYPE_4 = mccGetTubeLevel ( 4 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE107_NUMBER_OF_COIN_TYPE_4 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE108_NUMBER_OF_COIN_TYPE_5 = mccGetTubeLevel ( 5 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE108_NUMBER_OF_COIN_TYPE_5 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE109_NUMBER_OF_COIN_TYPE_6 = mccGetTubeLevel ( 6 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE109_NUMBER_OF_COIN_TYPE_6 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE110_NUMBER_OF_COIN_TYPE_7 = mccGetTubeLevel ( 7 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE110_NUMBER_OF_COIN_TYPE_7 );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)CA15 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE101_VALUE_OF_TUBE_CONTENTS );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)TUBE2 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE203_NUMBER_OF_COIN_TYPE_8 = mccGetTubeLevel ( 8 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE203_NUMBER_OF_COIN_TYPE_8 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE204_NUMBER_OF_COIN_TYPE_9 = mccGetTubeLevel ( 9 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE204_NUMBER_OF_COIN_TYPE_9 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE205_NUMBER_OF_COIN_TYPE_10 = mccGetTubeLevel ( 10 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE205_NUMBER_OF_COIN_TYPE_10 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE206_NUMBER_OF_COIN_TYPE_11 = mccGetTubeLevel ( 11 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE206_NUMBER_OF_COIN_TYPE_11 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE207_NUMBER_OF_COIN_TYPE_12 = mccGetTubeLevel ( 12 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE207_NUMBER_OF_COIN_TYPE_12 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE208_NUMBER_OF_COIN_TYPE_13 = mccGetTubeLevel ( 13 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE208_NUMBER_OF_COIN_TYPE_13 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE209_NUMBER_OF_COIN_TYPE_14 = mccGetTubeLevel ( 14 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE209_NUMBER_OF_COIN_TYPE_14 );
                    DexDataStructure.DexCoinTubeField.CA15TUBE210_NUMBER_OF_COIN_TYPE_15 = mccGetTubeLevel ( 15 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexCoinTubeField.CA15TUBE210_NUMBER_OF_COIN_TYPE_15 );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)CA16 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.CA1601_VALUE_CREDITED_FROM_MACHINE_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.CA1602_VALUE_CREDITED_FROM_MACHINE_SINCE_INITIALIZATION );
                }
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)BA1 );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.BA101_BILL_VALIDATOR_SERIAL_NUMBER );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.BA102_BILL_VALIDATOR_MODEL_NUMBER );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( SHORT_BCD, (uint32_t*)&DexDataStructure.DexDeviceIDFields.BA103_BILL_VALIDATOR_SOFTWARE_REVISION );
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DA1 );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.DA101_CASHLESS_1_SERIAL_NUMBER );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.DA102_CASHLESS_1_MODEL_NUMBER );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( SHORT_BCD, (uint32_t*)&DexDataStructure.DexDeviceIDFields.DA103_CASHLESS_1_SOFTWARE_REVISION );
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DA2 );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DA201_VALUE_OF_CASHLESS_1_SALES_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DA202_NUMBER_OF_CASHLESS_1_SALES_SINCE_INITIALIZATION );
                if ( !TinyDEX )
                {
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DA203_VALUE_OF_CASHLESS_1_SALES_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DA204_NUMBER_OF_CASHLESS_1_SALES_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)DA4 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DA401_VALUE_OF_CREDIT_TO_CASHLESS_1_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DA402_VALUE_OF_CREDIT_TO_CASHLESS_1_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)DA5 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DA501_VALUE_OF_CASHLESS_1_DISCOUNTS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DA502_NUMBER_OF_DISCOUNT_CASHLESS_1_VENDS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DA503_VALUE_OF_CASHLESS_1_DISCOUNTS_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DA504_NUMBER_OF_DISCOUNT_CASHLESS_1_VENDS_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DA505_VALUE_OF_CASHLESS_1_SURCHARGES_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DA506_NUMBER_OF_SURCHARGE_CASHLESS_1_VENDS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DA507_VALUE_OF_CASHLESS_1_SURCHARGES_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DA508_NUMBER_OF_SURCHARGE_CASHLESS_1_VENDS_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)DA6 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DA601_REVALUATION_INCENTIVE_ON_CASHLESS_1_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DA602_REVALUATION_INCENTIVE_ON_CASHLESS_1_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)DA9 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DA901_VALUE_OF_CASHLESS_1_OVERPAY_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DA902_VALUE_OF_CASHLESS_1_OVERPAY_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)DA10 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DA1001_CASHLESS_1_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DA1002_CASHLESS_1_VALUE_OF_MIXED_PAYMENT_CASHLESS_AMOUNT_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DA1003_CASHLESS_1_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DA1004_CASHLESS_1_VALUE_OF_MIXED_PAYMENT_CASHLESS_AMOUNT_SINCE_LAST_RESET );
                }
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DB1 );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.DB101_CASHLESS_2_SERIAL_NUMBER );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.DexDeviceIDFields.DB102_CASHLESS_2_MODEL_NUMBER );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( SHORT_BCD, (uint32_t*)&DexDataStructure.DexDeviceIDFields.DB103_CASHLESS_2_SOFTWARE_REVISION );
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DB2 );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DB201_VALUE_OF_CASHLESS_2_SALES_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DB202_NUMBER_OF_CASHLESS_2_SALES_SINCE_INITIALIZATION );
                if ( !TinyDEX )
                {
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DB203_VALUE_OF_CASHLESS_2_SALES_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DB204_NUMBER_OF_CASHLESS_2_SALES_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)DB4 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DB401_VALUE_OF_CREDIT_TO_CASHLESS_2_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DB402_VALUE_OF_CREDIT_TO_CASHLESS_2_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)DB5 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DB501_VALUE_OF_CASHLESS_2_DISCOUNTS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DB502_NUMBER_OF_DISCOUNT_CASHLESS_2_VENDS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DB503_VALUE_OF_CASHLESS_2_DISCOUNTS_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DB504_NUMBER_OF_DISCOUNT_CASHLESS_2_VENDS_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DB505_VALUE_OF_CASHLESS_2_SURCHARGES_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DB506_NUMBER_OF_SURCHARGE_CASHLESS_2_VENDS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DB507_VALUE_OF_CASHLESS_2_SURCHARGES_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DB508_NUMBER_OF_SURCHARGE_CASHLESS_2_VENDS_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)DB6 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DB601_REVALUATION_INCENTIVE_ON_CASHLESS_2_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DB602_REVALUATION_INCENTIVE_ON_CASHLESS_2_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)DB9 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DB901_VALUE_OF_CASHLESS_2_OVERPAY_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DB902_VALUE_OF_CASHLESS_2_OVERPAY_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)DB10 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DB1001_CASHLESS_2_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.DB1002_CASHLESS_2_VALUE_OF_MIXED_PAYMENT_CASHLESS_AMOUNT_SINCE_INITIALISATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DB1003_CASHLESS_2_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.DB1004_CASHLESS_2_VALUE_OF_MIXED_PAYMENT_CASHLESS_AMOUNT_SINCE_LAST_RESET );
                }
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)TA2 );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.TA201_VALUE_OF_TOKEN_COUPON_SALES_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.TA202_NUMBER_OF_TOKEN_COUPON_SALES_SINCE_INITIALIZATION );
                if ( !TinyDEX )
                {
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.TA203_VALUE_OF_TOKEN_COUPON_SALES_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.TA204_NUMBER_OF_TOKEN_COUPON_SALES_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.TA205_VALUE_OF_VALUE_TOKEN_COUPON_SALES_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexUnResetableFields.TA206_NUMBER_OF_VALUE_TOKEN_COUPON_SALES_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.TA207_VALUE_OF_VALUE_TOKEN_COUPON_SALES_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.DexResetableFields.TA208_NUMBER_OF_VALUE_TOKEN_COUPON_SALES_SINCE_LAST_RESET );
                }
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                SelectionOffset = 0;
                while ( SelectionOffset < 168 )
                {
                    dex_comm_thread_run ();
                    if ( dbcGetSelectionDigits () == 2 )
                    {
                        selection = SelectionOffset + 10;
                        selection_status = dbcGetPriceForItem ( selection, (uint16_t*)&price );
                        if ( selection == 109 )
                        {
                            selection_status = false;
                        }
                    }
                    // Convert offset to selection
                    else if ( ( dbcGetSelectionDigits () == 3 ) && ( dbcGetMAXColumns () <= 10 ) )
                    {
                        row = ( ( SelectionOffset / 10 ) + 1 );
                        col = ( SelectionOffset % 10 );
                        selection = ( ( row + 9 ) * 10 ) + col;
                        selection_status = dbcGetPriceForItem ( selection, (uint16_t*)&price );
                        if ( ( selection == 199 ) || ( selection > 239 ) )
                        {
                            selection_status = false;
                        }
                    }
                    else
                    {
                        row = ( ( SelectionOffset / 12 ) + 1 );
                        col = ( SelectionOffset % 12 );
                        selection = ( row * 100 ) + col;
                        selection_status = dbcGetPriceForItem ( selection, (uint16_t*)&price );
                        if ( selection == 1009 )
                        {
                            selection_status = false;
                        }
                    }

                    if ( ( selection_status ) || ( DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA201_NUMBER_OF_PAID_PRODUCTS_VENDED_SINCE_INITIALIZATION != 0 ) )
                    {
                        // Get product identification product_identification
                        dbcGetItemDefinition ( selection, product_identification );
                        TX_DEXmemcpy ( ASCII, (uint32_t*)PA1 );
                        if ( selection < 100 )
                        { // if selection under 100 add a leading 0 digit to make mapping easier for DEX usage.
                            TX_DEXmemcpy ( ASCII, (uint32_t*)ZERO );
                        }
                        TX_DEXmemcpy ( LONG, &selection );
                        TX_DEXmemcpy ( LONG, &price );
                        if ( !TinyDEX )
                        {
                            TX_DEXmemcpy ( ASCII, (uint32_t*)product_identification );
                            TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                            TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                            TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                            TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                            //  Present and good
                            if ( selection_status )
                            {
                                TX_DEXmemcpy ( ASCII, (uint32_t*)CHAR_MOTOR_PRESENT );
                            }
                            else
                            {
                                TX_DEXmemcpy ( ASCII, (uint32_t*)CHAR_MOTOR_MISSING );
                            }
                        }
                        TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                        TX_DEXmemcpy ( ASCII, (uint32_t*)PA2 );
                        TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA201_NUMBER_OF_PAID_PRODUCTS_VENDED_SINCE_INITIALIZATION );
                        TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA202_VALUE_OF_PAID_PRODUCTS_VENDED_SINCE_INITIALIZATION );
                        if ( !TinyDEX )
                        {
                            if ( FullDEX )
                            {
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA203_NUMBER_OF_PAID_PRODUCTS_VENDED_SINCE_LAST_RESET );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA204_VALUE_OF_PAID_PRODUCTS_VENDED_SINCE_LAST_RESET );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA205_NUMBER_OF_DISCOUNTS_SINCE_INITIALIZATION );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA206_VALUE_OF_DISCOUNTS_SINCE_INITIALIZATION );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA207_NUMBER_OF_DISCOUNTS_SINCE_LAST_RESET );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA208_VALUE_OF_DISCOUNTS_SINCE_LAST_RESET );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA209_NUMBER_OF_SURCHARGED_PAID_SINCE_INITIATLIZATION );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA2010_VALUE_OF_SURCHARGES_PAID_SINCE_INITIALIZATION );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA2011_NUMBER_OF_SURCHARGED_PAID_SINCE_LAST_RESET );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA2012_VALUE_OF_SURCHARGES_PAID_SINCE_LAST_RESET );
                            }
                            TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                            TX_DEXmemcpy ( ASCII, (uint32_t*)PA3 );
                            TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA301_NUMBER_OF_TEST_VENDS_SINCE_INITIALIZATION );
                            TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA302_VALUE_OF_TEST_VENDS_SINCE_INITIALIZATION );
                            if ( FullDEX )
                            {
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA303_NUMBER_OF_TEST_VENDS_SINCE_LAST_RESET );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA304_VALUE_OF_TEST_VENDS_SINCE_LAST_RESET );
                            }
                            TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                            TX_DEXmemcpy ( ASCII, (uint32_t*)PA4 );
                            TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA401_NUMBER_OF_FREE_VENDS_SINCE_INITIALIZATION );
                            if ( FullDEX )
                            {
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA402_VALUE_OF_FREE_VENDS_SINCE_INITIALIZATION );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA403_NUMBER_OF_FREE_VENDS_SINCE_LAST_RESET );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA404_VALUE_OF_FREE_VENDS_SINCE_LAST_RESET );
                            }
                            TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                            TX_DEXmemcpy ( ASCII, (uint32_t*)PA5 );
                            TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA501_DATEYEAR_OF_MOST_RECENT_SALE );
                            TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA501_DATEMONTH_OF_MOST_RECENT_SALE );
                            TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA501_DATEDAY_OF_MOST_RECENT_SALE );
                            TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                            TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA502_TIMEHOUR_OF_MOST_RECENT_SALE );
                            TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA502_TIMEMINUTE_OF_MOST_RECENT_SALE );
                            if ( FullDEX )
                            {
                                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA503_NUMBER_OF_TIMES_SOLD_OUT_PRODUCT_SELECTED );
                            }
                            TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                            // PA701_PRODUCT_NUMBER
                            // PA702_CASH_DEVICE
                            // PA703_PRICE LIST NUMBER
                            // PA704_APPLIED PRICE
                            TX_DEXmemcpy ( ASCII, (uint32_t*)PA7 );
                            TX_DEXmemcpy ( LONG, &selection );
                            TX_DEXmemcpy ( ASCII, (uint32_t*)PA7CASH );
                            TX_DEXmemcpy ( LONG, &price );
                            TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA705_NUMBER_OF_CASH_SALES_SINCE_INITIALISATION );
                            TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA706_VALUE_OF_CASH_SALES_SINCE_INITIALISATION );
                            if ( FullDEX )
                            {
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA707_NUMER_OF_CASH_SALES_SINCE_LAST_RESET );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA708_VALUE_OF_CASH_SALES_SINCE_LAST_RESET );
                            }
                            TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                            // PA701_PRODUCT NUMBER
                            // PA702_CASHLESS_DEVICE
                            // PA703_PRICE LIST NUMBER
                            // PA704_APPLIED PRICE       
                            TX_DEXmemcpy ( ASCII, (uint32_t*)PA7 );
                            TX_DEXmemcpy ( LONG, &selection );
                            TX_DEXmemcpy ( ASCII, (uint32_t*)PA7CASHLESS1 );
                            TX_DEXmemcpy ( LONG, &price );
                            TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA705_NUMBER_OF_CASHLESS_SALES_SINCE_INITIALISATION );
                            TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA706_VALUE_OF_CASHLESS_SALES_SINCE_INITIALISATION );
                            if ( FullDEX )
                            {
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA707_NUMER_OF_CASHLESS_SALES_SINCE_LAST_RESET );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA708_VALUE_OF_CASHLESS_SALES_SINCE_LAST_RESET );
                            }
                            TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                            TX_DEXmemcpy ( ASCII, (uint32_t*)PA8 );
                            TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA801_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_INITIALISATION );
                            TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_UnResetableField[SelectionOffset].PA802_VALUE_OF_MIXED_PAYMENT_CASH_AMOUNT_SINCE_INITIALISATION );
                            if ( FullDEX )
                            {
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA803_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_LAST_RESET );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_PA_ResetableField[SelectionOffset].PA804_VALUE_OF_MIXED_PAYMENT_CASH_AMOUNT_SINCE_LAST_RESET );
                            }

                            if ( FullDEX )
                            {
                                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                                TX_DEXmemcpy ( ASCII, (uint32_t*)EA1_EJL );
                                TX_DEXmemcpy ( LONG, &selection );
                                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA102_EJL_DATEYEAR_OF_EVENT );
                                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA102_EJL_DATEMONTH_OF_EVENT );
                                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA102_EJL_DATEDAY_OF_EVENT );
                                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA103_EJL_TIMEHOUR_OF_EVENT );
                                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA103_EJL_TIMEMINUTE_OF_EVENT );
                                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                                TX_DEXmemcpy ( ASCII, (uint32_t*)EA2_EJL );
                                TX_DEXmemcpy ( LONG, &selection );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ProductDispenseResetableField[SelectionOffset].EA202_EJL_NUMBER_OF_EVENTS_SINCE_LAST_RESET );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA203_EJL_NUMBER_OF_EVENTS_SINCE_INITIALIZATION );
                                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                                TX_DEXmemcpy ( SHORT_BCD, (uint32_t*)&DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA205_EJL_CURRENT_EVENT_ACTIVITY );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA206_EJL_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES );
                                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                                TX_DEXmemcpy ( ASCII, (uint32_t*)EA1_ELA );
                                TX_DEXmemcpy ( LONG, &selection );
                                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA102_ELA_DATEYEAR_OF_EVENT );
                                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA102_ELA_DATEMONTH_OF_EVENT );
                                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA102_ELA_DATEDAY_OF_EVENT );
                                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA103_ELA_TIMEHOUR_OF_EVENT );
                                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA103_ELA_TIMEMINUTE_OF_EVENT );
                                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                                TX_DEXmemcpy ( ASCII, (uint32_t*)EA2_ELA );
                                TX_DEXmemcpy ( LONG, &selection );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ProductDispenseResetableField[SelectionOffset].EA202_ELA_NUMBER_OF_EVENTS_SINCE_LAST_RESET );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA203_ELA_NUMBER_OF_EVENTS_SINCE_INITIALIZATION );
                                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                                TX_DEXmemcpy ( SHORT_BCD, (uint32_t*)&DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA205_ELA_CURRENT_EVENT_ACTIVITY );
                                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ProductDispenseUnResetableField[SelectionOffset].EA206_ELA_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES );
                            }
                        }
                        TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );

                    }
                    SelectionOffset++;
                }
                //EA101_EJS -  Door open   
                if ( ( DexDataStructure.Dex_EA_ResetableField.EA202_EGS_NUMBER_OF_EVENTS_SINCE_LAST_RESET != 0 ) || ( GetDoor () ) )
                {
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA1_EGS );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EGS_DATEYEAR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EGS_DATEMONTH_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EGS_DATEDAY_OF_EVENT );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EGS_TIMEHOUR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EGS_TIMEMINUTE_OF_EVENT );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA2_EGS );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ResetableField.EA202_EGS_NUMBER_OF_EVENTS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA203_EGS_NUMBER_OF_EVENTS_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    if ( GetDoor () )
                    { //if door open (active) send a 1 else *
                        TX_DEXmemcpy ( ASCII, (uint32_t*)CHAR_SET );
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    else
                    {
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA206_EGS_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                }

                //EA101_EJH -  Health rules have been violated for the temperature controlled  
                if ( ( DexDataStructure.Dex_EA_ResetableField.EA202_EJH_NUMBER_OF_EVENTS_SINCE_LAST_RESET != 0 ) || ( dbcGetHealthAlert () ) )
                {
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA1_EJH );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EJH_DATEYEAR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EJH_DATEMONTH_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EJH_DATEDAY_OF_EVENT );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EJH_TIMEHOUR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EJH_TIMEMINUTE_OF_EVENT );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA2_EJH );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ResetableField.EA202_EJH_NUMBER_OF_EVENTS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA203_EJH_NUMBER_OF_EVENTS_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    if ( dbcGetHealthAlert () )
                    { //if health alert (active) send a 1 else *
                        TX_DEXmemcpy ( ASCII, (uint32_t*)CHAR_SET );
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    else
                    {
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA206_EJH_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                }

                //EA101_EJJ -  cooling unit does not cool down to the pre-set temperature
                if ( ( DexDataStructure.Dex_EA_ResetableField.EA202_EJJ_NUMBER_OF_EVENTS_SINCE_LAST_RESET != 0 ) || ( dbcGetPullDownExceeded () ) )
                {
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA1_EJJ );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EJJ_DATEYEAR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EJJ_DATEMONTH_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EJJ_DATEDAY_OF_EVENT );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EJJ_TIMEHOUR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EJJ_TIMEMINUTE_OF_EVENT );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA2_EJJ );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ResetableField.EA202_EJJ_NUMBER_OF_EVENTS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA203_EJJ_NUMBER_OF_EVENTS_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    if ( dbcGetPullDownExceeded () )
                    { //if system not not cooling down to pre-set temperature(active) in 12 hours send a 1 else *
                        TX_DEXmemcpy ( ASCII, (uint32_t*)CHAR_SET );
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    else
                    {
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA206_EJJ_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                }

                //EA101_EJK - Temperature sensor is defective
                if ( ( DexDataStructure.Dex_EA_ResetableField.EA202_EJK_NUMBER_OF_EVENTS_SINCE_LAST_RESET != 0 ) || ( TemperatureSensorFailStatus () == true ) )
                {
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA1_EJK );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EJK_DATEYEAR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EJK_DATEMONTH_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EJK_DATEDAY_OF_EVENT );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EJK_TIMEHOUR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EJK_TIMEMINUTE_OF_EVENT );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA2_EJK );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ResetableField.EA202_EJK_NUMBER_OF_EVENTS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA203_EJK_NUMBER_OF_EVENTS_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    if ( TemperatureSensorFailStatus () == true )
                    { //if temperature sensor bad send a 1 else *
                        TX_DEXmemcpy ( ASCII, (uint32_t*)CHAR_SET );
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    else
                    {
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA206_EJK_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                }

                //EA101_EA - General non-specific Coin Mechanism fault 
                if ( ( DexDataStructure.Dex_EA_ResetableField.EA202_EA_NUMBER_OF_EVENTS_SINCE_LAST_RESET != 0 ) || ( dbcGetCoinMechFailure () == true ) )
                {
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA1_EA );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EA_DATEYEAR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EA_DATEMONTH_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EA_DATEDAY_OF_EVENT );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EA_TIMEHOUR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EA_TIMEMINUTE_OF_EVENT );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA2_EA );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ResetableField.EA202_EA_NUMBER_OF_EVENTS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA203_EA_NUMBER_OF_EVENTS_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    if ( dbcGetCoinMechFailure () == true )
                    { //if Coin Mechanism fault send a 1 else *
                        TX_DEXmemcpy ( ASCII, (uint32_t*)CHAR_SET );
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    else
                    {
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA206_EA_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                }

                //EA101_EK - General non-specific Cashless 1 Mechanism fault 
                if ( ( DexDataStructure.Dex_EA_ResetableField.EA202_EK_NUMBER_OF_EVENTS_SINCE_LAST_RESET != 0 ) || ( dbcGetCashless1Failure () == true ) )
                {
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA1_EK );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EK_DATEYEAR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EK_DATEMONTH_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EK_DATEDAY_OF_EVENT );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EK_TIMEHOUR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EK_TIMEMINUTE_OF_EVENT );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA2_EK );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ResetableField.EA202_EK_NUMBER_OF_EVENTS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA203_EK_NUMBER_OF_EVENTS_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    if ( dbcGetCashless1Failure () == true )
                    { //if Cashless Mechanism fault send a 1 else *
                        TX_DEXmemcpy ( ASCII, (uint32_t*)CHAR_SET );
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    else
                    {
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA206_EK_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                }

                //EA101_EK2 - General non-specific Cashless 2 Mechanism fault
                if ( ( DexDataStructure.Dex_EA_ResetableField.EA202_EK2_NUMBER_OF_EVENTS_SINCE_LAST_RESET != 0 ) || ( dbcGetCashless2Failure () == true ) )
                {
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA1_EK2 );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EK2_DATEYEAR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EK2_DATEMONTH_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EK2_DATEDAY_OF_EVENT );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EK2_TIMEHOUR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EK2_TIMEMINUTE_OF_EVENT );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA2_EK2 );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ResetableField.EA202_EK2_NUMBER_OF_EVENTS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA203_EK2_NUMBER_OF_EVENTS_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    if ( dbcGetCashless2Failure () == true )
                    { //if Cashless2 Mechanism fault send a 1 else *
                        TX_DEXmemcpy ( ASCII, (uint32_t*)CHAR_SET );
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    else
                    {
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA206_EK2_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                }

                //EA101_EN - General non-specific Bill Validator fault
                if ( ( DexDataStructure.Dex_EA_ResetableField.EA202_EN_NUMBER_OF_EVENTS_SINCE_LAST_RESET != 0 ) || ( dbcGetBillValidatorFailure () == true ) )
                {
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA1_EN );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EN_DATEYEAR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EN_DATEMONTH_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EN_DATEDAY_OF_EVENT );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EN_TIMEHOUR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EN_TIMEMINUTE_OF_EVENT );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA2_EN );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ResetableField.EA202_EN_NUMBER_OF_EVENTS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA203_EN_NUMBER_OF_EVENTS_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    if ( dbcGetBillValidatorFailure () == true )
                    { //if Bill Mechanism fault send a 1 else *
                        TX_DEXmemcpy ( ASCII, (uint32_t*)CHAR_SET );
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    else
                    {
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA206_EN_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                }

                //EA101_EC1F - A malfunction has been detected in the selection board  
                if ( ( DexDataStructure.Dex_EA_ResetableField.EA202_EC1F_NUMBER_OF_EVENTS_SINCE_LAST_RESET != 0 ) || ( dbcGetKeyboardFailure () == true ) )
                {
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA1_EC1F );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EC1F_DATEYEAR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EC1F_DATEMONTH_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA102_EC1F_DATEDAY_OF_EVENT );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EC1F_TIMEHOUR_OF_EVENT );
                    TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA103_EC1F_TIMEMINUTE_OF_EVENT );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)EA2_EC1F );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ResetableField.EA202_EC1F_NUMBER_OF_EVENTS_SINCE_LAST_RESET );
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA203_EC1F_NUMBER_OF_EVENTS_SINCE_INITIALIZATION );
                    TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    if ( dbcGetKeyboardFailure () == true )
                    { //if Keypad fault send a 1 else *
                        TX_DEXmemcpy ( ASCII, (uint32_t*)CHAR_SET );
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    else
                    {
                        TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                    }
                    TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA206_EC1F_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES );
                    TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                }

                TX_DEXmemcpy ( ASCII, (uint32_t*)EA3 );
                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA301_NUMBER_OF_READS_WITH_RESET_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA302_DATEYEAR_OF_THIS_READOUT );
                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA302_DATEMONTH_OF_THIS_READOUT );
                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA302_DATEDAY_OF_THIS_READOUT );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA303_TIMEHOUR_OF_THIS_READOUT );
                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA303_TIMEMINUTE_OF_THIS_READOUT );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.Dex_EA_UnResetableField.EA304_TIME_TERMINAL_INTERROGATOR_IDENTIFICATION );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA305_DATEYEAR_OF_LAST_READ_OUT );
                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA305_DATEMONTH_OF_LAST_READ_OUT );
                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA305_DATEDAY_OF_LAST_READ_OUT );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA306_TIMEHOUR_OF_LAST_READ_OUT );
                TX_DEXmemcpy ( LONGNS, &DexDataStructure.Dex_EA_UnResetableField.EA306_TIMEMINUTE_OF_LAST_READ_OUT );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)DexDataStructure.Dex_EA_UnResetableField.EA307_LAST_TERMINAL_INTERROGATOR_IDENTIFICATION );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA309_NUMBER_OF_READS_WITH_OR_WITHOUT_RESET_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA3010_NUMBER_OF_RESETS_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)EA7 );
                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_ResetableField.EA701_NUMBER_OF_POWER_OUTAGES_SINCE_LAST_RESET );
                TX_DEXmemcpy ( LONG, &DexDataStructure.Dex_EA_UnResetableField.EA702_NUMBER_OF_POWER_OUTAGES_SINCE_INITIALIZATION );
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );

                // Temperature transfer
                TX_DEXmemcpy ( ASCII, (uint32_t*)MA5_TMP ); // temp data line
                getRefrigerationMode ( (char*)product_identification );
                TX_DEXmemcpy ( ASCII, (uint32_t*)product_identification );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );

                temp = GetTemperature ( 1 );
                memset ( product_identification, 0, sizeof ( product_identification ) );
                if ( temp <= -1 )
                {
                    strcat ( (char*)product_identification, "-" );
                }
                temp = abs ( temp );
                ltoa ( temp, currentTemp, 10 );
                strcat ( (char*)product_identification, currentTemp );
                TX_DEXmemcpy ( ASCII, (uint32_t*)product_identification );
                TX_DEXmemcpy ( ASCII, (uint32_t*)SCALE );
                temp = GetTemperatureMAX ();
                memset ( product_identification, 0, sizeof ( product_identification ) );
                if ( temp <= -1 )
                {
                    strcat ( (char*)product_identification, "-" );
                }
                temp = abs ( temp );
                ltoa ( temp, currentTemp, 10 );
                strcat ( (char*)product_identification, currentTemp );
                TX_DEXmemcpy ( ASCII, (uint32_t*)product_identification );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                // Current time
                rtc_gettime ( &rtc_time );
                memset ( product_identification, 0, sizeof ( product_identification ) );
                ltoa ( rtc_time.year, currentTemp, 10 );
                strcpy ( (char*)product_identification, currentTemp );
                if ( rtc_time.month < 10 )
                {
                    strcat ( (char*)product_identification, "0" );
                }
                ltoa ( rtc_time.month, currentTemp, 10 );
                strcat ( (char*)product_identification, currentTemp );
                if ( rtc_time.mday < 10 )
                {
                    strcat ( (char*)product_identification, "0" );
                }
                ltoa ( rtc_time.mday, currentTemp, 10 );
                strcat ( (char*)product_identification, currentTemp );
                strcat ( (char*)product_identification, "*" );
                if ( rtc_time.hour < 10 )
                {
                    strcat ( (char*)product_identification, "0" );
                    if ( rtc_time.hour == 10 )
                    {
                        strcat ( (char*)product_identification, "0" );
                    }
                }
                ltoa ( rtc_time.hour, currentTemp, 10 );
                strcat ( (char*)product_identification, currentTemp );
                if ( rtc_time.min < 10 )
                {
                    strcat ( (char*)product_identification, "0" );
                    if ( rtc_time.min == 10 )
                    {
                        strcat ( (char*)product_identification, "0" );
                    }
                }
                ltoa ( rtc_time.min, currentTemp, 10 );
                strcat ( (char*)product_identification, currentTemp );
                TX_DEXmemcpy ( ASCII, (uint32_t*)product_identification );
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );

                TX_DEXmemcpy ( ASCII, (uint32_t*)SD1 );
                TX_DEXmemcpy ( LONG, &password1 );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( LONG, &password2 );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)AUTO );
                TX_DEXmemcpy ( ASCII, (uint32_t*)STAR );
                TX_DEXmemcpy ( ASCII, (uint32_t*)AUTO );
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)G85 );
								G85CRC_16_SUM = ( (G85CRC_16_SUM & 0xF000)>>12 |(G85CRC_16_SUM & 0x0F00)>>4 | (G85CRC_16_SUM & 0x00F0)<<4 | (G85CRC_16_SUM & 0x000F) << 12); //REVERSES CRC TO CORRECT CRC
                TX_DEXmemcpy ( SHORT_BCD, (uint32_t*)&G85CRC_16_SUM );
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                TX_DEXmemcpy ( ASCII, (uint32_t*)SE );
                TX_DEXmemcpy ( LONG, &SegmentCount );
                TX_DEXmemcpy ( ASCII, (uint32_t*)SE0001 );
                TX_DEXmemcpy ( END_OF_SEGMENT, (uint32_t*)CR_LF );
                logger_fput ( DEX_READ_TXT );
                if ( DEXNonCommTransferIP )
                {
                    DEXNonCommTransferBlockComplete = true;
                    if ( dbcGetDEXResetOnReadEnabled () )
                    {
                        Reset_Interval_DEX_Accounting ();
                    }
                    DEXReadAccounting ();
                    DEX_State = TRANSFER_COMPLETE;
                    break;
                }
                if ( TX_DEXmemcpy ( LAST, (uint32_t*)DXE ) )
                {
                    osDelay ( 50 );
                    TX_Buff[0] = EOT;
                    send_dex_data ( &TX_Buff[0], 1, 0x01, osWaitForever );
                    if ( dbcGetDEXResetOnReadEnabled () )
                    {
                        Reset_Interval_DEX_Accounting ();
                    }
                }
                else
                {
                    osDelay ( 50 );
                    TX_Buff[0] = NAK;
                    send_dex_data ( &TX_Buff[0], 1, 0x01, osWaitForever );
                }
                DEX_State = TRANSFER_COMPLETE;
                break;
            case TRANSFER_COMPLETE:
                DEXReadAccounting ();
                clrSBC_DB_Transf_flag ();
                if ( ( SBCGetCashLessVend () ) && ( GetOperate_State () == PROCESS_VEND ) )
                {
                    //Allow dexing in FlexConnect mode
                }
                else
                {
                    sendOperateThread ( IDLE_STATE, 0, 0 );
                }
                DEX_State = WAIT_FOR_ENQ;
                break;
            case EXTERNAL_DEX_PORT_TRANSFER:
                send_dex_data ( TX_Buff, ExternalDEXPortTransferSize, 0, 0 );
                DEX_State = WAIT_FOR_ENQ;
                osDelay ( 100 );
                break;
            case SCALE_DATA_COMMS:
                // Get scale data
                control_dex_comm ();
                memset ( RX_Buff, 0, sizeof ( RX_Buff ) );
                receive_dex_data ( &RX_Buff, sizeof ( RX_Buff ), 0x01, 500 );
                control_dex_comm ();
                // Decode Scale data
                sptr = memchr ( (char*)RX_Buff, 's', sizeof ( RX_Buff ) / 2 );
                //if scale data is received
                if ( sptr != NULL )
                {
                    eptr = memchr ( sptr, 'e', sizeof ( RX_Buff ) - ( sptr - (char*)RX_Buff ) );
                    if ( ( strstr ( eptr, "end of data" ) != NULL ) && ( strstr ( sptr, "start of data," ) ) != NULL )
                    {
                        sptr = strstr ( sptr, "start of data," ) + 14;
                        eptr = strstr ( eptr, "end of data" );
                        for ( counter = 0; counter < 120; counter++ )
                        {
                            // Serial number
                            bptr = strstr ( sptr, "," );
                            if ( bptr == 0 )
                            {
                                break;
                            }
                            memset ( lockerScaleData[counter].scaleSerialNumber, 0, sizeof ( lockerScaleData[counter].scaleSerialNumber ) );
                            memcpy ( lockerScaleData[counter].scaleSerialNumber, sptr, bptr - sptr );
                            sptr = bptr + 1;
                            bptr = strstr ( sptr, "," );
                            // store revision level
                            *bptr = 0;
                            lockerScaleData[counter].scaleRevision = (uint8_t)atoi ( sptr );
                            // Test to check revision
                            if ( lockerScaleData[counter].scaleRevision != 2 )
                            {
                                sptr = 0;
                            }
                            sptr = bptr + 1;
                            bptr = strstr ( sptr, "," );
                            *bptr = 0;
                            // Scale current weight
                            weight_save = atoi ( sptr );
                            // Scale change in weight
                            if ( ( lockerScaleData[counter].scaleCurrentWeight != 0 ) || ( lockerScaleData[counter].scaleChangeWeight != 0 ) )
                            {	// Think positive
                                if ( weight_save > lockerScaleData[counter].scaleCurrentWeight )
                                {
                                    lockerScaleData[counter].scaleChangeWeight = weight_save - lockerScaleData[counter].scaleCurrentWeight;
                                }
                                else
                                {
                                    lockerScaleData[counter].scaleChangeWeight = lockerScaleData[counter].scaleCurrentWeight - weight_save;
                                }
                            }
                            lockerScaleData[counter].scaleCurrentWeight = weight_save;
                            *bptr = ',';
                            sptr = bptr + 1;
                            bptr = sptr + 2;
                            if ( bptr >= eptr )
                            {
                                break;
                            }
                        }
                        osDelay ( 250 );
                    }
                }
                osDelay ( 250 );
                break;
            default:
                break;
        }
        if ( SendReceive == 'S' )
        {
            osDelay ( 1 );
        }
        else
        {
            osDelay ( 100 );
        }
    }
}




void ExternalDEXPortTransfer ( char* porttransfer, uint8_t  stringsize )
{
    if ( DEX_State == WAIT_FOR_ENQ )
    {
        strncpy ( (char*)TX_Buff, porttransfer, stringsize );
        ExternalDEXPortTransferSize = stringsize;
        DEX_State = EXTERNAL_DEX_PORT_TRANSFER;
    }
}



bool DEXNonCommTransfer ( char* dexdatablock )
{
    //dexdatablock must be 256 bytes minimum
    uint32_t loopcount;
    if ( DEX_State == WAIT_FOR_ENQ )
    {
        DEXNonCommTransferIP = true;
        DEX_State = BLOCK_TRANSFER_OUT;
    }
    if ( ( ( DEX_State == BLOCK_TRANSFER_OUT ) || ( DEX_State == TRANSFER_COMPLETE ) ) && ( DEXNonCommTransferIP ) )
    {
        loopcount = 150;
        while ( ( loopcount > 0 ) && ( DEXNonCommTransferBlockComplete == false ) )
        {
            if ( DEX_State == WAIT_FOR_ENQ )
            {
            	//is done with dex transfer
                DEXNonCommTransferIP = false;
                return true;
            }
            loopcount--;
            osDelay ( 15 );
        }
        if ( DEXNonCommTransferBlockComplete == true )
        {
            strcpy ( dexdatablock, (char*)TX_Buff );
            memset ( &TX_Buff, 0x00, sizeof ( TX_Buff ) );
            DEXNonCommTransferBlockComplete = false;
        }
        else
        {   //  Failure - just get out and return done
            DEX_State = WAIT_FOR_ENQ;
            DEXNonCommTransferIP = false;
            return true;
        }
        if ( DEX_State == TRANSFER_COMPLETE )
        {
            DEX_State = WAIT_FOR_ENQ;
            DEXNonCommTransferIP = false;
            return true;
        }
        return false;
    }
    else
    {
        DEXNonCommTransferIP = false;
        return true;
    }
}
/*++
 *=============================================================
 *
 * Copyright © 2024 Wittern
 *
 *=============================================================
*--*/

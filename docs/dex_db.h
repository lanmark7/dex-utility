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
#ifndef __DEX_DB_H_
#define __DEX_DB_H_
#include "WrapperOs.h"

#define MAX_MOTOR_COUNT			(168U)

typedef struct
{
    uint32_t VA103_VALUE_OF_ALL_PAID_VEND_SALES_SINCE_LAST_RESET;
    uint32_t VA104_NUMBER_OF_ALL_PAID_VEND_SALES_SINCE_LAST_RESET;
    uint32_t VA107_VALUE_OF_ALL_DISCOUNTS_SINCE_LAST_RESET;
    uint32_t VA108_NUMBER_OF_ALL_DISCOUNTED_PAID_VENDS_SINCE_LAST_RESET;
    uint32_t VA203_TEST_VEND_VALUE_OF_SALES_SINCE_LAST_RESET;
    uint32_t VA204_NUMBER_OF_TEST_VENDS_SINCE_LAST_RESET;
    uint32_t VA303_FREE_VEND_VALUE_OF_SALES_SINCE_LAST_RESET;
    uint32_t VA304_NUMBER_OF_FREE_VEND_SALES_SINCE_LAST_RESET;
    uint32_t CA203_VALUE_OF_CASH_SALES_SINCE_LAST_RESET;
    uint32_t CA204_NUMBER_OF_CASH_VENDS_SINCE_LAST_RESET;
    uint32_t CA301_VALUE_OF_CASH_IN_SINCE_LAST_RESET;
    uint32_t CA302_VALUE_OF_CASH_TO_CASH_BOX_SINCE_LAST_RESET;
    uint32_t CA303_VALUE_OF_CASH_TO_TUBES_SINCE_LAST_RESET;
    uint32_t CA304_VALUE_OF_BILLS_IN_SINCE_LAST_RESET;
    uint32_t CA309_VALUE_OF_BILLS_IN_SINCE_LAST_RESET;
    uint32_t CA311_VALUE_OF_BILLS_TO_RECYCLER_SINCE_LAST_RESET;
    uint32_t CA401_VALUE_OF_CASH_DISPENSED_SINCE_LAST_RESET;
    uint32_t CA402_VALUE_OF_MANUAL_CASH_DISPENSED_SINCE_LAST_RESET;
    uint32_t CA405_VALUE_OF_BILLS_DISPENSED_SINCE_LAST_RESET;
    uint32_t CA406_VALUE_OF_BILLS_MANUALLY_DISPENSED_SINCE_LAST_RESET;
    uint32_t CA407_VALUE_OF_BILLS_TRANSFERRED_TO_RECYCLER_SINCE_LAST_RESET;
    uint32_t CA801_VALUE_OF_CASH_OVERPAY_SINCE_LAST_RESET;
    uint32_t CA901_VALUE_OF_PAY_VENDS_EXACT_CHANGE_SINCE_LAST_RESET;
    uint32_t CA1001_VALUE_OF_CASH_FILLED_SINCE_LAST_RESET;
    uint32_t CA1003_VALUE_OF_BILLS_FILLED_SINCE_LAST_RESET;
    uint32_t CA1601_VALUE_CREDITED_FROM_MACHINE_SINCE_LAST_RESET;
    uint32_t DA203_VALUE_OF_CASHLESS_1_SALES_SINCE_LAST_RESET;
    uint32_t DA204_NUMBER_OF_CASHLESS_1_SALES_SINCE_LAST_RESET;
    uint32_t DA402_VALUE_OF_CREDIT_TO_CASHLESS_1_SINCE_LAST_RESET;
    uint32_t DA501_VALUE_OF_CASHLESS_1_DISCOUNTS_SINCE_LAST_RESET;
    uint32_t DA502_NUMBER_OF_DISCOUNT_CASHLESS_1_VENDS_SINCE_LAST_RESET;
    uint32_t DA505_VALUE_OF_CASHLESS_1_SURCHARGES_SINCE_LAST_RESET;
    uint32_t DA506_NUMBER_OF_SURCHARGE_CASHLESS_1_VENDS_SINCE_LAST_RESET;
    uint32_t DA602_REVALUATION_INCENTIVE_ON_CASHLESS_1_SINCE_LAST_RESET;
    uint32_t DA901_VALUE_OF_CASHLESS_1_OVERPAY_SINCE_LAST_RESET;
    uint32_t DA1003_CASHLESS_1_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_LAST_RESET;
    uint32_t DA1004_CASHLESS_1_VALUE_OF_MIXED_PAYMENT_CASHLESS_AMOUNT_SINCE_LAST_RESET;
    uint32_t DB203_VALUE_OF_CASHLESS_2_SALES_SINCE_LAST_RESET;
    uint32_t DB204_NUMBER_OF_CASHLESS_2_SALES_SINCE_LAST_RESET;
    uint32_t DB402_VALUE_OF_CREDIT_TO_CASHLESS_2_SINCE_LAST_RESET;
    uint32_t DB501_VALUE_OF_CASHLESS_2_DISCOUNTS_SINCE_LAST_RESET;
    uint32_t DB502_NUMBER_OF_DISCOUNT_CASHLESS_2_VENDS_SINCE_LAST_RESET;
    uint32_t DB505_VALUE_OF_CASHLESS_2_SURCHARGES_SINCE_LAST_RESET;
    uint32_t DB506_NUMBER_OF_SURCHARGE_CASHLESS_2_VENDS_SINCE_LAST_RESET;
    uint32_t DB602_REVALUATION_INCENTIVE_ON_CASHLESS_2_SINCE_LAST_RESET;
    uint32_t DB901_VALUE_OF_CASHLESS_2_OVERPAY_SINCE_LAST_RESET;
    uint32_t DB1003_CASHLESS_2_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_LAST_RESET;
    uint32_t DB1004_CASHLESS_2_VALUE_OF_MIXED_PAYMENT_CASHLESS_AMOUNT_SINCE_LAST_RESET;
    uint32_t TA203_VALUE_OF_TOKEN_COUPON_SALES_SINCE_LAST_RESET;
    uint32_t TA204_NUMBER_OF_TOKEN_COUPON_SALES_SINCE_LAST_RESET;
    uint32_t TA207_VALUE_OF_VALUE_TOKEN_COUPON_SALES_SINCE_LAST_RESET;
    uint32_t TA208_NUMBER_OF_VALUE_TOKEN_COUPON_SALES_SINCE_LAST_RESET;
    uint32_t EA202_ELA_TOTAL_NUMBER_OF_EVENTS_SINCE_LAST_RESET;
    uint32_t EA202_EJL_TOTAL_NUMBER_OF_EVENTS_SINCE_LAST_RESET;
    uint32_t COINS_ACCEPTED_SINCE_LAST_RESET;
    uint32_t COINS_REJECTED_SINCE_LAST_RESET;
    uint32_t BILLS_ACCEPTED_SINCE_LAST_RESET;
    uint32_t BILLS_REJECTED_SINCE_LAST_RESET;
} DEX_RESETABLE_FIELD_TYPE;

typedef struct
{
    uint32_t VA101_VALUE_OF_ALL_PAID_VEND_SALES_SINCE_INITIALIZATION;
    uint32_t VA102_NUMBER_OF_ALL_PAID_VEND_SALES_SINCE_INITIALIZATION;
    uint32_t VA105_VALUE_OF_ALL_DISCOUNTS_SINCE_SINCE_INITIALIZATION;
    uint32_t VA106_NUMBER_OF_ALL_DISCOUNTED_PAID_VENDS_SINCE_INITIALIZATION;
    uint32_t VA201_TEST_VEND_VALUE_OF_SALES_SINCE_INITIALIZATION;
    uint32_t VA202_NUMBER_OF_TEST_VENDS_SINCE_INITIALIZATION;
    uint32_t VA301_FREE_VEND_VALUE_OF_SALES_SINCE_INITIALIZATION;
    uint32_t VA302_NUMBER_OF_FREE_VEND_SALES_SINCE_INITIALIZATION;
    uint32_t CA201_VALUE_OF_CASH_SALES_SINCE_INITIALIZATION;
    uint32_t CA202_NUMBER_OF_CASH_VENDS_SINCE_INITIALIZATION;
    uint32_t CA305_VALUE_OF_CASH_IN_SINCE_INITIALIZATION;
    uint32_t CA306_VALUE_OF_CASH_TO_BOX_SINCE_INITIALIZATION;
    uint32_t CA307_VALUE_OF_CASH_TO_TUBES_SINCE_INITIALIZATION;
    uint32_t CA308_VALUE_OF_BILLS_IN_SINCE_INITIALIZATION;
    uint32_t CA310_VALUE_OF_BILLS_IN_SINCE_INITIALIZATION;
    uint32_t CA312_VALUE_OF_BILLS_TO_RECYCLER_SINCE_INITIALIZATION;
    uint32_t CA403_VALUE_OF_CASH_DISPENSED_SINCE_INITIALIZATION;
    uint32_t CA404_VALUE_OF_MANUAL_CASH_DISPENSED_SINCE_INITIALIZATION;
    uint32_t CA408_VALUE_OF_BILLS_DISPENSED_SINCE_INITIALISATION;
    uint32_t CA409_VALUE_OF_BILLS_MANUALLY_DISPENSED_SINCE_INITIATLISATION;
    uint32_t CA410_VALUE_OF_BILLS_TRANSFERRED_TO_RECYCLER_SINCE_INITIATLISATION;
    uint32_t CA802_VALUE_OF_CASH_OVERPAY_SINCE_INITIALIZATION;
    uint32_t CA902_VALUE_OF_PAY_VENDS_EXACT_CHANGE_SINCE_INITIALIZATION;
    uint32_t CA1002_VALUE_OF_CASH_FILLED_SINCE_INITIALIZATION;
    uint32_t CA1004_VALUE_OF_BILLS_FILLED_SINCE_INITIALISATION;
    uint32_t CA1602_VALUE_CREDITED_FROM_MACHINE_SINCE_INITIALIZATION;
    uint32_t DA201_VALUE_OF_CASHLESS_1_SALES_SINCE_INITIALIZATION;
    uint32_t DA202_NUMBER_OF_CASHLESS_1_SALES_SINCE_INITIALIZATION;
    uint32_t DA401_VALUE_OF_CREDIT_TO_CASHLESS_1_SINCE_INITIALIZATION;
    uint32_t DA503_VALUE_OF_CASHLESS_1_DISCOUNTS_SINCE_INITIALISATION;
    uint32_t DA504_NUMBER_OF_DISCOUNT_CASHLESS_1_VENDS_SINCE_INITIALISATION;
    uint32_t DA507_VALUE_OF_CASHLESS_1_SURCHARGES_SINCE_INITIALISATION;
    uint32_t DA508_NUMBER_OF_SURCHARGE_CASHLESS_1_VENDS_SINCE_INITIALISATION;
    uint32_t DA601_REVALUATION_INCENTIVE_ON_CASHLESS_1_SINCE_INITIALISATION;
    uint32_t DA902_VALUE_OF_CASHLESS_1_OVERPAY_SINCE_INITIALISATION;
    uint32_t DA1001_CASHLESS_1_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_INITIALISATION;
    uint32_t DA1002_CASHLESS_1_VALUE_OF_MIXED_PAYMENT_CASHLESS_AMOUNT_SINCE_INITIALISATION;
    uint32_t DB201_VALUE_OF_CASHLESS_2_SALES_SINCE_INITIALIZATION;
    uint32_t DB202_NUMBER_OF_CASHLESS_2_SALES_SINCE_INITIALIZATION;
    uint32_t DB401_VALUE_OF_CREDIT_TO_CASHLESS_2_SINCE_INITIALIZATION;
    uint32_t DB503_VALUE_OF_CASHLESS_2_DISCOUNTS_SINCE_INITIALISATION;
    uint32_t DB504_NUMBER_OF_DISCOUNT_CASHLESS_2_VENDS_SINCE_INITIALISATION;
    uint32_t DB507_VALUE_OF_CASHLESS_2_SURCHARGES_SINCE_INITIALISATION;
    uint32_t DB508_NUMBER_OF_SURCHARGE_CASHLESS_2_VENDS_SINCE_INITIALISATION;
    uint32_t DB601_REVALUATION_INCENTIVE_ON_CASHLESS_2_SINCE_INITIALISATION;
    uint32_t DB902_VALUE_OF_CASHLESS_2_OVERPAY_SINCE_INITIALISATION;
    uint32_t DB1001_CASHLESS_2_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_INITIALISATION;
    uint32_t DB1002_CASHLESS_2_VALUE_OF_MIXED_PAYMENT_CASHLESS_AMOUNT_SINCE_INITIALISATION;
    uint32_t TA201_VALUE_OF_TOKEN_COUPON_SALES_SINCE_INITIALIZATION;
    uint32_t TA202_NUMBER_OF_TOKEN_COUPON_SALES_SINCE_INITIALIZATION;
    uint32_t TA205_VALUE_OF_VALUE_TOKEN_COUPON_SALES_SINCE_INITIALIZATION;
    uint32_t TA206_NUMBER_OF_VALUE_TOKEN_COUPON_SALES_SINCE_INITIALIZATION;
    uint32_t EA202_ELA_TOTAL_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
    uint32_t EA202_EJL_TOTAL_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
} DEX_UNRESETABLE_FIELD_TYPE;

typedef struct
{
    char ID101_MACHINE_SERIAL_NUMBER[21];
    char ID102_MACHINE_MODEL_NUMBER[21];
    char ID103_MACHINE_BUILD_STANDARD[5];
    char ID104_MACHINE_LOCATION[31];
    char ID106_MACHINE_ASSET_NUMBER[21];
    char ID401_DECIMAL_POINT_POSITION[3];
    char ID402_CURRENCY_NUMERIC_CODE[5];
    char ID403_CURRENCY_ALPHABETIC_CODE[5];
    char ID601_CURRENT_CASH_BAG_NUMBER[21];
    char CB101_VMC_CONTROL_BOARD_SERIAL_NUMBER[21];
    char CB102_VMC_CONTROL_BOARD_MODEL_NUMBER[21];
    char CB103_VMC_CONTROL_BOARD_BUILD_STANDARD[21];
    char CA101_CHANGER_SERIAL_NUMBER[21];
    char CA102_COIN_MECHANISM_MODEL_NUMBER[21];
    char CA103_COIN_MECHANISM_SOFTWARE_REVISION[5];
    char BA101_BILL_VALIDATOR_SERIAL_NUMBER[21];
    char BA102_BILL_VALIDATOR_MODEL_NUMBER[21];
    char BA103_BILL_VALIDATOR_SOFTWARE_REVISION[5];
    char DA101_CASHLESS_1_SERIAL_NUMBER[21];
    char DA102_CASHLESS_1_MODEL_NUMBER[21];
    char DA103_CASHLESS_1_SOFTWARE_REVISION[5];
    char DB101_CASHLESS_2_SERIAL_NUMBER[21];
    char DB102_CASHLESS_2_MODEL_NUMBER[21];
    char DB103_CASHLESS_2_SOFTWARE_REVISION[5];
} DEX_DEVICE_ID_FIELD_TYPE;

typedef struct
{
    uint32_t CA15TUBE101_VALUE_OF_TUBE_CONTENTS;
    uint32_t CA15TUBE103_NUMBER_OF_COIN_TYPE_0;
    uint32_t CA15TUBE104_NUMBER_OF_COIN_TYPE_1;
    uint32_t CA15TUBE105_NUMBER_OF_COIN_TYPE_2;
    uint32_t CA15TUBE106_NUMBER_OF_COIN_TYPE_3;
    uint32_t CA15TUBE107_NUMBER_OF_COIN_TYPE_4;
    uint32_t CA15TUBE108_NUMBER_OF_COIN_TYPE_5;
    uint32_t CA15TUBE109_NUMBER_OF_COIN_TYPE_6;
    uint32_t CA15TUBE110_NUMBER_OF_COIN_TYPE_7;
    uint32_t CA15TUBE201_VALUE_OF_TUBE_CONTENTS;
    uint32_t CA15TUBE203_NUMBER_OF_COIN_TYPE_8;
    uint32_t CA15TUBE204_NUMBER_OF_COIN_TYPE_9;
    uint32_t CA15TUBE205_NUMBER_OF_COIN_TYPE_10;
    uint32_t CA15TUBE206_NUMBER_OF_COIN_TYPE_11;
    uint32_t CA15TUBE207_NUMBER_OF_COIN_TYPE_12;
    uint32_t CA15TUBE208_NUMBER_OF_COIN_TYPE_13;
    uint32_t CA15TUBE209_NUMBER_OF_COIN_TYPE_14;
    uint32_t CA15TUBE210_NUMBER_OF_COIN_TYPE_15;
} DEX_COIN_TUBE_FIELD_TYPE;




// VEND ACCOUNTING BY SELECTION
// 

typedef struct
{
    // PA101_PRODUCT_IDENTIFIER
    // PA102_PRICE
    // PA103_PRODUCT_IDENTIFICATION
    // PA107_SELECTION_STATUS
    // PA108_CURRENT_PRODUCT_LEVEL
    // PA109_MINIMUM_PRODUCT_LEVEL
    uint32_t PA203_NUMBER_OF_PAID_PRODUCTS_VENDED_SINCE_LAST_RESET;
    uint32_t PA204_VALUE_OF_PAID_PRODUCTS_VENDED_SINCE_LAST_RESET;
    uint32_t PA207_NUMBER_OF_DISCOUNTS_SINCE_LAST_RESET;
    uint32_t PA208_VALUE_OF_DISCOUNTS_SINCE_LAST_RESET;
    uint32_t PA2011_NUMBER_OF_SURCHARGED_PAID_SINCE_LAST_RESET;
    uint32_t PA2012_VALUE_OF_SURCHARGES_PAID_SINCE_LAST_RESET;
    uint32_t PA303_NUMBER_OF_TEST_VENDS_SINCE_LAST_RESET;
    uint32_t PA304_VALUE_OF_TEST_VENDS_SINCE_LAST_RESET;
    uint32_t PA403_NUMBER_OF_FREE_VENDS_SINCE_LAST_RESET;
    uint32_t PA404_VALUE_OF_FREE_VENDS_SINCE_LAST_RESET;
    // PA701_PRODUCT_NUMBER
    // PA702_CASH_DEVICE
    // PA703_PRICE LIST NUMBER
    // PA704_APPLIED PRICE
    uint32_t PA707_NUMER_OF_CASH_SALES_SINCE_LAST_RESET;
    uint32_t PA708_VALUE_OF_CASH_SALES_SINCE_LAST_RESET;
    // PA701_PRODUCT NUMBER
    // PA702_CASHLESS_DEVICE
    // PA703_PRICE LIST NUMBER
    // PA704_APPLIED PRICE
    uint32_t PA707_NUMER_OF_CASHLESS_SALES_SINCE_LAST_RESET;
    uint32_t PA708_VALUE_OF_CASHLESS_SALES_SINCE_LAST_RESET;
    uint32_t PA803_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_LAST_RESET;
    uint32_t PA804_VALUE_OF_MIXED_PAYMENT_CASH_AMOUNT_SINCE_LAST_RESET;
    //    uint32_t PA_SECOND_VEND_SINCE_LAST_RESET;
} DEX_PA_RESETABLE_FIELD_TYPE;



typedef struct
{
    // PA101_PRODUCT_IDENTIFIER
    // PA102_PRICE
    // PA103_PRODUCT_IDENTIFICATION
    // PA107_SELECTION_STATUS
    // PA108_CURRENT_PRODUCT_LEVEL
    // PA109_MINIMUM_PRODUCT_LEVEL
    uint32_t PA201_NUMBER_OF_PAID_PRODUCTS_VENDED_SINCE_INITIALIZATION;
    uint32_t PA202_VALUE_OF_PAID_PRODUCTS_VENDED_SINCE_INITIALIZATION;
    uint32_t PA205_NUMBER_OF_DISCOUNTS_SINCE_INITIALIZATION;
    uint32_t PA206_VALUE_OF_DISCOUNTS_SINCE_INITIALIZATION;
    uint32_t PA209_NUMBER_OF_SURCHARGED_PAID_SINCE_INITIATLIZATION;
    uint32_t PA2010_VALUE_OF_SURCHARGES_PAID_SINCE_INITIALIZATION;
    uint32_t PA301_NUMBER_OF_TEST_VENDS_SINCE_INITIALIZATION;
    uint32_t PA302_VALUE_OF_TEST_VENDS_SINCE_INITIALIZATION;
    uint32_t PA401_NUMBER_OF_FREE_VENDS_SINCE_INITIALIZATION;
    uint32_t PA402_VALUE_OF_FREE_VENDS_SINCE_INITIALIZATION;
    uint32_t PA501_DATEYEAR_OF_MOST_RECENT_SALE;
    uint32_t PA501_DATEMONTH_OF_MOST_RECENT_SALE;
    uint32_t PA501_DATEDAY_OF_MOST_RECENT_SALE;
    uint32_t PA502_TIMEHOUR_OF_MOST_RECENT_SALE;
    uint32_t PA502_TIMEMINUTE_OF_MOST_RECENT_SALE;
    uint32_t PA503_NUMBER_OF_TIMES_SOLD_OUT_PRODUCT_SELECTED;
    // PA701_PRODUCT_NUMBER
    // PA702_CASH_DEVICE
    // PA703_PRICE LIST NUMBER
    // PA704_APPLIED PRICE
    uint32_t PA705_NUMBER_OF_CASH_SALES_SINCE_INITIALISATION;
    uint32_t PA706_VALUE_OF_CASH_SALES_SINCE_INITIALISATION;
    // PA701_PRODUCT NUMBER
    // PA702_CASHLESS_DEVICE
    // PA703_PRICE LIST NUMBER
    // PA704_APPLIED PRICE
    uint32_t PA705_NUMBER_OF_CASHLESS_SALES_SINCE_INITIALISATION;
    uint32_t PA706_VALUE_OF_CASHLESS_SALES_SINCE_INITIALISATION;
    uint32_t PA801_NUMBER_OF_MIXED_PAYMENT_VENDS_SINCE_INITIALISATION;
    uint32_t PA802_VALUE_OF_MIXED_PAYMENT_CASH_AMOUNT_SINCE_INITIALISATION;
    //    uint32_t PA_SECOND_VEND_SINCE_INITIALIZATION;
} DEX_PA_UNRESETABLE_FIELD_TYPE;

typedef struct
{
    // EVENT ACCOUNTING BY SELECTION
    //EA201_EJL_PRODUCT_NUMBER
    uint32_t EA202_EJL_NUMBER_OF_EVENTS_SINCE_LAST_RESET;
    uint32_t EA202_ELA_NUMBER_OF_EVENTS_SINCE_LAST_RESET;
} DEX_EA_PRODUCT_DISP_RESETABLE_FIELD_TYPE;


typedef struct
{
    char DexDataStructureSignature[20];
} DEX_DATA_STRUCTURE_SIGNATURE_TYPE;

typedef struct
{
    // EVENT ACCOUNTING BY SELECTION
    //EA101_EJL_PRODUCT_NUMBER
    uint32_t EA102_EJL_DATEYEAR_OF_EVENT;
    uint32_t EA102_EJL_DATEMONTH_OF_EVENT;
    uint32_t EA102_EJL_DATEDAY_OF_EVENT;
    uint32_t EA103_EJL_TIMEHOUR_OF_EVENT;
    uint32_t EA103_EJL_TIMEMINUTE_OF_EVENT;
    //EA201_EJL_PRODUCT_NUMBER
    uint32_t EA203_EJL_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
    uint8_t  EA205_EJL_CURRENT_EVENT_ACTIVITY;
    uint32_t EA206_EJL_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES;
    // EA101_ELA_PRODUCT_NUMBER
    uint32_t EA102_ELA_DATEYEAR_OF_EVENT;
    uint32_t EA102_ELA_DATEMONTH_OF_EVENT;
    uint32_t EA102_ELA_DATEDAY_OF_EVENT;
    uint32_t EA103_ELA_TIMEHOUR_OF_EVENT;
    uint32_t EA103_ELA_TIMEMINUTE_OF_EVENT;
    // EA201_ELA_PRODUCT_NUMBER
    uint32_t EA203_ELA_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
    uint8_t  EA205_ELA_CURRENT_EVENT_ACTIVITY;
    uint32_t EA206_ELA_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES;
} DEX_EA_PRODUCT_DISP_UNRESETABLE_FIELD_TYPE;

typedef struct
{
    // EVENT ACCOUNTING RESETABLE
    //EA201_EGS
    uint32_t EA202_EGS_NUMBER_OF_EVENTS_SINCE_LAST_RESET;
    //EA201_EJH
    uint32_t EA202_EJH_NUMBER_OF_EVENTS_SINCE_LAST_RESET;
    //EA201_EJJ    
    uint32_t EA202_EJJ_NUMBER_OF_EVENTS_SINCE_LAST_RESET;
    //EA201_EJK    
    uint32_t EA202_EJK_NUMBER_OF_EVENTS_SINCE_LAST_RESET;
    //EA201_EA - General non-specific Coin Mechanism fault    
    uint32_t EA202_EA_NUMBER_OF_EVENTS_SINCE_LAST_RESET;
    //EA201_EK - General non-specific Cashless 1 Mechanism fault    
    uint32_t EA202_EK_NUMBER_OF_EVENTS_SINCE_LAST_RESET;
    //EA201_EK2 - General non-specific Cashless 2 Mechanism fault   
    uint32_t EA202_EK2_NUMBER_OF_EVENTS_SINCE_LAST_RESET;
    //EA201_EN - General non-specific Bill Validator fault    
    uint32_t EA202_EN_NUMBER_OF_EVENTS_SINCE_LAST_RESET;
    //EA201_EC1F - A malfunction has been detected in the selection board    
    uint32_t EA202_EC1F_NUMBER_OF_EVENTS_SINCE_LAST_RESET;

    //Power Outages
    uint32_t EA701_NUMBER_OF_POWER_OUTAGES_SINCE_LAST_RESET;
} DEX_EA_RESETABLE_FIELD_TYPE;



typedef struct
{
    // EVENT ACCOUNTING HISTORICAL
    //EA101_EGS
    uint32_t EA102_EGS_DATEYEAR_OF_EVENT;
    uint32_t EA102_EGS_DATEMONTH_OF_EVENT;
    uint32_t EA102_EGS_DATEDAY_OF_EVENT;
    uint32_t EA103_EGS_TIMEHOUR_OF_EVENT;
    uint32_t EA103_EGS_TIMEMINUTE_OF_EVENT;
    //EA201_EGS
    uint32_t EA203_EGS_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
    uint8_t  EA205_EGS_CURRENT_EVENT_ACTIVITY;
    uint32_t EA206_EGS_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES;

    //EA101_EJH -  Health rules have been violated for the temperature controlled
    uint32_t EA102_EJH_DATEYEAR_OF_EVENT;
    uint32_t EA102_EJH_DATEMONTH_OF_EVENT;
    uint32_t EA102_EJH_DATEDAY_OF_EVENT;
    uint32_t EA103_EJH_TIMEHOUR_OF_EVENT;
    uint32_t EA103_EJH_TIMEMINUTE_OF_EVENT;
    //EA201_EJH
    uint32_t EA203_EJH_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
    uint32_t EA206_EJH_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES;

    //EA101_EJJ -  cooling unit does not cool down to the pre-set temperature
    uint32_t EA102_EJJ_DATEYEAR_OF_EVENT;
    uint32_t EA102_EJJ_DATEMONTH_OF_EVENT;
    uint32_t EA102_EJJ_DATEDAY_OF_EVENT;
    uint32_t EA103_EJJ_TIMEHOUR_OF_EVENT;
    uint32_t EA103_EJJ_TIMEMINUTE_OF_EVENT;
    //EA201_EJJ
    uint32_t EA203_EJJ_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
    uint32_t EA206_EJJ_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES;

    //EA101_EJK - Temperature sensor is defective
    uint32_t EA102_EJK_DATEYEAR_OF_EVENT;
    uint32_t EA102_EJK_DATEMONTH_OF_EVENT;
    uint32_t EA102_EJK_DATEDAY_OF_EVENT;
    uint32_t EA103_EJK_TIMEHOUR_OF_EVENT;
    uint32_t EA103_EJK_TIMEMINUTE_OF_EVENT;
    //EA201_EJH
    uint32_t EA203_EJK_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
    uint32_t EA206_EJK_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES;

    //EA101_EA - General non-specific Coin Mechanism fault 
    uint32_t EA102_EA_DATEYEAR_OF_EVENT;
    uint32_t EA102_EA_DATEMONTH_OF_EVENT;
    uint32_t EA102_EA_DATEDAY_OF_EVENT;
    uint32_t EA103_EA_TIMEHOUR_OF_EVENT;
    uint32_t EA103_EA_TIMEMINUTE_OF_EVENT;
    //EA201_EA
    uint32_t EA203_EA_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
    uint32_t EA206_EA_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES;

    //EA101_EK - General non-specific Cashless 1 Mechanism fault 
    uint32_t EA102_EK_DATEYEAR_OF_EVENT;
    uint32_t EA102_EK_DATEMONTH_OF_EVENT;
    uint32_t EA102_EK_DATEDAY_OF_EVENT;
    uint32_t EA103_EK_TIMEHOUR_OF_EVENT;
    uint32_t EA103_EK_TIMEMINUTE_OF_EVENT;
    //EA201_EK
    uint32_t EA203_EK_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
    uint32_t EA206_EK_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES;

    //EA101_EK2 - General non-specific Cashless 2 Mechanism fault 
    uint32_t EA102_EK2_DATEYEAR_OF_EVENT;
    uint32_t EA102_EK2_DATEMONTH_OF_EVENT;
    uint32_t EA102_EK2_DATEDAY_OF_EVENT;
    uint32_t EA103_EK2_TIMEHOUR_OF_EVENT;
    uint32_t EA103_EK2_TIMEMINUTE_OF_EVENT;
    //EA201_EK2
    uint32_t EA203_EK2_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
    uint32_t EA206_EK2_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES;

    //EA101_EN - General non-specific Bill Validator fault
    uint32_t EA102_EN_DATEYEAR_OF_EVENT;
    uint32_t EA102_EN_DATEMONTH_OF_EVENT;
    uint32_t EA102_EN_DATEDAY_OF_EVENT;
    uint32_t EA103_EN_TIMEHOUR_OF_EVENT;
    uint32_t EA103_EN_TIMEMINUTE_OF_EVENT;
    //EA201_EN
    uint32_t EA203_EN_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
    uint32_t EA206_EN_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES;

    //EA101_EC1F - A malfunction has been detected in the selection board
    uint32_t EA102_EC1F_DATEYEAR_OF_EVENT;
    uint32_t EA102_EC1F_DATEMONTH_OF_EVENT;
    uint32_t EA102_EC1F_DATEDAY_OF_EVENT;
    uint32_t EA103_EC1F_TIMEHOUR_OF_EVENT;
    uint32_t EA103_EC1F_TIMEMINUTE_OF_EVENT;
    //EA201_EC1F
    uint32_t EA203_EC1F_NUMBER_OF_EVENTS_SINCE_INITIALIZATION;
    uint32_t EA206_EC1F_CUMULATIVE_DURATION_OF_EVENT_IN_MINUTES;

    uint32_t EA301_NUMBER_OF_READS_WITH_RESET_SINCE_INITIALIZATION;
    uint32_t EA302_DATEYEAR_OF_THIS_READOUT;
    uint32_t EA302_DATEMONTH_OF_THIS_READOUT;
    uint32_t EA302_DATEDAY_OF_THIS_READOUT;
    uint32_t EA303_TIMEHOUR_OF_THIS_READOUT;
    uint32_t EA303_TIMEMINUTE_OF_THIS_READOUT;
    uint8_t  EA304_TIME_TERMINAL_INTERROGATOR_IDENTIFICATION[20];
    uint32_t EA305_DATEYEAR_OF_LAST_READ_OUT;
    uint32_t EA305_DATEMONTH_OF_LAST_READ_OUT;
    uint32_t EA305_DATEDAY_OF_LAST_READ_OUT;
    uint32_t EA306_TIMEHOUR_OF_LAST_READ_OUT;
    uint32_t EA306_TIMEMINUTE_OF_LAST_READ_OUT;
    uint8_t  EA307_LAST_TERMINAL_INTERROGATOR_IDENTIFICATION[12];
    uint32_t EA309_NUMBER_OF_READS_WITH_OR_WITHOUT_RESET_SINCE_INITIALIZATION;
    uint32_t EA3010_NUMBER_OF_RESETS_SINCE_INITIALIZATION;
    uint32_t EA702_NUMBER_OF_POWER_OUTAGES_SINCE_INITIALIZATION;
} DEX_EA_UNRESETABLE_FIELD_TYPE;





typedef struct
{
    DEX_RESETABLE_FIELD_TYPE DexResetableFields;
    DEX_UNRESETABLE_FIELD_TYPE DexUnResetableFields;
    DEX_DEVICE_ID_FIELD_TYPE DexDeviceIDFields;
    DEX_COIN_TUBE_FIELD_TYPE DexCoinTubeField;
    DEX_PA_RESETABLE_FIELD_TYPE Dex_PA_ResetableField[MAX_MOTOR_COUNT];
    DEX_DATA_STRUCTURE_SIGNATURE_TYPE DEX_Data_Signature;
    DEX_PA_UNRESETABLE_FIELD_TYPE Dex_PA_UnResetableField[MAX_MOTOR_COUNT];
    DEX_EA_PRODUCT_DISP_RESETABLE_FIELD_TYPE Dex_EA_ProductDispenseResetableField[MAX_MOTOR_COUNT];
    DEX_EA_PRODUCT_DISP_UNRESETABLE_FIELD_TYPE Dex_EA_ProductDispenseUnResetableField[MAX_MOTOR_COUNT];
    DEX_EA_RESETABLE_FIELD_TYPE Dex_EA_ResetableField;
    DEX_EA_UNRESETABLE_FIELD_TYPE Dex_EA_UnResetableField;
} DEX_DATA_STRUCTURE;

#endif /* __DEX_DB_H_ */
/*++
 *=============================================================
 *
 * Copyright © 2024 Wittern
 *
 *=============================================================
*--*/
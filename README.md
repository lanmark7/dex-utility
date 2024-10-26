# Dex Parsing

Reference:  https://github.com/mdisibio/dex-parse
EVA - DTS Reference: https://evadts.com/

## Sentences

| Sentence | Description |
| -- | -- |
| DXS | Transaction (START) |
| DXE | Transaction (END) |
| ST | Transmission Set (START) |
| SE | Transmission Set (END)|
| G85 | Integrity Check Value |
| -- |  |
| MA5 | Machine Configuration |
| -- |  |
| AC | |
| AM1 | |
| BA1 | Bill Validator |
| CA1 | Coin Mechanism |
| CA10 | Coin Mechanism |
| CA15 | Coin Mechanism  |
| CA16 | Coin Mechanism  |
| CA2 | Coin Mechanism |
| CA3 | Cash In data segment |
| CA4 | Cash Out data segment |
| CA8 | Coin Mechanism |
| CA9 | Coin Mechanism |
| CB1 | Vending Machine Controller |
| DA1 | Cashless |
| DA2 | Cashless |
| DA4 | Cashless |
| DA5 | Cashless |
| DA6 | Cashless |
| DA9 | Cashless |
| DB1 |
| DB10  |
| DB2 |
| DB4 |
| DB7 |
| EA1 | Event |
| EA2 | Event Information data segment |
| EA3 | Number of Reads data segment |
| EA7 | Event |
| HA1  | |
| HA2 | |
| HA3 | |
| HB1 | |
| HB2 | |
| HB3 | |
| ID1 | Vending Machine |
| ID4 | Vending Machine |
| PA1 | Product 2 Information data segment |
| PA2 | Product |
| PA3 | Product |
| PA4 | Product 2 Free Vend Transaction data segment |
| PA5 | Product |
| PA7 | Cashless Transaction data segment |
| PA8 | Cashless Transaction data segment |
| PC1 | |
| SD1 | Other |
| TA2 | Token Acceptor |
| VA1 | Vend Transaction data segment |
| VA2 | Free Vend Transaction data segment |
| VA3 | Value of All... |


## Sentence Structures

Below is a break down of the various DEX sentences that and their field element definitions

### DXS
| Element | Required | Description | Type |
| -- | -- | -- | -- |
| 01 | M | COMMUNICATION ID OF SENDER | AN |
| 02 | M | FUNCTIONAL IDENTIFIER (VA) | ID |
| 03 | M | VERSION (0/6) | ID |
| 04 | M | TRANSMISSION CONTROL NUMBER (1) | N0 |
| 05 | O | COMMUNICATION ID OF RECIPIENT | AN |

### ST
| Element | Required | Description | Type |
| -- | -- | -- | -- |
| 01 | M | TRANSACTION SET HEADER (001) | ID |
| 02 | M | TRANSACTION SET CONTROL NUMBER (0001) | AN |

### AM1
| Element | Required | Description | Type |
| -- | -- | -- | -- |
| 01 | O | AUDIT MODULE / COMMS GATEWAY SERIAL NUMBER | AN | 
| 02 | O | AUDIT MODULE / COMMS GATEWAY MODEL NUMBER | AN | 
| 03 | O | AUDIT MODULE / COMMS GATEWAY SOFTWARE REVISION | N0 | 

### AC
| Element | Required | Description | Type |
| -- | -- | -- | -- |
1
05 – CMCG AUDIT MODULE / COMMS GATEWAY ASSET NUMBER AN

### ID1
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | MACHINE SERIAL NUMBER | AN |
| 02 | O | MACHINE MODEL NUMBER | AN |
| 03 | O | MACHINE BUILD STANDARD | N0 |
| IC1 04 | O | MACHINE LOCATION | AN |
| IC1 06 | M | MACHINE ASSET NUMBER | AN |

### ID4
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | DECIMAL POINT POSITION | N0 |
| 02 | O | CURRENCY NUMERIC CODE | N0 |
| 03 | O | CURRENCY ALPHABETIC CODE | AN |
| ID6 IC6 01 | M | CURRENT CASH BAG NUMBER | AN |

### CB1
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | VMC CONTROL BOARD SERIAL NUMBER | AN |
| 02 | O | VMC CONTROL BOARD MODEL NUMBER | AN |
| 03 | CM | VMC CONTROL BOARD BUILD STANDARD | AN |

### VA1
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | M | VALUE OF ALL PAID VEND SALES SINCE INITIALIZATION | Nc |
| 02 | M | NUMBER OF ALL PAID VEND SALES SINCE INITIALIZATION | N0 |
| 03 | O | VALUE OF ALL PAID VEND SALES SINCE LAST RESET | Nc |
| 04 | O | NUMBER OF ALL PAID VEND SALES SINCE LAST RESET | N0 |

### VA2
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | TEST VEND VALUE OF SALES SINCE INITIALIZATION | Nc |
| 02 | O | NUMBER OF TEST VENDS SINCE INITIALIZATION | N0 |
| 03 | O | TEST VEND VALUE OF SALES SINCE LAST RESET | Nc |
| 04 | O | NUMBER OF TEST VENDS SINCE LAST RESET | N0 |

### VA3

| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | M | FREE VEND VALUE OF SALES SINCE INITIALIZATION | Nc |
| 02 | M | NUMBER OF FREE VEND SALES SINCE INITIALIZATION | N0 |
| 03 | O | FREE VEND VALUE OF SALES SINCE LAST RESET | Nc |
| 04 | O | NUMBER OF FREE VEND SALES SINCE LAST RESET | N0 |

### CA1
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
01 – M CM / O VMC/VMD CHANGER SERIAL NUMBER AN
02 – M CM / O VMC/VMD COIN MECHANISM MODEL NUMBER AN
03 – M CM / O VMC/VMD COIN MECHANISM SOFTWARE REVISION N0

### CA2
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | CM / O | VMC/VMD VALUE OF CASH SALES SINCE INITIALIZATION | Nc |
| 02 | M CM / O|  VMC/VMD NUMBER OF CASH VENDS SINCE INITIALIZATION | N0 |
| 03 | M CM / O|  VMC/VMD VALUE OF CASH SALES SINCE LAST RESET | Nc |
| 04 | M CM / O|  VMC/VMD NUMBER OF CASH VENDS SINCE LAST RESET | N0 |

### CA3
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | VALUE OF CASH IN SINCE LAST RESET | Nc |
| 02 | O | VALUE OF CASH TO CASH BOX SINCE LAST RESET | Nc |
| 03 | O | VALUE OF CASH TO TUBES SINCE LAST RESET | Nc |
| 04 | O (NOTE 6NOTE 6) | VALUE OF BILLS IN SINCE LAST RESET N0 / | Nc |
| 05 | O | VALUE OF CASH IN SINCE INITIALIZATION | Nc |
| 06 | CM | VALUE OF CASH TO BOX SINCE INITIALIZATION | Nc |
| 07 | CM | VALUE OF CASH TO TUBES SINCE INITIALIZATION | Nc |
| 08 | CM (NOTE 6) | VALUE OF BILLS IN SINCE INITIALIZATION N0 / | Nc |
| 09 | O (NOTE 6) | VALUE OF BILLS IN SINCE LAST RESET | Nc |
| 10 | CM (NOTE 6) | VALUE OF BILLS IN SINCE INITIALIZATION | Nc |
| 11 | CM | VALUE OF BILLS TO RECYCLER SINCE LAST RESET | Nc |
| 12 | CM | VALUE OF BILLS TO RECYCLER SINCE INITIALIZATION | Nc |

### CA4
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | VALUE OF CASH DISPENSED SINCE LAST RESET | Nc |
| 02 | O | VALUE OF MANUAL CASH DISPENSED SINCE LAST RESET | Nc |
| 03 | CM | VALUE OF CASH DISPENSED SINCE INITIALIZATION | Nc |
| 04 | CM | VALUE OF MANUAL CASH DISPENSED SINCE INITIALIZATION| Nc |
| 05 | CM | VALUE OF BILLS DISPENSED SINCE LAST RESET | Nc |
| 06 | CM | VALUE OF BILLS MANUALLY DISPENSED SINCE LAST RESET | Nc |
| 07 | CM | VALUE OF BILLS TRANSFERRED TO RECYCLER SINCE LAST RESET | Nc |
| 08 | CM | VALUE OF BILLS DISPENSED SINCE INITIALISATION | Nc |
| 09 | CM | VALUE OF BILLS MANUALLY DISPENSED SINCE INITIATLISATION | Nc |
| 10 | CM | VALUE OF BILLS TRANSFERRED TO RECYCLER SINCE INITIATLISATION | Nc |

### CA8
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | VALUE OF CASH OVERPAY SINCE LAST RESET | Nc |
| 02 | CM| VALUE OF CASH OVERPAY SINCE INITIALIZATION | Nc |

### CA10
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | VALUE OF CASH FILLED SINCE LAST RESET | Nc |
| 02 | CM | VALUE OF CASH FILLED SINCE INITIALIZATION | Nc |
| 03 | O | VALUE OF BILLS FILLED SINCE LAST RESET | Nc |
| 04 | CM | VALUE OF BILLS FILLED SINCE INITIALISATION | Nc |

### CA16
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | VALUE CREDITED FROM MACHINE SINCE LAST RESET | Nc |
| 02 | CM | (Note 7) VALUE CREDITED FROM MACHINE SINCE INITIALIZATION | Nc |

### BA1
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | BILL VALIDATOR SERIAL NUMBER | AN |
| 02 | O | BILL VALIDATOR MODEL NUMBER | AN |
| 03 | O | BILL VALIDATOR SOFTWARE REVISION | N0 |

### DA1
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | M | CD / OVMC CASHLESS 1 SERIAL NUMBER | AN |
| 02 | M | CD / OVMC CASHLESS 1 MODEL NUMBER | AN |
| 03 | M | CD / OVMC CASHLESS 1 SOFTWARE REVISION | N0 |

### DA2
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | CM | VALUE OF CASHLESS 1 SALES SINCE INITIALIZATION | Nc |
| 02 | CM |  NUMBER OF CASHLESS 1 SALES SINCE INITIALIZATION | N0 |
| 03 | O | VALUE OF CASHLESS 1 SALES SINCE LAST RESET | Nc |
| 04 | O | NUMBER OF CASHLESS 1 SALES SINCE LAST RESET | N0 |

### DA4
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | CM | VALUE OF CREDIT TO CASHLESS 1 SINCE INITIALIZATION | Nc |
| 02 | O | VALUE OF CREDIT TO CASHLESS 1 SINCE LAST RESET | Nc |

### DA7
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | CM | (Note 5) CASHLESS 1 USER GROUP NUMBER | N0 |
| 02 | O | CASHLESS 1 USER GROUP NET SALES SINCE LAST RESET | Nc |
| 03 | O | CASHLESS 1 USER GROUP VALUE ADDED TO CARD SINCE LAST RESET | Nc |
| 04 | O | CASHLESS 1 USER GROUP VALUE OF CARD DISCOUNTS SINCE LAST RESET | Nc |
| 07 | CM | (Note 5) CASHLESS 1 USER GROUP NET SALES SINCE INITIALISATION | Nc |
| 08 | CM | (Note 5) USER GROUP VALUE ADDED TO CASHLESS 1 SINCE INITIALISATION | Nc |
| 09 | CM | (Note 5) USER GROUP VALUE OF CASHLESS 1 DISCOUNTS SINCE INITIALISATION | Nc |

### DA10
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | CM | CASHLESS 1 NUMBER OF MIXED PAYMENT VENDS SINCE INITIALISATION | N0 |
| 02 | CM | CASHLESS 1 VALUE OF MIXED PAYMENT CASHLESS AMOUNT SINCE INITIALISATION | Nc |
| 03 | CM | CASHLESS 1 NUMBER OF MIXED PAYMENT VENDS SINCE LAST RESET | N0 |
| 04 | CM | CASHLESS 1 VALUE OF MIXED PAYMENT CASHLESS AMOUNT SINCE LAST RESET | Nc |

### DB1
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | M CD / O VMC | CASHLESS 2 SERIAL NUMBER | AN |
| 02 | M CD / O VMC | CASHLESS 2 MODEL NUMBER | AN |
| 03 | M CD / O VMC | CASHLESS 2 SOFTWARE REVISION | N0 |

### DB2
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | CM | VALUE OF CASHLESS 2 SALES SINCE INITIALIZATION | Nc | 
| 02 | CM | NUMBER OF CASHLESS 2 SALES SINCE INITIALIZATION | N0 | 
| 03 | O | VALUE OF CASHLESS 2 SALES SINCE LAST RESET | Nc | 
| 04 | O | NUMBER OF CASHLESS 2 SALES SINCE LAST RESET | N0 | 

### DB4
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | CM | VALUE OF CREDIT TO CASHLESS 2 SINCE INITIALIZATION | Nc |
| 02 | O | VALUE OF CREDIT TO CASHLESS 2 SINCE LAST RESET | Nc |

### DB7
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | CM | (Note 5) USER GROUP NUMBER | N0 |
| 02 | O |CASHLESS 2 USER GROUP NET SALES SINCE LAST RESET | Nc |
| 03 | O |CASHLESS 2 USER GROUP VALUE ADDED TO CARD SINCE LAST RESET | Nc |
| 04 | O |CASHLESS 2 USER GROUP VALUE OF CARD DISCOUNTS SINCE LAST RESET | Nc |
| 07 | CM (Note 5) | CASHLESS 2 USER GROUP NET SALES SINCE INITIALISATION | Nc |
| 08 | CM (Note 5) | USER GROUP VALUE ADDED TO CASHLESS 2 SINCE INITIALISATION | Nc |
| 09 | CM (Note 5) | USER GROUP VALUE OF CASHLESS 2 DISCOUNTS SINCE INITIALISATION | Nc |

### DB10 
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | CM | CASHLESS 2 NUMBER OF MIXED PAYMENT VENDS SINCE INITIALISATION | N0 |
| 02 | CM | CASHLESS 2 VALUE OF MIXED PAYMENT CASHLESS AMOUNT SINCE INITIALISATION | Nc |
| 03 | CM | CASHLESS 2 NUMBER OF MIXED PAYMENT VENDS SINCE LAST RESET | N0 |
| 04 | CM | CASHLESS 2 VALUE OF MIXED PAYMENT CASHLESS AMOUNT SINCE LAST RESET | Nc |

### HA1 
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | M CM / O VMC/VMD | HOPPER/DISPENSER SERIAL NUMBER | AN |
| 02 | M CM / O VMC/VMD | HOPPER/DISPENSER MODEL NUMBER | AN |
| 03 | M CM / O VMC/VMD | HOPPER/DISPENSER SOFTWARE REVISION | N0 |

### HA2
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | VALUE OF CASH IN SINCE LAST RESET | Nc |
| 02 | O | VALUE OF CASH FILLED SINCE LAST RESET | Nc |
| 03 | CM | VALUE OF CASH IN SINCE INITIALISATION | Nc |
| 04 | CM | VALUE OF CASH FILLED SINCE INITIALISATION | N0 / Nc |

### HA3
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | VALUE OF CASH DISPENSED SINCE LAST RESET | Nc |
| 02 | O | VALUE OF CASH MANUALLY DISPENSED SINCE LAST RESET | Nc |
| 03 | CM | VALUE OF CASH DISPENSED SINCE INITIALISATION | Nc |
| 04 | CM | VALUE OF CASH MANUALLY DISPENSED SINCE INITIALISATION | Nc |

### HB1
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | M CM / O VMC/VMD | HOPPER/DISPENSER SERIAL NUMBER | AN |
| 02 | M CM / O VMC/VMD | HOPPER/DISPENSER MODEL NUMBER | AN |
| 03 | M CM / O VMC/VMD | HOPPER/DISPENSER SOFTWARE REVISION | N0 |

### HB2
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | VALUE OF CASH IN SINCE LAST RESET | Nc |
| 02 | O | VALUE OF CASH FILLED SINCE LAST RESET | Nc |
| 03 | CM | VALUE OF CASH IN SINCE INITIALISATION | Nc |
| 04 | CM | VALUE OF CASH FILLED SINCE INITIALISATION N0 / | Nc |

### HB3
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | VALUE OF CASH DISPENSED SINCE LAST RESET | Nc |
| 02 | O | VALUE OF CASH MANUALLY DISPENSED SINCE LAST RESET | Nc |
| 03 | CM | VALUE OF CASH DISPENSED SINCE INITIALISATION | Nc |
| 04 | CM | VALUE OF CASH MANUALLY DISPENSED SINCE INITIALISATION | Nc |

### TA2
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | CM | VALUE OF TOKEN (COUPON) SALES SINCE INITIALIZATION | Nc |
| 02 | CM | NUMBER OF TOKEN (COUPON) SALES SINCE INITIALIZATION | N0 |
| 03 | O | VALUE OF TOKEN (COUPON) SALES SINCE LAST RESET | Nc |
| 04 | O | NUMBER OF TOKEN (COUPON) SALES SINCE LAST RESET | N0 |
| 05 | CM | VALUE OF VALUE TOKEN (COUPON) SALES SINCE INITIALIZATION | Nc |
| 06 | CM | NUMBER OF VALUE TOKEN (COUPON) SALES SINCE INITIALIZATION | N0 |
| 07 | O | VALUE OF VALUE TOKEN (COUPON) SALES SINCE LAST RESET | Nc |
| 08 | O | NUMBER OF VALUE TOKEN (COUPON) SALES SINCE LAST RESET | N0 |

### PA1 / PC1 
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | M | (Note 2) PRODUCT IDENTIFIER (Recommend: Product Identifier = Panel Selection #) | AN |
| 02 | O | (Note 2) PRICE | Nc | 
| 03 | O | (Note 2) PRODUCT IDENTIFICATION | AN | 
| 07 | M | (Note 2,3) SELECTION STATUS (0 or blank (recommended) = Selection Present) (1 = Selection Not Present) | N0 | 
| 08 | CM | CURRENT PRODUCT LEVEL | N0 | 
| 09 | CM | MINIMUM PRODUCT LEVEL | N0 | 

### PA2
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | CM | (Note 1) NUMBER OF PAID PRODUCTS VENDED SINCE INITIALIZATION | N0 |
| 02 | CM | (Note 1) VALUE OF PAID PRODUCTS VENDED SINCE INITIALIZATION | Nc |
| 03 | CM | (Note 1) NUMBER OF PAID PRODUCTS VENDED SINCE LAST RESET | N0 |
| 04 | O | VALUE OF PAID PRODUCTS VENDED SINCE LAST RESET | Nc |
| 05 | CM | (Note 1) NUMBER OF DISCOUNTS SINCE INITIALIZATION | N0 |
| 06 | CM | (Note 1) VALUE OF DISCOUNTS SINCE INITIALIZATION | Nc |
| 07 | O | NUMBER OF DISCOUNTS SINCE LAST RESET | N0 |
| 08 | O | VALUE OF DISCOUNTS SINCE LAST RESET | Nc |
| 09 | O | NUMBER OF SURCHARGED PAID SINCE INITIATLIZATION | N0 |
| 10 | O | VALUE OF SURCHARGES PAID SINCE INITIALIZATION | Nc |
| 11 | O | NUMBER OF SURCHARGED PAID SINCE LAST RESET | N0 |
| 12 | O | VALUE OF SURCHARGES PAID SINCE LAST RESET | Nc |

### PA4
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | M | NUMBER OF FREE VENDS SINCE INITIALIZATION | N0 |
| 02 | O | VALUE OF FREE VENDS SINCE INITIALIZATION | Nc |
| 03 | O | NUMBER OF FREE VENDS SINCE LAST RESET | N0 |
| 04 | O | VALUE OF FREE VENDS SINCE LAST RESET | Nc |
| 05 | CM | NUMBER OF FREE VENDS WITHOUT CUP SINCE INITIALISATION | N0 |
| 06 | CM | NUMBER OF FREE VENDS WITHOUT CUP SINCE LAST RESET | N0 |

### PA5
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | DATE OF MOST RECENT SALE | Date |
| 02 | O | TIME OF MOST RECENT SALE | Time |

### PA7
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | CM (Note 1) | PRODUCT NUMBER | AN |
| 02 | CM (Note 1) | PAYMENT DEVICE | AN |
| 03 | CM (Note 1) | PRICE LIST NUMBER | N0 |
| 04 | CM (Note 1) | APPLIED PRICE | Nc |
| 05 | CM (Note 1) | NUMBER OF SALES SINCE INITIALISATION | N0 |
| 06 | CM (Note 1) | VALUE OF SALES SINCE INITIALISATION | Nc |
| 07 | CM (Note 1) | NUMER OF SALES SINCE LAST RESET | N0 |
| 08 | CM (Note 1) | VALUE OF SALES SINCE LAST RESET | Nc |

### PA8
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | CM | NUMBER OF MIXED PAYMENT VENDS SINCE INITIALISATION | N0 |
| 02 | CM | VALUE OF MIXED PAYMENT CASH AMOUNT SINCE INITIALISATION | Nc |
| 03 | CM | NUMBER OF MIXED PAYMENT VENDS SINCE LAST RESET | N0 |
| 04 | CM | VALUE OF MIXED PAYMENT CASH AMOUNT SINCE LAST RESET | Nc |

### EA1 
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | (Note 4) EVENT IDENTIFICATION | AN |
| 02 | O | DATE OF EVENT | DT |
| 03 | O | TIME OF EVENT | TM |
| 04 | O | DURATION OF EVENT IN MINUTES | N0 |

### EA2
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | M | (Note 4) EVENT IDENTIFICATION | AN |
| 02 | O | NUMBER OF EVENTS SINCE LAST RESET | N0 |
| 03 | O | NUMBER OF EVENTS SINCE INITIALIZATION | N0 |
| 05 | M | CURRENT EVENT ACTIVITY ( 1=ACTIVE / 0 (OR BLANK) =INACTIVE ) | N0 |
| 06 | O | DURATION OF EVENT(S) IN MINUTES (includes multiple occurrences) | N0 |

### EA3
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | NUMBER OF READS WITH RESET SINCE INITIALIZATION | N0 |
| 02 | O | DATE OF THIS READOUT | DT |
| 03 | O | TIME OF THIS READOUT | TM |
| 04 | O | TIME TERMINAL / INTERROGATOR IDENTIFICATION | AN |
| 05 | O | DATE OF LAST READ OUT | DT |
| 06 | O | TIME OF LAST READ OUT | TM |
| 07 | O | LAST TERMINAL / INTERROGATOR IDENTIFICATION | AN |
| 09 | M | NUMBER OF READS WITH OR WITHOUT RESET SINCE INITIALIZATION | N0 |
| 10 | M | NUMBER OF RESETS SINCE INITIALIZATION (Not Event Resets) (RESETs can be automatic after a read, via the SD105 RESET command, partial via SD104, or manual via a machine service mode) | N0 |

### EA7 
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | NUMBER OF POWER OUTAGES SINCE LAST RESET | N0 |
| 02 | O | NUMBER OF POWER OUTAGES SINCE INITIALIZATION | N0 |

### SD1
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | O | CURRENT PASSWORD | AN |
| 02 | O | NEW PASSWORD | CD |
| 03 | O | REPORTING INSTRUCTIONS | CD |
| 04 | O | RESET SELECTED DATA | CD |
| 05 | M | RESET ALL INTERVAL DATA CONTROL (AUTO, SAVE, RESET) | CD |
| 06 | M | RESET EVENTS CONTROL (AUTO, SAVE, RESET) | CD |

### G85 
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | M | RECORD INTEGRITY CHECK | AN |

### SE
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | M NUMBER OF INCLUDED SETS (Variable Number) | N0 |
| 02 | M TRANSACTION SET CONTROL NUMBER (Normally 0001) | N0 |

### DXE
| Element | Required? | Description | Type |
| -- | -- | -- | -- |
| 01 | M TRANSMISSION CONTROL NUMBER (1) | N0 |
| 02 | M NUMBER OF INCLUDED SETS (1) | N0 |

### Appendix

* M = Mandatory (must be transferred)
* O = Optional (recommended element if manufacturer can support)
* CM = Conditional Mandatory (required if the device or feature is present in vending system)

MCM/OVMC = Mandatory if VMD is Coin Mech or Optional if VMD is VMC
MCD/OVMC = Mandatory if VMD is Cashless Device or Optional if VMD is VMC
MCG/OVMC = Mandatory if VMD is communication gateway or Optional if VMD is VMC

The column "Block Identifier" also lists configuration elements (IC1, ...). The list shows the minimum requirement.

* Note 1 – Refer chapter 5.1.7 (As from EVA-DTS version 6.1 the PA2 and LA elements shall be grandfathered as far as PA7 reporting is fully implemented. In markets where the price list feature is uncommon and rarely used, it is still allowed to use PA2. If cashless in combination with pricelists are used in the machines, PA7 must be implemented. It is also recommended to use PA7 for all new designs).
* Note 2 – If motor / actuator / mechanism present or historical (since initialization) data is present.
* Note 3 – An example of a 1 being sent would be if the motor/actuator/mechanism was present previously, and something occurred so that it is not being currently detected (i.e., removed, broken wire, etc.) It is not intended to indicate that a product is not available for vending (i.e., sold out). 
* Note 4 - If the machine detects events, see section 2.8 for reporting details.
* Note 5 – If cashless device supports User Groups.
* Note 6 – Historically CA304 and CA308 have used two different formats. These formats are used in a wide installed base and must be maintained in existing and new designs for 5 years from the introduction of Version 6.0 (until 2009). After this date, new designs need only send CA309 andCA310. Check with equipment suppliers to verify their formatting

# Competitors:

* VendOn: https://vendon.net/products/telemetry/
* Cantaloupe
* Nayak
* VendNovation

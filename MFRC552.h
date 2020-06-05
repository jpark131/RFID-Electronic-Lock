/*
    File: MFRC522.h
    MFRC522 - Library for Electronic lock with MFRC552 RFID readers
    Created by Eelco Rouw - Mainly based on code from Grant Gibson (www.grantgibson.co.uk) and Dr.Leong ( WWW.B2CQSHOP.COM )
    Ported to C by Stephen S. M.
    Ported to MSP432P401R and specialized for this project by Jake Park
    Released into the public domain
*/

/*
    MFRC522 and Card specific defines. For the role of each define, please consult the datasheet.
*/
#ifndef MFRC552_H_
#define MFRC552_H_

#include "SPI.h"

#define MAX_LEN 16

#define CARD_LENGTH 5

//MF522 - Commands
#define PCD_IDLE              0x00               
#define PCD_AUTHENT           0x0E               
#define PCD_RECEIVE           0x08               
#define PCD_TRANSMIT          0x04               
#define PCD_TRANSCEIVE        0x0C               
#define PCD_RESETPHASE        0x0F               
#define PCD_CALCCRC           0x03               

//Mifare_One - Commands
#define PICC_REQIDL           0x26
#define PICC_REQALL           0x52
#define PICC_ANTICOLL         0x93
#define PICC_GETUID           0x20
#define PICC_SElECTTAG        0x93
#define PICC_AUTHENT1A        0x60
#define PICC_AUTHENT1B        0x61
#define PICC_READ             0x30
#define PICC_WRITE            0xA0
#define PICC_DECREMENT        0xC0
#define PICC_INCREMENT        0xC1
#define PICC_RESTORE          0xC2
#define PICC_TRANSFER         0xB0
#define PICC_HALT             0x50

//MF522 - Status
#define MI_OK                 0
#define MI_ERR                2

//MF522 - Registers
//Page 0:Command and Status
#define     Reserved00            0x00    
#define     CommandReg            0x01    
#define     CommIEnReg            0x02    
#define     DivlEnReg             0x03    
#define     CommIrqReg            0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     Reserved01            0x0F

//Page 1:Command
#define     Reserved10            0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     Reserved11            0x1A
#define     Reserved12            0x1B
#define     MifareReg             0x1C
#define     Reserved13            0x1D
#define     Reserved14            0x1E
#define     SerialSpeedReg        0x1F

//Page 2:CFG
#define     Reserved20            0x20
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     Reserved21            0x23
#define     ModWidthReg           0x24
#define     Reserved22            0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsPReg              0x28
#define     ModGsPReg             0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F

//Misc
#define     TXLASTBUTONE          0x07 //set TxLastBits all but one last bit
#define     READMODE              0x80 //Tx will send read address


void MFRC552_WriteReg(uint8_t addr, uint8_t val); //write to register
uint8_t MFRC552_ReadReg(uint8_t addr); //read from register
void MFRC552_SetBitMask(uint8_t reg, uint8_t mask); //set only certain bits
void MFRC552_ClearBitMask(uint8_t reg, uint8_t mask); //clear only certain bits
void MFRC552_AntennaOn(void); //turn on MFRC552 antenna
void MFRC552_AntennaOff(void); //turn off MFRC552 antenna
void MFRC552_Reset(void); //reset all registers
void MFRC552_Init(void); //initialize MFRC552
uint8_t MFRC552_FindCard(uint8_t reqMode, uint8_t *TagType); //search for card
uint8_t MFRC552_ToCard(uint8_t *sendData, uint8_t sendLen, uint8_t *backData);
uint32_t MFRC552_GetUID(uint8_t *serNum); //get UID from card

#endif

/*
    File: MFRC522.h
    MFRC522 - Library for Electronic lock with MFRC552 RFID readers
    Created by Eelco Rouw - Mainly based on code from Grant Gibson (www.grantgibson.co.uk) and Dr.Leong ( WWW.B2CQSHOP.COM )
    Ported to C by Stephen S. M.
    Ported to MSP432P401R and specialized for this project by Jake Park
    Released into the public domain
*/


#include "MFRC552.h"


/*
 * Function: WriteReg
 * Description: write a byte data into one register of MR RC522
 * Input parameter: addr--register address
 *                  val--the value that need to write in
 * Return: Null
 */

void MFRC552_WriteReg(uint8_t addr, uint8_t val)
{
    P3->OUT &= ~BIT3;
    SPI_transfer(addr<<1); //write to reg at addr
    SPI_transfer(val);
    P3->OUT |= BIT3;

}

/*
 * Function: ReadReg
 * Description: read a byte data into one register of MR RC522
 * Input parameter: addr--register address
 * Return: return the read value
 */
uint8_t MFRC552_ReadReg(uint8_t addr)
{
    uint8_t val;

    P3->OUT &= ~BIT3;
    SPI_transfer((addr<<1) | READMODE); //read from reg at addr
    val = SPI_transfer(0x00);
    P3->OUT |= BIT3;

    return val;
}


/*
 * Function: SetBitMask
 * Description: set RC522 register bit
 * Input parameter: reg--register address;mask--value
 * Return: null
 */
void MFRC552_SetBitMask(uint8_t reg, uint8_t mask)
{
    uint8_t tmp;
    tmp = MFRC552_ReadReg(reg);
    MFRC552_WriteReg(reg, tmp | mask);  // set bit mask
}


/*
 * Function: ClearBitMask
 * Description: clear RC522 register bit
 * Input parameter: reg--register address;mask--value
 * Return: null
 */
void MFRC552_ClearBitMask(uint8_t reg, uint8_t mask)
{
    uint8_t tmp;
    tmp = MFRC552_ReadReg(reg);
    MFRC552_WriteReg(reg, tmp & (~mask));  // clear bit mask
}


/*
 * Function: AntennaOn
 * Description: Turn on antenna, every time turn on or shut down antenna need at least 1ms delay
 * Input parameter: null
 * Return: null
 */
void MFRC552_AntennaOn(void)
{
    uint8_t temp;

    temp = MFRC552_ReadReg(TxControlReg);
    if (!(temp & 0x03))
    {
        MFRC552_SetBitMask(TxControlReg, 0x03);
    }
}


/*
 * Function: AntennaOff
 * Description: Turn off antenna, every time turn on or shut down antenna need at least 1ms delay
 * Input parameter: null
 * Return: null
 */
void MFRC552_AntennaOff(void)
{
    MFRC552_ClearBitMask(TxControlReg, 0x03);
}


/*
 * Function: Reset
 * Description:  reset RC522
 * Input parameter: null
 * Return: null
 */
void MFRC552_Reset(void)
{
    MFRC552_WriteReg(CommandReg, PCD_RESETPHASE);
}


/*
 * Function: Init
 * Description: initilize RC522
 * Input parameter: null
 * Return: null
 */
void MFRC552_Init(void)
{

    MFRC552_Reset();

    //Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
    MFRC552_WriteReg(TModeReg, 0x8D);        //Tauto=1; f(Timer) = 6.78MHz/TPreScaler
    MFRC552_WriteReg(TPrescalerReg, 0x3E);    //TModeReg[3..0] + TPrescalerReg
    MFRC552_WriteReg(TReloadRegL, 30);
    MFRC552_WriteReg(TReloadRegH, 0);

    MFRC552_WriteReg(TxAutoReg, 0x40);        //100%ASK
    MFRC552_WriteReg(ModeReg, 0x3D);

    //ClearBitMask(Status2Reg, 0x08);        //MFCrypto1On=0
    //WriteReg(RxSelReg, 0x86);        //RxWait = RxSelReg[5..0]
    //WriteReg(RFCfgReg, 0x7F);           //RxGain = 48dB

    MFRC552_AntennaOn();
}


/*
 * Function: Request
 * Description: Searching card, read card type
 * Input parameter: reqMode--search methods:
 *             TagType--return card types
 *                 0x4400 = Mifare_UltraLight
 *                0x0400 = Mifare_One(S50)
 *                0x0200 = Mifare_One(S70)
 *                0x0800 = Mifare_Pro(X)
 *                0x4403 = Mifare_DESFire
 * return: return MI_OK if successed
 */
uint8_t MFRC552_FindCard(uint8_t reqMode, uint8_t *TagType)
{
    uint8_t status;

    MFRC552_WriteReg(BitFramingReg, 0x07); //set to transmit all but one bit

    TagType[0] = reqMode;
    status = MFRC552_ToCard(TagType, 1, TagType); //search for card/get type

    return status;
}


/*
 * Function: ToCard
 * Description: communicate between RC522 and card
 * Input parameter:
 *             sendData--send data to card via rc522
 *             sendLen--send data length
 *             backData--the return data from card
 * return: return MI_OK if success
 */
uint8_t MFRC552_ToCard(uint8_t *sendData, uint8_t sendLen, uint8_t *backData)
{
    uint8_t status = MI_ERR;
    uint8_t RxIRq = 0x00;
    uint8_t n;
    uint8_t i;

    RxIRq = 0x20;

    MFRC552_WriteReg(CommIEnReg, RxIRq|0x80); //Enable Rx and timer interrupts
    MFRC552_ClearBitMask(CommIrqReg, 0x80); //Save interrupts
    MFRC552_SetBitMask(FIFOLevelReg, 0x80); //clear FIFO

    MFRC552_WriteReg(CommandReg, PCD_IDLE); //reader in IDLE mode

    for (i=0; i < sendLen; i++)
    {
        MFRC552_WriteReg(FIFODataReg, sendData[i]); //write sendData to FIFO
    }

    MFRC552_WriteReg(CommandReg, PCD_TRANSCEIVE); //reader in transceive mode
    MFRC552_SetBitMask(BitFramingReg, 0x80);      //start transmission


    do
    {
        n = MFRC552_ReadReg(CommIrqReg); //read irq flags
    }
    while (!(n&0x01)  && !(n&RxIRq)); //exit if timer or Rx done

    MFRC552_ClearBitMask(BitFramingReg, 0x80);     //Stop transmission

    if (!(n&0x01)) //if Rx finished before timer, status=OK
    {
        status = MI_OK;

        for (i=0; i<CARD_LENGTH; i++)
        {
            backData[i] = MFRC552_ReadReg(FIFODataReg); //read UID from FIFO
        }
    }
    else
    {
        status = MI_ERR; //defaulted to error
    }

    return status;
}


/*
 * Function: MFRC522_GetUID
 * Description: Get the ID from RFID card
 * Input parameter: serNum--return the 4 bytes card serial number
 * return: return UID
 */
uint32_t MFRC552_GetUID(uint8_t *serNum)
{
    uint8_t i=0;
    uint32_t UID;

    MFRC552_WriteReg(BitFramingReg, 0x00); //Set to transmit all last bits

    serNum[0] = PICC_ANTICOLL;
    serNum[1] = PICC_GETUID;
    //Code to transmit to receive UID from card

    MFRC552_ToCard(serNum, 2, serNum);

    while(i<CARD_LENGTH-1){
        UID = (UID<<8) | serNum[i];
        i++;
    }
    return UID;
}

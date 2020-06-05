#include "msp.h"
#include "P3.h"
#include "LCD.h"
#include "delay.h"
#include "MFRC552.h"
#include "users.h"

uint8_t currentState = ST_LOCKED;


void determine_RFID_state(void)
{
    uint8_t unlocked=0;
    uint32_t UID;
    uint8_t status=MI_ERR;
    uint8_t backData[MAX_LEN];
    switch (currentState)
    {
        case ST_LOCKED:
            status = MFRC552_FindCard(PICC_REQIDL, backData);
            if(status == MI_OK)
            {
                UID = MFRC552_GetUID(backData);
                unlocked = checkUID(UID);
                if (unlocked)
                {
                    P1->OUT &= ~BIT0;
                    P2->OUT |= BIT1;
                    clear_LCD();
                    home_LCD();
                    write_string_LCD("UNLOCKED");
                    currentState = ST_UNLOCKED;
                }
                else{
                    clear_LCD();
                    home_LCD();
                    write_string_LCD("ACCESS DENIED");
                    blinkRED(2);
                    clear_LCD();
                    home_LCD();
                    write_string_LCD("LOCKED");
                }
            }

            break;

        case ST_UNLOCKED:
            next_line_LCD();
            write_string_LCD("3..");
            delay_s(1);
            write_string_LCD("2..");
            delay_s(1);
            write_string_LCD("1..");
            delay_s(1);

            P1->OUT |= BIT0;
            P2->OUT &= ~BIT1;
            clear_LCD();
            home_LCD();
            write_string_LCD("LOCKED");
            currentState = ST_LOCKED;
            break;

        default:
            currentState = ST_LOCKED;
            break;
    }

}

uint8_t checkUID(uint32_t UID){
    int i;
    uint32_t users[] = USERS;
    uint32_t numUsers = NUMUSERS;
    uint8_t access = 0;

    for(i=0; i<numUsers; i++){
        if(users[i]==UID){
            access = 1;
        }
    }
    return access;
}

void blinkRED(uint8_t n){ //blink red LED at rate 3/sec for n seconds
    uint8_t i=0;
    for(i=0; i<n; i++){
        P1->OUT |= BIT0;
        delay_ms(167);
        P1->OUT &= ~BIT0;
        delay_ms(167);
        P1->OUT |= BIT0;
        delay_ms(167);
        P1->OUT &= ~BIT0;
        delay_ms(167);
        P1->OUT |= BIT0;
        delay_ms(167);
        P1->OUT &= ~BIT0;
        delay_ms(167);
    }
    P1->OUT |= BIT0;
}

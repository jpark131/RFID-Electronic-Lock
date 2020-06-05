#ifndef P3_H
#define P3_H


#define ST_LOCKED 0
#define ST_UNLOCKED 1

void determine_RFID_state(void);
uint8_t checkUID(uint32_t UID);
void blinkRED(uint8_t n);

#endif

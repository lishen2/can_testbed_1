#ifndef _MAIN_H_
#define _MAIN_H_

void SendCanMsg(uint32_t id, uint8_t exflag, const uint8_t *data, uint8_t len);
uint32_t TimerGetTick();

#endif

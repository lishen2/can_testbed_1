#include <stdint.h>
#include "debug.h"
#include "main.h"

/*  
Test case 
“11-32_430_NM_NormalStatus_0201_DUTAwakeFormertoSuccessorVirNode2  ”

1、等待设备启动
2、等待设备休眠发出beep，手工再次启动设备
3、持续和设备进行交互
*/

enum{
	STATE_WAIT_START,
	STATE_WAIT_SLEEP,
	STATE_WAIT_TEST,
	STATE_FINISH,
};

static uint32_t g_msgts;
static uint32_t g_state;

void TestInit(void)
{
	g_msgts = 0;
	g_state = STATE_WAIT_START;
}

void TestOnMessage(uint32_t id, const uint8_t *data, uint8_t len)
{
	g_msgts = TimerGetTick();
	return;
}

void TestPoll(void)
{
	if (STATE_WAIT_START == g_state){
		if (0 != g_msgts){
			g_state = STATE_WAIT_SLEEP;
			DbgPrintfD("Device started\r\n");
		}
	} else if (STATE_WAIT_SLEEP == g_state){
		if (TimerGetTick() - g_msgts > 1000){
			Beep(1000, 500);
			DbgPrintfD("Device sleeped\r\n");
			g_state = STATE_WAIT_TEST;
		}
	} else if (STATE_WAIT_TEST == g_state){
		if (TimerGetTick() - g_msgts > 5000) {
			Beep(2000, 500);
			DbgPrintfD("Time escaped\r\n");
			g_state = STATE_FINISH;
		}
	}
}



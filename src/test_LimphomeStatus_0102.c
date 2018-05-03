#include <stdint.h>
#include "debug.h"
#include "main.h"

/*  
Test case 
“0 NM_LimpHomeStatus_0102_LimpHomeResetBySendSISARingWhenKL15On: Failed ”

1、等待设备启动
2、等待设备休眠发出beep，手工再次启动设备
3、430消息发送opcode为04后，等待150ms后发送id为440，长度为8，数据为40 32 00 00 00 00 00 00的消息
*/

enum{
	STATE_WAIT_START,
	STATE_WAIT_SLEEP,
	STATE_WAIT_TEST,
	STATE_FINISH,
};

static uint32_t g_msgts;
static uint32_t g_msg430_04count;
static uint32_t g_msg430_04ts;

static uint32_t g_state;

void TestInit(void)
{
	g_msgts = 0;
	g_msg430_04count = 0;
	g_state = STATE_WAIT_START;
}

void TestOnMessage(uint32_t id, const uint8_t *data, uint8_t len)
{
	g_msgts = TimerGetTick();
	
	if (0x430 == id && 0x04 == data[1]) {
		g_msg430_04ts = TimerGetTick();
		g_msg430_04count += 1;
	}

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
			Beep(1000, 300);
			DbgPrintfD("Device sleeped\r\n");
			g_state = STATE_WAIT_TEST;
			g_msg430_04count = 0;
			g_msg430_04ts = 0;
		}
	} else if (STATE_WAIT_TEST == g_state){
		if (1 == g_msg430_04count && TimerGetTick() - g_msg430_04ts > 150){
			SendCanMsg(0x440, 0, "\x40\x32\x00\x00\x00\x00\x00\x00", 8);
			g_state = STATE_FINISH;
			DbgPrintfD("Test triggered\r\n");
		}
	}
}



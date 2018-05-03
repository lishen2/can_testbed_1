#include <stdint.h>
#include "debug.h"
#include "main.h"

/*  
Test case 
“24 NM_NormalStatus_0202_DUTSkippedVirNode1: Failed  ”

1、等待设备启动
2、等待设备休眠发出beep，手工再次启动设备
3、建环，休眠时发送测试消息。
*/

enum{
	STATE_WAIT_START,
	STATE_WAIT_SLEEP,
	STATE_WAIT_TEST,
	STATE_FINISH,
};

static uint32_t g_msgts;
static uint32_t g_state;
static uint32_t g_msg430ts;
static uint32_t g_msg430_12_count;
static uint8_t g_msg430buf[8];

void TestInit(void)
{
	g_msgts = 0;
	g_msg430ts = 0;
	g_msg430_12_count = 0;
	g_state = STATE_WAIT_START;
}

void TestOnMessage(uint32_t id, const uint8_t *data, uint8_t len)
{
	g_msgts = TimerGetTick();
	
	if (0x430 == id) {
		g_msg430ts = TimerGetTick();
		memcpy(g_msg430buf, data, len);
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
		}
	} else if (STATE_WAIT_TEST == g_state){
		if (0 != g_msg430ts) {
			if (0x30 == g_msg430buf[0] && 0x01 == g_msg430buf[1]) {
				SendCanMsg(0x440, 0, "\x40\x01\x00\x00\x00\x00\x00\x00", 8);
				g_msg430ts = 0;
			}
			else if (0x40 == g_msg430buf[0] && 0x02 == g_msg430buf[1] && TimerGetTick() - g_msg430ts > 100) {
				SendCanMsg(0x440, 0, "\x30\x02\x00\x00\x00\x00\x00\x00", 8);
				g_msg430ts = 0;
			}
			else if (0x40 == g_msg430buf[0] && 0x12 == g_msg430buf[1]) {
				if (g_msg430_12_count < 2) {
					SendCanMsg(0x440, 0, "\x30\x02\x00\x00\x00\x00\x00\x00", 8);
					g_msg430_12_count += 1;
				}
				else {
					SendCanMsg(0x440, 0, "\x40\x02\x00\x00\x00\x00\x00\x00", 8);
				}
				g_msg430ts = 0;
			}
		}

		if (2 == g_msg430_12_count) {
			Sleep(20);
			SendCanMsg(0x440, 0, "\x40\x02\x00\x00\x00\x00\x00\x00", 8);
			DbgPrintfD("Test triggered\r\n");
		}
	}
}



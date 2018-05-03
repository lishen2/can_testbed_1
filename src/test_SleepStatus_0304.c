#include <stdint.h>
#include "debug.h"
#include "main.h"

/*  
Test case 
“72 NM_SleepStatus_0304_AfterSILimpHomeSendAMInTwbs: Failed”

1、等待休眠后使用KL15唤醒设备（手工操作）
2、再次休眠前，430消息发送opcode为14后，等待5052.649838 - 5049.448525 = 3.201313秒后发送id为123，长度为8，数据全0的消息
*/

static uint32_t g_ts;

void TestInit(void)
{
	g_ts = 0;
}

void TestOnMessage(uint32_t id, const uint8_t *data, uint8_t len)
{
	//printf("ID: %08x, len: %hhu\r\n", frame->ID, frame->DataLen);

	if (0x430 == id && 0x14 == data[1]) {
		g_ts = TimerGetTick();
	}

	return;
}

void TestPoll(void)
{
	if (0 != g_ts && g_ts - TimerGetTick() > 3201) {
		SendCanMsg(0x123, 0, "\x00\x00\x00\x00\x00\x00\x00\x00", 8);
		g_ts = 0;
	}
}



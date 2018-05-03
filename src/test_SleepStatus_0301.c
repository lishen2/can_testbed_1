#include <stdint.h>
#include "debug.h"
#include "main.h"

/*  
Test case 
“65 NM_SleepStatus_0301_AfterSILimpHomeSendAliveInTwbs: Failed”

1、等待休眠后使用KL15唤醒设备（手工操作）
2、再次休眠前，430消息发送opcode为14后，等待4408.977448 - 4405.776337 = 3.201111秒后发送id为440，长度为8，数据为40 01 00 00 00 00 00 00的报文
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
	if (0 != g_ts && TimerGetTick() - g_ts > 3200) {
		SendCanMsg(0x440, 0, "\x40\x01\x00\x00\x00\x00\x00\x00", 8);
		g_ts = 0;
	}
}



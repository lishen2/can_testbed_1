#include <stdint.h>
#include "debug.h"
#include "main.h"

/*  
Test case 
��65 NM_SleepStatus_0301_AfterSILimpHomeSendAliveInTwbs: Failed��

1���ȴ����ߺ�ʹ��KL15�����豸���ֹ�������
2���ٴ�����ǰ��430��Ϣ����opcodeΪ14�󣬵ȴ�4408.977448 - 4405.776337 = 3.201111�����idΪ440������Ϊ8������Ϊ40 01 00 00 00 00 00 00�ı���
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



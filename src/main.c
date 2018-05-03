#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <Windows.h>
#define CHAR   signed char
#define BYTE   unsigned char
#define UCHAR  unsigned char
#define USHORT unsigned short
#define INT    signed int
#define UINT   unsigned int
#define DWORD  unsigned int
#define ULONG  unsigned long
#define PVOID  void *
#include "EcanVci.h"
#include "debug.h"

int g_devType = USBCAN2;
int g_devIndex = 0;
int g_devCanIdx = 0;

extern void TestOnMessage(uint32_t id, const uint8_t *data, uint8_t len);
extern void TestPoll(void);
extern void TestInit(void);
/*
void OnMessage(CAN_OBJ *frame)
{
	//printf("ID: %08x, len: %hhu\r\n", frame->ID, frame->DataLen);
	return;
}
*/

void SendCanMsg(uint32_t id, uint8_t exflag, const uint8_t *data, uint8_t len)
{
	int ret;
	CAN_OBJ frame;
	frame.SendType = 0;
	frame.DataLen = len;
	memcpy(frame.Data, data, len);
	frame.RemoteFlag = 0;

	frame.ExternFlag = exflag;
	frame.ID = id;

	ret = Transmit(g_devType, g_devIndex, g_devCanIdx, &frame, 1);
	if (1 != ret) {
		printf("Send failed, ID: %08x\r\n", frame.ID);
	}

	return;
}

uint32_t TimerGetTick()
{
	return GetTickCount();
}

int main(int argc, const char* argv[])
{
	INIT_CONFIG init_config;
	CAN_OBJ frameinfo[50];
	int len;
	int ret;
	int i;

	DbgInit();
	TestInit();

	ret = OpenDevice(g_devType, g_devIndex, 0);
	if (STATUS_OK != ret) {
		DbgPrintfE("Error open device\r\n");
		return -1;
	}

	init_config.AccCode = 0;
	init_config.AccMask = 0xffffff;
	init_config.Filter = 0;
	init_config.Timing0 = 0x00;
	init_config.Timing1 = 0x1c;
	init_config.Mode = 0;
	ret = InitCAN(g_devType, g_devIndex, g_devCanIdx, &init_config);
	if (STATUS_OK != ret) {
		DbgPrintfE("Error init device\r\n");
		return -1;
	}

	ret = StartCAN(g_devType, g_devIndex, g_devCanIdx);
	if (1 != ret) {
		DbgPrintfE("Error start can\r\n");
		return -1;
	}

	DbgPrintfV("CAN Started \r\n");

	//SendCanMsg(0x12345678, 1, (const uint8_t*)"\x11\x22\x33\x44\x55\x66\x77", 8);

	while(1) {
		TestPoll();
		len = Receive(g_devType, g_devIndex, g_devCanIdx, frameinfo, sizeof(frameinfo)/sizeof(frameinfo[0]), 10);
		if (len>0)
		{
			for (i = 0; i<len; i++)
			{
				TestOnMessage(frameinfo[i].ID, frameinfo[i].Data, frameinfo[i].DataLen);
			}
		}
	}

	return 0;
}


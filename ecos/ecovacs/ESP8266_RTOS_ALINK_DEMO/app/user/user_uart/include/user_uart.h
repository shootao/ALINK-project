/*
 * ESPRSSIF MIT License
 *
 * Copyright (c) 2015 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */ 

#ifndef __USER_UART_H__
#define __USER_UART_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "../../../include/user_config.h"


#define CUS_UART0_QUEUE_LENGTH (10)
#define CUS_UART_TX_MAX     (128+64)  // uart tx buf max len
extern int dbg_get_recv_times;

#if USER_UART_CTRL_DEV_EN

extern xQueueHandle xQueueCusUart;
extern unsigned char device_status_change;
extern VIRTUAL_DEV virtual_device;

typedef struct
{
	int rx_len;
	char rx_buf[CUS_UART_TX_MAX];
}CusUartIntrPtr;

/////////////////ecovacs///////////

#if 1

#define MCU2WIFI_DATA_START		(0xC3)
#define MCU2WIFI_DATA_END		(0xF3)

#define MCU2WIFI_S_InEASYLINK  	(0xD1)
#define MCU2WIFI_S_WIFIDISCONN  (0xD2)
#define MCU2WIFI_S_WIFICONNED   (0xD4)
#define MCU2WIFI_S_ALINKCONNED  (0xD5)
#define MCU2WIFI_CMD_RST		(0xCB)
#define MCU2WIFI_CMD_FAC_TEST	(0xCC)
#define MCU2WIFI_CMD_MAP		(0xCC)

#define APP_TRD_workmode	(0xE1)
#define APP_TRD_OnOff_Direction_Forward	(0xE2)
#define APP_TRD_OnOff_Direction_Backward	(0xE3)
#define APP_TRD_OnOff_Direction_Left	(0xE4)
#define APP_TRD_OnOff_Direction_Right         	(0xE5)
#define APP_TRD_DownAdjust	(0xE6)
#define APP_TRD_SideAdjust	(0xE7)
#define APP_TRD_CleaningSpeed	(0xE8)
#define APP_TRD_RoomMode	(0xE9)
#define APP_TRD_Stop_Cleaning	(0xEA)

enum
{
	WIFI2MCU_ROUTER_DISCONN = 0xd2,
	WIFI2MCU_ROUTER_CONN = 0xd4,
	WIFI2MCU_ALINK_CONN = 0xd5
};

typedef struct
{
	u8 app_start;
	u8 app_trd;
	u8 app_data;
	u8 app_addchk;
	u8 app_end;
}WIFI2MCU_DATA;


enum
{
	MCUD1_workmode_SUSPEND 		= 0x00,
	MCUD1_workmode_APPOINT		= 0x01,
	MCUD1_workmode_AUTO			= 0x02,
	MCUD1_workmode_RETURNCHARGE 	= 0x03,
	MCUD1_workmode_EDGE			= 0x04,
	MCUD1_workmode_PRECISECLEAN	= 0x05
};

enum
{
	MCUD2_OnOff_Direction_Forward_STOP = 0x00,
	MCUD2_OnOff_Direction_Forward_FORWARD = 0x01
};

enum
{
	MCUD3_OnOff_Direction_Backward_STOP = 0x00,
	MCUD3_OnOff_Direction_Backward_Backward = 0x01
};
enum
{
	MCUD4_OnOff_Direction_Lef_STOP = 0x00,
	MCUD4_OnOff_Direction_Lef_Lef = 0x01
};
enum
{
	MCUD5_OnOff_Direction_Right_STOP = 0x00,
	MCUD5_OnOff_Direction_Right_Right = 0x01
};

enum
{
	MCUD6_DownAdjust_LONG = 0x00,
	MCUD6_DownAdjust_NEAR = 0x01
};
enum
{
	MCUD7_SideAdjust_LONG = 0x00,
	MCUD7_SideAdjust_NEAR = 0x01
};
enum
{
	MCUD8_CleaningSpeed_NORMAL = 0x00,
	MCUD8_CleaningSpeed_FORCE = 0x01,
	MCUD8_CleaningSpeed_CARPET = 0x02,
	MCUD8_CleaningSpeed_LOUDSPEAKER = 0x03		
};


enum
{
	MCUD9_RoomMode_BIG = 0x00,
	MCUD9_RoomMode_SMALL = 0x00,
	MCUD9_RoomMode_SECONDARY = 0x00
};

// MCUD10_BatteryCapacity  0x00 ~ 0x64

enum
{
	MCUD11_ChargerStatus_STOP = 0x00,
	MCUD11_ChargerStatus_CHARGING = 0x01
};

// MCUD12_ErrorCode  0x00 ~ 0x6

enum
{
	MCUD13_Stop_Cleaning_WORKING = 0x00,		
	MCUD13_Stop_Cleaning_STOP = 0x01
};

typedef struct
{
	u8 McuStart;
	u8 McuD1_workmode;
	u8 McuD2_OnOff_Direction_Forward;
	u8 McuD3_OnOff_Direction_Backward;
	u8 McuD4_OnOff_Direction_Lef;
	u8 McuD5_OnOff_Direction_Right;
	u8 McuD6_DownAdjust;
	u8 McuD7_SideAdjust;
	u8 McuD8_CleaningSpeed;
	u8 McuD9_RoomMode;
	u8 McuD10_BatteryCapacity;
	u8 McuD11_ChargerStatus;
	u8 McuD12_ErrorCode;
	u8 McuD13_Stop_Cleaning;
	u8 McuAddCHK;
	u8 McuEnd;
}MCU2WIFI_DATA;

#endif

extern MCU2WIFI_DATA mcu2wifi_data;

//////////////////
void debug_print_hex_data(char*buf, int len);
void ICACHE_FLASH_ATTR cus_wifi_handler_alinkdata2mcu(u8 dat_index, int dat_value);
void ICACHE_FLASH_ATTR user_uart_task(void *pvParameters);
void ICACHE_FLASH_ATTR user_uart_dev_start(void);
void ICACHE_FLASH_ATTR user_key_short_press(void);
int ICACHE_FLASH_ATTR uart0_write_data(u8 *data, int len);
void ICACHE_FLASH_ATTR wifi2mcu_sys_data(u8 data);

#endif

#endif


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

#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__
#define RESTORE_KEEP_TIMER 0

#define USE_OPTIMIZE_PRINTF
extern  int need_factory_reset ;

#define LFILE_START_ADDR  0x1f8000  // flash size is 0x200000
#define LFILE_SIZE 4096

#define USER_UART_CTRL_DEV_EN 	 (1)   // support allink uart smart device
#define USER_PWM_LIGHT_EN        (0)   // support alink pwm light  
#define USER_VIRTUAL_DEV_TEST	 (1)

#define USER_ALINK_GLOBAL_VER "A[Alink1.0|194b173-dirty]M[ESP8266]OS[1.3.0(68c9e7b]"

//victor

#define CONFIGNETWORK 1
#define UARTSHAKEPROTOCL 1
#define HEARTBEATTIMER 1
//#define HANKANG_DEBUG 1
#define QUEUE_FUNC 1




//#define TESTHAVEAPLAN 1

//#define SLOPE_ENABLE	

//extern int smart_config_status;

extern int haveaplan;
extern int haveareport;
extern int uartsendsuccessfully;
extern int convertdatatypisok;
extern int runningmachineisbusy;
extern u32 currentAUid;
extern char* value_str_DetailPlans;
//extern char* value_str_Auid;
extern char value_byte_DetailPlans[36];

extern int manufacturetestmode;

extern int cfgnetsta;
extern int confignetworkkey;
extern int getssidpasswdflag;

extern int shakeok;
extern int shakeok1;
extern int shakeok2;

//extern char hankang_uart_shakehand_data[];  // test uart beat data "12345678"
//extern char hankang_uart_shakehand_echodata[];  //

/*victor del
//--------------------------


typedef struct virtual_dev {   // real device update this Structure parameters
	u32 power;
	u32 temp_value;
	u32 light_value;
	u32 time_delay;
	u32 work_mode;
}VIRTUAL_DEV;// virtual_device = {
//0x01, 0x30, 0x50, 0, 0x01};
*/
//--------------------------v
/*
"{
\"Auid\": { \"value\": \"%d\" }, 
\"DetailPlans\": { \"value\": \"%d\" }, 
\"Distance\": { \"value\": \"%d\" }, 
\"End_Time\": { \"value\": \"%d\"}, 
\"Heat\": { \"value\": \"%d\"}, 
\"Light_Brightness\": { \"value\": \"%d\"}, 
\"Pace\": { \"value\": \"%d\"}, 
\"Period\": { \"value\": \"%d\"}, 
\"Period_Duration\": { \"value\": \"%d\"}, 
\"Period_Gradient\": { \"value\": \"%d\"}, 
\"Period_Speed\": { \"value\": \"%d\"}, 
\"Plan_Message\": { \"value\": \"%d\"}, 
\"Start_Time\": { \"value\": \"%d\"}, 
\"Step\": { \"value\": \"%d\"}, 
\"tmp_attr\": { \"value\": \"%d\"}}";
*/

typedef struct virtual_dev {   // real device update this Structure parameters
	u32 Auid;
	char DetailPlans[36];
	u32 Distance;
	u32 End_Time;
	u32 Heat;
	u32 Light_Brightness;
	u32 Pace;
	u32 Period;
	u32 Period_Duration;
	u32 Period_Gradient;
	u32 Period_Speed;
	u32 Plan_Message;
	u32 Start_Time;
	u32 Step;
	u32 tmp_attr;

}VIRTUAL_DEV;// virtual_device = {
//0x01, 0x30, 0x50, 0, 0x01};
extern VIRTUAL_DEV virtual_device;


typedef struct
{
	unsigned char own_flag;
	uint32 Distance;
	uint32 Heat;
	uint32 time_s;
	char steps[2];
}USER_RUN_DATA;

extern USER_RUN_DATA user_running_data;


#endif


/*
* Copyright (c) 2014-2015 Alibaba Group. All rights reserved.
*
* Alibaba Group retains all right, title and interest (including all
* intellectual property rights) in and to this computer program, which is
* protected by applicable intellectual property laws.  Unless you have
* obtained a separate written license from Alibaba Group., you are not
* authorized to utilize all or a part of this computer program for any
* purpose (including reproduction, distribution, modification, and
* compilation into object code), and you must immediately destroy or
* return to Alibaba Group all copies of this computer program.  If you
* are licensed by Alibaba Group, your rights to utilize this computer
* program are limited by the terms of that license.  To obtain a license,
* please contact Alibaba Group.
*
* This computer program contains trade secrets owned by Alibaba Group.
* and, unless unauthorized by Alibaba Group in writing, you agree to
* maintain the confidentiality of this computer program and related
* information and to not disclose this computer program and related
* information to any other person or entity.
*
* THIS COMPUTER PROGRAM IS PROVIDED AS IS WITHOUT ANY WARRANTIES, AND
* Alibaba Group EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED,
* INCLUDING THE WARRANTIES OF MERCHANTIBILITY, FITNESS FOR A PARTICULAR
* PURPOSE, TITLE, AND NONINFRINGEMENT.
*/
#include "c_types.h"
#include "alink_export.h"
#include "alink_json.h"
#include <stdio.h>
#include <string.h>
#include "alink_export_rawdata.h"
#include "esp_common.h"
#include "user_config.h"
#include <ctype.h>



#include "freertos/semphr.h"

#include <string.h>

#if USER_UART_CTRL_DEV_EN
#include "user_uart.h" // user uart handler head
#endif
#if USER_PWM_LIGHT_EN
#include "user_light.h"  // user pwm light head
#endif

#include "../../../include/driver/user_time.h"

//#define ALINK_TEST_NORMAL   
#define wsf_deb  os_printf
#define wsf_err os_printf
//#define PASS_THROUGH 
//#define SUB_DEV_ENABLE //◊”…Ë±∏π¶ƒ‹sample Œ¥ µœ÷
/* …Ë±∏–≈œ¢£∫∏˘æ›Õ¯“≥◊¢≤·–≈œ¢µº≥ˆµƒµÁ◊”±Ì∏Ò∏¸–¬∂‘”¶–≈œ¢ */
/* device info */
#if 0
#define DEV_NAME "A4"//"ALINKTEST"
#define DEV_CATEGORY "HEALTH"//"LIVING"
#define DEV_TYPE "RUNNINGMACHINE"//"LIGHT"
#ifdef PASS_THROUGH
#define DEV_MODEL "ALINKTEST_LIVING_LIGHT_SMARTLED_LUA"
#define ALINK_KEY "bIjq3G1NcgjSfF9uSeK2"
#define ALINK_SECRET "W6tXrtzgQHGZqksvJLMdCPArmkecBAdcr2F5tjuF"
#else
#define DEV_MODEL "DINGKANG_HEALTH_RUNNINGMACHINE_A4"//"ALINKTEST_LIVING_LIGHT_SMARTLED"
#define ALINK_KEY "KEdNWM32xKMbhtZX0fJK"//"ljB6vqoLzmP8fGkE6pon"
#define ALINK_SECRET "3zHS0q4hctA0i6jWvX04B1imA7nuzK8judd47O8q"//"YJJZjytOCXDhtQqip4EjWbhR95zTgI92RVjzjyZF"
#endif
#define DEV_MANUFACTURE "Dingkang"//"ALINKTEST"
/*sandbox key/secret*/
#define ALINK_KEY_SANDBOX "KEdNWM32xKMbhtZX0fJK"
#define ALINK_SECRET_SANDBOX "3zHS0q4hctA0i6jWvX04B1imA7nuzK8judd47O8q"
/*…Ë±∏”≤º˛–≈œ¢:œµÕ≥…œµÁ∫Û∂¡»°µƒ”≤º˛/πÃº˛–≈œ¢,¥À¥¶Œ™—› æ–Ë“™,÷±Ω”∂®“ÂŒ™∫Í.≤˙∆∑∂‘Ω” ±,–Ë“™µ˜”√◊‘…ÌΩ”ø⁄ªÒ»°*/
#define DEV_SN "1234567890"
#define DEV_VERSION "1.0.0"


#define DEV_MAC "19:FE:34:A2:C7:1A"	//"AA:CC:CC:CA:CA:01" // need get from device
#define DEV_CHIPID "3D0044000F47333139373030"	// need get from device
/*alink-sdk –≈œ¢ */

///////////////////////////////////////////////////

#define DEV_NAME "test2"
#define DEV_CATEGORY "HEALTH"
#define DEV_TYPE "RUNNINGMACHINE"
#ifdef PASS_THROUGH
#define DEV_MODEL "ALINKTEST_LIVING_LIGHT_SMARTLED_LUA"
#define ALINK_KEY "bIjq3G1NcgjSfF9uSeK2"
#define ALINK_SECRET "W6tXrtzgQHGZqksvJLMdCPArmkecBAdcr2F5tjuF"
#else


#ifdef ALINK_TEST_NORMAL
#define DEV_MODEL "ALINKTEST_LIVING_LIGHT_SMARTLED"
#define ALINK_KEY "ljB6vqoLzmP8fGkE6pon"
#define ALINK_SECRET "YJJZjytOCXDhtQqip4EjWbhR95zTgI92RVjzjyZF"

////////////////////////
#else
#define DEV_MODEL "ESPRESSIF_HEALTH_RUNNINGMACHINE_TEST"
#define ALINK_KEY "PQIRTgrldKMugoslRA2f"
#define ALINK_SECRET "PIKSXySpe07tKhJ6o0WrJevCvYBbYnzrd8wEXys0"
#endif
//////////////////////

#endif

#ifdef ALINK_TEST_NORMAL   

#define DEV_MANUFACTURE "ALINKTEST"
/*sandbox key/secret*/
#define ALINK_KEY_SANDBOX "dpZZEpm9eBfqzK7yVeLq"
#define ALINK_SECRET_SANDBOX "THnfRRsU5vu6g6m9X6uFyAjUWflgZ0iyGjdEneKm"

#else
/////////////////////////////
#define DEV_MANUFACTURE "RUNNINGMACHINE"
/*sandbox key/secret*/
#define ALINK_KEY_SANDBOX "10n0NikaekN0IZ0fAcD0 "
#define ALINK_SECRET_SANDBOX "19n2KDP5TYW7nKY5Bw9ArE4VBzU4IO6RpuiIFQdA "
#endif

#endif

//from xutao
#if 0
#define DEV_NAME "test"
#define DEV_CATEGORY "HEALTH"
#define DEV_TYPE "RUNNINGMACHINE"
#ifdef PASS_THROUGH
#define DEV_MODEL "ALINKTEST_LIVING_LIGHT_SMARTLED_LUA"
#define ALINK_KEY "bIjq3G1NcgjSfF9uSeK2"
#define ALINK_SECRET "W6tXrtzgQHGZqksvJLMdCPArmkecBAdcr2F5tjuF"
#else
#ifdef ALINK_TEST_NORMAL
#define DEV_MODEL "ALINKTEST_LIVING_LIGHT_SMARTLED"
#define ALINK_KEY "ljB6vqoLzmP8fGkE6pon"
#define ALINK_SECRET "YJJZjytOCXDhtQqip4EjWbhR95zTgI92RVjzjyZF"
////////////////////////
#else
#define DEV_MODEL "ESPRESSIF_HEALTH_RUNNINGMACHINE_TEST"
#define ALINK_KEY "a2thrVwPqoN7cCGw3UoK"
#define ALINK_SECRET "EhySq0C4kyuSbey6eB7wYwJcubYZahX2F5fACFjf"
#endif
//////////////////////
#endif
#ifdef ALINK_TEST_NORMAL   
#define DEV_MANUFACTURE "Espressif"/*sandbox key/secret*/
#define ALINK_KEY_SANDBOX "dpZZEpm9eBfqzK7yVeLq"
#define ALINK_SECRET_SANDBOX "THnfRRsU5vu6g6m9X6uFyAjUWflgZ0iyGjdEneKm"
#else
/////////////////////////////
#define DEV_MANUFACTURE "Espressif"/*sandbox key/secret*/
#define ALINK_KEY_SANDBOX "10n0NikaekN0IZ0fAcD0"
#define ALINK_SECRET_SANDBOX "19n2KDP5TYW7nKY5Bw9ArE4VBzU4IO6RpuiIFQdA"
#endif
#endif

#if 1
#define DEV_NAME "æ´¡ÈELF"
#define DEV_CATEGORY "HEALTH"
#define DEV_TYPE "RUNNINGMACHINE"
#ifdef PASS_THROUGH
#define DEV_MODEL "ALINKTEST_LIVING_LIGHT_SMARTLED_LUA"
#define ALINK_KEY "bIjq3G1NcgjSfF9uSeK2"
#define ALINK_SECRET "W6tXrtzgQHGZqksvJLMdCPArmkecBAdcr2F5tjuF"
#else


#ifdef ALINK_TEST_NORMAL
#define DEV_MODEL "ALINKTEST_LIVING_LIGHT_SMARTLED"
#define ALINK_KEY "ljB6vqoLzmP8fGkE6pon"
#define ALINK_SECRET "YJJZjytOCXDhtQqip4EjWbhR95zTgI92RVjzjyZF"

////////////////////////
#else
#define DEV_MODEL "DINGKANG_HEALTH_RUNNINGMACHINE_ELF"
#define ALINK_KEY "NhdmbV0I5Rq7YqjtIhth"
#define ALINK_SECRET "IPwETGEJ2kTTUV3pLOm6JtexPIzydIRXuw1hgFyv"
#endif
//////////////////////

#endif

#ifdef ALINK_TEST_NORMAL   

#define DEV_MANUFACTURE "Espressif"
/*sandbox key/secret*/
#define ALINK_KEY_SANDBOX "dpZZEpm9eBfqzK7yVeLq"
#define ALINK_SECRET_SANDBOX "THnfRRsU5vu6g6m9X6uFyAjUWflgZ0iyGjdEneKm"

#else
/////////////////////////////
#define DEV_MANUFACTURE "Dingkang"
/*sandbox key/secret*/
#define ALINK_KEY_SANDBOX "KEdNWM32xKMbhtZX0fJK"
#define ALINK_SECRET_SANDBOX "3zHS0q4hctA0i6jWvX04B1imA7nuzK8judd47O8q"
#endif
#endif


#if 0//20170217
#define DEV_NAME "test"
#define DEV_CATEGORY "HEALTH"
#define DEV_TYPE "RUNNINGMACHINE"
#ifdef PASS_THROUGH
#define DEV_MODEL "ALINKTEST_LIVING_LIGHT_SMARTLED_LUA"
#define ALINK_KEY "bIjq3G1NcgjSfF9uSeK2"
#define ALINK_SECRET "W6tXrtzgQHGZqksvJLMdCPArmkecBAdcr2F5tjuF"
#else


#ifdef ALINK_TEST_NORMAL
#define DEV_MODEL "ALINKTEST_LIVING_LIGHT_SMARTLED"
#define ALINK_KEY "ljB6vqoLzmP8fGkE6pon"
#define ALINK_SECRET "YJJZjytOCXDhtQqip4EjWbhR95zTgI92RVjzjyZF"

////////////////////////
#else
#define DEV_MODEL "ESPRESSIF_HEALTH_RUNNINGMACHINE_TEST"
#define ALINK_KEY "a2thrVwPqoN7cCGw3UoK"
#define ALINK_SECRET "EhySq0C4kyuSbey6eB7wYwJcubYZahX2F5fACFjf"
#endif
//////////////////////

#endif

#ifdef ALINK_TEST_NORMAL   

#define DEV_MANUFACTURE "Espressif"
/*sandbox key/secret*/
#define ALINK_KEY_SANDBOX "10n0NikaekN0IZ0fAcD0"
#define ALINK_SECRET_SANDBOX "19n2KDP5TYW7nKY5Bw9ArE4VBzU4IO6RpuiIFQdA"

#else
/////////////////////////////
#define DEV_MANUFACTURE "Espressif"
/*sandbox key/secret*/
#define ALINK_KEY_SANDBOX "10n0NikaekN0IZ0fAcD0 "
#define ALINK_SECRET_SANDBOX "19n2KDP5TYW7nKY5Bw9ArE4VBzU4IO6RpuiIFQdA"
#endif

#endif
//////////////////////////////////
/*ËÆæÂ§áÁ°¨‰ª∂‰ø°ÊÅØ:Á≥ªÁªü‰∏äÁîµÂêéËØªÂèñÁöÑÁ°¨‰ª∂/Âõ∫‰ª∂‰ø°ÊÅØ,Ê≠§Â§Ñ‰∏∫ÊºîÁ§∫ÈúÄË¶Å,Áõ¥Êé•ÂÆö‰πâ‰∏∫ÂÆè.‰∫ßÂìÅÂØπÊé•Êó∂,ÈúÄË¶ÅË∞ÉÁî®Ëá™Ë∫´Êé•Âè£Ëé∑Âèñ*/

#if 0
#define DEV_NAME "æ´¡ÈELF"
#define DEV_CATEGORY "HEALTH"
#define DEV_TYPE "RUNNINGMACHINE"
#ifdef PASS_THROUGH
#define DEV_MODEL "ALINKTEST_LIVING_LIGHT_SMARTLED_LUA"
#define ALINK_KEY "bIjq3G1NcgjSfF9uSeK2"
#define ALINK_SECRET "W6tXrtzgQHGZqksvJLMdCPArmkecBAdcr2F5tjuF"
#else


	#ifdef ALINK_TEST_NORMAL
	#define DEV_MODEL "ALINKTEST_LIVING_LIGHT_SMARTLED"
	#define ALINK_KEY "ljB6vqoLzmP8fGkE6pon"
	#define ALINK_SECRET "YJJZjytOCXDhtQqip4EjWbhR95zTgI92RVjzjyZF"

////////////////////////
	#else
	#define DEV_MODEL "DINGKANG_HEALTH_RUNNINGMACHINE_A4"
	#define ALINK_KEY "t8oucddLNjqrzcNG4fsJ"
	#define ALINK_SECRET "5kJYBqSGfsd9Mlsn0deDizJUOsZr4Eg6gN9MOlmj"
	#endif
//////////////////////

#endif

#ifdef ALINK_TEST_NORMAL   

#define DEV_MANUFACTURE "Espressif"
/*sandbox key/secret*/
#define ALINK_KEY_SANDBOX "10n0NikaekN0IZ0fAcD0"
#define ALINK_SECRET_SANDBOX "19n2KDP5TYW7nKY5Bw9ArE4VBzU4IO6RpuiIFQdA"

#else
/////////////////////////////
#define DEV_MANUFACTURE "Dingkang"
/*sandbox key/secret*/
#define ALINK_KEY_SANDBOX "KEdNWM32xKMbhtZX0fJK "
#define ALINK_SECRET_SANDBOX "3zHS0q4hctA0i6jWvX04B1imA7nuzK8judd47O8q "
#endif
#endif


//////////////////////////////////
/*ËÆæÂ§áÁ°¨‰ª∂‰ø°ÊÅØ:Á≥ªÁªü‰∏äÁîµÂêéËØªÂèñÁöÑÁ°¨‰ª∂/Âõ∫‰ª∂‰ø°ÊÅØ,Ê≠§Â§Ñ‰∏∫ÊºîÁ§∫ÈúÄË¶Å,Áõ¥Êé•ÂÆö‰πâ‰∏∫ÂÆè.‰∫ßÂìÅÂØπÊé•Êó∂,ÈúÄË¶ÅË∞ÉÁî®Ëá™Ë∫´Êé•Âè£Ëé∑Âèñ*/
#define DEV_SN "1234567890"
#define DEV_VERSION "1.0.0"


#define DEV_MAC "19:FE:34:A2:C7:1A"	//"AA:CC:CC:CA:CA:01" // need get from device
#define DEV_CHIPID "3D0044000F47333139373030"	// need get from device


extern void alink_sleep(int);
/*do your job here*/
/*’‚¿Ô «“ª∏ˆ–Èƒ‚µƒ…Ë±∏,Ω´5∏ˆ…Ë±∏ Ù–‘∂‘”¶µƒ÷µ±£¥ÊµΩ»´æ÷±‰¡ø,’Ê µµƒ…Ë±∏–Ë“™»•∞¥’’ µº “µŒÒ¥¶¿Ì’‚–© Ù–‘÷µ */


#define _wordlength 1//byte number
#define _Auid 0
#define _DetailPlans 1
#define _Distance 2
#define _End_Time 3
#define _Heat 4
#define _Light_Brightness 5
#define _Pace 6
#define _Period 7
#define _Period_Duration 8
#define _Period_Gradient 9
#define _Period_Speed 10
#define _Plan_Message 11
#define _Start_Time 12
#define _Step 13
#define _tmp_attr 14

//v
int haveaplan=0;
int haveareport=0;
int uartsendsuccessfully=0;
int convertdatatypisok=0;
int runningmachineisbusy=0;
u32 currentAUid=0;
//char *current_str_Auid="500001155061670974----";
char current_str_Auid[24];

USER_RUN_DATA user_running_data;

int cfgnetsta;
int confignetworkkey;
//---------------------------

char *value_str_DetailPlans=NULL;
//char *value_str_Auid=NULL;

VIRTUAL_DEV virtual_device;// = {0x01, 0x30, 0x50, 0, 0x01};

//const char *main_dev_params =
  //  "{\"attrSet\": [ \"OnOff_Power\", \"Color_Temperature\", \"Light_Brightness\", \"TimeDelay_PowerOff\", \"WorkMode_MasterLight\"], \"OnOff_Power\": { \"value\": \"%d\" }, \"Color_Temperature\": { \"value\": \"%d\" }, \"Light_Brightness\": { \"value\": \"%d\" }, \"TimeDelay_PowerOff\": { \"value\": \"%d\"}, \"WorkMode_MasterLight\": { \"value\": \"%d\"}}";
/*
char *device_attr[5] = { "OnOff_Power", "Color_Temperature", "Light_Brightness",
	"TimeDelay_PowerOff", "WorkMode_MasterLight"
};   // this is a demo json package data, real device need to update this package
*/
char *device_attr[15] = { "Auid", "DetailPlans", "Distance","End_Time", "Heat", "Light_Brightness", "Pace", "Period", "Period_Duration", "Period_Gradient", "Period_Speed", "Plan_Message", "Start_Time", "Step", "tmp_attr"};  // this is a demo json package data, real device need to update this package
//-----------------------------------------------------------------------V
/*
const char *main_dev_params =
    "{\"OnOff_Power\": { \"value\": \"%d\" }, \"Color_Temperature\": { \"value\": \"%d\" }, \"Light_Brightness\": { \"value\": \"%d\" }
    , \"TimeDelay_PowerOff\": { \"value\": \"%d\"}, \"WorkMode_MasterLight\": { \"value\": \"%d\"}}";
*/


void HexStrToByte(const char* source, unsigned char* dest, int sourceLen)  
{  
    short i;  
    unsigned char highByte, lowByte;  
      
    for (i = 0; i < sourceLen; i += 2)  
    {  
        highByte = toupper(source[i]);  
        lowByte  = toupper(source[i + 1]);  
  
        if (highByte > 0x39)  
            highByte -= 0x37;  
        else  
            highByte -= 0x30;  
  
        if (lowByte > 0x39)  
            lowByte -= 0x37;  
        else  
            lowByte -= 0x30;  
  
        dest[i / 2] = (highByte << 4) | lowByte;  
    }  
    return ;  
}  

const char *main_dev_params ="{\"Auid\": { \"value\": \"%d\" }, \"DetailPlans\": { \"value\": \"%d\" }, \"Distance\": { \"value\": \"%d\" }, \"End_Time\": { \"value\": \"%d\"}, \"Heat\": { \"value\": \"%d\"}, \"Light_Brightness\": { \"value\": \"%d\"}, \"Pace\": { \"value\": \"%d\"}, \"Period\": { \"value\": \"%d\"}, \"Period_Duration\": { \"value\": \"%d\"}, \"Period_Gradient\": { \"value\": \"%d\"}, \"Period_Speed\": { \"value\": \"%d\"}, \"Plan_Message\": { \"value\": \"%d\"}, \"Start_Time\": { \"value\": \"%d\"}, \"Step\": { \"value\": \"%d\"}, \"tmp_attr\": { \"value\": \"%d\"}}";


//-----------------------------------------------------------------------V
unsigned char device_status_change = 1;
/*…Ë±∏…œ±® ˝æ›,–Ë“™øÕªß∏˘æ›æﬂÃÂ“µŒÒ»• µœ÷*/


/**
*    ”√¿¥ªÒ»°“™±ª∑÷∏Ó◊÷∑˚¥Æ÷–Ãÿ∂®∑÷∏Ù∑˚µƒ∏ˆ ˝£∫∑÷∏Ù∑˚ ˝+1=ø…“‘ªÆ∑÷µƒ◊÷∑˚ ˝◊È ˝
*   @para    char *str    –Ë“™±ª∑÷∏Óµƒ‘¥◊÷∑˚¥Æ
*   @para    const char *del   –Ë“™÷∏∂®µƒ∑÷∏Ó∑˚
*/
int strsplinum(char *str, const char*del)   //≈–∂œ‘¥◊÷∑˚¥Æ÷–”–∂‡…Ÿ∏ˆ÷∏∂®µƒ∑÷∏Ù∑˚£¨æ›¥Àø…“‘÷™µ¿ø…“‘ªÆ∑÷≥ˆ∂‡…Ÿ∏ˆ◊÷∑˚¥Æ∑÷◊È
{
     char *first = NULL;
     char *second = NULL;
     int num = 0;
     first = strstr(str,del);
     while(first != NULL)
     {
           second = first+1;
           num++;
           first = strstr(second,del);
      }
     return num;
}
 
/**
*   ◊÷∑˚∑÷∏Ó∫Ø ˝
*    @para    char **arr    ∑÷∏Ó∫√µƒ◊÷∑˚¥ÆÕ®≥£ «∂˛Œ¨◊÷∑˚ ˝◊È£¨”√∂˛Œ¨÷∏’Î¿¥ªÒ»°
*    @para    char *str      “™±ª∑÷∏Óµƒ◊÷∑˚¥Æ
*    @para    const char *del    Ãÿ∂®µƒ∑÷∏Ó∑˚
*/
void split( char **arr, char *str, const char *del)
{
      char *s =NULL;
      s=strtok(str,del);
      while(s != NULL)
     {
          *arr++ = s;
          s = strtok(NULL,del);     
      }
}



void debug_printf_data(char *data, int len)
{
	int i = 0;
	os_printf("\n*******print len[%d]******\n",len);
	for(i=0; i<len; i++)
	{
		os_printf("%x ",*(data+i));
	}
	os_printf("\n__________end_________\n");
	return;
}


#if 0
static int user_get_post_data_normal(char *buffer)
{
	int ret;
//	int when_t;
//	when_t = alink_get_time();
	ret = 0;

	sprintf(buffer, "{\"Auid\":{ \"value\": \"%s\"},","500001155061670974");
	ret = strlen(buffer);

	sprintf(buffer+ret, "\"Distance\": { \"value\": \"%s\"},","5.3");
	ret = strlen(buffer);
	sprintf(buffer+ret, "\"End_Time\": { \"value\": \"%s\"},","2017-01-20 21:30:30");
	ret = strlen(buffer);
	sprintf(buffer+ret, "\"Heat\": { \"value\": \"%d\"},",150);
	ret = strlen(buffer);
	
	sprintf(buffer+ret, "\"Period\": { \"value\": [{\"Period_Speed\": \"10.0\",\"Period_Gradient\": 0,\"Period_Duration\": 180},{\"Period_Speed\": \"10.0\",\"Period_Gradient\": 0,\"Period_Duration\": 180},{\"Period_Speed\": \"10.0\",\"Period_Gradient\": 0,\"Period_Duration\": 180},{\"Period_Speed\": \"10.0\",\"Period_Gradient\": 0,\"Period_Duration\": %d} ]},",118);
	ret = strlen(buffer);

	sprintf(buffer+ret, "\"Pace\": { \"value\": %d},",1);
	ret = strlen(buffer);

	sprintf(buffer+ret, "\"Step\": { \"value\": \"%d\"},",1000);
	ret = strlen(buffer);
	sprintf(buffer+ret, "\"Start_Time\": { \"value\": \"%s\"}}","2017-01-20 20:30:30");
	ret = strlen(buffer);
	

	os_printf("status len:%d",ret);

	return ret;
}

#else
static int user_get_post_data_normal(char *buffer)
{

int ret;

	unsigned int when_t;
	when_t = alink_get_time();

	//when_t_start=
	struct esp_tm *alm_tm,*alm_tm1;
	char time_buf[60];
	alm_tm = malloc(sizeof(struct esp_tm));
	alm_tm1 = malloc(sizeof(struct esp_tm));
	memset(alm_tm, 0, sizeof(struct esp_tm));
	memset(alm_tm1, 0, sizeof(struct esp_tm));
	memset(time_buf, 0, 60);
	when_t = when_t + (8*60*60);
	Sec_to_real_time(alm_tm,when_t);
	Sec_to_real_time(alm_tm1,when_t-user_running_data.time_s);
	user_running_data.time_s=0;
	sprintf(time_buf,"%04d-%02d-%02d %02d:%02d:%02d",alm_tm->tm_year,alm_tm->tm_mon,alm_tm->tm_mday,alm_tm->tm_hour,alm_tm->tm_min,alm_tm->tm_sec);
	printf("endtime:%04d-%02d-%02d %02d:%02d:%02d\n",alm_tm->tm_year,alm_tm->tm_mon,alm_tm->tm_mday,alm_tm->tm_hour,alm_tm->tm_min,alm_tm->tm_sec);
	printf("starttime:%04d-%02d-%02d %02d:%02d:%02d\n",alm_tm1->tm_year,alm_tm1->tm_mon,alm_tm1->tm_mday,alm_tm1->tm_hour,alm_tm1->tm_min,alm_tm1->tm_sec);

	ESP_DBG(("*******TIME[%s]***********\n",time_buf));


//	free(alm_tm);
//	free(alm_tm1);


#if 1

	ret = 0;
	if(user_running_data.own_flag ==1){	
		//sprintf(buffer, "{\"Auid\":{ \"value\": \"%s\"},","500001155061670974");currentAUid	
		if(current_str_Auid!=NULL){
		sprintf(buffer, "{\"Auid\":{ \"value\": \"%s\"},",current_str_Auid);
		printf("current_str_Auid=%s\n",current_str_Auid);}
		printf("ownflag=1\n");
	}else{
		sprintf(buffer, "{\"Auid\":{ \"value\": \"%s\"},","0");
		printf("ownflag=0\n");
	}
	ret = strlen(buffer);
	
	sprintf(buffer+ret, "\"Distance\": { \"value\": \"%d\"},",user_running_data.Distance);
	ret = strlen(buffer);
	sprintf(buffer+ret, "\"End_Time\": { \"value\": \"%s\"},",time_buf);//"2017-01-20 21:38:30");
	ret = strlen(buffer);
	sprintf(buffer+ret, "\"Heat\": { \"value\": \"%d\"},",user_running_data.Heat);
	ret = strlen(buffer);
#if 0	
	sprintf(buffer+ret, "\"Period\": { \"value\": [{\"Period_Speed\": \"10.0\",\"Period_Gradient\": 0,\"Period_Duration\": 180} ]},",118);
	ret = strlen(buffer);
#endif
	sprintf(buffer+ret, "\"Period\": { \"value\": [{\"Period_Speed\": \"10.0\",\"Period_Gradient\": 0,\"Period_Duration\": 180} ],\"when\":\"%d\"},",alink_get_time());//\"Height\": 600	
	ret = strlen(buffer);



	sprintf(buffer+ret, "\"Pace\": { \"value\": %d},",1);
	ret = strlen(buffer);

	sprintf(buffer+ret, "\"Step\": { \"value\": \"%d\"},",1000);
	ret = strlen(buffer);
	
	memset(time_buf, 0, 60);
	sprintf(time_buf,"%04d-%02d-%02d %02d:%02d:%02d",alm_tm1->tm_year,alm_tm1->tm_mon,alm_tm1->tm_mday,alm_tm1->tm_hour,alm_tm1->tm_min,alm_tm1->tm_sec);
	sprintf(buffer+ret, "\"Start_Time\": { \"value\": \"%s\"}}",time_buf);//"2017-01-20 21:31:30");
	ret = strlen(buffer);


	
	os_printf("status len:%d",ret);

	free(alm_tm);

	free(alm_tm1);
	alm_tm =NULL;


	printf("%s\n",buffer);


	return ret;

#endif
}


#endif





#define buffer_size 512
static int ICACHE_FLASH_ATTR alink_device_post_data(alink_down_cmd_ptr down_cmd)
{
	alink_up_cmd up_cmd;
	int ret = ALINK_ERR;
	//char buffer[1024];
	char *buffer = NULL;
	
	os_printf("DEV_VERSION:%s\r\n",DEV_VERSION);
	if (device_status_change) {


		wsf_deb("POST DATA..##[%s][%s|%d]Malloc %u. Available memory:%d.\n", __FILE__, __FUNCTION__, __LINE__,
			buffer_size, system_get_free_heap_size());

		buffer = (char *)malloc(buffer_size);
		if (buffer == NULL)
			return -1;

		memset(buffer, 0, buffer_size);
/*victor del
		sprintf(buffer, main_dev_params, virtual_device.power,
			virtual_device.temp_value, virtual_device.light_value,
			virtual_device.time_delay, virtual_device.work_mode);

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
------------------v*/

#if 0
		sprintf(buffer, main_dev_params,virtual_device.Auid,virtual_device.DetailPlans, virtual_device.Distance,
		virtual_device.End_Time,virtual_device.Heat,virtual_device.Light_Brightness,virtual_device.Pace,
		virtual_device.Period,virtual_device.Period_Duration,virtual_device.Period_Gradient,virtual_device.Period_Speed,
		virtual_device.Plan_Message,virtual_device.Start_Time,virtual_device.Step, virtual_device.tmp_attr);
	#endif
	if(haveareport==1){
	
		user_get_post_data_normal(buffer);
		haveareport=0;
	}else{
		sprintf(buffer, main_dev_params,0,0,0,0,0,0,0,0);
	}
	
		up_cmd.param = buffer;
		if (down_cmd != NULL) {
			up_cmd.target = down_cmd->account;
			up_cmd.resp_id = down_cmd->id;
		} else {
			up_cmd.target = NULL;
			up_cmd.resp_id = -1;
		}
		ret = alink_post_device_data(&up_cmd);

		if (ret == ALINK_ERR) {
			wsf_err("post failed!\n");
			alink_sleep(2000);
		} else {
			wsf_deb("dev post data success!\n");
			device_status_change = 0;
		}

		if (buffer)
			free(buffer);
		wsf_deb("##[%s][%s][%d]  Free |Aviable Memory|%5d| \n", __FILE__, __FUNCTION__, __LINE__,
			system_get_free_heap_size());

		stack_free_size();
	}
	return ret;

}

/* do your job end */

int sample_running = ALINK_TRUE;



/*get json cmd from server ∑˛ŒÒ∆˜œ¬∑¢√¸¡Ó,–Ë“™…Ë±∏∂À∏˘æ›æﬂÃÂ“µŒÒ…Ë∂®»•Ω‚Œˆ¥¶¿Ì*/
int ICACHE_FLASH_ATTR main_dev_set_device_status_callback(alink_down_cmd_ptr down_cmd)
{
os_printf("################main_dev_set_device_status_callback########################\n");
//#if 0
	json_value *jptr;
	json_value *jstr;
	json_value *jstr_value;
	int value = 0, i = 0;
	char *value_str = NULL;
	char get_server_data[15]={0};
//	char *value_str_DetailPlans=NULL;
//	char *value_str_Auid=NULL;
	
#if USER_UART_CTRL_DEV_EN
	if(dbg_get_recv_times<0xfffffff0)
		dbg_get_recv_times++;
	else
		dbg_get_recv_times = 0;


	wsf_deb("%s %d,recv_get_t[%d] \n",__FUNCTION__,__LINE__,dbg_get_recv_times);
#endif

	wsf_deb("%s %d\n%s\n",down_cmd->uuid,down_cmd->method, down_cmd->param);
     
	jptr = json_parse(down_cmd->param, strlen(down_cmd->param));
#if USER_UART_CTRL_DEV_EN  //////////////////////

#ifdef TESTHAVEAPLAN
	jstr = json_object_object_get_e(jptr, "DetailPlans");
			if(NULL != jstr)
			{
				char buf[71]={0};
				jstr_value = json_object_object_get_e(jstr, "value");
				value_str = json_object_to_json_string_e(jstr_value);
				
				os_printf("set:%s",value_str);

				HexStrToByte(value_str, buf, 72);
				printf("\n");
				printf("\n");

				for(i=0;i<36;i++)
		printf("0x%2X ",buf[i]);
	printf("\n");
			}
#else

	if(haveaplan==0&&runningmachineisbusy==0){

		char *value_str_Auid;
		jstr = json_object_object_get_e(jptr, device_attr[_Auid]);
		if(jstr!=NULL){
			
			value_str_Auid = zalloc(32);
			jstr_value = json_object_object_get_e(jstr, "value");
			value_str_Auid = json_object_to_json_string_e(jstr_value);
			memset(current_str_Auid, 0, strlen(current_str_Auid));
			memcpy(current_str_Auid,value_str_Auid,strlen(value_str_Auid));
			currentAUid = atoi(value_str_Auid);
			printf("currentAUid:%s \n",current_str_Auid);
			free(value_str_Auid);
			value_str_Auid = NULL;
		}
		
		jstr = json_object_object_get_e(jptr, device_attr[_DetailPlans]);
		if(jstr!=NULL){
			char buf[72]={0};
			jstr_value = json_object_object_get_e(jstr, "value");
			value_str_DetailPlans = json_object_to_json_string_e(jstr_value);
			HexStrToByte(value_str_DetailPlans, value_byte_DetailPlans, 72);
			haveaplan=1;
			uartsendsuccessfully=0;
	#if 0
			printf("\n");
			printf("\n");
			for(i=0;i<36;i++)
				printf("0x%2X ",value_byte_DetailPlans[i]);
			printf("\n");
	#endif		//cloudplandata.send_data=value_str_DetailPlans;
		}
	}
#endif
//	char *value_str_Auid=NULL;
		
/*victor del
for (i = 0; i < 5; i++) 
	{
		jstr = json_object_object_get_e(jptr, device_attr[i]);
		jstr_value = json_object_object_get_e(jstr, "value");
		value_str = json_object_to_json_string_e(jstr_value);
		
		if (value_str) {
			value = atoi(value_str);
			cus_wifi_handler_alinkdata2mcu(i, value);
//victor del
//			ESP_DBG(("power:0x%X,temp_value:0x%X,light_value:0x%X,time_delay:0x%X,woke_mode:0x%X",virtual_device.power,\
//				virtual_device.temp_value,virtual_device.light_value,virtual_device.time_delay,virtual_device.work_mode));
//---------------------v
		}
	}
	--------------------------------v*/
#endif
#if USER_PWM_LIGHT_EN
	USER_LIGHT_DATA *user_light_data_ptr = zalloc(sizeof(USER_LIGHT_DATA));

/*victor del------------v*/
	   user_light_data_ptr->light_period=1000;
	   user_light_data_ptr->light_r=virtual_device.temp_value;
	   user_light_data_ptr->light_g=virtual_device.light_value;
	// Set r g b w data,Developers need to parse the real device parameters corresponding to the Json package
	light_set_aim(user_light_data_ptr->light_r,user_light_data_ptr->light_g,user_light_data_ptr->light_b,user_light_data_ptr->light_cw,user_light_data_ptr->light_ww,user_light_data_ptr->light_period);

	free(user_light_data_ptr);
#endif
#if 0//USER_VIRTUAL_DEV_TEST
os_printf("^^^^^^^^^^%s  %d ^^^^^^^^^^\r\n",__FUNCTION__,__LINE__);
	for (i = 0; i < 15; i++) 
	{
	
	os_printf("^^^^^^^^^^%s  %d  i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
		jstr = json_object_object_get_e(jptr, device_attr[i]);
		jstr_value = json_object_object_get_e(jstr, "value");
		value_str = json_object_to_json_string_e(jstr_value);
		
		if (value_str) {
			
			os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
			value = atoi(value_str);
			switch (i) {
			case 0:
				if (virtual_device.Auid != value) {
					virtual_device.Auid = value;
				}
				get_server_data[0] = virtual_device.Auid;
				os_printf("Auid:%x\r\n",virtual_device.Auid);
				os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				break;
			case 1:
				//if (virtual_device.DetailPlans != value) {
					//virtual_device.DetailPlans = value;
				//}
			//	get_server_data[1] = virtual_device.DetailPlans;
				os_printf("DetailPlans:%s\r\n",value);
				os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				break;
			case 2:
				os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				if (virtual_device.Distance != value) {
					virtual_device.Distance = value;
				}
				get_server_data[2] = virtual_device.Distance;
				os_printf("Distance:%x\r\n",virtual_device.Distance);
				break;
			case 3:
				os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				if (virtual_device.End_Time != value) {
					virtual_device.End_Time = value;
					get_server_data[3] = virtual_device.End_Time;
				os_printf("End_Time:%x\r\n",virtual_device.End_Time);
				}
				break;
			case 4:
				os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				if (virtual_device.Heat != value) {
					virtual_device.Heat = value;
				}
				get_server_data[4] = virtual_device.Heat;
				os_printf("Heat:%x\r\n",virtual_device.Heat);
				break;
				case 5:
					os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				if (virtual_device.Light_Brightness != value) {
					virtual_device.Light_Brightness = value;
				}
				get_server_data[5] = virtual_device.Auid;
				os_printf("Light_Brightness:%x\r\n",virtual_device.Auid);
				break;
			case 6:
				os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				if (virtual_device.Pace != value) {
					virtual_device.Pace = value;
				}
				get_server_data[6] = virtual_device.Pace;
				os_printf("Pace:%x\r\n",virtual_device.Pace);
				break;
			case 7:
				os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				if (virtual_device.Period != value) {
					virtual_device.Period = value;
				}
				get_server_data[7] = virtual_device.Period;
				os_printf("Period:%x\r\n",virtual_device.Period);
				break;
			case 8:
				os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				if (virtual_device.Period_Duration != value) {
					virtual_device.Period_Duration = value;
				}
				get_server_data[8] = virtual_device.Period_Duration;
				os_printf("Period_Duration:%x\r\n",virtual_device.Period_Duration);
				break;
			case 9:
				os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				if (virtual_device.Period_Gradient != value) {
					virtual_device.Period_Gradient = value;
				}
				get_server_data[9] = virtual_device.Period_Gradient;
				os_printf("Period_Gradient:%x\r\n",virtual_device.Period_Gradient);
				break;
			case 10:
				os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				if (virtual_device.Period_Speed != value) {
					virtual_device.Period_Speed = value;
				}
				get_server_data[10] = virtual_device.Period_Speed;
				os_printf("Period_Speed:%x\r\n",virtual_device.Period_Speed);
				break;
			case 11:
				os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				if (virtual_device.Plan_Message != value) {
					virtual_device.Plan_Message = value;
				}
				get_server_data[11] = virtual_device.Plan_Message;
				os_printf("Plan_Message:%x\r\n",virtual_device.Plan_Message);
				break;
			case 12:
				os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				if (virtual_device.Start_Time != value) {
					virtual_device.Start_Time = value;
				}
				get_server_data[12] = virtual_device.Start_Time;
				os_printf("Start_Time:%x\r\n",virtual_device.Start_Time);
				break;
			case 13:
				os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				if (virtual_device.Step != value) {
					virtual_device.Step = value;
				}
				get_server_data[13] = virtual_device.Step;
				os_printf("Step:%x\r\n",virtual_device.Step);
				break;
			case 14:
				os_printf("^^^^^^^^^^%s  %d i=%d^^^^^^^^^^\r\n",__FUNCTION__,__LINE__,i);
				if (virtual_device.tmp_attr != value) {
					virtual_device.tmp_attr = value;
				}
				get_server_data[14] = virtual_device.tmp_attr;
				os_printf("tmp_attr:%x\r\n",virtual_device.tmp_attr);
				break;
			default:
				break;
			} 
		}
	}
//	uart0_write_data(get_server_data,15);
#endif

	json_value_free(jptr);

	if(!device_status_change)
		device_status_change = 1;   // event send current real device status to the alink server

//#endif
	return 0;		// alink_device_post_data(down_cmd);
	/* do your job end! */
}

/*∑˛ŒÒ∆˜≤È—Ø…Ë±∏◊¥Ã¨,–Ë“™…Ë±∏…œ±®◊¥Ã¨*/

int ICACHE_FLASH_ATTR main_dev_get_device_status_callback(alink_down_cmd_ptr down_cmd)
{
   os_printf("+++++++++++++++++++++++++++++++++++++++\r\n");
	wsf_deb("%s %d \n", __FUNCTION__, __LINE__);
	wsf_deb("%s %d\n%s\n", down_cmd->uuid, down_cmd->method, down_cmd->param);

	if(!device_status_change)
		device_status_change = 1;   // event send current real device status to the alink server

	return 0;		//alink_device_post_data(down_cmd);
}

/* ∏˘æ›≤ªÕ¨œµÕ≥¥Ú”° £”‡ƒ⁄¥Ê,”√”⁄∆ΩÃ®µ˜ ‘ */
int ICACHE_FLASH_ATTR print_mem_callback(void *a, void *b)
{
	int ret = 0;
	ret = system_get_free_heap_size();
	os_printf("heap_size %d\n", ret);
	return ret;
}

#ifdef PASS_THROUGH
/* device response server command,”√ªß–Ë“™◊‘º∫ µœ÷’‚∏ˆ∫Ø ˝,¥¶¿Ì∑˛ŒÒ∆˜œ¬∑¢µƒ÷∏¡Ó*/
/* this sample save cmd value to virtual_device*/
static int ICACHE_FLASH_ATTR execute_cmd(const char *rawdata, int len)
{
	int ret = 0, i = 0;
	if (len < 1)
		ret = -1;
	for (i = 0; i < len; i++) {
		wsf_deb("%2x ", rawdata[i]);
		switch (i) {
		case 2:
			if (virtual_device.power != rawdata[i]) {
				virtual_device.power = rawdata[i];
			}
			break;
		case 4:
			if (virtual_device.temp_value != rawdata[i]) {
				virtual_device.temp_value = rawdata[i];
			}
			break;
		case 5:
			if (virtual_device.light_value != rawdata[i]) {
				virtual_device.light_value = rawdata[i];
			}
/*			// for test alink unbind
			if (virtual_device.light_value == 0x5a) {
				wsf_deb("test for alink unbind\n");
				virtual_device.light_value = 0x59;
				alink_unbind();
			} else
				wsf_deb("0x%2x", virtual_device.light_value);
*/
			break;
		case 6:
			if (virtual_device.time_delay != rawdata[i]) {
				virtual_device.time_delay = rawdata[i];
			}
			break;
		case 3:
			if (virtual_device.work_mode != rawdata[i]) {
				virtual_device.work_mode = rawdata[i];
			}
			break;
		default:
			break;
		}
	}
	return ret;
}

/*ªÒ»°…Ë±∏–≈œ¢,–Ë“™”√ªß µœ÷ */

static int ICACHE_FLASH_ATTR get_device_status(char *rawdata, int len)
{
	/* do your job here */
	int ret = 0;
	if (len > 7) {
		rawdata[0] = 0xaa;
		rawdata[1] = 0x07;
		rawdata[2] = virtual_device.power;
		rawdata[3] = virtual_device.work_mode;
		rawdata[4] = virtual_device.temp_value;
		rawdata[5] = virtual_device.light_value;
		rawdata[6] = virtual_device.time_delay;
		rawdata[7] = 0x55;
	} else {
		ret = -1;
	}
	/* do your job end */
	return ret;
}

static unsigned int delta_time = 0;

/*÷˜∂Ø…œ±®…Ë±∏◊¥Ã¨,–Ë“™”√ªß◊‘º∫ µœ÷*/
int ICACHE_FLASH_ATTR alink_device_post_raw_data(void)
{
	/* do your job here */
	int len = 8, ret = 0;
	char rawdata[8] = { 0 };
	if (device_status_change) {

		ESP_DBG(("********POST DATA*************\n"));
		wsf_deb("[%s][%d|  Available memory:%d.\n", __FILE__, __LINE__,system_get_free_heap_size());

		delta_time = system_get_time() - delta_time;
		wsf_deb("%s %d \n delta_time = %d ", __FUNCTION__, __LINE__, delta_time / 1000);
		get_device_status(rawdata, len);

		ret = alink_post_device_rawdata(rawdata, len);
		
		device_status_change = 0;	// event send current real device status to the alink server

		if (ret) {
			wsf_err("post failed!\n");
		} else {
			wsf_deb("dev post raw data success!\n");
		}
	}
	/* do your job end */
	return ret;
}

/*Õ∏¥´∑Ω Ω∑˛ŒÒ∆˜≤È—Ø÷∏¡Óªÿµ˜∫Ø ˝*/

int ICACHE_FLASH_ATTR rawdata_get_callback(const char *in_rawdata, int in_len, char *out_rawdata, int *out_len)
{
	//TODO: œ¬∑¢÷∏¡ÓµΩMCU
	int ret = 0;
	wsf_deb("%s %d \n", __FUNCTION__, __LINE__);
	//ret=alink_device_post_raw_data(); //  ¥À¿˝ «ºŸ…Ëƒ‹Õ¨≤ΩªÒ»°µΩ–Èƒ‚…Ë±∏ ˝æ›,  µº ”¶”√÷–,¥¶¿Ì∑˛ŒÒ∆˜÷∏¡Ó «“Ï≤Ω∑Ω Ω,–Ë“™≥ßº“¥¶¿ÌÕÍ±œ∫Û÷˜∂Ø…œ±®“ª¥Œ…Ë±∏◊¥Ã¨

// do your job end!

	if(!device_status_change)
		device_status_change = 1;   // event send current real device status to the alink server

	return ret;
}

/*Õ∏¥´∑Ω Ω∑˛ŒÒ∆˜œ¬∑¢÷∏¡Óªÿµ˜∫Ø ˝*/
/* µº ”¶”√÷–,¥¶¿Ì∑˛ŒÒ∆˜÷∏¡Ó «“Ï≤Ω∑Ω Ω,–Ë“™≥ßº“¥¶¿ÌÕÍ±œ∫Û÷˜∂Ø…œ±®“ª¥Œ…Ë±∏◊¥Ã¨*/

int ICACHE_FLASH_ATTR rawdata_set_callback(char *rawdata, int len)
{
	// TODO: 
	//get cmd from server, do your job here!
	int ret = 0;
	wsf_deb("%s %d \n", __FUNCTION__, __LINE__);
	ret = execute_cmd(rawdata, len);
	//ret=alink_device_post_raw_data();
	// do your job end!
	delta_time = system_get_time();

	if(!device_status_change)
		device_status_change = 1;   // event send current real device status to the alink server
	return ret;
}

#endif //PASS_THROUGH

/*alink-sdk ◊¥Ã¨≤È—Øªÿµ˜∫Ø ˝*/
int ICACHE_FLASH_ATTR alink_handler_systemstates_callback(void *dev_mac, void *sys_state)
{
	char uuid[33] = { 0 };
	char *mac = (char *)dev_mac;
	enum ALINK_STATUS *state = (enum ALINK_STATUS *)sys_state;
	switch (*state) {
	case ALINK_STATUS_INITED:
		os_printf("============ALINK_STATUS_INITED===================\r\n");
		break;
	case ALINK_STATUS_REGISTERED:
		
		os_printf("=============ALINK_STATUS_REGISTERED==================\r\n");
		//sprintf(uuid, "%s", alink_get_uuid(NULL));
		//wsf_deb("ALINK_STATUS_REGISTERED, mac %s uuid %s \n", mac, uuid);
		break;
	case ALINK_STATUS_LOGGED:
		os_printf("===============ALINK_STATUS_LOGGED================\r\n");
		//sprintf(uuid, "%s", alink_get_uuid(NULL));
		//wsf_deb("ALINK_STATUS_LOGGED, mac %s uuid %s\n", mac, uuid);
//victor
		cfgnetsta=1;
		//////////////////////v
		break;
	default:
		break;
	}
	return 0;
}

/* fill device info ËøôÈáåËÆæÂ§á‰ø°ÊÅØÈúÄË¶Å‰øÆÊîπÂØπÂ∫îÂÆèÂÆö‰πâ,ÂÖ∂‰∏≠DEV_MACÂíåDEV_CHIPID ÈúÄË¶ÅÁî®Êà∑Ëá™Â∑±ÂÆûÁé∞Êé•Âè£ÂáΩÊï∞*/
void ICACHE_FLASH_ATTR alink_fill_deviceinfo(struct device_info *deviceinfo)
{
	uint8 macaddr[6];
	//fill main device info here
	strcpy(deviceinfo->name, DEV_NAME);
	strcpy(deviceinfo->sn, DEV_SN);
	strcpy(deviceinfo->key, ALINK_KEY);
	strcpy(deviceinfo->model, DEV_MODEL);
	strcpy(deviceinfo->secret, ALINK_SECRET);
	strcpy(deviceinfo->type, DEV_TYPE);
	strcpy(deviceinfo->version, DEV_VERSION);
	strcpy(deviceinfo->category, DEV_CATEGORY);
	strcpy(deviceinfo->manufacturer, DEV_MANUFACTURE);
	strcpy(deviceinfo->key_sandbox, ALINK_KEY_SANDBOX);
	strcpy(deviceinfo->secret_sandbox, ALINK_SECRET_SANDBOX);

	if (wifi_get_macaddr(0, macaddr)) {
		wsf_deb("macaddr=%02x:%02x:%02x:%02x:%02x:%02x\n", MAC2STR(macaddr));
		snprintf(deviceinfo->mac, sizeof(deviceinfo->mac), "%02x:%02x:%02x:%02x:%02x:%02x", MAC2STR(macaddr));
	} else
		strcpy(deviceinfo->mac, DEV_MAC);
//	if ((macaddr[4] == 0xc7) && (macaddr[5] == 0x18))	// the mac  18:fe:34:a2:c7:18   binding CHIPID  "3D0044000F47333139373030" 
//	{
//		strcpy(deviceinfo->cid, DEV_CHIPID);
//	} else {
		snprintf(deviceinfo->cid, sizeof(deviceinfo->cid), "%024d", system_get_chip_id());
//	}
	wsf_deb("DEV_MODEL:%s \n", DEV_MODEL);
}


//#define GET_ALIGN_STRING_LEN(str)    ((strlen(str) + 3) & ~3)
/*ALINK_CONFIG_LEN 2048, len need < ALINK_CONFIG_LEN */
static int ICACHE_FLASH_ATTR write_config(unsigned char *buffer, unsigned int len)
{
	int res = 0, pos = 0;
	ESP_DBG(("...write cfg....."));

	if (buffer == NULL) {
		return ALINK_ERR;
	}
	if (len > ALINK_CONFIG_LEN)
		len = ALINK_CONFIG_LEN;

	res = spi_flash_erase_sector(LFILE_START_ADDR / 4096);	//one sector is 4KB 
	if (res != SPI_FLASH_RESULT_OK) {
		wsf_err("erase flash data fail\n");
		return ALINK_ERR;
	} else {
		wsf_err("erase flash data %d Byte\n", res);
	}
	os_printf("write data:\n");

//	debug_printf_data(buffer, len);
//	ESP_DBG((">>>>>>>>>>>>>>>"));

	res = spi_flash_write((LFILE_START_ADDR), (uint32 *) buffer, len);
	if (res != SPI_FLASH_RESULT_OK) {
		wsf_err("write data error\n");
		return ALINK_ERR;
	}
	wsf_deb("write key(%s) success.", buffer);
	return ALINK_OK;
}

 /*ALINK_CONFIG_LEN 2048, len need < ALINK_CONFIG_LEN */
static int ICACHE_FLASH_ATTR read_config(unsigned char *buffer, unsigned int len)
{

	int res = 0;
	int pos = 0;
	ESP_DBG(("...test read cfg len [%d].....", len));
	res = spi_flash_read(LFILE_START_ADDR, (uint32 *) buffer, len);
//	debug_printf_data(buffer, len);
//	ESP_DBG((">>>>>>>>>>>>>>>"));
	if (res != SPI_FLASH_RESULT_OK) {
		wsf_err("read flash data error\n");
		return ALINK_ERR;
	}
	os_printf("read data:\n");
	return ALINK_OK;
}

int ICACHE_FLASH_ATTR alink_get_debuginfo(info_type type, char *status)
{
	int used;  
	switch (type) {    
		case MEMUSED:    
			used = 100 - ((system_get_free_heap_size()*100)/(96*1024));   
			sprintf(status, "%d%%", used);    
			break;    
		case WIFISTRENGTH:    
			sprintf(status , "%ddB",wifi_station_get_rssi());    
			break;    
		default:    
			status[0] = '\0';    
			break;  
	}  
	return 0;
}
int esp_ota_firmware_update( char * buffer, int len)
{
    os_printf("esp_ota_firmware_update \n");
   return upgrade_download(buffer , len);
}

int esp_ota_upgrade(void)
{
    os_printf("esp_ota_upgrade \n");
    system_upgrade_reboot();
    return 0;
}
extern int need_notify_app;
extern int  need_factory_reset ;

void set_thread_stack_size(struct thread_stacksize * p_thread_stacksize)
{
    p_thread_stacksize->alink_main_thread_size = 0xc00;
    p_thread_stacksize->send_work_thread_size = 0x800;
    p_thread_stacksize->wsf_thread_size = 0x1000;
    p_thread_stacksize->func_thread_size = 0x800;
}
   
int ICACHE_FLASH_ATTR alink_demo()
{
	os_printf("\n[******Alink[%s] boot Module start 120813*******]\n",DEV_VERSION);

	struct device_info main_dev;
	#if USER_UART_CTRL_DEV_EN
	dbg_get_recv_times = 0;
	#endif
	memset(&main_dev, 0, sizeof(main_dev));
	alink_fill_deviceinfo(&main_dev);	// ±ÿ–Î∏˘æ›PRD±Ì∏Ò∏¸–¬…Ë±∏–≈œ¢
	alink_set_loglevel(ALINK_LL_DEBUG | ALINK_LL_INFO | ALINK_LL_WARN | ALINK_LL_ERROR);
	//alink_set_loglevel(ALINK_LL_ERROR);
//actived by victor	
	//alink_enable_sandbox_mode()();                                      //œﬂ…œª∑æ≥–Ë“™◊¢Ω‚¥À∫Ø ˝
//---------------------------------------v
	main_dev.sys_callback[ALINK_FUNC_SERVER_STATUS] = alink_handler_systemstates_callback;
	alink_set_callback(ALINK_FUNC_AVAILABLE_MEMORY, print_mem_callback);

	/* ALINK_CONFIG_LEN 2048 */
	alink_register_cb(ALINK_FUNC_READ_CONFIG, (void *)&read_config);
	alink_register_cb(ALINK_FUNC_WRITE_CONFIG, (void *)&write_config);
	alink_register_cb(ALINK_FUNC_GET_STATUS, alink_get_debuginfo);
//victor
//alink_enable_sandbox(&main_dev);                                      //œﬂ…œª∑æ≥–Ë“™◊¢Ω‚¥À∫Ø ˝
    alink_register_cb(ALINK_FUNC_OTA_FIRMWARE_SAVE, esp_ota_firmware_update);
    alink_register_cb(ALINK_FUNC_OTA_UPGRADE, esp_ota_upgrade);
	/*start alink-sdk */
    set_thread_stack_size(&g_thread_stacksize);
    #ifdef PASS_THROUGH		
	alink_start_rawdata(&main_dev, rawdata_get_callback, rawdata_set_callback);
#else // ∑«Õ∏¥´∑Ω Ω(…Ë±∏”Î∑˛ŒÒ∆˜≤…”√json∏Ò Ω ˝æ›Õ®—∂)

	main_dev.dev_callback[ACB_GET_DEVICE_STATUS] = main_dev_get_device_status_callback;
	main_dev.dev_callback[ACB_SET_DEVICE_STATUS] = main_dev_set_device_status_callback;
	
	alink_start(&main_dev);	//register main device here
#endif //PASS_THROUGH

	os_printf("%s %d wait time=%d \n", __FUNCTION__, __LINE__, ALINK_WAIT_FOREVER);

	#if 1
	//if(ALINK_OK == alink_wait_connect(NULL, ALINK_WAIT_FOREVER))	//wait main device login, -1 means wait forever
	//{
	//victor
	if(ALINK_OK == alink_wait_connect(NULL, 30))	//v wait main device login, 30 means 30second
	{
		ESP_DBG(("CONN OK.."));
		
//actived by victor
#ifdef CONFIGNETWORK//USER_UART_CTRL_DEV_EN#if 1//USER_UART_CTRL_DEV_EN
		//char send_buf_alink_connOK[]={0x0a, 0x01, 0x01}; // demo data to tell uart mcu dev, alink conn success
		//uart0_write_data(send_buf_alink_connOK,sizeof(send_buf_alink_connOK));
		cfgnetsta=1;//smart config network successfully
#endif
	}
	else
	{
//actived by victor
#ifdef CONFIGNETWORK//USER_UART_CTRL_DEV_EN//#if 0//USER_UART_CTRL_DEV_EN
		//char send_buf_alink_connFailed[]={0x0a, 0x01, 0x03}; // demo data to tell uart mcu dev, alink conn failure
		//uart0_write_data(send_buf_alink_connFailed,sizeof(send_buf_alink_connFailed));
		cfgnetsta=3;//smart config network failed
#endif
	}
	#endif
	if(need_notify_app) {
		need_notify_app = 0;
		uint8 macaddr[6] ={0};
		char mac[17+1] = {0};
		if (wifi_get_macaddr(0, macaddr)) {
			os_printf("macaddr=%02x:%02x:%02x:%02x:%02x:%02x\n", MAC2STR(macaddr));
			snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x", MAC2STR(macaddr));
			
		//	aws_notify_app(DEV_MODEL, mac, ""); // if not factory reset , 
			aws_notify_app();
		}
	}
	//printf("%s %d \n",__FUNCTION__,__LINE__);

	//printf("alink_demo heap_size %d\n",system_get_free_heap_size());
	//system_print_meminfo();


	/* …Ë±∏÷˜∂Ø…œ±® ˝æ› */



	
	if(!device_status_change)
		device_status_change = 0x01;
	while(sample_running) {

		//os_printf("%s %d \n",__FUNCTION__,__LINE__);
#ifdef PASS_THROUGH
		alink_device_post_raw_data();
#else
		alink_device_post_data(NULL);
#endif //PASS_THROUGH

		if(need_factory_reset) {
			wsf_deb("key to factory_reset\n");
			need_factory_reset = 0;
			alink_factory_reset();
			wsf_deb("----------------system_restore-----------------\n");
			system_restore();
			//cfgnetsta=0;
			confignetworkkey=1;
			wsf_deb("confignetworkkey=1\n");
		}

		
		alink_sleep(1000);
	}

	/*  …Ë±∏ÕÀ≥ˆalink-sdk */
	alink_end();

	return 0;
}

char *platform_get_os_version(char os_ver[PLATFORM_OS_VERSION_LEN])
{
    return strncpy(os_ver, "1.3.0", PLATFORM_OS_VERSION_LEN);
}
char *platform_get_module_name(char name_str[PLATFORM_MODULE_NAME_LEN])
{
    return strncpy(name_str,"ESP-WROOM-02",PLATFORM_MODULE_NAME_LEN);
}


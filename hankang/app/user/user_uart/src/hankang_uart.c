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
//#include <time.h>
#include "c_types.h"
#include <stdio.h>
#include <string.h>
#include "esp_common.h"
#include "user_uart.h"
#include "../../../include/driver/gpio.h"
#include "../../../include/driver/key.h"

//#include "../../../include/driver/user_time.h"

//#include "user_light.h"

#if USER_UART_CTRL_DEV_EN

#include "../../../include/driver/uart.h"


xQueueHandle xQueueCusUart;

USER_RUN_DATA user_running_data={0};





struct SENDCOMMAND
{
	int data_len;
	char send_data[6];
};

struct CLOUDPLANDATA
{
	int data_len;
	char head[2];
	char *send_data;//send_data[3][12];
};
//char hankang_uart_shakehand_data[]={0x08, 0x04, 0x00, 0x00, 0x00, 0x00};  // test uart beat data "12345678"
//char hankang_uart_shakehand_echodata[];  //


//char hankang_uart_speed_data[]={0x01, 0x01, 0x00};  // test uart beat data "12345678"
//char hankang_uart_slope_data[]={0x04, 0x01, 0x00};  // test uart beat data "12345678"
struct SENDCOMMAND command_speed={3,{0x01, 0x01, 0x00}};
struct SENDCOMMAND command_cfgnetsta={3,{0x0A, 0x01, 0x00}};

struct CLOUDPLANDATA cloudplandata={36,{0x10,0x24}};



int haveaplan;
int haveareport;
char *value_str_DetailPlans;


struct SENDCOMMAND sendcommand;


#ifdef SLOPE_ENABLE	
struct SENDCOMMAND command_slope={3,{0x04, 0x01, 0x00}};
#endif

int DataRecvCounter=0;


int	startkey=0;
int	stopkey=0;
int speedaddkey=0;
int speedsubkey=0;
int upkey=0;
int downkey=0;
int oxykey=0;
int quickrunkey=0;
int multimediakey=0;
int soundopenkey=0;
int soundclosekey=0;
int voladdkey=0;
int volsubkey=0;
int forwardkey=0;
int backwardkey=0;
int secretarykey=0;
int fieldmodekey=0;
int freedommodekey=0;
int smartmodekey=0;
int compititionkey=0;
int motiontargetkey=0;
int assistancekey=0;
int realviewskey=0;
int streetviewskey=0;
int homekey=0;
int set_key=0;
int confignetworkkey=0;
uint16 hankang_speed_max;
uint16 hankang_speed_min;
uint16 hankang_slope_max;


u32 currentAUid;
int runningmachineisbusy;

int uartsendsuccessfully;

int convertdatatypisok;

int manufacturetestmode=0;

#ifdef TESTHAVEAPLAN

	int testhaveaplan=1;
	char send_data_sample[38]={0x10,0x24,0x08,0,1,9,0,1,16,0,1,32,0,1,32,0,1,64,0,1,64,0,1};//test for 7mins
#endif

       char send_data_DetailPlans[38];


	char value_byte_DetailPlans[36];


//=============soft timer================



#ifdef HEARTBEATTIMER
os_timer_t xTimerHeartbeat;

uint16  T_ms = 500;
uint16 hankang_plan_time=0;
uint16 hankang_plan_speed=0;
uint16 hankang_plan_slope=0;


int cfgnetsta;
int getssidpasswdflag;
int confignetwork_counter=0;



void hankang_heartbeat_proc(void *Targ)
{
#ifdef HANKANG_DEBUG
	ESP_DBG(("BEAT...\n"));
#endif
	if(getssidpasswdflag){
		confignetwork_counter++;
		if(confignetwork_counter>60){
			confignetwork_counter=0;
			getssidpasswdflag=0;
			cfgnetsta=3;
		}

	}
//	if(haveaplan==0)
//	{
		switch (cfgnetsta)
		{

			case 0:
				command_cfgnetsta.send_data[0]=0x0a;
				command_cfgnetsta.send_data[1]=0x01;
				command_cfgnetsta.send_data[2]=0x00;
				break;
			case 1:
				command_cfgnetsta.send_data[0]=0x0a;
				command_cfgnetsta.send_data[1]=0x01;
				command_cfgnetsta.send_data[2]=0x01;
				break;		
			case 2:
				command_cfgnetsta.send_data[0]=0x0a;
				command_cfgnetsta.send_data[1]=0x01;
				command_cfgnetsta.send_data[2]=0x02;
				break;
			case 3:
				command_cfgnetsta.send_data[0]=0x0a;
				command_cfgnetsta.send_data[1]=0x01;
				command_cfgnetsta.send_data[2]=0x03;
				break;



		}


		uart0_write_data(command_cfgnetsta.send_data,command_cfgnetsta.data_len);
//	}

#ifdef TESTHAVEAPLAN
	if(testhaveaplan==0)
	{

		vTaskDelay(1 / portTICK_RATE_MS);	 // 2 ms	

	
	
		//uart0_write_data(send_data_DetailPlans,38);
		uart0_write_data(send_data_sample,38);		

	
	}	
#else
	if(haveaplan==1&&uartsendsuccessfully==0&&convertdatatypisok==1)
	{

		vTaskDelay(1 / portTICK_RATE_MS);	 // 2 ms	

	
	
		uart0_write_data(send_data_DetailPlans,38);
		

	
	}
#endif

}
#endif
 //=============built by victor==============





void debug_print_hex_data(char*buf, int len)
{
	int i = 0;
	printf("\n_____________[%d]__________\n",len);
	for(i=0;i<len;i++)
	{
		printf("%X ",*(buf+i));
	}
	printf("\n____________________________\n");
	return;
}	

int uart0_write_data(u8 *data, int len)
{
	int re_len = 0;
	int i = 0;
	for(i  = 0; i <len; i++)
	{
		uart0_write_char(*(data+i));
	}
	return i;
}

void ICACHE_FLASH_ATTR cus_wifi_handler_alinkdata2mcu(u8 dat_index, int dat_value)
{
	ESP_DBG(("data2mcu handler, index[%x],data_value[%x]",dat_index,dat_value));
	// here handler user own uart protocol...
	return;
}




static u8 ICACHE_FLASH_ATTR cus_uart_data_handle(char *dat_in, int in_len, char *dat_out)
{
	//ESP_DBG(("uart data handler.."));
	//debug_print_hex_data(dat_in, in_len);
	
//printf("dat_in0:%x,dat_in1 %x",dat_in[0],dat_in[1]);
					

//==================hangkang console port protocal===================

//parse the data received
	//need to find out error-command at first and then execute it

	switch(dat_in[0])
	{
		case 0x0d:
			if (dat_in[1]==0x01&&dat_in[2]==0x01){//plan is overtime

				haveaplan=0;

				sendcommand.send_data[0]=0x0b;
				sendcommand.send_data[1]=0x01;
				sendcommand.send_data[2]=0x0d;
				uart0_write_data(sendcommand.send_data,sendcommand.data_len);

				
			}
			if (dat_in[1]==0x01&&dat_in[2]==0x02){//plan is cancl

				haveaplan=0;

				sendcommand.send_data[0]=0x0b;
				sendcommand.send_data[1]=0x01;
				sendcommand.send_data[2]=0x0d;
				uart0_write_data(sendcommand.send_data,sendcommand.data_len);

				
			}			
			break;
		case 0x00:
			if (dat_in[1]==0x00){
			//hankang_uart_shakehand_data[]={0x08, 0x01, 0x00};  // test uart beat data "12345678"
				sendcommand.data_len=3;
				sendcommand.send_data[0]=0x08;
				sendcommand.send_data[1]=0x01;
				sendcommand.send_data[2]=0x00;
#ifdef HANKANG_DEBUG
				ESP_DBG((" shakeok=%d",shakeok));
#endif
				uart0_write_data(sendcommand.send_data,sendcommand.data_len);
			}
			break;
		case 0x08:
			if(dat_in[1]==0x04){
						
				sendcommand.data_len=6;
				sendcommand.send_data[0]=0x08;
				sendcommand.send_data[1]=0x04;
				sendcommand.send_data[2]=0x00;
				sendcommand.send_data[3]=0x00;
				sendcommand.send_data[4]=0x00;
				sendcommand.send_data[5]=0x00;
	
				uart0_write_data(sendcommand.send_data,sendcommand.data_len);
			}
			break;
		case 0x0c://clouddata received confirming code 
			if (dat_in[2]==0x01){//running-machine is busy
				runningmachineisbusy=1;//haveaplan=1;
			}
			if(dat_in[2]==0x02){//running-machine is idle
			//cancl the current plan 
				runningmachineisbusy=0;
				//haveaplan=0;
			}
			uartsendsuccessfully=1;
			sendcommand.send_data[0]=0x0b;
			sendcommand.send_data[1]=0x01;
			sendcommand.send_data[2]=0x0c;
			uart0_write_data(sendcommand.send_data,sendcommand.data_len);

			
			#ifdef TESTHAVEAPLAN
				testhaveaplan=0;
			#endif
			break;
	//data reciving
		case 0x10:
			switch(dat_in[1])
			{

				case 0x0a:	
					DataRecvCounter++;
					Data1=dat_in;
					if(DataRecvCounter==2)
					{
						Data2=dat_in;
						DataRecvCounter=0;
						runningmachineisbusy=0;
						haveaplan=0;
						if(strcmp(Data1,Data2)==0)
						{
							
							
							sendcommand.data_len=3;
							sendcommand.send_data[0]=0x0b;
							sendcommand.send_data[1]=0x01;
							sendcommand.send_data[2]=0x10;
#ifdef HANKANG_DEBUG
				ESP_DBG((" receive 12-byte sport result data ok\n"));
#endif
				uart0_write_data(sendcommand.send_data,sendcommand.data_len);
							switch(dat_in[2])
							{
								case 0x00://
								user_running_data.own_flag=0;

								//local mode
								break;
								case 0x03:
								user_running_data.own_flag=1;

								//cloud mode
								break;
						
							
							}
						//send confirm info
						//unsigned int when_t;
						//memcpy(user_running_data.time_s, dat_in+8, 2);
						//user_running_data.time_s[2]='\0';
//		uint32 timeduration=99;

	
//		timeduration=(uint32 *)time_;


	
							//timelen=atoi(user_running_data.time_s);
						//printf("timeduration :%ds\n",timelen);
						//printf("sportTime1:0x%x-0x%x-0x%x\n",user_running_data.time_s[0],user_running_data.time_s[1],user_running_data.time_s[2]);	
						//printf("currentAUid:%d\n",virtual_device.Auid);


						
						user_running_data.Distance=dat_in[5]<<16;
						user_running_data.Distance=user_running_data.Distance+dat_in[6]<<8;
						user_running_data.Distance=user_running_data.Distance+dat_in[7];
#ifdef HANKANG_DEBUG
						printf("DistanceH2H1L0:0x%x-0x%x-0x%x\n",dat_in[5],dat_in[6],dat_in[7]);
						printf("Distance-d:%d\n",user_running_data.Distance);
#endif
						//printf("Distance-s:%s\n",user_running_data.Distance);
						//printf("Distance-x:%x\n",user_running_data.Distance);
						
						user_running_data.time_s=dat_in[8]<<8;
						user_running_data.time_s=user_running_data.time_s+dat_in[9];
#ifdef HANKANG_DEBUG
						printf("sportTime:0x%x-0x%x\n",dat_in[8],dat_in[9]);	
						printf("sportTime:%d\n",user_running_data.time_s);	
#endif

						user_running_data.Heat=dat_in[3]<<8;
						user_running_data.Heat=user_running_data.time_s+dat_in[4];
#ifdef HANKANG_DEBUG
						printf("HeatH1L0:0x%x-0x%x\n",dat_in[8],dat_in[9]);
						printf("Heat:%d\n",user_running_data.Heat);
#endif						
							device_status_change=1;
							haveareport=1;
							haveaplan=0;
						}
					}
					//dat_in[2]=0;
					break;
						//speedadd-key

			}
			

	
	//waiting for key-command

		case 0x05:
		switch(dat_in[2])
		{
			case 0x01:	//start-key
				startkey=1;
				break;
						//stop-key
			case 0x02:	
				startkey=0;
				break;
						//speedadd-key
			case 0x03:	
				break;
						//speedsub-key
			case 0x04:	
				break;
						//up-key
			case 0x05:	
				break;
						//down-key
			case 0x06:	
				break;
						//oxy-key
			case 0x07:	
				break;
						//quickrun-key
			case 0x08:	
				break;
						//multi-media-key
			case 0x09:	
				break;
						//sound-open-key
			case 0x0a:	
				break;
						//sound-close-key
			case 0x0b:	
				break;
						//voladd-key
			case 0x0c:	
				break;
						//volsub-key
			case 0x0d:	
				break;
						//forward-key
			case 0x0e:	
				break;
						//backward-key
			case 0x0f:	
				break;
						//secretary-key
			case 0x10:	
				break;
						//field-mode-key
			case 0x11:	
				break;
						//freedom-mode-key
			case 0x12:	
				break;
						//smart-mode-key
			case 0x15:	
				break;
						//compitition-key
			case 0x16:	
				break;
						//motiontarget-key
			case 0x17:	
				break;
						//assistance-key
			case 0x18:	
				break;
						//realviews-key
			case 0x19:	
				break;
						//streetviews-key
			case 0x1a:	
				break;
							//home-key
			case 0x1b:	
				break;
						//set-key
			case 0x1c:	
				break;
						//confignetwork-key
			case 0x1d:	
				confignetworkkey=1;
				sendcommand.data_len=3;
				sendcommand.send_data[0]=0x0b;
				sendcommand.send_data[1]=0x01;
				sendcommand.send_data[2]=0x05;			
				uart0_write_data(sendcommand.send_data,sendcommand.data_len);
				break;
							//waiting for configuration-network command
							//to allow start config network
							//uart0 send build network status 
										//dark		0
										//bright		1
										//fastblink	2
										//slowblink	3			
			case 0x46:	
				confignetworkkey=0;
				sendcommand.data_len=3;
				sendcommand.send_data[0]=0x0b;
				sendcommand.send_data[1]=0x01;
				sendcommand.send_data[2]=0x05;			
				uart0_write_data(sendcommand.send_data,sendcommand.data_len);
				need_factory_reset = 1;
				
				os_printf("need_factory_reset =%d \n", need_factory_reset);
				cfgnetsta=0;
				break;

		}

			break;

							//read heartbeat data from maincontroller board for verifying the work status
		case 0x02:
			break;
							//if abnormal then ...
							//if ok then goto next
	
	//waiting for error-command 
		case 0x03:
			if(dat_in[1]==0x01&&dat_in[2]==0x01){
				manufacturetestmode=1;

			}
			break;
					//to set speed and angle to "0" to stop the device



//waiting for stepnumber
		case 0x07:
			break;

		case 0x09:
			switch(dat_in[1])
			{
		//waiting for speedminmaxvalue
				case 0x02:
					shakeok1=1;
					hankang_speed_min=dat_in[2];					
					hankang_speed_max=dat_in[3];
#ifdef HANKANG_DEBUG
					printf("speed_min:%x speed_max:%x",	hankang_speed_min,hankang_speed_max);
#endif
					sendcommand.data_len=3;
					sendcommand.send_data[0]=0x0b;
					sendcommand.send_data[1]=0x01;
					sendcommand.send_data[2]=0x09;			
					uart0_write_data(sendcommand.send_data,sendcommand.data_len);
					break;
#ifdef SLOPE_ENABLE	
		//waiting for upmaxvalue
				case 0x01:
					shakeok2=1;					
					hankang_slope_max=dat_in[2];
					break;
#endif

		//waiting for versionnumber
				case 0x03:
					break;

			}
			
			break;		

//waiting for currentspeed
		case 0xe0:
			break;
//waiting for currentangle
		case 0xe1:
			break;
//waiting for motor-calibration-finished
		case 0xe2:
			break;
		}

//end running-plan
	//uart0 send running-plan-end status








//=======================build by victor=========================

	return 0x00;
}

static void ICACHE_FLASH_ATTR user_key_cycle_handle(void)
{
	u32 sys_time_value = system_get_time();
	static u32 key_press_start = 0;
	if(GPIO_INPUT_GET(GPIO_ID_PIN(USER_CFG_KEY_GPIO_NO)) == 0x00)
	{
		if(key_press_start == 0)
		{
			vTaskDelay(30 / portTICK_RATE_MS);	 	// 30 ms
			if(GPIO_INPUT_GET(GPIO_ID_PIN(USER_CFG_KEY_GPIO_NO)) == 0x00)  // press low
			{
				vTaskDelay(30 / portTICK_RATE_MS);  // 30 ms
				if(GPIO_INPUT_GET(GPIO_ID_PIN(USER_CFG_KEY_GPIO_NO)) == 0x00)  // press low
				{
					key_press_start = system_get_time();
					ESP_DBG(("key pressed"));
				}
			}
		}
		else
		{	
			if(( (system_get_time()-key_press_start)/1000)>=4*1000)
			{
				while(1)
				{
					setLed1State(1);
					vTaskDelay(300 / portTICK_RATE_MS);  // 300 ms
					setLed1State(0);
					vTaskDelay(300 / portTICK_RATE_MS);  // 300 ms
					if(GPIO_INPUT_GET(GPIO_ID_PIN(USER_CFG_KEY_GPIO_NO)) != 0x00)
						break;
				}
			}
		}
	}
	else
	{
		if(key_press_start != 0x00)
		{
			ESP_DBG(("press[%d]ms", (system_get_time()-key_press_start)/1000));
			if(4*1000>( (system_get_time()-key_press_start)/1000)>0)
			{
				ESP_DBG(("short pressed"));
				user_key_short_press();
				
			}
			else if(( (system_get_time()-key_press_start)/1000)>=4*1000) // >5s press
			{
				
				ESP_DBG(("long pressed"));
				user_key_long_press();
			}
			else
			{
				ESP_DBG(("no pressed"));
			}

			key_press_start = 0;
		}
	}
	
	return;
}




int shakeok=0;
int shakeok1=0;
int shakeok2=0;
void ICACHE_FLASH_ATTR hankang_user_uart_task(void *pvParameters)
{
    CusUartIntrPtr uartptrData;
	u32 sys_time_value = system_get_time();
	u32 key_press_start = 0;
	int ret = 0;
	char uart_beat_data[]={0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};  // test uart beat data "12345678"


	
#ifdef HANKANG_DEBUG 
	ESP_DBG((" hankang_user_uart_task"));
#endif



//===========================uart protocl=======================victor
	
//-----------------------------uart shake protocl----------------------------


	
/**/

//---------------------------open the heartbeat TIMER----------------------------


#ifdef HEARTBEATTIMER

//
	os_timer_disarm(&xTimerHeartbeat);
	os_timer_setfn(&xTimerHeartbeat, (os_timer_func_t *)hankang_heartbeat_proc, NULL);
	os_timer_arm(&xTimerHeartbeat, T_ms, 1);
	
#endif
//-----------------------------------------------------------built by victor




	while(1)
	{
		user_key_cycle_handle();	// a demo key func	
		
		vTaskDelay(50 / portTICK_RATE_MS);	 // 100 ms

//victor

         
		if((system_get_time()-sys_time_value)>=(30*1000*1000))	//about 2min, send data to uart0, demo beat data
				{
					ESP_DBG(("uart beat data.[%d][%d]",sys_time_value,system_get_time()));
					ESP_DBG(("***free heap_size %d*****\n", system_get_free_heap_size()));
				//	uart0_write_data(uart_beat_data,sizeof(uart_beat_data));
					sys_time_value = system_get_time();
			}
		if(shakeok1==0)
		{
			//while(shakeok1==0){
				sendcommand.data_len=3;//hankang_uart_shakehand_data[]={0x09, 0x01, 0x01};  //
				sendcommand.send_data[0]=0x09;
				sendcommand.send_data[1]=0x01;
				sendcommand.send_data[2]=0x01;
				uart0_write_data(sendcommand.send_data,sendcommand.data_len);
				//uart0_write_data(hankang_uart_shakehand_data,sizeof(hankang_uart_shakehand_data));
			//}
		}
		vTaskDelay(100 / portTICK_RATE_MS);	 // 200 ms
		
#ifdef SLOPE_ENABLE		
		if(shakeok2==0)
		{
			//while(shakeok2==0){	
				//hankang_uart_shakehand_data[]={0x09, 0x01, 0x02};  //
				sendcommand.data_len=3;//hankang_uart_shakehand_data[]={0x09, 0x01, 0x01};  //
				sendcommand.send_data[0]=0x09;
				sendcommand.send_data[1]=0x01;
				sendcommand.send_data[2]=0x02;
				uart0_write_data(sendcommand.send_data,sendcommand.data_len);
				//uart0_write_data(hankang_uart_shakehand_data,sizeof(hankang_uart_shakehand_data));	
			//}
		}
#endif



	//waiting for running-plan data receiving ok
		//uart0 send data-ready-ok status	
		//to allow sending runner-plan data package to maincontroller board

#ifdef TESTHAVEAPLAN
		
		if(testhaveaplan==1)
		{

			//cloudplandata.send_data=send_data_sample;
			haveaplan=1;
			runningmachineisbusy=0;
			uartsendsuccessfully=0;
			//int running_plan_data_receiving_ok=1;
		}
#endif
//config network
#if 0//#ifdef HANKANG_DEBUG
		ESP_DBG(("cfgnetsta=%d\n",cfgnetsta));
#endif

		//ESP_DBG(("alink_get_time=%d\n",alink_get_time()));
		if((confignetworkkey==1&&cfgnetsta==0)||(confignetworkkey==1&&cfgnetsta==1)||(confignetworkkey==1&&cfgnetsta==3))
		{
			//confignetworkkey=0;
#if 1//#ifdef HANKANG_DEBUG
			ESP_DBG(("config network need to restart..."));
#endif
			setSmartConfigFlag(0x1);
			vTaskDelay(500 / portTICK_RATE_MS);	 // 1000 ms
			system_restart();
			//user_key_short_press();
			
		}

#ifdef QUEUE_FUNC
		if (xQueueReceive(xQueueCusUart, (void *)&uartptrData, (portTickType)500/*portMAX_DELAY*/)) // wait about 5sec 
		{
//			ESP_DBG(("data uart recv.."));
//			debug_print_hex_data(uartptrData.rx_buf,uartptrData.rx_len);
//			ESP_DBG(("xQueueCusUart re_len=%d\n",uartptrData.rx_len));

			if(uartptrData.rx_len>0x00){
				cus_uart_data_handle(uartptrData.rx_buf, uartptrData.rx_len,NULL);
			}
		}
#endif

	if(haveaplan==1&&uartsendsuccessfully==0&&convertdatatypisok==0)
	{
		
		vTaskDelay(1 / portTICK_RATE_MS);	 // 2 ms	
		//////ESP_DBG(("BEAT...aplandatasent "));

		strcpy(send_data_DetailPlans,"");
		strcat(send_data_DetailPlans,cloudplandata.head);
		int i;		
		for(i=0;i<36;i++)
		{
			send_data_DetailPlans[i+2]=value_byte_DetailPlans[i];
		}

#ifdef HANKANG_DEBUG
		printf("\n");
		printf("send_data_DetailPlans:\n");

		for(i=0;i<38;i++)
			printf("0x%2X ",send_data_DetailPlans[i]);
		printf("send_data_DetailPlans====\n");
#endif
		convertdatatypisok=1;
	
	}


	if(manufacturetestmode==1){


			
		

		while(1){

			printf("---------------manufacture test mode---------------\n");

			




		}


	}




		#if 0
		if((system_get_time()-sys_time_value)>=(60*1000*1000))  //about 1min, send data to uart0, demo beat data
		{
			ESP_DBG(("uart beat data.[%d][%d]",sys_time_value,system_get_time()));
			ESP_DBG(("heap_size %d\n", system_get_free_heap_size()));
			uart0_write_data(uart_beat_data,sizeof(uart_beat_data));
			sys_time_value = system_get_time();
		}
    
		#endif


	}

    vTaskDelete(NULL);
	
}

void ICACHE_FLASH_ATTR user_uart_dev_start(void)
{
	spi_flash_read((ALINK_USER_CFG_SEC) * SPI_FLASH_SEC_SIZE,(uint32 *)&alink_user_cfg_data, sizeof(ALINK_USER_OWN_CFG));
	ESP_DBG(("cus init uart debug flag[%x]",alink_user_cfg_data.uart_debug_en));
	if(alink_user_cfg_data.uart_debug_en == 0x01)  // init debug cfg
		user_uart_print_flag = 0x01;
	else
		user_uart_print_flag = 0x00;

	//removed by victor
	//uart_init_new();   // cfg uart0 connection device MCU, cfg uart1 TX debug output
#ifdef QUEUE_FUNC
    xQueueCusUart = xQueueCreate((unsigned portBASE_TYPE)CUS_UART0_QUEUE_LENGTH, sizeof(CusUartIntrPtr));
#endif
//victor
//initial SOFT TIMER for heartbeat 
 ETS_UART_INTR_ENABLE();
    xTaskCreate(hankang_user_uart_task, (uint8 const *)"uart", /*128*//*256*/512+64, NULL, tskIDLE_PRIORITY + 2, NULL);

	return;
}

#endif

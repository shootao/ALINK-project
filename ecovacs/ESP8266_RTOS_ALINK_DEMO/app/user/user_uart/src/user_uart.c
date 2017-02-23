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

#include "c_types.h"
#include <stdio.h>
#include <string.h>
#include "esp_common.h"
#include "user_uart.h"
#include "../../../include/driver/gpio.h"
#include "../../../include/driver/key.h"

//#include "user_light.h"

#if USER_UART_CTRL_DEV_EN

#include "../../../include/driver/uart.h"

xQueueHandle xQueueCusUart;
MCU2WIFI_DATA mcu2wifi_data;

void debug_print_hex_data(char*buf, int len)
{
	int i = 0;
	os_printf("\n____[%d]___\n",len);
	for(i=0;i<len;i++)
	{
		printf("%X ",*(buf+i));
	}
	return;
}	

int ICACHE_FLASH_ATTR uart0_write_data(u8 *data, int len)
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

static void ICACHE_FLASH_ATTR cus_handle_map_data(char *data)
{
	u8 dat_p;
	ESP_DBG(("MAP DATA."));
	dat_p = *(data+1)+5;
	debug_printf_data(data,dat_p+1);
	if((*(data+dat_p)!=0xfc)&&(*(data+(dat_p-1))!=0x33))
	{
		ESP_DBG(("map_data error[%x]",*(data+dat_p)));
		return;
	}
	///////////////
	ESP_DBG(("handle map data."));

	/////////////

	return;
}
static u8 ICACHE_FLASH_ATTR cus_uart_data_handle(char *dat_in, int in_len, char *dat_out)
{
	ESP_DBG(("uart data handler.."));
	debug_print_hex_data(dat_in, in_len);
	u8 need_post_flag;
	need_post_flag = 0;
	u8 dat_p;
	dat_p = 0x01;
	if(in_len == 16)  // status cmd
	{
		if((dat_in[0]!=0xc3)||(dat_in[15]!=0xfc))
		{
			ESP_DBG(("cmd chk error."));
			return 0x00;
		}
		memcpy((char*)&mcu2wifi_data, dat_in, sizeof(MCU2WIFI_DATA));
		
		need_post_flag =1;
	}
	else if(in_len == 3)  // sys cmd
	{
		if((dat_in[0]==0xc3)&&(dat_in[1]==0xd1)&&((dat_in[2]==0xfc)))  // normal data
		{
			ESP_DBG(("smartcofig start...."));
			user_key_short_press();
		}
		if((dat_in[0]==0xc3)&&(dat_in[1]==0xcb)&&((dat_in[2]==0xfc)))  // normal data
		{
			ESP_DBG(("reset device.."));
			user_key_long_press();
		}
		
		if((dat_in[0]==0xc3)&&(dat_in[1]==0xcc)&&((dat_in[2]==0xfc)))  // normal data
		{
			ESP_DBG(("factory test mode.."));
		
		}
		
		need_post_flag =0x00;
	}
	else // for mul packet
	{
		if((in_len > 16)||(*(dat_in+0)==0xcc))
		{
			if((*(dat_in+0)== 0xc3)&&(*(dat_in+15)==0xfc)) // enter smartconfig
			{
				memcpy((char*)&mcu2wifi_data, dat_in, sizeof(MCU2WIFI_DATA)); //dev status
				dat_p = 16;
				
				ESP_DBG(("%x",*(dat_in+dat_p)));
				need_post_flag =1;
				
			}
			if(*(dat_in+dat_p) == 0xcc) // map data
			{
				cus_handle_map_data((dat_in+dat_p));
				
				need_post_flag =1;
			}
			else
			{
				ESP_DBG(("DATA ERROR..."));
			}
		}
		
	}

	if(need_post_flag)
	{
		if(!device_status_change)
			device_status_change = 1;	// event send current real device status to the alink server

	}
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

void ICACHE_FLASH_ATTR user_uart_task(void *pvParameters)
{
    CusUartIntrPtr uartptrData;
	u32 sys_time_value;
	sys_time_value = system_get_time();
	int ret;
	ret = 0;
	memset((char*)&mcu2wifi_data, 0, sizeof(MCU2WIFI_DATA));
//	char uart_beat_data[]={0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};  // test uart beat data "12345678"
	while(1)
	{
		user_key_cycle_handle();	// a demo key func	
		
		vTaskDelay(10 / portTICK_RATE_MS);	 // 100 ms
		#if 1

		if(xQueueReceive(xQueueCusUart, (void *)&uartptrData, (portTickType)50/*portMAX_DELAY*/)) // wait about 500 MS 
		{
			ESP_DBG(("uart recv.."));
			debug_print_hex_data(uartptrData.rx_buf,uartptrData.rx_len);

			if(uartptrData.rx_len>0x00){
				cus_uart_data_handle(uartptrData.rx_buf, uartptrData.rx_len,NULL);
			}
		}

		if((system_get_time()-sys_time_value)>=(15*1000*1000))  //about 1min, send data to uart0, demo beat data
		{
			ESP_DBG(("uart beat data.[%d][%d]",sys_time_value,system_get_time()));
			ESP_DBG(("***heap_size %d*****\n", system_get_free_heap_size()));
		//	uart0_write_data(uart_beat_data,sizeof(uart_beat_data));
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

	uart_init_new();   // cfg uart0 connection device MCU, cfg uart1 TX debug output
    xQueueCusUart = xQueueCreate((unsigned portBASE_TYPE)CUS_UART0_QUEUE_LENGTH, sizeof(CusUartIntrPtr));
    xTaskCreate(user_uart_task, (uint8 const *)"uart", 128+128, NULL, tskIDLE_PRIORITY + 2, NULL);

	return;
}

#endif


#ifndef _TIME_H_
#define _TIME_H_

#define USER_TIME_EN  1
#define USER_SET_T 0

typedef unsigned int esp_time_t;
typedef long suseconds_t;

struct esp_tm {
	int tm_sec;         /* seconds [0-59] */
	int tm_min;         /* minutes [0-59] */
	int tm_hour;        /* hours [0-23] */
	int tm_mday;        /* day of the month [1-31] */
	int tm_mon;         /* month [0-11] */
	int tm_year;        /* year. The number of years since 1900 */
	int tm_wday;        /* day of the week [0-6] 0-Sunday...6-Saturday */
};

//int dev_timer_updata_by_wifi();
int esp_real_time_set(struct esp_tm *esp_tm);
int esp_real_time_get(struct esp_tm *esp_tm);
unsigned int Real_time_to_sec(struct esp_tm *esp_tm);
int Sec_to_real_time(struct esp_tm *esp_tm, unsigned int sec);
int Real_time_init(void);
int esp_rtc_time_set(esp_time_t time);
unsigned int esp_rtc_time_get(void);
void rtc_init(void);


#endif/*_TIME_H_*/

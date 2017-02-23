
#include "c_types.h"
#include "esp_common.h"

#include "../include/driver/user_time.h"


#if USER_TIME_EN

//typedef long time_t;

#define NULL ((void *)0)
unsigned int esp_clock_tick_count;

static unsigned int SEC_PER_YR[2] = { 31536000, 31622400 };
static unsigned int SEC_PER_MT[2][12] =  {
{ 2678400, 2419200, 2678400, 2592000, 2678400, 2592000, 2678400, 2678400, 2592000, 2678400, 2592000, 2678400 },
{ 2678400, 2505600, 2678400, 2592000, 2678400, 2592000, 2678400, 2678400, 2592000, 2678400, 2592000, 2678400 },
};
static unsigned int SEC_PER_DY = 86400;
static unsigned int SEC_PER_HR = 3600;

#if 0
static int month_from_string_short(const char *month)
{
	if (strncmp(month, "Jan", 3) == 0)
		return 0;
	if (strncmp(month, "Feb", 3) == 0)
		return 1;
	if (strncmp(month, "Mar", 3) == 0)
		return 2;
	if (strncmp(month, "Apr", 3) == 0)
		return 3;
	if (strncmp(month, "May", 3) == 0)
		return 4;
	if (strncmp(month, "Jun", 3) == 0)
		return 5;
	if (strncmp(month, "Jul", 3) == 0)
		return 6;
	if (strncmp(month, "Aug", 3) == 0)
		return 7;
	if (strncmp(month, "Sep", 3) == 0)
		return 8;
	if (strncmp(month, "Oct", 3) == 0)
		return 9;
	if (strncmp(month, "Nov", 3) == 0)
		return 10;
	if (strncmp(month, "Dec", 3) == 0)
		return 11;
	/* not a valid date */
	return 12;
}
#endif
/**
 * Returns 1 if current year id a leap year
 */
static inline int is_leap(int yr)
{
	if (!(yr%100))
		return (yr%400 == 0) ? 1 : 0;
	else
		return (yr%4 == 0) ? 1 : 0;
}

static unsigned char day_of_week_get(unsigned char month, unsigned char day, unsigned short year)
{
	/* Month should be a number 0 to 11, Day should be a number 1 to 31 */

	static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	year -= month < 3;
	return (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;
}

static esp_time_t esp_mktime(const struct esp_tm *esp_tm)
{
	int i;
	int leapyr=0;

	esp_time_t ret = 0;

	for (i = 1970; i < (esp_tm->tm_year + 1900); i++)
		ret += SEC_PER_YR[is_leap(i)];

	if (is_leap(esp_tm->tm_year + 1900))
		leapyr = 1;

	for (i = 0; i < (esp_tm->tm_mon); i++) {
		ret += SEC_PER_MT[leapyr][i];
	}

	ret += ((esp_tm->tm_mday)-1) * SEC_PER_DY;
	ret += (esp_tm->tm_hour) * SEC_PER_HR;
	ret += (esp_tm->tm_min) * 60;
	ret += esp_tm->tm_sec;

	return ret;
}

static struct esp_tm *gmtime_r(const esp_time_t *esp_tm, struct esp_tm *result)
{
	int leapyr = 0;
	esp_time_t ltm = *esp_tm;

	memset(result, 0, sizeof(struct esp_tm));
	result->tm_year = 1970;

	while(1) {
		if(ltm < SEC_PER_YR[is_leap(result->tm_year)]) {
			break;
		}
		ltm -= SEC_PER_YR[is_leap(result->tm_year)];
		result->tm_year++;
	}

	leapyr = is_leap(result->tm_year);

	while(1) {
		if(ltm < SEC_PER_MT[leapyr][result->tm_mon])
			break;
		ltm -= SEC_PER_MT[leapyr][result->tm_mon];
		result->tm_mon++;
	}

	result->tm_mday = ltm / SEC_PER_DY;
	result->tm_mday++;
	ltm = ltm % SEC_PER_DY;

	result->tm_hour = ltm / SEC_PER_HR;
	ltm = ltm % SEC_PER_HR;

	result->tm_min = ltm / 60;
	result->tm_sec = ltm % 60;

	result->tm_wday = day_of_week_get(result->tm_mon + 1, result->tm_mday, result->tm_year);

	/*
	 * Solve bug WMSDK-27. 'man gmtime' says:
	 * "tm_year   The number of years since 1900."
	 */
	result->tm_year -= 1900;

	return result;
}

static int validate_date_time(const struct esp_tm *esp_tm)
{
	if (esp_tm->tm_sec > 59)
		return -1;
	if (esp_tm->tm_min > 59)
		return -1;
	if (esp_tm->tm_hour > 23)
		return -1;
	if (((esp_tm->tm_year + 1900) < 1970) || ((esp_tm->tm_year + 1900) > 2111))
		return -1;
	if (esp_tm->tm_mon >= 12)
		return -1;
	if (esp_tm->tm_mon == 1) {
		if (!is_leap(esp_tm->tm_year + 1900)) {
			if (esp_tm->tm_mday > 28)
				return -1;
		} else  {
			if (esp_tm->tm_mday > 29)
				return -1;
		}
	}
	switch (esp_tm->tm_mon) {
		case 3:
		case 5:
		case 8:
		case 10:
			if(esp_tm->tm_mday > 30)
				return -1;
	}

	if ( (esp_tm->tm_mday < 1) || (esp_tm->tm_mday > 31) )
		return -1;

   	return 0;
}

/**
 * Set date and time
 */
 unsigned int Real_time_to_sec(struct esp_tm *esp_tm)
{
	esp_time_t time;

	esp_tm->tm_year -= 1900;
	esp_tm->tm_mon -= 1;
	
	if(validate_date_time(esp_tm) == 0) 
	{
		time = esp_mktime(esp_tm);
		esp_tm->tm_year += 1900;  //bill add
		esp_tm->tm_mon += 1;     //bill add
		return time;
	} 
	else 
	{
		esp_tm->tm_year += 1900;   //bill add
		esp_tm->tm_mon += 1;     //bill add
		return 0xFFFFFFFF;
	}
}

int esp_rtc_time_set(esp_time_t time)
{
#if USER_SET_T
//	esp_clock_tick_count=(int)time;
	struct timeval	iots_tm;
	iots_tm.tv_sec = (long)time;
	
	iots_tm.tv_usec = 0;
	settimeofday(&iots_tm, NULL);
#endif
	return 1;
}
unsigned int esp_rtc_time_get(void)
{
#if USER_SET_T
	struct timeval  iots_tm;
	unsigned int tmp;
	gettimeofday(((struct timeval*)&iots_tm),(void *) NULL);	
	tmp = iots_tm.tv_sec;

	return tmp;
#endif
	return 0;
}

void rtc_init(void)
{
#if USER_SET_T
	struct timeval  iots_tm;
	iots_tm.tv_sec = 0;
	iots_tm.tv_usec = 0;
	settimeofday((const struct timeval*)&iots_tm, NULL);
#endif
	return;
}

int Sec_to_real_time(struct esp_tm *esp_tm, unsigned int sec)
{
	esp_time_t curtime;
	curtime = sec;

	if(gmtime_r((const esp_time_t *)&curtime, esp_tm) == NULL) 
	{
		return -1;
	}

	esp_tm->tm_year += 1900;
	esp_tm->tm_mon += 1;
	return 0;
}

int esp_real_time_set(struct esp_tm *esp_tm)
{
#if USER_SET_T
	esp_time_t time;

	esp_tm->tm_year -= 1900;
	esp_tm->tm_mon -= 1;
	
	if(validate_date_time(esp_tm) == 0) 
	{
		time = esp_mktime(esp_tm);
		return esp_rtc_time_set(time);
	} 
	else 
	{
		return -1;
	}

#endif
	return -1;
}

/**
 * Get date and time
 */
int esp_real_time_get(struct esp_tm *esp_tm)
{
#if USER_SET_T

	esp_time_t curtime;
	curtime = esp_rtc_time_get();
	if(gmtime_r((const esp_time_t *)&curtime, esp_tm) == NULL) 
	{
		return -1;
	}

	esp_tm->tm_year += 1900;
	esp_tm->tm_mon += 1;
	return 0;
#endif 
	return 0;
}


int Real_time_init(void)
{
#if USER_SET_T

	static char timer_init_done=0;
	
	if (timer_init_done)
		return 0;

	rtc_init();

	timer_init_done = 1;
	return 1;
#endif
	return 1;
}




#if 0
int dev_timer_updata_by_wifi()
{
	int ret;	
	extern u8 m2m_sta_linked;
	if(m2m_sta_linked != 0)
	{
		if((ret=NTP_update_time()) < 0)
		{
			HF_DBG(("%s","updata timer failed!!!"));
		}
		else
		{
			HF_DBG(("UPDATA time and set timer"));
			esp_rtc_time_set(ret);

		//	sprintf(rep, "+ok=%d-%d-%d  %d:%d:%d  %s",tm_q.tm_year, tm_q.tm_mon, tm_q.tm_mday,tm_q.tm_hour , tm_q.tm_min , tm_q.tm_sec , weekday);	
			return 0;
		}
	}
	else
	{
		HF_DBG(("sta not conn!!"));
		
	}
	return ret;
}

#endif

#endif

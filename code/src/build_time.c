/**
* @file    build_time.c
* @brief   Print Porgram Build Time.
* @author  think3r
* @date    2017/12/23

* @note \n History:
  1.日    期: 2017/12/23
    作    者: think3r
    修改历史: 创建文件
*/

/*--------------------------------------------------------*/
/*                        文件包含                            */
/*--------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include "build_time.h"
#include "string.h"
#include "time.h"

/*--------------------------------------------------------*/
/*                          宏定义                           */
/*--------------------------------------------------------*/
//#define __DEBUG__

#ifdef __DEBUG__
    #define DEBUG(format,...) printf("*["__FILE__" L:%04d] |  "format"", __LINE__, ##__VA_ARGS__)
#else
    #define DEBUG(format,...)
#endif

/*
1. 一三五七八十腊，三十一天永不差，是说公历里一三五七八十十二月是31天，除二月分闰年（29天
    ）平年（28天）外其他月（四六九十一月）是30天，恒久不变的。#严格来说，腊月的叫法也该归到农历里。
2. 闰年平年的规定：如果某年能被4整除即闰年，否则为平年。例，2015除以4得503有余数即为不整除,
   2015年为平年，那么今年二月就有28天，而2016年就是闰年了。
   如果遇到整百年如2000年需被400整除   才算闰年。

作者：纳汀罕卓德
链接：https://www.zhihu.com/question/19952385/answer/37259688
来源：知乎
*/


/*--------------------------------------------------------*/
/*                        全局变量                            */
/*--------------------------------------------------------*/
static const char *short_char_months[] = {
    " ", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static const char *long_char_months[] = {
    " ", "January", "February", "March", "April", "May", "June",
         "July", "Aug", "September", "October", "November", "December"
};
static const char *long_char_week[]={
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};
static const char *short_char_week[]={
    "Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"
};

/*--------------------------------------------------------*/
/*                        函数定义                            */
/*--------------------------------------------------------*/

/**
 * @function:   getTime_ms
 * @brief:      获取当前时间 ms
 * @param[in]:  void
 * @param[out]: None
 * @return:     inline uint32_t
 */
uint32_t getTime_ms(void)
{
    struct timespec curTime;

    clock_gettime(CLOCK_MONOTONIC, &curTime);
    return (uint32_t)((curTime.tv_sec * 1000) + curTime.tv_nsec / 1000000);
}

/**
 * @function:   sleep_ms
 * @brief:      sleep ms
 * @param[in]:  uint32_t u32Ms  
 * @param[out]: None
 * @return:     int32_t
 */
int32_t sleep_ms(uint32_t u32Ms)
{    
    return usleep(u32Ms * 1000);
}

/**
 * @function:   get_Now_Time (需调用库 "time.h" 中的库函数)
 * @brief:      生成程序编译时间
 * @param[in]:  DATE_TIME *pDate / NULL       
 * @param[out]: DATE_TIME *pDate / NULL
 * @return:     err
 */
unsigned int get_Now_Time(DATE_TIME *pDate, ...)
{
    time_t local_time_s = 0;
    TM *pTblock = NULL;

  //time(&local_time_s);		//得到从标准计时点（一般是1970年1月1日午夜）到当前时间的秒数。
	local_time_s = time(NULL);  //得到从标准计时点（一般是1970年1月1日午夜）到当前时间的秒数。

    pTblock = (TM *)localtime(&local_time_s);

    {
        DEBUG("date and time is total_sec:[%d] \n", local_time_s);

        DEBUG("ctime is %s\n", ctime(&local_time_s));   //得到日历时间, 将 s 转换为字符串形式;

        DEBUG("\t pTblock:%4u-%u-%u %02u:%02u:%02u\n",
                pTblock->tm_year + 1900, pTblock->tm_mon, pTblock->tm_mday,
                pTblock->tm_hour, pTblock->tm_min, pTblock->tm_sec);
    }

    if (pDate)
    {
        pDate->year = (unsigned int)pTblock->tm_year + 1900u;
        pDate->month =(unsigned int)pTblock->tm_mon;
        pDate->day = (unsigned int)pTblock->tm_mday;
        pDate->hour = (unsigned int)pTblock->tm_hour;
        pDate->minute = (unsigned int)pTblock->tm_min;
        pDate->second = (unsigned int)pTblock->tm_sec;
        pDate->dayOfWeek = (unsigned int)pTblock->tm_wday;

    #ifdef SHORT_DATA_CHAR__
        strcpy(pDate->char_month, short_char_months[pDate->month]);
        strcpy(pDate->char_dayOfWeek, short_char_week[pDate->dayOfWeek]);
    #else
        strcpy(pDate->char_month, long_char_months[pDate->month]);
        strcpy(pDate->char_dayOfWeek, long_char_week[pDate->dayOfWeek]);
    #endif

        printf("      now Time:\t %4u-%u-%u_%s %02u:%02u:%02u \n",
                pDate->year, pDate->month, pDate->day, pDate->char_dayOfWeek,
                pDate->hour, pDate->minute, pDate->second);
    }
    else
    {

    #ifdef SHORT_DATA_CHAR__
        printf("[null] now Time:\t%4u-%u-%u_%s %02u:%02u:%02u\n",
                    pTblock->tm_year + 1900, pTblock->tm_mon, pTblock->tm_mday, short_char_week[pTblock->tm_wday],
                    pTblock->tm_hour, pTblock->tm_min, pTblock->tm_sec);
    #else
        printf("[null] now Time:\t%4u-%u-%u_%s %02u:%02u:%02u\n",
                    pTblock->tm_year + 1900, pTblock->tm_mon, pTblock->tm_mday, long_char_week[pTblock->tm_wday],
                    pTblock->tm_hour, pTblock->tm_min, pTblock->tm_sec);
    #endif

    }

    return OK;
}



/**
 * @function:   Calc_Week_Day
 * @brief:      计算输入年月日对应的星期               
 * @param[in]:  unsigned int year       
 * @param[in]:  unsigned int month  
 * @param[in]:  unsigned int day         
 * @param[out]: None
 * @return:     unsigned char   0:星期日, 6:星期六
 */
unsigned char __Calc_Week_Day(int year, int month, int day)
{
 /*
unsigned char __Calc_Week_Day( DATE_TIME *pDate)
{
    unsigned int year =  pDate->year, month = pDate->month, day = pDate->day;
*/

    unsigned int i = 0, days = 0, s = 0;
    unsigned char week_number = 0;
    unsigned char month_days[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};

    /*计算公历闰年*/
    if (0 == year % 4)
    {
        month_days[2] = 29;

        if(0 == year % 100)
        {
            if(0 == year % 400)
            {
                month_days[2] = 29;
            }
            else
            {
                month_days[2] = 28;
            }
        }
    }

    for(i = 0; i<month; i++)
    {
        days += month_days[i];
    }

    days += day;

    s = year-1 + (int)((year-1)/4) - (int)((year-1)/100) + (int)((year-1)/400) + days;

    week_number = (unsigned char)(s % 7);

    DEBUG("[%s L:%d] %d-%d-%d is [%s %d].\n",
            __func__, __LINE__, year, month, day, long_char_week[week_number], week_number);

    return week_number;
}

/**
 * @function:   mk_Build_Date
 * @brief:      生成程序编译时间
 * @param[in]:  DATE_TIME *pDate  / NULL
 * @param[out]: DATE_TIME *pDate  / NULL
 * @return:     err
 */
unsigned int mk_Build_Date(DATE_TIME *pDate, ...)
{
    int	year = 0, month = 0, day = 0;
    int hour = 0, minute = 0, seconds = 0;
    char m[4] = {0};

    sscanf(__DATE__, "%s%2d%4d", m, &day, &year);

    for(month = 0; month < 12; month++)
    {
        if (strcmp(m, short_char_months[month]) == 0)
        {
            break;
        }
    }

    sscanf(__TIME__, "%2d:%2d:%2d", &hour, &minute, &seconds);

    if (pDate)
    {
        pDate->year = year;
        pDate->month = month;
        pDate->day = day;
        pDate->hour = hour;
        pDate->minute = minute;
        pDate->second = seconds;
        pDate->dayOfWeek = __Calc_Week_Day(year, month, day);

    #ifdef SHORT_DATA_CHAR__
        strcpy(pDate->char_month, short_char_months[month]);
        strcpy(pDate->char_dayOfWeek, short_char_week[pDate->dayOfWeek]);
    #else
        strcpy(pDate->char_month, long_char_months[month]);
        strcpy(pDate->char_dayOfWeek, long_char_week[pDate->dayOfWeek]);
    #endif

        printf("     ** Build at:\t%4u-%u-%u_%s %02u:%02u:%02u\n",
            pDate->year, pDate->month, pDate->day, pDate->char_dayOfWeek,
            pDate->hour, pDate->minute, pDate->second);

        {
            DEBUG("builddata: month:[%s]\n", pDate->char_month);
            DEBUG("builddata: week:[%d]\n", pDate->dayOfWeek);
        }
    }
    else
    {

    #ifdef SHORT_DATA_CHAR__
        Cprintf_underline("[null]  ** Build at:\t%4u-%u-%u_%s %02u:%02u:%02u\n",
                year, month, day, short_char_week[__Calc_Week_Day(year, month, day)],
                hour, minute,seconds);

    #else
        Cprintf_underline("****** Build at:  [%4u-%u-%u_%s %02u:%02u:%02u] ******\n\n",
                year, month, day, long_char_week[__Calc_Week_Day(year, month, day)],
                hour, minute,seconds);
    #endif

    }
    
    DEBUG("buildDate: %s %s\n", __DATE__, __TIME__);

    return OK;
}


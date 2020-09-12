/**
* @file    build_time.h
* @brief   Print Porgram Build Time.
* @author  think3r
* @date    2017/12/23

* @note \n History:
  1.日    期: 2017/12/23
    作    者: think3r
    修改历史: 创建文件
*/
#include "common.h"

#ifndef BUILD_TIME_H_
#define BUILD_TIME_H_

/*--------------------------------------------------------*/
/*                        宏配置区                            */
/*--------------------------------------------------------*/
#define GET_FLOAT_TIME_S(timeMs)    ( ((float)(timeMs)) / 1000.0f )

//#define SHORT_DATA_CHAR__       /*æ—¥æœŸå­—ç¬¦ä¸²ä½¿ç”¨ç¼©å†?*/
//#define BUILD_TIMES_C_TEST_     /*å¯¹æœ¬æ–‡ä»¶è¿›è¡Œæµ‹è¯•*/

typedef struct
{
    unsigned int    year;
    unsigned int    month;
    unsigned int    day;
    unsigned int    hour;
    unsigned int    minute;
    unsigned int    second;
    unsigned int    milliSecond;
             char   char_month[10];
    unsigned int    dayOfWeek;        /* 0:星期日-6:星期六 */
             char   char_dayOfWeek[10];
} DATE_TIME;


#ifndef _TM_DEFINED

typedef struct  {
    int tm_sec;     /* 秒 ? 取值区间为[0,59] */
    int tm_min;     /* 分 - 取值区间为[0,59] */
    int tm_hour;    /* 时 - 取值区间为[0,23] */
    int tm_mday;    /* 一个月中的日期 - 取值区间为[1,31] */
    int tm_mon;     /* 月份（从一月开始，0代表一月） - 取值区间为[0,11] */
    int tm_year;    /* 年份，其值等于实际年份减去1900 */
    int tm_wday;    /* 星期 ? 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 */
    int tm_yday;    /* 从每年的1月1日开始的天数 ? 取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推 */
    int tm_isdst;   /* 夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的进候，tm_isdst为0；
                        不了解情况tm=_isdst()为负。*/
}TM;

#define _TM_DEFINED

#endif

uint32_t getTime_ms(void);
int32_t sleep_ms(uint32_t u32Ms);
unsigned int get_Now_Time(DATE_TIME     *pDate, ...);
unsigned int mk_Build_Date(DATE_TIME *pDate, ...);

uint32_t getFileSize(const char * ps8FileName, uint64_t *pFileSize);
void * readFile(void * pMem, const char * ps8FileName, size_t fileSize);
uint32_t writeFile(void * pMem, const char * ps8FileName, const char * ps8WiteMode, size_t fileSize);

#endif

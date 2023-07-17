#define RTC_ADDR            0x6f

#define RTCSEC              0x00
#define RTCSEC_ST           7

#define RTCMIN              0x01

#define RTCHOUR             0x02
#define RTCHOUR_1224        6

#define RTCWKDAY            0x03
#define RTCWKDAY_OSCRUN     5
#define RTCWKDAY_PWRFAIL    4
#define RTCWKDAY_VBATEN     3

#define CONTROL             0x07
#define CONTROL_OUT         7
#define CONTROL_SQWEN       6
#define CONTROL_ALM1EN      5
#define CONTROL_ALM0EN      4
#define CONTROL_EXTOSC      3
#define CONTROL_CRSTRIM     2
#define CONTROL_SQWFS       0

#define OSCTRIM             0x08

#define PWRDNMIN            0x18
#define PWRDNHOUR           0x19

#define PWRUPMIN            0x1c
#define PWRUPHOUR           0x1d

#define RTC_SRAM_START      0x20
#define RTC_SRAM_END        0x5f
#include "RTC.h"
#include "lpc17xx_rtc.h"
#include "lpc17xx_libcfg_default.h"
#include "lcd.h"
#include "leds.h"
#include "cmsis_os.h"     
RTC_TIME tiempo;
char hora[20];
char fecha[20];
extern bool alarma;
//MANEJADOR INTERRUPCIONES
void RTC_IRQHandler(void)
{
	uint32_t minval;
	int i;
	/* This is increment counter interrupt*/
	if (RTC_GetIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE))
	{
		minval = RTC_GetTime (LPC_RTC, RTC_TIMETYPE_MINUTE);
		if(tiempo.SEC==0)
		{
			alarma=true;
		}
		// Clear pending interrupt
		RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE);
	}
}
//MAIN
void c_entry(void)
{
	/* Initialize debug via UART0
	 * – 115200bps
	 * – 8 data bit
	 * – No parity
	 * – 1 stop bit
	 * – No flow control
	 */

	/* RTC Block section ------------------------------------------------------ */
	// Init RTC module
	RTC_Init(LPC_RTC);
	
	RTC_ResetClockTickCounter(LPC_RTC);
	RTC_Cmd(LPC_RTC, ENABLE);
    
	/* Set current time for RTC */
	// Current time is 8:00:00PM, 2009-04-24
  RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, 0);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE, 0);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR,0);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH,1);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH, 1);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, 2022);
	
  /* Enable rtc (starts increase the tick counter and second counter register) */
//	RTC_CalibConfig(LPC_RTC, 5, RTC_CALIB_DIR_FORWARD);
//	RTC_CalibCounterCmd(LPC_RTC, ENABLE);

	/* Set the CIIR for second counter interrupt*/
	RTC_CntIncrIntConfig (LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
	/* Set ALARM time for second */
	RTC_SetAlarmTime (LPC_RTC, RTC_TIMETYPE_SECOND, 10);
    /* Enable RTC interrupt */
    NVIC_EnableIRQ(RTC_IRQn);
		RTC_ResetClockTickCounter(LPC_RTC);
}

void RTC_getTime_Date(void)
{
  tiempo.SEC = RTC_GetTime (LPC_RTC, RTC_TIMETYPE_SECOND);
  tiempo.MIN = RTC_GetTime (LPC_RTC, RTC_TIMETYPE_MINUTE);
  tiempo.HOUR = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_HOUR);
  
  tiempo.DOM = RTC_GetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH);
  tiempo.MONTH = RTC_GetTime (LPC_RTC, RTC_TIMETYPE_MONTH);
  tiempo.YEAR = RTC_GetTime(LPC_RTC, RTC_TIMETYPE_YEAR);
  
  sprintf(hora,"%.2d:%.2d:%.2d",tiempo.HOUR, tiempo.MIN,tiempo.SEC);
  EscribeFrase(hora,1);
  sprintf(fecha,"%.2d/%.2d/%.2d",tiempo.DOM, tiempo.MONTH,tiempo.YEAR);
  EscribeFrase(fecha,2);
}

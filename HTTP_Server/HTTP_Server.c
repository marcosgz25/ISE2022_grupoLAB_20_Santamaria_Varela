/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server.c
 * Purpose: HTTP Server example
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "cmsis_os.h"                   /* CMSIS RTOS definitions             */
#include "rl_net.h"                     /* Network definitions                */
#include "LEDS.h"
#include "lcd.h"
#include "ADC.h"
#include "RTC.h"
#include "time.h"
#include "rebotes.h"

RTC_TIME tiempo;
struct tm horaSNTP;
bool LEDrun;
bool LCDupdate;
char lcd_text[2][20+1];
bool alarma;
bool timer;
bool ledSNTP;

void get_time(void);
static void BlinkLed (void const *arg);
static void Display (void const *arg);
static void time_cback (uint32_t time);//PROTOCOLO SNTP

osThreadDef(BlinkLed, osPriorityNormal, 1, 0);
osThreadDef(Display, osPriorityNormal, 1, 0);

//TIMER
static void Timer2_Callback (void const *arg);                  // prototype for timer callback function

static osTimerId id2;                                           // timer id
static uint32_t  exec2;                                         // argument for the timer call back function
static osTimerDef (Timer2, Timer2_Callback);
 
 static void Timer2_Callback (void const *arg)
{
	get_time();
	ledSNTP=true;
}

void Init_Timers (void) {
  osStatus status;                                              // function return status
  // Create periodic timer
  exec2 = 2;
  id2 = osTimerCreate (osTimer(Timer2), osTimerPeriodic, &exec2);
  if (id2 != NULL) {    // Periodic timer created
    // start timer with periodic 1000ms interval
    status = osTimerStart (id2,180000);            
    if (status != osOK) {
      // Timer could not be started
    }
  }
}

//PULSADOR



uint16_t AD_in (uint32_t ch) {
  int32_t val = 0;

  if (ch == 0) {
    ADC_StartConversion();
    while (ADC_ConversionDone () < 0);
    val = ADC_GetValue();
  }
  return (val);
}

/// IP address change notification
void dhcp_client_notify (uint32_t if_num,
                         dhcpClientOption opt, const uint8_t *val, uint32_t len) {
  if (opt == dhcpClientIPaddress) {
    // IP address has changed
    sprintf (lcd_text[0],"IP address:");
    sprintf (lcd_text[1],"%s", ip4_ntoa (val));
    LCDupdate = true;
  }
}

/*----------------------------------------------------------------------------
  Thread 'Display': LCD display handler
 *---------------------------------------------------------------------------*/
static void Display (void const *arg) 
{
	
  init();
  LCD_reset();
  borrarLCD();
  copy_to_lcd();
	
  //sprintf (lcd_text[0], "");
  //sprintf (lcd_text[1], "Waiting for DHCP");
  LCDupdate = true;
  while(1) 
	{
		RTC_getTime_Date();	
    //RTC_ver_hora();
//  sprintf(fecha,"%.2d/%.2d/%.2d",dayRTC, monRTC,yearRTC);
//  sprintf(RTC_fecha,"%-20s",fecha);
//  Escribir_linea(RTC_fecha,1);
//      borrarLCD();
//			EscribeFrase(lcd_text[0],1);
//			EscribeFrase(lcd_text[1],2);
      copy_to_lcd();
//      LCDupdate = false;
    //}
    osDelay (250);
  }
}

/*----------------------------------------------------------------------------
  Thread 'BlinkLed': Blink the LEDs on an eval board
 *---------------------------------------------------------------------------*/
static void BlinkLed (void const *arg) {
  //const uint8_t led_val[16] = { 0x48,0x88,0x84,0x44,0x42,0x22,0x21,0x11,
  //                              0x12,0x0A,0x0C,0x14,0x18,0x28,0x30,0x50 };
  const uint8_t led_val[8]=  {0x48,0x88,0x84,0x44,0x42,0x22,0x21,0x11};
  int cnt = 0;
	int i;
  LEDrun = true;
  while(1) {
    // Every 100 ms
    if(ledSNTP)
    {
      ledSNTP=false;
      LED_SetOut (led_val[1]);
      osDelay(500);
      LED_SetOut (0);
      osDelay(500);
    }
		if(alarma)
		{
			for(i=0;i<5;i++)
			{
				LED_SetOut (led_val[0]);
				osDelay(500);
				LED_SetOut (0);
				osDelay(500);
			}
			alarma=false;
		}
    else if (LEDrun == true) {
      LED_SetOut (led_val[cnt]);
      if (++cnt >= sizeof(led_val)) {
        cnt = 0;
      }
    }
    osDelay (100);
  }
}

//SNTP
void get_time(void)
{
	const uint8_t ntp_server[4]={130,206,0,1};
	if(sntp_get_time(&ntp_server[0],time_cback)==netOK)
	{
    borrarLCD();
    EscribeFrase("Solicitud enviada",1);
    copy_to_lcd();
	}
	else
	{
    borrarLCD();
    EscribeFrase("Fallo SNTP",1);
    copy_to_lcd();
	}
    
}

static  void time_cback (uint32_t time)
{
    time_t tiemp;
    if(time==0){
      EscribeFrase("Error servidor ",2);
    
    }else{
      EscribeFrase("Hora SNTP        ",2);
      tiemp=time;
      horaSNTP=*localtime(&tiemp);
      RTC_Set_Time();
    }
}


/*----------------------------------------------------------------------------
  Main Thread 'main': Run Network
 *---------------------------------------------------------------------------*/
int main (void) {
  LED_Initialize     ();
  //Buttons_Initialize ();
  ADC_Initialize     ();
  net_initialize     ();
  osDelay(10000);
  get_time();
	c_entry();
	osThreadCreate (osThread(BlinkLed), NULL);
  osThreadCreate (osThread(Display), NULL);
  //Init_Thread_Rebotes();
  Inicializar_Pulsadores();
	Init_Timers();
  while(1) {
    net_main ();
    osThreadYield ();
  }
}

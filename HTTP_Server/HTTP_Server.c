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
#include "rl_net_lib.h" 
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
uint16_t ganancy=1;

uint8_t mod;

uint32_t valor;
uint32_t* direccion;
uint8_t valor_P2;

void get_time(void);
static void Display (void const *arg);
static void time_cback (uint32_t time);//PROTOCOLO SNTP

osThreadDef(Display, osPriorityNormal, 1, 0);


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
  LCDupdate = true;
  while(1) 
	{
		borrarLCD();
		RTC_getTime_Date();	
		sprintf (lcd_text[0],"%.2d:%.2d:%.2d %.2d/%.2d/%.2d",tiempo.HOUR, tiempo.MIN,tiempo.SEC,tiempo.DOM, tiempo.MONTH,tiempo.YEAR);
		sprintf (lcd_text[1],"La Ganancia es: %d",ganancy);
		EscribeFrase(lcd_text[0],1);
		EscribeFrase(lcd_text[1],2);
    copy_to_lcd();
    osDelay (250);
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
int main (void) 
{
  net_initialize     ();
  osDelay(10000);
  get_time();
  osThreadCreate (osThread(Display), NULL);
//	escribir_IAP();

  while(1) {
    net_main ();
    osThreadYield ();
  }
}

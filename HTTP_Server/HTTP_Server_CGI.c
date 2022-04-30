/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server_CGI.c
 * Purpose: HTTP Server CGI Module
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "rl_net.h"
#include "rl_net_lib.h"
#include "LEDS.h"
#include "RTC.h"

//#include "Board_LED.h"

// http_server.c

//extern uint8_t  get_button (void);

// net_sys.c
extern  LOCALM localm[];
#define LocM   localm[NETIF_ETH]

// Net_Config.c
extern struct tcp_cfg   tcp_config;
extern struct http_cfg  http_config;
#define tcp_NumSocks    tcp_config.NumSocks
#define tcp_socket      tcp_config.Scb
#define http_EnAuth     http_config.EnAuth
#define http_auth_passw http_config.Passw
extern uint16_t ganancy;
double umbral=0;
extern bool LEDrun;
extern bool LCDupdate;
extern char lcd_text[2][20+1];
char text_umbral[20];
bool interrupt;
extern void leer_LEDS(uint8_t P2, int modoleds);



// Local variables.
static uint8_t P2;
extern uint8_t valor_P2;
// My structure of CGI status variable.
typedef struct {
  uint16_t xcnt;
  uint16_t unused;
} MY_BUF;
#define MYBUF(p)        ((MY_BUF *)p)

// Process query string received by GET request.
void cgi_process_query (const char *qstr) {
  char var[40];

  do {
    // Loop through all the parameters
    qstr = http_get_env_var (qstr, var, sizeof (var));
    // Check return string, 'qstr' now points to the next parameter
    if (var[0] != 0) {
      // First character is non-null, string exists
      if (strncmp (var, "ip=", 3) == 0) {
        // Local IP address
        ip4_aton (&var[3], LocM.IpAddr);
      }
      else if (strncmp (var, "msk=", 4) == 0) {
        // Local network mask
        ip4_aton (&var[4], LocM.NetMask);
      }
      else if (strncmp (var, "gw=", 3) == 0) {
        // Default gateway IP address
        ip4_aton (&var[3], LocM.DefGW);
      }
      else if (strncmp (var, "pdns=", 5) == 0) {
        // Primary DNS server IP address
        ip4_aton (&var[5], LocM.PriDNS);
      }
      else if (strncmp (var, "sdns=", 5) == 0) {
        // Secondary DNS server IP address
        ip4_aton (&var[5], LocM.SecDNS);
      }
    }
  } while (qstr);
}

// Process data received by POST request.
// Type code: - 0 = www-url-encoded form data.
//            - 1 = filename for file upload (null-terminated string).
//            - 2 = file upload raw data.
//            - 3 = end of file upload (file close requested).
//            - 4 = any XML encoded POST data (single or last stream).
//            - 5 = the same as 4, but with more XML data to follow.
void cgi_process_data (uint8_t code, const char *data, uint32_t len) {
  char var[40];
	interrupt=true;
  if (code != 0) {
    // Ignore all other codes
    return;
  }

  do {
    // Parse all parameters
    data = http_get_env_var (data, var, sizeof (var));
    if (var[0] != 0) {
      if (strcmp (var, "ctrl=Ganancia1") == 0) 
			{
        ganancy=1;
      }
			else if (strcmp (var, "ctrl=Ganancia5") == 0) 
			{
        ganancy=5;
      }
			else if (strcmp (var, "ctrl=Ganancia10") == 0) 
			{
        ganancy=10;
      }
			else if (strcmp (var, "ctrl=Ganancia50") == 0) 
			{
        ganancy=50;
      }
			else if (strcmp (var, "ctrl=Ganancia100") == 0) 
			{
        ganancy=100;
      }
			else if (strcmp (var, "ctrl=Enable") == 0) 
			{
				interrupt=false;
			}
      else if (strncmp (var, "lcd1=", 5) == 0)
			{
        strcpy (text_umbral, var+5);
        LCDupdate = true;
				umbral=atof(text_umbral);
				umbral=fabs(umbral);
				if(umbral>10)
				{
					umbral=10;
				}
      }
    }
  } while (data);
}

// Generate dynamic web data from a script line.
uint32_t cgi_script (const char *env, char *buf, uint32_t buflen, uint32_t *pcgi) {
  uint32_t len = 0;
  switch (env[0]) {
    case 'b':
      if (env[2] == 'c')
			{
				len=sprintf(buf, &env[4], ganancy);
				break;
      }
    case 'f':
				if (env[2] == 'c')
				{
				// Select Control
					len = sprintf (buf, &env[4], interrupt ? "" : "selected",
					interrupt ? "selected" : "" );
					break;
				}
				if(env[2]=='1')
				{
					len = sprintf (buf, &env[4], text_umbral);
					break;
				}
  }
  return (len);
}

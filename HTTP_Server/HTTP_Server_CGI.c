/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server_CGI.c
 * Purpose: HTTP Server CGI Module
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/

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
extern bool LEDrun;
extern bool LCDupdate;
extern char lcd_text[2][20+1];
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
      else if (strncmp (var, "lcd1=", 5) == 0) {
        // LCD Module line 1 text
        strcpy (lcd_text[0], var+5);
        LCDupdate = true;
      }
      else if (strncmp (var, "lcd2=", 5) == 0) {
        // LCD Module line 2 text
        strcpy (lcd_text[1], var+5);
        LCDupdate = true;
      }
    }
  } while (data);
	//leer_LEDS(P2,LEDrun);
}




// Generate dynamic web data from a script line.
uint32_t cgi_script (const char *env, char *buf, uint32_t buflen, uint32_t *pcgi) {
  TCP_INFO *tsoc;
  const char *lang;
  uint32_t len = 0;
  uint8_t id;
  static uint32_t adv;

  switch (env[0]) {
    // Analyze a 'c' script line starting position 2
    case 'a' :
      // Network parameters from 'network.cgi'
      switch (env[2]) {
        case 'i':
          // Write local IP address
          len = sprintf (buf, &env[4], ip4_ntoa (LocM.IpAddr));
          break;
        case 'm':
          // Write local network mask
          len = sprintf (buf, &env[4], ip4_ntoa (LocM.NetMask));
          break;
        case 'g':
          // Write default gateway IP address
          len = sprintf (buf, &env[4], ip4_ntoa (LocM.DefGW));
          break;
        case 'p':
          // Write primary DNS server IP address
          len = sprintf (buf, &env[4], ip4_ntoa (LocM.PriDNS));
          break;
        case 's':
          // Write secondary DNS server IP address
          len = sprintf (buf, &env[4], ip4_ntoa (LocM.SecDNS));
          break;
      }
      break;

    case 'b':
      // LED control from 'led.cgi'
      if (env[2] == 'c') {
        // Select Control
//        len = sprintf (buf, &env[4], ganancy ?     ""     : "selected",
//                                     ganancy ? "selected" :    ""     );
				len=sprintf(buf, &env[4], ganancy);
      }
			break;
    case 'c':
      // TCP status from 'tcp.cgi'
      while ((len + 150) < buflen) {
        tsoc = &tcp_socket[MYBUF(pcgi)->xcnt];
        MYBUF(pcgi)->xcnt++;
        // 'sprintf' format string is defined here
        len += sprintf (buf+len,   "<tr align=\"center\">");
        if (tsoc->State <= tcpStateCLOSED) {
          len += sprintf (buf+len, "<td>%d</td><td>%s</td><td>-</td><td>-</td>"
                                   "<td>-</td><td>-</td></tr>\r\n",
                                   MYBUF(pcgi)->xcnt,tcp_ntoa(tsoc->State));
        }
        else if (tsoc->State == tcpStateLISTEN) {
          len += sprintf (buf+len, "<td>%d</td><td>%s</td><td>%d</td><td>-</td>"
                                   "<td>-</td><td>-</td></tr>\r\n",
                                   MYBUF(pcgi)->xcnt, tcp_ntoa(tsoc->State), tsoc->LocPort);
        }
        else {
          len += sprintf (buf+len, "<td>%d</td><td>%s</td><td>%d</td>"
                                   "<td>%d</td><td>%s</td><td>%d</td></tr>\r\n",
                                   MYBUF(pcgi)->xcnt, tcp_ntoa(tsoc->State), tsoc->LocPort,
                                   tsoc->AliveTimer, ip4_ntoa (tsoc->RemAddr), tsoc->RemPort);
        }
        // Repeat for all TCP Sockets
        if (MYBUF(pcgi)->xcnt == tcp_NumSocks) {
          break;
        }
      }
      if (MYBUF(pcgi)->xcnt < tcp_NumSocks) {
        // Hi bit is a repeat flag
        len |= (1u << 31);
      }
      break;

    case 'd':
      // System password from 'system.cgi'
      switch (env[2]) {
        case '1':
          len = sprintf (buf, &env[4], http_EnAuth ? "Enabled" : "Disabled");
          break;
        case '2':
          len = sprintf (buf, &env[4], http_auth_passw);
          break;
      }
      break;

    case 'e':
      // Browser Language from 'language.cgi'
      lang = http_server_get_lang ();
      if      (strncmp (lang, "en", 2) == 0) {
        lang = "English";
      }
      else if (strncmp (lang, "de", 2) == 0) {
        lang = "German";
      }
      else if (strncmp (lang, "fr", 2) == 0) {
        lang = "French";
      }
      else if (strncmp (lang, "sl", 2) == 0) {
        lang = "Slovene";
      }
      else {
        lang = "Unknown";
      }
      len = sprintf (buf, &env[2], lang, http_server_get_lang());
      break;

    case 'f':
      // LCD Module control from 'lcd.cgi'
      switch (env[2]) {
        case '1':
          len = sprintf (buf, &env[4], lcd_text[0]);
          break;
        case '2':
          len = sprintf (buf, &env[4], lcd_text[1]);
          break;
      }
      break;
   		case 'w':
      // Button state from 'button.cgi'
			switch (env[2]) {
        case '1':
          len = sprintf (buf, &env[4], lcd_text[0]);
          break;
        case '2':
          len = sprintf (buf, &env[4], lcd_text[1]);
          break;
      }
      break;
		
    case 'y':
      // Button state from 'button.cgx'

      len = sprintf (buf, &env[1], lcd_text[0]);
      
      break;

       case 'z':
      // Button state from 'button.cgx'

      len = sprintf (buf, &env[1], lcd_text[1]);
      break;
  }
  return (len);
}

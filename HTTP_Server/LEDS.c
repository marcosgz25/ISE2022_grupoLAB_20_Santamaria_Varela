#include "LEDS.h"

/* Do not use LED3--LED7, if trace pins are used! */
#ifdef TRACE_PIN_ENABLE
 #define LED_COUNT (3)
#else
 #define LED_COUNT (4)
#endif

/*LEDS pins:
		-LED1: P1_18
		-LED2: P1_20
		-LED3: P1_21
		-LED4: P1_23
*/

//Array con los puertos y pins de los leds
static const PIN LED_PIN[]={
	{1,18},
	{1,20},
	{1,21},
	{1,23},
};

//Funcion de configuracion de los leds
int32_t LED_Initialize (void) {
  uint32_t n;
  /* Enable GPIO clock */
  GPIO_PortClock     (1U);
  /* Configure pins: Output Mode with Pull-down resistors */
  for (n = 0; n < LED_COUNT; n++) {
    PIN_Configure (LED_PIN[n].Portnum, LED_PIN[n].Pinnum, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
    GPIO_SetDir   (LED_PIN[n].Portnum, LED_PIN[n].Pinnum, GPIO_DIR_OUTPUT);
    GPIO_PinWrite (LED_PIN[n].Portnum, LED_PIN[n].Pinnum, 0);
  }
  return 0;
}

//Funcion de desconfiguracion de los leds
int32_t LED_Uninitialize1 (void) {
  uint32_t n;

  /* Unconfigure pins: turn off Pull-up/down resistors */
  for (n = 0; n < LED_COUNT; n++) {
    PIN_Configure (LED_PIN[n].Portnum, LED_PIN[n].Pinnum, 0 , 0, 0);
  }

  return 0;
}

//Funcion que enciende un numero de leds
int32_t LED_On (uint32_t num) {
  int32_t retCode = 0;
  if (num < LED_COUNT)
	{
    GPIO_PinWrite (LED_PIN[num].Portnum, LED_PIN[num].Pinnum, 1);
  }
  else
	{
    retCode = -1;
  }
  return retCode;
}

//Funcion que apaga los leds
int32_t LED_Off (uint32_t num) {
  int32_t retCode = 0;
  if (num < LED_COUNT) 
	{
    GPIO_PinWrite (LED_PIN[num].Portnum, LED_PIN[num].Pinnum, 0);
  }
  else
	{
    retCode = -1;
  }
  return retCode;
}

//Funcion que escribe valores a los leds
int32_t LED_SetOut (uint32_t val) {
  uint32_t n;
  for (n = 0; n < LED_COUNT; n++)
	{
    if (val & (1 << n)) 
			LED_On(n);
    else
			LED_Off(n);
  }
  return 0;
}

//Funcion que devuelve el numero de leds disponibles
uint32_t LED_GetCount (void) {
  return LED_COUNT;
}

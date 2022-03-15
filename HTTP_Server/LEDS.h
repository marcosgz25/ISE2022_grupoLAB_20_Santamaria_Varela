#ifndef LEDS_H
#define LEDS_H

//Incluimos las librerias para la inicializacion de los LEDS
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "LPC17xx.h"


//Funciones
extern int32_t LED_Initialize(void);
extern int32_t LED_Unitialize(void);
extern int32_t LED_On(uint32_t num);
extern int32_t LED_Off(uint32_t num);
extern int32_t LED_SetOut(uint32_t val);
extern uint32_t LED_GetCount(void);
#endif

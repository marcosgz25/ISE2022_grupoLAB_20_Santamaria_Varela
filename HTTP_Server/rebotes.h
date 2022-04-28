#ifndef _REBOTES_H_
#define _REBOTES_H_

#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"

#define PUERTO_JOYSTICK  0
#define CENTER 16
#define DOWN   17
#define LEFT   15
#define UP   	 23
#define RIGHT  24

#define DERECHA_BAJADA     0x01
#define DERECHA_SUBIDA     0x10
#define ARRIBA_BAJADA      0x02
#define ARRIBA_SUBIDA      0x20
#define ABAJO_BAJADA       0x03
#define ABAJO_SUBIDA       0x30
#define IZQUIERDA_BAJADA   0x04
#define IZQUIERDA_SUBIDA   0x40
#define CENTRO_BAJADA      0x05
#define CENTRO_SUBIDA      0x50
//FUNCIONES
void Inicializar_Pulsadores(void);
int Init_Thread_Rebotes(void);

//VARIABLES
extern osThreadId  rebotes_id;    
#endif

#include "rebotes.h"
#include "RTC.h"
void Rebotes (void const *argument); 
osThreadId rebotes_id;
osThreadDef (Rebotes, osPriorityNormal, 1, 0);


//FUNCION QUE INICIALIZA LOS HILOS
int Init_Thread_Rebotes(void) 
{
	rebotes_id = osThreadCreate (osThread(Rebotes),NULL);	
	if(!rebotes_id) return (-1);
  return(0);
}
// HILO QUE GESTIONA LOS REBOTES POR FLANCO DE SUBIDA Y BAJADA
// ATIENDE A LAS SEÑALES DE ARRIBA,ABAJO,DERECHA,IZQUIERDA

void Rebotes (void const *argument)
{
	osEvent evento;
	while(1)
	{
		evento=osSignalWait(0,osWaitForever);
		if(evento.value.signals == DERECHA_SUBIDA)
		{
			osDelay(300);
			LPC_GPIOINT->IO0IntEnF |= 1 <<RIGHT;
		}
		if(evento.value.signals == DERECHA_BAJADA)
		{
			osDelay(300);
			LPC_GPIOINT->IO0IntEnR |= 1 <<RIGHT;
		}
		if(evento.value.signals == IZQUIERDA_BAJADA)
		{
			osDelay(300);
			LPC_GPIOINT->IO0IntEnR |= 1 <<LEFT;
		}
		if(evento.value.signals == IZQUIERDA_SUBIDA)
		{
			osDelay(300);
			LPC_GPIOINT->IO0IntEnF |= 1 <<LEFT;
		}
		if(evento.value.signals == CENTRO_BAJADA)
		{
			osDelay(300);
			LPC_GPIOINT->IO0IntEnR |= 1 <<CENTER;
		}
		if(evento.value.signals == CENTRO_SUBIDA)
		{
			osDelay(300);
			LPC_GPIOINT->IO0IntEnF |= 1 <<CENTER;
		}
		if(evento.value.signals == ARRIBA_BAJADA)
		{
			osDelay(300);
			LPC_GPIOINT->IO0IntEnR |= 1 <<UP;
		}
		if(evento.value.signals == ARRIBA_SUBIDA)
		{
			osDelay(300);
			LPC_GPIOINT->IO0IntEnF |= 1 <<UP;
		}
		if(evento.value.signals == ABAJO_BAJADA)
		{
			osDelay(300);
			LPC_GPIOINT->IO0IntEnR |= 1 <<DOWN;
		}
		if(evento.value.signals == ABAJO_SUBIDA)
		{
			osDelay(300);
			LPC_GPIOINT->IO0IntEnF |= 1 <<DOWN;
		}
	}
}

//FUNCION QUE INICIALIZA LOS PULSADORES

void Inicializar_Pulsadores() 
{
//	PIN_Configure(PUERTO_JOYSTICK,DOWN,PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
//  PIN_Configure(PUERTO_JOYSTICK,UP,PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
//  PIN_Configure(PUERTO_JOYSTICK,LEFT,PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
//  PIN_Configure(PUERTO_JOYSTICK,RIGHT,PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
	PIN_Configure(PUERTO_JOYSTICK,CENTER,PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
	
	LPC_GPIOINT->IO0IntEnR = 1 << CENTER;//| 1<<UP | 1<<DOWN | 1<<LEFT | 1<<RIGHT ;
	NVIC_EnableIRQ(EINT3_IRQn);
}


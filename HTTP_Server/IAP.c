/**********************************************************************
* $Id$		iaptest.c			2012-04-18
*//**
* @file		lpc17xx_iap.h
 * @brief	IAP demo
* @version	1.0
* @date		18. April. 2012
* @author	NXP MCU SW Application Team
* 
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/
#include "IAP.h"
#include "LEDS.h"
#include "cmsis_os.h"
uint8_t __attribute__ ((aligned (4))) bufer[BUFF_SIZE];
uint8_t *ptr;
uint8_t *ptr_2;
uint32_t flash_prog_area_sec_start;
uint32_t flash_prog_area_sec_end;
IAP_STATUS_CODE status;

uint8_t memor;
/*********************************************************************//**
 * @brief		The entry of the program
 *
 * @param[in]None
 *
 * @return 	None.
 *
 **********************************************************************/
void IAP_init (uint8_t array[])
{	    
	int i;
	for(i=0;i<10;i++)
	{
		bufer[i]=array[i];
	}
  flash_prog_area_sec_start = GetSecNum(FLASH_PROG_AREA_START);
  flash_prog_area_sec_end =  GetSecNum(FLASH_PROG_AREA_START + FLASH_PROG_AREA_SIZE);
  status = EraseSector(flash_prog_area_sec_start, flash_prog_area_sec_end); 

	//Escribir las 16 primeras posiciones
	ptr=(uint8_t*)(FLASH_PROG_AREA_START);
	status =  CopyRAM2Flash(ptr, bufer,IAP_WRITE_1024);
	//Leer 16 posiciones
}
void leer_LEDS(uint8_t P2, int modoleds)
{
	if(modoleds==1)
		bufer[10]=0x10;
	else
		bufer[10]=P2;
	flash_prog_area_sec_start = GetSecNum(FLASH_PROG_AREA_START);
  flash_prog_area_sec_end =  GetSecNum(FLASH_PROG_AREA_START + FLASH_PROG_AREA_SIZE);
  status = EraseSector(flash_prog_area_sec_start, flash_prog_area_sec_end); 

	//Escribir las 16 primeras posiciones
	ptr=(uint8_t*)(FLASH_PROG_AREA_START);
	status =  CopyRAM2Flash(ptr, bufer,IAP_WRITE_1024);
}

void leer_ADC(uint8_t adv)
{
	ptr_2=(uint8_t*)0x4000B;
	bufer[11]=*ptr_2;
	osDelay(200);
	if(bufer[11]<adv)
	{
		GPIO_PinWrite (PUERTO_LEDRGB, RED, 1);
		GPIO_PinWrite (PUERTO_LEDRGB, BLUE, 0);
		GPIO_PinWrite (PUERTO_LEDRGB, GREEN, 1);
	}
	else
	{
		GPIO_PinWrite (PUERTO_LEDRGB, RED, 1);
		GPIO_PinWrite (PUERTO_LEDRGB, BLUE, 1);
		GPIO_PinWrite (PUERTO_LEDRGB, GREEN, 1);
	}
	bufer[11]=adv;
	status = EraseSector(flash_prog_area_sec_start, flash_prog_area_sec_end); 
	status =  CopyRAM2Flash(ptr, bufer,IAP_WRITE_1024);
}

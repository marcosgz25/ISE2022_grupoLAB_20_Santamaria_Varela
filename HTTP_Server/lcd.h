#include "LPC17xx.h"      
#include "GPIO_LPC17xx.h" 
#include "PIN_LPC17xx.h"
#include "RTE_Device.h"
#include "SPI_LPC17xx.h"
#include "Driver_SPI.h"
#include <stdio.h>
#include <string.h>
#ifndef _LCD_H_
#define _LCD_H_
#define PUERTO 0
#define A0     6  //PIN8
#define RESET  8  //PIN6
#define CS     18 //PIN11

extern unsigned char buffer[512];

void retardo(uint32_t microsegundos);
void reset (void);
void wr_data(unsigned char data);
void wr_cmd(unsigned char cmd);
void LCD_reset(void);
void copy_to_lcd(void);
void init(void);
int EscribeLetra_L1 ( uint8_t letra);
int EscribeLetra_L2 ( uint8_t letra);
void EscribeFrase (char *frase,int num_pagina);
void borrarLCD(void);
void pintar_pelota(int posicion);

#endif

#include "lcd.h"
#include "Arial12x12.h"

extern ARM_DRIVER_SPI Driver_SPI1;
ARM_DRIVER_SPI* SPIdrv = &Driver_SPI1;

void init(void);
void retardo(uint32_t microsegundos);
void reset(void);
unsigned char buffer[512];
uint32_t posicionL1;
uint32_t posicionL2=256;

//char frase[25];

void retardo(uint32_t microsegundos)
{
  int i;
  for( i=0; i<(microsegundos*25) ; i++); //Cada iteración del bucle tarda 40 ns * 25 => 1 us

}

void reset (void)
  
{
  GPIO_PinWrite(PUERTO, RESET, 1);
  retardo(2); //Usamos dos microsegundos porque nuestra placa es de 3,3 V
  GPIO_PinWrite(PUERTO, RESET, 0);
}

void wr_data(unsigned char data)
{
	GPIO_PinWrite (PUERTO, CS, 0);
	GPIO_PinWrite (PUERTO, A0, 1);
	SPIdrv -> Send(&data, sizeof(data));
	GPIO_PinWrite (PUERTO, CS, 1);
}

void wr_cmd(unsigned char cmd)
{
	GPIO_PinWrite (PUERTO, CS, 0);
	GPIO_PinWrite (PUERTO, A0, 0);
	SPIdrv -> Send(&cmd, sizeof(cmd));
	GPIO_PinWrite (PUERTO, CS, 1);
}

void LCD_reset()
{
	wr_cmd(0xAE);
	wr_cmd(0xA2);
	wr_cmd(0xA0);
	wr_cmd(0xC8);
	wr_cmd(0x22);
	wr_cmd(0x2F);
	wr_cmd(0x40);
	wr_cmd(0xAF);
	wr_cmd(0x81);
	wr_cmd(0x17);
	wr_cmd(0xA4);
	wr_cmd(0xA6);
}

void copy_to_lcd()
{
	int i;
	wr_cmd(0x00);
	wr_cmd(0x10);
	wr_cmd(0xB0);//pagina 0
	for(i=0; i<128; i++)
	{
		wr_data(buffer[i]);
	}
	
	wr_cmd(0x00);
	wr_cmd(0x10);
	wr_cmd(0xB1);	//pagina 1
	for(i=128; i<256; i++)
	{
	wr_data(buffer[i]);
	}
	
	wr_cmd(0x00);
	wr_cmd(0x10);
	wr_cmd(0xB2);//pagina 2
	for(i=256; i<384; i++)
	{
		wr_data(buffer[i]);
	}
	
	wr_cmd(0x00);
	wr_cmd(0x10);
	wr_cmd(0xB3);//pagina 3
	for(i=384; i<512; i++)
	{
	wr_data(buffer[i]);
	}
}
void init(void)
{
  //Inicializo el driver
  SPIdrv->Initialize(NULL);
  SPIdrv->PowerControl(ARM_POWER_FULL);
  SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_DATA_BITS(8), 20000000); //Ultimo valor es la frecuencia
    
    //Configuracion de los GPIO como salidas a nivel alto inicialmente:
  GPIO_SetDir(PUERTO,RESET, GPIO_DIR_OUTPUT); 
  GPIO_SetDir(PUERTO,CS, GPIO_DIR_OUTPUT); 
  GPIO_SetDir(PUERTO,A0, GPIO_DIR_OUTPUT); 
  
  GPIO_PinWrite(PUERTO, A0, 1);
  GPIO_PinWrite(PUERTO, CS, 1);
  GPIO_PinWrite(PUERTO, RESET, 1);

  //GENERAMOS PULSO RESET Y RETARDO DE 1 ms

  reset();
  retardo(1000);
}

int EscribeLetra_L1 ( uint8_t letra)
{

	uint8_t i, valor1, valor2;
	uint16_t comienzo = 0;

	comienzo = 25 * (letra - ' ');//son 25 porque 12x2+1

	for(i=0; i<12; i++)
	{
		valor1 = Arial12x12[comienzo+i*2+1];//escribe en pagina 0
		valor2 = Arial12x12[comienzo+i*2+2];//escribe en pagina 1

		buffer[i+ posicionL1] = valor1;
		buffer[i+128+ posicionL1] = valor2;
	}

	posicionL1 = posicionL1 + Arial12x12[comienzo];

	return 0;
}

int EscribeLetra_L2 ( uint8_t letra)
{
	uint8_t i, valor1, valor2;
	uint16_t comienzo = 0;
	
	comienzo = 25 * (letra - ' ');//son 25 porque 12x2+1
	for(i=0; i<12; i++)
	{
		valor1 = Arial12x12[comienzo+i*2+1];//escribe en pagina 0
		valor2 = Arial12x12[comienzo+i*2+2];//escribe en pagina 1

		buffer[i+ posicionL2] = valor1;
		buffer[i+128+ posicionL2] = valor2;
	}

	posicionL2 = posicionL2 + Arial12x12[comienzo];

	return 0;
	
}
void EscribeFrase (char *frase, int num_pagina)
{
  int i;
	int longitud = strlen(frase);
	posicionL1=0;
	posicionL2=256;
    if(num_pagina==1)
    {
      for( i=0;i<longitud;i++)
      {
        if(posicionL1+12 <127)
        EscribeLetra_L1(frase[i]);
      }
    }
    if(num_pagina==2)
    {
      for( i=0;i<longitud;i++)
      {
        if(posicionL2+12 <383)
          EscribeLetra_L2(frase[i]);
      }
    }
}
void borrarLCD(void)
{
  int i;
	for( i=0;i<512;i++)
	{
		buffer[i]=0x00;
	}

}
void lcd_on(void)
{
	wr_cmd(0xAE);
}
void lcd_off(void)
{
	wr_cmd(0xAF);
}
void pintar_pelota(int posicion)
{
  int i;
  for( i=0;i<8;i++)
  {
    buffer[i+posicion]=0xFF;
  }
  copy_to_lcd();
}

#include "lpc17xx_iap.h"
#include "lpc_types.h"
#include "lpc17xx.h"/*FLASH*/
#include <stdio.h>

/** The area will be erase and program */
#define FLASH_PROG_AREA_START       0x0040000
#define FLASH_PROG_AREA_SIZE				0x1000


/** The origin buffer on RAM */
#define BUFF_SIZE           1024

extern uint32_t flash_prog_area_sec_start;
extern uint32_t flash_prog_area_sec_end;
extern IAP_STATUS_CODE status;

extern 	uint8_t *ptr;
extern char bufferIAP[BUFF_SIZE];
extern IAP_STATUS_CODE status;
extern void IAP_init(uint8_t array[]);
extern void leer_LEDS(uint8_t P2, int modoleds);
extern void leer_ADC(uint8_t adv);
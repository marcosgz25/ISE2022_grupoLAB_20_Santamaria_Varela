#ifndef _RTC_H_
#define _RTC_H_

#include "LPC17xx.h"


typedef struct {
	uint32_t SEC; 		/*!< Seconds Register */
	uint32_t MIN; 		/*!< Minutes Register */
	uint32_t HOUR; 		/*!< Hours Register */
	uint32_t DOM;		/*!< Day of Month Register */
	//uint32_t DOW; 		/*!< Day of Week Register */
	// DOY; 		/*!< Day of Year Register */
	uint32_t MONTH; 	/*!< Months Register */
	uint32_t YEAR; 		/*!< Years Register */
} RTC_TIME;//estructura RTC para la hora

extern void RTC_IRQHandler(void);
extern void c_entry(void);
extern void RTC_getTime_Date(void);

#endif

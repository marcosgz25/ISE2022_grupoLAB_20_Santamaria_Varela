#ifndef ADC_H
#define ADC_H

#include "LPC17xx.h"
#include "PIN_LPC17xx.h"

extern int32_t  ADC_Initialize(void);
extern int32_t  ADC_Uninitialize(void);
extern int32_t  ADC_StartConversion(void);
extern int32_t  ADC_ConversionDone(void);
extern int32_t  ADC_GetValue(void);
extern uint32_t ADC_GetResolution(void);
#endif

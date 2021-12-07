#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "main.h"
#define TMPR_SAMPLES 8 // max is 16
extern int16_t xdata Adc_data[TMPR_SAMPLES]; // ADC samples (0 element is not filled)

typedef struct
	{
        uint8_t samples_count;
        int16_t temperature_raw;
	} adcglob_t;

extern adcglob_t adcglob;

extern bit ADC_buffer_ready;

extern int16_t ConvertADC2temp (void);

void initADC(void);

#endif /* INC_ADC_H_ */

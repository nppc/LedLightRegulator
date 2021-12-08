//#pragma src
#include "temperature.h"

#include "main.h"
#include "gen.h"
#ifdef DEBUGUART
	#include "uart1.h"
#endif

volatile int16_t xdata Adc_data[TMPR_SAMPLES]; // ADC samples (255 samples. 0s element is not filled)
volatile adcglob_t adcglob;
volatile bit ADC_buffer_ready; // indicates when new ADC readings are ready

void initADC(void){
  int8_t tmp;
	glob.samples_count=TMPR_SAMPLES;
	ADC0CN0_ADINT = 0;	// reset ADC complete interrupt
	ADC0CN0_ADBUSY = 1;	// start ADC conversions
	// wait for 500 conversions
	ADC_buffer_ready = 0;
	for(tmp=0;tmp<100;tmp++){
    while(ADC_buffer_ready==0);
    glob.samples_count=TMPR_SAMPLES;
    ADC_buffer_ready=1;
    ADC0CN0_ADBUSY = 1;
	}
	glob.temp_cntr = 50; // every second
  ADC_buffer_ready = 0;
  ADC0CN0_ADBUSY = 1; // start ADC conversions
}


// input: accumulated raw data
int16_t ConvertADC2temp(void){
  uint16_t adcval, sum = 0;
  uint8_t i;
  // Lets sum...
  for(i=0;i<TMPR_SAMPLES;i++){
    sum += (uint16_t)Adc_data[i];
  }
  adcval = sum / TMPR_SAMPLES; // average
  // Offset (Temperature 0 offset) is 757mV - from datasheet
  // Slope is 2.85mV/C - from datasheet
  // ADCresolution = 4092
  // Vref = 2400mV
  #define tSlope 1244 // Slope * ADCresolution / Vref * 256
  #define tVoffset 1291 // Offset * ADCresolution / Vref
  // return only positive temp
  if(adcval>tVoffset){
	  int16_t tmp = (uint32_t)(adcval-tVoffset) * 256 / tSlope;
	  tmp = tmp + 14; // adjust temperature according to calibration. While in calibration do not adjust temperature
    return tmp;
  }else{
    return 0;
  }
}


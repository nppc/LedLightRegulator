//#pragma src
#include "gen.h"

volatile uint16_t tmp_millis;
volatile bit delay_on;


void delay_ms(uint16_t ms_count)
 {
  tmp_millis = ms_count;
  delay_on = 1;
  while(delay_on);
 }

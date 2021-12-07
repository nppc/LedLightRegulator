#include "main.h"
#include "pwm.h"

static const int16_t code led_steps_pwm[] = {
    LIGHTPANELPWM_MIN, LIGHTPANELPWM_MAX/2+(LIGHTPANELPWM_MAX/4), LIGHTPANELPWM_MAX // 10%, 75% 100%
};

void setval_PWMout(int16_t val){
  if(val <= LIGHTPANELPWM_MAX){
    IE_EA = 0;
    pwmglob.pwmchangecntr = 0; // change during first Timer2 Interrupt
    pwmglob.set_out = val;
    IE_EA = 1;
  }
}

void setLEDpower(void){
  uint8_t step = glob.LED_step;
  if(step >= LEDSTEPS){step = LEDSTEPS-1;}
  setval_PWMout(led_steps_pwm[step]);
}

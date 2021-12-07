//#pragma src
// Button routines
#include "button.h"
#include "pwm.h"
#include "pgm.h"

volatile uint16_t buttonTmr;
volatile bool buttonLastState;

// use some big timer number for the counter base after debounce (let's say 1000).
// if button is pressed and last state of button is different, start a timer for 20ms (debounce)
// if button is released then set last state of button to 0 (timer will stop)
// if after debounce time button still pressed, do action but continue to run timer
// if after 1000ms (wait for repeat) button is still pressed, do action again and reduce timer by 250 (repeat value)

void processButtons(void){
  bool but1inc, but1dec, butPwr, tmp;
  uint16_t tmpTmr;
  // 1 - pressed, 0 - not pressed
  butPwr = !(P0 & P0_B5__BMASK);
  but1inc = !(P1 & P1_B5__BMASK);
  but1dec = !(P1 & P1_B4__BMASK);
  // if any button is pressed
  tmp = but1inc | but1dec | butPwr; // 1 if any button is pressed
  if(tmp){
    // read timer only if button is pressed. Otherwise we not interested in timer
    IE_EA = 0;
    tmpTmr = buttonTmr; // the variable buttonTmr is updated in interrupt
    IE_EA = 1;
  }else{
    // if no buttons pressed, then just exit
    buttonLastState = 0; // stop the timer
    return;
  }
  if(tmp && !buttonLastState){
    // button is just pressed, start the timer
    IE_EA = 0;
    buttonTmr = 0; // the variable buttonTmr is updated in interrupt
    IE_EA = 1;
  }
  buttonLastState = 1; // update button last state

  // check for debounce
  if(tmpTmr<BUTTON_DEBOUNCE){
    return; // exit as we not yet debounced
  }
  // are we ready to run the action for the first time after debounce?
  if(tmpTmr>=BUTTON_DEBOUNCE && tmpTmr<1000){
    // run the action again
    buttonAction(but1inc, but1dec, butPwr);
    // base the timer above debounce mark
    IE_EA = 0;
    buttonTmr = 1000; // the variable buttonTmr is updated in interrupt
    IE_EA = 1;
    return;
  }

  // are we passed delay for repeat?
  if(tmpTmr>=BUTTON_DELAYBEFOREREPEAT+1000){
    // run the action again
    if(!butPwr) buttonAction(but1inc, but1dec, butPwr);
    // return the timer to the repeat rate
    IE_EA = 0;
    buttonTmr -= BUTTON_REPEATRATE; // the variable buttonTmr is updated in interrupt
    IE_EA = 1;
  }
}

void buttonAction(bool but1inc, bool but1dec, bool butPwr){
  if(glob.LIGHT_on==1){ // only check inc/dec buttons if power is on
    if(but1inc){
      if(glob.LED_step<(LEDSTEPS-1)){glob.LED_step++;}
      setLEDpower();
      allowStoreEEPROM = 1;
    };
    if(but1dec){
      if(glob.LED_step>0){glob.LED_step--;}
      setLEDpower();
      allowStoreEEPROM = 1;
    };
  }
  if(butPwr){
    if(glob.LIGHT_on==1){
        glob.LIGHT_on=0;
        if(allowStoreEEPROM) storeSettingsEE();
        setval_PWMout(0); // LED is off
    }else{
        glob.LIGHT_on=1;
        setLEDpower();
    }
  }

}

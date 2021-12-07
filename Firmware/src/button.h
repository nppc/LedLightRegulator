
#ifndef BUTTON_H_
#define BUTTON_H_

#include "main.h"

// values in ms
#define BUTTON_DEBOUNCE 40
#define BUTTON_DELAYBEFOREREPEAT 1000
#define BUTTON_REPEATRATE 400

extern void processButtons(void);
extern void buttonAction(bool but1inc, bool but1dec, bool butPwr);

extern uint16_t buttonTmr;
extern bool buttonLastState;

#endif /* BUTTON_H_ */


#ifndef MAIN_H_
#define MAIN_H_

#include <SI_EFM8BB1_Register_Enums.h>                  // SFR declarations
#include <SI_EFM8BB1_Defs.h>
#include <stdint.h>

/***** COMPILATION VARIANTS *****/
//#define DEBUGUART
//#define DEBUG
//#define PREVENTFLASHWRITE

SI_SBIT(PIN_BUTPWR, SFR_P0, 5);
SI_SBIT(PIN_BUT1INC, SFR_P1, 5);
SI_SBIT(PIN_BUT1DEC, SFR_P1, 4);
SI_SBIT(PIN_PWM0, SFR_P0, 7);

#define LEDSTEPS 3
//#define LIGHTPANELPWM_MAX 1000 // 10 bit PWM value
//#define LIGHTPANELPWM_MIN 140 // 10 bit PWM value
#define LIGHTPANELPWM_MAX 511 // 9 bit PWM value
#define LIGHTPANELPWM_MIN 90 // 9 bit PWM value
#define LIGHTPANELSPEED 2 // Adjust Light Panel PWM every N ms.

#define TEMPERATUREMAX 45 // if higher - reduce light to previous step


// Structure for accessing 16bit number by 2 8 bit (back and forth)
// u16 and u8[] sharing the same memory space
// Usage:
// U16_U8 ptr;
// ptr.u8[0] = high;
// ptr.u8[1] = low;
// ptr.u16 = 16bit;
typedef union
   {
   unsigned short u16;
   unsigned char u8[2];
   } U16_U8;


typedef struct
{
  uint8_t samples_count;
  uint16_t temp_cntr;
	uint8_t LIGHT_on; // 1 - ON, 0 - OFF
	uint8_t LED_step;
} glob_t;

typedef struct
{
  int16_t set_out;
  int16_t cur_out;
  uint8_t pwmchangecntr;
} pwmglob_t;

extern bit pwmOut0_update, allowStoreEEPROM;

extern glob_t glob;
extern pwmglob_t pwmglob;


#endif /* MAIN_H_ */


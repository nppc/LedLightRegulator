//#pragma src
//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!!
//=========================================================

// USER INCLUDES
#include "main.h"
#include "gen.h"
#include "button.h"
#include "temperature.h"

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
// enters interrupt every 1ms
//-----------------------------------------------------------------------------
SI_INTERRUPT (TIMER2_ISR, TIMER2_IRQn)
  {
    U16_U8 tmp;
    if(buttonLastState)
      { buttonTmr++;}

    // advance delay timer
    if(delay_on)
      {
        if(tmp_millis!=0)
          { tmp_millis--;}
        else
          { delay_on=0;}
      }

    if(pwmglob.pwmchangecntr==0)
      {
        pwmglob.pwmchangecntr = LIGHTPANELSPEED;
        if(pwmglob.cur_out<pwmglob.set_out)
          { pwmglob.cur_out++;pwmOut0_update = 1;} // adjust
        else if(pwmglob.cur_out>pwmglob.set_out)
          { pwmglob.cur_out--;pwmOut0_update = 1;} // adjust

        if(pwmOut0_update)
          {
            // disable output if needed
            if(pwmglob.cur_out==0)
              {
                P0MDOUT &= ~P0MDOUT_B7__BMASK;  // Set opendrain
                PCA0CPM0 &= ~PCA0CPM0_ECOM__BMASK;// disable led PWM
                PIN_PWM0 = 0;
              }
            PCA0PWM |= PCA0PWM_ARSEL__AUTORELOAD;
            tmp.u16 = pwmglob.cur_out;
            // convert 16bit to 2x8bit
            PCA0CPL0 = tmp.u8[1];
            PCA0CPH0 = tmp.u8[0];
            pwmOut0_update = 0;
            // enable output if needed
            if(pwmglob.cur_out==1 && pwmglob.set_out>0)
              {
                PCA0CPM0 |= PCA0CPM0_ECOM__ENABLED; // enable led PWM
                P0MDOUT |= P0MDOUT_B7__PUSH_PULL;// Set pushpull
              }
          }
      }
    else
      {
        pwmglob.pwmchangecntr--;
      }

    TMR2CN0_TF2H = 0;
  }

//-----------------------------------------------------------------------------
// ADC0EOC_ISR
//-----------------------------------------------------------------------------
//
// ADC0EOC ISR Content goes here. Remember to clear flag bits:
// ADC0CN0::ADINT (Conversion Complete Interrupt Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT (ADC0EOC_ISR, ADC0EOC_IRQn)
  {
  ADC0CN0_ADINT = 0; // reset interrupt flag
  Adc_data[glob.samples_count]=ADC0;
  ADC_buffer_ready = 1; // assume buffer is filled
  if(glob.samples_count>0){
    glob.samples_count--;
    ADC0CN0_ADBUSY = 1; // start NEXT ADC reading
    ADC_buffer_ready = 0; // buffer is not filled yet
  }

  }

//-----------------------------------------------------------------------------
// TIMER3_ISR
//-----------------------------------------------------------------------------
//
// TIMER3 ISR Content goes here. Remember to clear flag bits:
// TMR3CN0::TF3H (Timer # High Byte Overflow Flag)
// TMR3CN0::TF3L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------

// 30Hz
SI_INTERRUPT (TIMER3_ISR, TIMER3_IRQn)
  {
  TMR3CN0 &=~TMR3CN0_TF3H__BMASK; // clear interrupt
  if(glob.temp_cntr > 0){glob.temp_cntr--;}else{ADC0CN0_ADBUSY = 1;glob.samples_count = TMPR_SAMPLES;glob.temp_cntr = 50;} // every second
  }


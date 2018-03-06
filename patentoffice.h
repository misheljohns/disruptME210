#ifndef PATENT_OFFICE_H
#define PATENT_OFFICE_H

#include "params.h"

class patent_office
{


private: 
  teetertotter_states state;
  const int tilt_left_button;
  const int tilt_right_button;
  
  teetertotter_states prev_state; //previous side before it got to neutral
  unsigned long tilt_left_time;
  unsigned long tilt_right_time;
  unsigned long untilt_left_time;
  unsigned long untilt_right_time;
  bool leftbutton, rightbutton;

  void update_timers() 
  {
    if(!leftbutton) //previously pressed
    {
      if((leftbutton = digitalRead(tilt_left_button))) //now released
      {
        untilt_left_time = millis();
        tilt_left_time = (unsigned long) 2000000000; 
      }
    }
    else //previously released
    {
      if(!(leftbutton = digitalRead(tilt_left_button))) //now pressed
      {
        tilt_left_time = millis();
        untilt_left_time = (unsigned long) 2000000000; 
      }
    }

    if(!rightbutton) //previously pressed
    {
      if((rightbutton = digitalRead(tilt_right_button))) //now released
      {
        untilt_right_time = millis();
        tilt_right_time = (unsigned long) 2000000000; 
      }
    }
    else //previously released
    {
      if(!(rightbutton = digitalRead(tilt_right_button))) //now pressed
      {
        tilt_right_time = millis();
        untilt_right_time = (unsigned long) 2000000000; 
      }
    }
  }
  
public:
  patent_office(int tilt_left_button_in,int tilt_right_button_in):
    tilt_left_button(tilt_left_button_in),
    tilt_right_button(tilt_right_button_in)
  {

    
  }

  teetertotter_states get_state()
  {
    return state;
  }

  void setup() {
    pinMode(tilt_left_button, INPUT_PULLUP);
    pinMode(tilt_right_button, INPUT_PULLUP);
    
    state = STATE_START;
    prev_state = STATE_START;
    tilt_left_time = (unsigned long) 2000000000; //2000000000 ~ half of max value 4294967295 ms = ~23 days
    tilt_right_time = (unsigned long) 2000000000;
    untilt_left_time = (unsigned long) 2000000000; 
    untilt_right_time = (unsigned long) 2000000000; 
    leftbutton = 0;
    rightbutton = 0;
  }

  void update()
  {
    update_timers();
    
    switch(state) {
      case STATE_TILT_LEFT:
        if((millis() > untilt_left_time + DEBOUNCE_TIME_MILLIS) && leftbutton) { //left button not pressed, and has been unpressed for DEBOUNCE_TIME_MILLIS
          state = STATE_NEUTRAL;
          prev_state = STATE_TILT_LEFT;
        }
        break;
        
      case STATE_TILT_RIGHT:
        if((millis() > untilt_right_time + DEBOUNCE_TIME_MILLIS)) {
          state = STATE_NEUTRAL;
          prev_state = STATE_TILT_RIGHT;
        }
        break;
  
      case STATE_START:
  
        //break; - no break, we want the stuff in STATE_NEUTRAL to run in STATE_START
        
      case STATE_NEUTRAL:
        if(!digitalRead(tilt_right_button) && (millis() > tilt_right_time + BOUNCE_TIME_MILLIS)) { //is currently tilted to the right, and the last change to the right was BOUNCE_TIME_MILLIS ago
          state = STATE_TILT_RIGHT;
        }
        else if(!digitalRead(tilt_left_button) && (millis() > tilt_left_time + BOUNCE_TIME_MILLIS)) { //is currently tilted to the left, and the last change to the left was BOUNCE_TIME_MILLIS ago
          state = STATE_TILT_LEFT;
        }
        break;
  
      case STATE_UNLOAD:
        //break; shouldn't get to this sate
        
      default:    // Should never get into an unhandled state
        Serial.println("What is this I do not even...");
    }
  }

  void reset_vals() //for future functionality - to reset using open button, if necessary
  {
    state = STATE_START;
    prev_state = STATE_START;
    tilt_left_time = (unsigned long) 2000000000; //2000000000 ~ half of max value 4294967295 ms = ~23 days
    tilt_right_time = (unsigned long) 2000000000;
    untilt_left_time = (unsigned long) 2000000000; 
    untilt_right_time = (unsigned long) 2000000000; 
    leftbutton = 0;
    rightbutton = 0;
  }
  
};



#endif


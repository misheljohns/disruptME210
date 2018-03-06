#ifndef FUNDING_ROUND_H
#define FUNDING_ROUND_H

#include <Servo.h> 
#include "params.h"

class funding_round
{
private: 
  teetertotter_states state;
  const int tilt_left_button;
  const int tilt_right_button;
  const int servo_pin;
  const int open_button;
  
  teetertotter_states prev_state; //previous side before it got to neutral
  Servo servo;
  unsigned long tilt_left_time;
  unsigned long tilt_right_time;
  unsigned long untilt_left_time;
  unsigned long untilt_right_time;
  unsigned long servo_open_time;
  bool servo_open;
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
    
  void check_servo_timer() 
  {
    if(servo_open && (millis() > servo_open_time + SERVO_CLOSE_DELAY)) {
      servo.write(SERVO_CLOSE_ANGLE);
      servo_open = 0;
    }
  }

  
public:
  funding_round(int tilt_left_button_in,int tilt_right_button_in,int servo_pin_in,int open_button_in):
    tilt_left_button(tilt_left_button_in),
    tilt_right_button(tilt_right_button_in),
    servo_pin(servo_pin_in),
    open_button(open_button_in)
  {

    
  }

  teetertotter_states get_state()
  {
    return state;
  }

  void setup() {
    pinMode(tilt_left_button, INPUT_PULLUP);
    pinMode(tilt_right_button, INPUT_PULLUP);
    pinMode(open_button, INPUT_PULLUP);
  
    servo.attach(servo_pin, 1000, 2000); //adjust pulse width (microsecs)
    
    state = STATE_START;
    prev_state = STATE_START;
    servo.write(SERVO_CLOSE_ANGLE);
    servo_open = 0;
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
        if(!digitalRead(open_button)) {//open button pressed
          servo.write(SERVO_OPEN_ANGLE);
          state = STATE_UNLOAD;
        }
        else {
          if((millis() > untilt_left_time + DEBOUNCE_TIME_MILLIS) && leftbutton) { //left button not pressed, and has been unpressed for DEBOUNCE_TIME_MILLIS
            state = STATE_NEUTRAL;
            prev_state = STATE_TILT_LEFT;
          }
          else {
            check_servo_timer();
          }
        }
        break;
        
      case STATE_TILT_RIGHT:
        if(!digitalRead(open_button)) {//open button pressed
          servo.write(SERVO_OPEN_ANGLE);
          state = STATE_UNLOAD;
        }
        else {
          if((millis() > untilt_right_time + DEBOUNCE_TIME_MILLIS)) {
            state = STATE_NEUTRAL;
            prev_state = STATE_TILT_RIGHT;
          }
          else{
            check_servo_timer();
          }
        }
        break;
  
      case STATE_START:
  
        //break; - no break, we want the stuff in STATE_NEUTRAL to run in STATE_START
        
      case STATE_NEUTRAL:
        if(!digitalRead(open_button)) {//open button pressed
          servo.write(SERVO_OPEN_ANGLE);
          state = STATE_UNLOAD;
        }
        else {
          if(!digitalRead(tilt_right_button) && (millis() > tilt_right_time + BOUNCE_TIME_MILLIS)) { //is currently tilted to the right, and the last change to the right was BOUNCE_TIME_MILLIS ago
            if((prev_state == STATE_TILT_LEFT) || (prev_state == STATE_START)) {
              servo.write(SERVO_OPEN_ANGLE);
              servo_open_time = millis();
              servo_open = 1;
            }
            state = STATE_TILT_RIGHT;
          }
          else if(!digitalRead(tilt_left_button) && (millis() > tilt_left_time + BOUNCE_TIME_MILLIS)) { //is currently tilted to the left, and the last change to the left was BOUNCE_TIME_MILLIS ago
            if((prev_state == STATE_TILT_RIGHT) || (prev_state == STATE_START)) {
              servo.write(SERVO_OPEN_ANGLE);
              servo_open_time = millis();
              servo_open = 1;
            }
            state = STATE_TILT_LEFT;
          }
          check_servo_timer();
        }
        break;
  
      case STATE_UNLOAD:
        if(digitalRead(open_button)) { //open button released
          servo.write(SERVO_CLOSE_ANGLE);

          state = STATE_START;
          prev_state = STATE_START;
          servo_open = 0;
          tilt_left_time = (unsigned long) 2000000000; //2000000000 ~ half of max value 4294967295 ms = ~23 days
          tilt_right_time = (unsigned long) 2000000000;
          untilt_left_time = (unsigned long) 2000000000; 
          untilt_right_time = (unsigned long) 2000000000; 
          leftbutton = 0;
          rightbutton = 0;
        }
        break;
        
      default:    // Should never get into an unhandled state
        Serial.println("What is this I do not even...");
    }
  }
};

#endif

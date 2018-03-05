#ifndef PARAMS_H
#define PARAMS_H

const int BOUNCE_TIME_MILLIS = 1000; //1 seconds
const int DEBOUNCE_TIME_MILLIS = 1000; //1 seconds - time to get into neutral state after leaving either side
const int SERVO_OPEN_ANGLE = 55;
const int SERVO_CLOSE_ANGLE = 140;
const int SERVO_CLOSE_DELAY = 1000; //close the door 1000 ms after it was opened

const int LEDPIN = 13;

typedef enum {
  STATE_TILT_RIGHT, STATE_TILT_LEFT, STATE_NEUTRAL, STATE_UNLOAD, STATE_START
} teetertotter_states ;

typedef enum {
  STATE_A_COMPLETE, STATE_GATE_OPEN, STATE_SIDE_START
} side_states ;

//pins
const int FR1_tilt_left_button = 7;
const int FR1_tilt_right_button = 8;
const int FR1_servo_pin = 6;
const int FR1_open_button = 11;

const int FR2_tilt_left_button = 12;
const int FR2_tilt_right_button = 13;
const int FR2_servo_pin = 14;
const int FR2_open_button = 15;

const int PO_tilt_left_button = 16;
const int PO_tilt_right_button = 17;

const int gate_right_servo_pin = 20;
const int gate_left_servo_pin = 21;
#endif

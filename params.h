#ifndef PARAMS_H
#define PARAMS_H

const int BOUNCE_TIME_MILLIS = 1000; //1 seconds
const int DEBOUNCE_TIME_MILLIS = 1000; //1 seconds - time to get into neutral state after leaving either side
const int SERVO_OPEN_ANGLE = 55;
const int SERVO_CLOSE_ANGLE = 140;
const int SERVO_CLOSE_DELAY = 1000; //close the door 1000 ms after it was opened

const int LEDPIN = 13;

const int GATE_INTERMEDIATE_DELAY = 100;//delay ata intermediate position while closing so it doesn't slam into the field

typedef enum {
  STATE_TILT_RIGHT, STATE_TILT_LEFT, STATE_NEUTRAL, STATE_UNLOAD, STATE_START
} teetertotter_states ;

typedef enum {
  STATE_A_COMPLETE, STATE_GATE_OPEN, STATE_SIDE_START
} side_states ;

//pins
const int FR1_tilt_left_button = 8;
const int FR1_tilt_right_button = 7;
const int FR1_servo_pin = 6;
const int FR1_open_button = 4;

const int FR2_tilt_left_button = 18;
const int FR2_tilt_right_button = 19;
const int FR2_servo_pin = 20;
const int FR2_open_button = 5;

const int PO_tilt_left_button = 14;
const int PO_tilt_right_button = 15;

const int gate_right_servo_pin = 10;
const int gate_right_sensor_pin = A9;
const int gate_left_servo_pin = 9;
const int gate_left_sensor_pin = A2;

const int gate_left_open_button = 17;
const int gate_right_open_button = 11;


#endif

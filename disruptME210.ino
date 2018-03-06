#include <Servo.h> 
#include "params.h"
#include "fundinground.h"
#include "patentoffice.h"
#include "side.h"

teetertotter_states FR1_state;
//teetertotter_states FR2_state;
//teetertotter_states PO_state;

funding_round fr1(FR1_tilt_left_button,FR1_tilt_right_button,FR1_servo_pin,FR1_open_button);
//funding_round fr2(FR2_tilt_left_button,FR2_tilt_right_button,FR2_servo_pin,FR2_open_button);
//patent_office po(PO_tilt_left_button,PO_tilt_right_button);

//side left_side(gate_left_servo_pin);
//side right_side(gate_right_servo_pin);

void setup() {
  
  pinMode(LEDPIN,OUTPUT);
  digitalWrite(LEDPIN, HIGH);

  fr1.setup();
//  fr2.setup();
//  po.setup();

//  left_side.setup();
//  right_side.setup();
  
  Serial.begin(9600); //debug print
  Serial1.begin(9600); //beacon print

}

void loop() {

  fr1.update(); //check inputs and control servo to open the door
//  fr2.update();
//  po.update(); //check inputs

//  left_side.update((fr1.get_state() == STATE_TILT_LEFT),(fr2.get_state() == STATE_TILT_LEFT),(po.get_state() == STATE_TILT_LEFT));
//  right_side.update((fr1.get_state() == STATE_TILT_RIGHT),(fr2.get_state() == STATE_TILT_RIGHT),(po.get_state() == STATE_TILT_RIGHT));

  //Serial.println(fr1.get_state());

  /*
   * Beacons:
   * FRA beacon always on, 950 Hz. D.C: 25% losing, 50% neutral,  75% winning //1 25% , 2 50% ,3 75%
   * FRB beacon on after FRA tipped once, 1350 Hz, D.C: 25% losing, 50% neutral, 75% winning//11 25%,12 50%,13 75% ,14 off, 15 on
   * Startup Incubator beacon always on, 3500 Hz
   * Gate on when FRB & (FRA | PO), 4125 Hz //21 off , 22 on
   */


}


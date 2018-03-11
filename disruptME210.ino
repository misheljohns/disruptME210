#include <Servo.h> 
#include "params.h"
#include "fundinground.h"
#include "patentoffice.h"
#include "side.h"

/*
 * todo:
 * gate resistor
 * button open for gates x2 done
 * button open FR x2 done
 * gate sensor to stop gate_close - todo - Kirsten/Gabby?
 * change state_unload open for bins to door open so we can override in the middle of a game if necessary
 * reset after round if all 4 buttons flipped then unflipped - done
 * 
 */

funding_round fr1(FR1_tilt_left_button,FR1_tilt_right_button,FR1_servo_pin,FR1_open_button); //FRA for left side
funding_round fr2(FR2_tilt_left_button,FR2_tilt_right_button,FR2_servo_pin,FR2_open_button); //FRB for left side
patent_office po(PO_tilt_left_button,PO_tilt_right_button);

side left_side(gate_left_servo_pin, gate_left_open_button, gate_left_sensor_pin, 20, 40, 60, 15, 130, 135, 'L');
side right_side(gate_right_servo_pin, gate_right_open_button, gate_right_sensor_pin, 40, 20, 0, 15, 140, 147, 'R');

bool waiting_for_reset = false;

void setup() {
  
  pinMode(LEDPIN,OUTPUT);
  digitalWrite(LEDPIN, LOW);
  
  Serial.begin(9600); //debug print
  Serial1.begin(9600); //beacon control

  fr1.setup();
  fr2.setup();
  po.setup();

  left_side.setup();
  right_side.setup();
  
  for(int i = 0; i <= 80000000; i++); //80million cycles ~ 1s
  setupbeacons();
  
  digitalWrite(LEDPIN, HIGH);
}

void loop() {

  if(waiting_for_reset)
  {
    if(digitalRead(FR1_open_button) && digitalRead(FR2_open_button) && digitalRead(gate_left_open_button) && digitalRead(gate_right_open_button)) //all of them are unflipped
    {
      //reset when everything is unflipped
      waiting_for_reset = false;
      
      digitalWrite(LEDPIN, LOW);
      
      fr1.setup();
      fr2.setup();
      po.setup();
    
      left_side.setup();
      right_side.setup();
      
      for(int i = 0; i <= 80000000; i++); //80million cycles ~ 1s
      setupbeacons();
      
      digitalWrite(LEDPIN, HIGH);
    }
  }
  else
  {
    if(!(digitalRead(FR1_open_button) || digitalRead(FR2_open_button) || digitalRead(gate_left_open_button) || digitalRead(gate_right_open_button))) //none of them are unflipped
    {
      waiting_for_reset = true;
    }//finish the loop so the system responds to the command by opening doors and gates
    fr1.update(); //check inputs and control servo to open the door
    fr2.update();
    po.update(); //check inputs
  
    //1 if tipped in your favour, 0 if neutral, -1 if against you
    left_side.update((fr1.get_state() == STATE_TILT_LEFT)? 1 : (fr1.get_state() == STATE_TILT_RIGHT)? -1 : 0,
                     (fr2.get_state() == STATE_TILT_LEFT)? 1 : (fr2.get_state() == STATE_TILT_RIGHT)? -1 : 0,
                     (po.get_state() == STATE_TILT_LEFT)? 1 : (po.get_state() == STATE_TILT_RIGHT)? -1 : 0);
    right_side.update((fr2.get_state() == STATE_TILT_RIGHT)? 1 : (fr2.get_state() == STATE_TILT_LEFT)? -1 : 0,
                      (fr1.get_state() == STATE_TILT_RIGHT)? 1 : (fr1.get_state() == STATE_TILT_LEFT)? -1 : 0,
                      (po.get_state() == STATE_TILT_RIGHT)? 1 : (po.get_state() == STATE_TILT_LEFT)? -1 : 0);
  }

}

void setupbeacons()
{
  //gate_right
  Serial1.write((char) 0 + 6);//switch off top half
  Serial1.write((char) 0 + 8);//switch off bot half

  //FRA_Left
  Serial1.write((char) 20 + 7);//switch on top half
  Serial1.write((char) 20 + 1);//top half at 50% DC
  Serial1.write((char) 20 + 8);//switch off bot half

  //FRB_Left
  Serial1.write((char) 40 + 7);//switch on top half
  Serial1.write((char) 40 + 1);//top half at 50% DC
  Serial1.write((char) 40 + 8);//switch off bot half
  
  //gate_left
  Serial1.write((char) 60 + 6);//switch off left half
  Serial1.write((char) 60 + 8);//switch off right half
}

/*
 * Beacons:
 * FRA beacon always on, 950 Hz. D.C: 25% losing, 50% neutral,  75% winning //offset 20
 * FRB beacon on after FRA tipped once, 1350 Hz, D.C: 25% losing, 50% neutral, 75% winning//offset 40
 * Startup Incubator beacon always on, 3500 Hz
 * Gate on when FRB & (FRA | PO), 4125 Hz //offset 0, offset 60
 */
   
/*
 * 
 * FR beacons: Top = 950Hz = FRA, bottom = 1350 Hz = FRB
 * Offsets:
 * Gate_Right: 0
 * FRA_Left: 20
 * FRB_Left: 40
 * Gate_Left: 60
 * 
 * Left = Top,
 * Right = Bottom
 * Beacons:
 *  case 0: // Set 25% DC to Top LEDs
    case 1: // Set 50% DC to Top LEDs
    case 2: // Set 75% DC to Top LEDs
    case 3: // Set 25% DC to Bot LEDs
    case 4: // Set 50% DC to Bot LEDs
    case 5: // Set 75% DC to Bot LEDs
    case 6: // Turn OFF Top LEDs
    case 7: // Turn ON Top LEDs
    case 8: // Turn OFF Bot LEDs
    case 9: // Turn On Bot LEDs
 *
 */

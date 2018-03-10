#ifndef side_h
#define side_h

#include <Servo.h>

class side
{
private:              
   const int gate_servo_pin;
   const int gate_open_pin;   
   const int gate_sensor_pin;
   const int gate_open_angle;
   const int gate_intermediate_angle;
   const int gate_close_angle; 
   const char sidename;
   const byte frA_offset,frB_offset, gate_offset;
   
   Servo servo;
   
   side_states state;
   int frA_tipped_t, frB_tipped_t;
   unsigned long gate_intermed_time;
   bool robot_at_gate;
   
public:
  side(int gate_servo_pin_in, int gate_open_pin_in, int gate_sensor_pin_in, byte fra_o, byte frb_o, byte g_o, int gate_open_angle_in, int gate_intermediate_angle_in, int gate_close_angle_in, char sidename_in):
	gate_servo_pin(gate_servo_pin_in),
  gate_open_pin(gate_open_pin_in),
  gate_sensor_pin(gate_sensor_pin_in),
	frA_offset(fra_o),
	frB_offset(frb_o),
	gate_offset(g_o),
  gate_open_angle(gate_open_angle_in),
  gate_intermediate_angle(gate_intermediate_angle_in),
  gate_close_angle(gate_close_angle_in),
  sidename(sidename_in)
  {
	  
  }
  
  void setup()
  {
    pinMode(gate_open_pin, INPUT_PULLUP);
    pinMode(gate_sensor_pin, INPUT);
    
	  servo.attach(gate_servo_pin, 1000, 2000); //adjust pulse width (microsecs)
    servo.write(gate_close_angle);
    state = STATE_SIDE_START;
    gate_intermed_time = (unsigned long) 2000000000; 
    robot_at_gate = false;
    frA_tipped_t = 0;
    frB_tipped_t = 0;
  }
  
  void update(int frA_tipped, int frB_tipped, int po_tipped)
  {
    check_for_gate_robot();
    
    switch(state) {
      case STATE_SIDE_START:
      //beacon
      //change state, beacons if A tipped
        if(frA_tipped == 1)
        {
          state = STATE_A_COMPLETE;
          //turn on frB beacon at a DC based on the state
          Serial1.write((char)(frB_offset + 9));
          Serial.print(sidename);
          Serial.println(" A complete");
        }
      break;
      
      case STATE_A_COMPLETE: 
        if(millis() > gate_intermed_time + GATE_INTERMEDIATE_DELAY)
        {
          servo.write(gate_close_angle);
          gate_intermed_time = (unsigned long) 2000000000; 
        }
        if((frB_tipped == 1) && ((frA_tipped == 1)|(po_tipped == 1)) || (!digitalRead(gate_open_pin))) //conditions met for gate open!
        {
          //open gate (change state)
          servo.write(gate_open_angle);
          gate_intermed_time = (unsigned long) 2000000000; 
          state = STATE_GATE_OPEN;
          //turn on gate beacon
          Serial1.write((char)(gate_offset + 7));
          Serial.print(sidename);
          Serial.println(" Gate open");
        }
      break;
      
      case STATE_GATE_OPEN:
      //change state if bin untipped AND sensor does not detect a robot
        if(!((frB_tipped == 1) && ((frA_tipped == 1)|(po_tipped == 1))) && (digitalRead(gate_open_pin))) //lost a bin!
        {
          //close gate
          servo.write(gate_intermediate_angle);
          gate_intermed_time = millis();
          state = STATE_A_COMPLETE;
          //turn off gate beacon
          Serial1.write((char)(gate_offset + 6));
          Serial.print(sidename);
          Serial.println(" Gate close");
        }
      break;
      
      default:    // Should never get into an unhandled state
        Serial.println("What is this I do not even...");
    }
    
    //change duty cycles when flipped, in all states
    if(frA_tipped != frA_tipped_t)
    {
      Serial1.write((char)(frA_offset + 1 + frA_tipped)); 
      Serial.print(sidename);
      Serial.println(" FRA_change");
      frA_tipped_t = frA_tipped;
    }
    if(frB_tipped_t != frB_tipped)
    {
      Serial1.write((char)(frB_offset + 4 + frB_tipped)); 
      Serial.print(sidename);
      Serial.println(" FRB_change");
      frB_tipped_t = frB_tipped;
    }
  }

  void check_for_gate_robot() {
    //Serial.println(analogRead(gate_sensor_pin));
    if(analogRead(gate_sensor_pin) >= 100)
      robot_at_gate = true;
  }
                     
};

#endif


/*
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

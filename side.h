#ifndef side_h
#define side_h

#include <Servo.h>

class side
{
private:              
   const int gate_servo_pin;    
   Servo servo;
   side_states state;
   const byte frA_offset,frB_offset, gate_offset;
   int frA_tipped_t, frB_tipped_t;
   
public:
  side(int gate_servo_pin_in, byte fra_o, byte frb_o, byte g_o):
	gate_servo_pin(gate_servo_pin_in),
  frA_offset(fra_o),
  frB_offset(frb_o),
  gate_offset(g_o)
  {
	
  }
  
  void setup()
  {
	  servo.attach(gate_servo_pin, 1000, 2000); //adjust pulse width (microsecs)
    servo.write(GATE_CLOSE_ANGLE);
    state = STATE_SIDE_START;
  }
  
  void update(int frA_tipped, int frB_tipped, int po_tipped)
  {
    switch(state) {
      case STATE_SIDE_START:
      //beacon
      //change state, beacons if A tipped
        if(frA_tipped == 1)
        {
          state = STATE_A_COMPLETE;
          //turn on frB beacon at a DC based on the state
          Serial1.print((char)(frB_offset + 9));
          Serial1.print((char)(frB_offset + 4 + frB_tipped));
        }
      break;
      
      case STATE_A_COMPLETE: 
        if((frB_tipped == 1) && ((frA_tipped == 1)|(po_tipped == 1))) //conditions met for gate open!
        {
          //open gate (change state)
          servo.write(GATE_OPEN_ANGLE);
          state = STATE_GATE_OPEN;
          //turn on gate beacon
          Serial1.print((char)(frB_offset + 7));
        }
      break;
      
      case STATE_GATE_OPEN:
      //change state if bin untipped AND sensor does not detect a robot
        if(!((frB_tipped == 1) && ((frA_tipped == 1)|(po_tipped == 1)))) //lost a bin!
        {
          //close gate
          servo.write(GATE_CLOSE_ANGLE);
          state = STATE_A_COMPLETE;
          //turn on gate beacon
          Serial1.print((char)(frB_offset + 7));
        }
      break;
      default:    // Should never get into an unhandled state
        Serial.println("What is this I do not even...");
    }
    
    //change duty cycles when flipped, in all states
    if(frA_tipped != frA_tipped_t)
    {
      Serial1.print((char)(frA_offset + 1 + frA_tipped));
      frA_tipped_t = frA_tipped;
    }
    if(frB_tipped_t != frB_tipped)
    {
      Serial1.print((char)(frB_offset + 4 + frB_tipped));
      frB_tipped_t = frB_tipped;
    }
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

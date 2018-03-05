#ifndef side_h
#define side_h

#include <Servo.h>

class side
{
private:              
   const int gate_servo_pin;    
   Servo servo;
   side_states state;
   
public:
  side(int gate_servo_pin_in):
	gate_servo_pin(gate_servo_pin_in)
  {
	
  }
  
  void setup()
  {
	  servo.attach(gate_servo_pin, 1000, 2000); //adjust pulse width (microsecs)
  }
  
  void update(bool frA_tipped, bool frB_tipped, bool po_tipped)
  {
    switch(state) {
      case STATE_SIDE_START:
      //change state, beacons if A tipped
      
      break;
      
      case STATE_A_COMPLETE:
      //change beacons if B tipped, A untipped, A tipped
      //open gate (change state) if B tipped + either A or PO

      break;
      
      case STATE_GATE_OPEN:
      //change state if bin untipped AND sensor does not detect a robot

      break;
      default:    // Should never get into an unhandled state
        Serial.println("What is this I do not even...");
    }
  }
                     
};

#endif

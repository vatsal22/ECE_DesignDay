#include "ArduinoMotorShieldR3.h"

#define TIMER_MAX 781 //OCR1A = [16 MHz / (2 * N * fDesired)] - 1, N is prescalar (1024)
//I put in a timer interrupt if you want one. Use the equation above and set TIMER_MAX to get fDesired.
//That is, it will call ISR(TIMER1_COMPA_vect) every 1/fDesired seconds. The default value gives 10 Hz.

ArduinoMotorShieldR3 md;
int speedA = 0;
int speedB = 0;

void setup()
{
  md.init();
  md.setSpeed2(0, 0);
  md.clearBrake2();
  pinMode(ENCODER_1, INPUT); // set ENCODER_1 to input
  pinMode(ENCODER_2, INPUT); // set ENCODER_2 to input
  InitializeInterrupt();
  interrupts();
 // while (!Serial) {
 //   ; // wait for serial port to connect. Needed for native USB port only
 // }
  Serial.println("Hi");
  Serial.begin(115200); //115200 baud, 8 data bits, 1 stop bit, no parity, XON/XOFF flow control

  Serial.println("UW ECE Ideas Clinic Pitching Machine");
  
}


void loop()
{
    //Serial.read();
    //speedA=50;
    md.clearBrake2();
    md.setSpeed2(speedA, speedB);

 //   while (Serial.available()){
      if (Serial.available() == 0){
        md.setBrake2();
      }else{
        //md.setSpeed2(0,0);
        md.clearBrake2();
        int temp = Serial.parseInt();
        if (temp!=0){
          speedA = temp;
        }
        speedB = speedA;
      }
    //}
    /*
    if (Serial.available()) {
    Serial.read();
    speedA = Serial.parseInt();
    }
  //while (Serial.available() > 0) {
  //    Serial.read();
 // }
  //return;
  */
}



void InitializeInterrupt() //Don't mess with this function - it sets up the control registers for the IR sensor and timer interrupts
{
  cli();    // switch interrupts off while messing with their settings
  
  PCICR   = 0x02;   // Enable PCINT1 interrupt
  PCMSK1  = 0b00001100;
  
  PRR    &= ~0x04;   //Enable Timer/Counter1
  TCCR1A  = 0b00000000; //No output pins, WGM11 = 0, WGM10 = 0
  TCCR1B  = 0b00001101; //WGM12 = 1, WGM13 = 0, CS1 = 0b101 (clk/1024)
  OCR1A   = TIMER_MAX; //Set count
  TIMSK1 |= 0b00000010; //OCIE1A = 1 (interrupt enable for OCR1A)
  
  sei();    // turn interrupts back on
}

ISR(PCINT1_vect) //Encoder Interrupt Service Routine
{
//This will trigger each time either of the IR sensors experiences any change in state
}

ISR(TIMER1_COMPA_vect) //Timer Interrupt Service Routine
{
//This will trigger at a frequency determined by TIMER_MAX
}



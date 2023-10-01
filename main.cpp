//====================================================
// title block
//====================================================
/*  
 *  Author: Jacob Owens - Owens Tech LLC
 *  Customer: Dawson Swan
 *  Date: 09302023
 */
//====================================================
// end title block
//====================================================

//====================================================
// included libraries
//====================================================
#include <Arduino.h>
//====================================================
// end included libraries
//====================================================

//====================================================
// stepper definitions
//====================================================
#include <AccelStepper.h>
#define STEPPER_ENABLE_PIN 3
#define STEPPER_STEP_PIN 4
#define STEPPER_DIRECTION_PIN 5
AccelStepper stepper1(AccelStepper::FULL2WIRE, STEPPER_STEP_PIN, STEPPER_DIRECTION_PIN);
int stepper1Position = 0;
int stepperCommand = 0;
int speedMode = 0; // 0 set speed, 1 auto speed
long stepperSpeed = 0;
//====================================================
// end stepper definitions
//====================================================





//====================================================
// light sensor definitions
//====================================================
#define ULTRASONIC_PIN A1
long light_value = 0;
volatile int lightDelay = 0;
bool lightFlag = 0;
long ave_light_value = 0;

const int numSamples = 10;
long samples[numSamples] = {0};
int sampleNumber = 0;
//====================================================
// end light sensor definitions
//====================================================



//====================================================
// led definitions
//====================================================
#define HEART_LED 13
bool heartLedFlag = false;
volatile int heartLedDelay = 0;
//====================================================
// end led definitions
//====================================================


//====================================================
// function prototypes
//====================================================
void setup(void);
void loop(void);
void HeartLedMachine(void);
void LightSensorMachine(void);
void stepperMachine(void);
//====================================================
// end function prototypes
//====================================================




//====================================================
// states
//====================================================
enum LedStates {
  LED_OFF,
  LED_ON
};
LedStates HeartLedState;

enum LightStates {
  UT_WAITING,
  UT_READING
};
LightStates LightState;
//====================================================
// end states
//====================================================


//====================================================
// timer
//====================================================
volatile int interruptCounter;
ISR(TIMER1_COMPA_vect) {
    interruptCounter++;

    // every second
    heartLedDelay--;

  // every 10 seconds
  if ((interruptCounter % 10) == 0) {
    //if (blueLedDelay) blueLedDelay--;
    interruptCounter = 0;
  }

}
//====================================================
// end timer
//====================================================



//====================================================
// setup
//====================================================
void setup() {

    pinMode(HEART_LED, OUTPUT);
    // initialize Timer1
    cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B

    // set compare match register to desired timer count:
    OCR1A = 15624;

    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);

    // Set CS10 and CS12 bits for 1024 prescaler:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);

    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);

    // enable global interrupts:
    sei();


    // setup light
    pinMode(ULTRASONIC_PIN, INPUT);

}
//====================================================
// end setup
//====================================================




//====================================================
// main loop
//====================================================
void loop() {

  HeartLedMachine();

//   LightSensorMachine();
// 
//   StepperMachine();
}
//====================================================
// end main loop
//====================================================




//====================================================
// functions
//====================================================
//====================================================
// heart led machine
//====================================================
void HeartLedMachine() {
  switch(HeartLedState) {
    case LED_OFF:
      if (!heartLedDelay && !heartLedFlag) {
        //ledFlag = false;
        heartLedDelay = 1;
        HeartLedState = LED_ON;
        digitalWrite(HEART_LED, LOW);
      }
    break;
    case LED_ON:
      if (!heartLedDelay && !heartLedDelay) {
        //ledFlag
        heartLedDelay = 1;
        HeartLedState = LED_OFF;
        digitalWrite(HEART_LED, HIGH);
      }
    break;
    default:
    break;
  }
}
//====================================================
// end heart led machine
//====================================================







//====================================================
// ultrasonic machine
//====================================================
//void UltrasonicMachine() {
//  switch(UltrasonicState){
//    case UT_WAITING:
//      if (!ultrasonicDelay) {
//        UltrasonicState = UT_READING;
//      }
//    break;
//    case UT_READING:
//      ultrasonic_value = analogRead(ULTRASONIC_PIN);
//      samples[sampleNumber++] = ultrasonic_value;
//
//      if (sampleNumber >= numSamples) {sampleNumber = 0;}
//
//      ave_ultrasonic_value = 0;
//
//      for(int i=0; i< numSamples; ++i){ave_ultrasonic_value += samples[i];}
//
//      ave_ultrasonic_value /= numSamples;
//
//      if (ave_ultrasonic_value >= 10000.0) ave_ultrasonic_value = 10000.0;
//      else if (ave_ultrasonic_value <= 0.0) ave_ultrasonic_value = 0;
//
//      ultrasonicDelay = 40;
//      UltrasonicState = UT_WAITING;
//    break;
//    default:
//    break;
//  }
//
//}
//
////====================================================
// end ultrasonic machine
//====================================================









//====================================================
// stepper machine
////====================================================
//void StepperMachine(void) {
//  switch(stepperState) {
//    case STEPPER_STOPPED:
//      stepper1.stop();
//    break;
//    case STEPPER_FORWARD:
//      stepper1.move(10);
//    break;
//    case STEPPER_BACKWARD:
//      stepper1.move(-10);
//    break;
//    default:
//    break;
//  }
//  stepper1.run();
//
//}
//====================================================
// end stepper machine
//====================================================




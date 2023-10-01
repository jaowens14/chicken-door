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
#include <Stepper.h>
const int stepsPerRevolution = 200; 
// number of revolutions desired
const int revs = 2;
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8,11,12,13);       
int stepperDelay = 0;    
//====================================================
// end stepper definitions
//====================================================



//====================================================
// light sensor definitions
//====================================================
#define LIGHT_PIN A0
long light_value = 0;
volatile int lightDelay = 0;
bool lightFlag = 0;
bool lastLightFlag = 0;
long ave_light_value = 500;

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
// door definitions
//====================================================
bool doorFlag = false;
volatile int doorDelay = 0;
int indicator = 0;
//====================================================
// end door definitions
//====================================================


//====================================================
// function prototypes
//====================================================
void setup(void);
void loop(void);
void HeartLedMachine(void);
void LightSensorMachine(void);
void StepperMachine(void);
void DoorMachine(void);
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
  WAITING,
  READING
};
LightStates LightState;

enum StepperStates { 
  OFF,
  CW,
  CCW,
};
StepperStates StepperState;

enum DoorStates {
  UP,
  DOWN,
  IDLE,
};
DoorStates DoorState;

//====================================================
// end states
//====================================================


//====================================================
// timer
//====================================================
volatile int interruptCounter;
ISR(TIMER1_COMPA_vect) {
    interruptCounter++;

  // every 1/10th second
  if(heartLedDelay) heartLedDelay--;
  if(lightDelay) lightDelay--;



  // every second
  if ((interruptCounter % 10) == 0) {
    if(doorDelay) doorDelay--;
    if(stepperDelay) stepperDelay--;
  }

  if ((interruptCounter % 1000) == 0) {
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
    delay(5000);
    pinMode(HEART_LED, OUTPUT);
    // initialize Timer1
    cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B

    // set compare match register to desired timer count:
    OCR1A = 24999;

    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);

    // Set CS10 and CS12 bits for 1024 prescaler:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS11);

    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);

    // enable global interrupts:
    sei();


    // setup light
    pinMode(LIGHT_PIN, INPUT);


    // set the speed at 30 rpm:
    myStepper.setSpeed(30);
    // initialize the serial port:
    Serial.begin(9600);
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);

    StepperState = OFF;
    DoorState = IDLE;

}
//====================================================
// end setup
//====================================================




//====================================================
// main loop
//====================================================
void loop() {

    HeartLedMachine();

    LightSensorMachine();
    DoorMachine();
    StepperMachine();

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
        heartLedDelay = indicator;
        HeartLedState = LED_ON;
        digitalWrite(HEART_LED, LOW);
      }
    break;
    case LED_ON:
      if (!heartLedDelay && !heartLedDelay) {
        //ledFlag
        heartLedDelay = indicator;
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
// light sensor machine
//====================================================
void LightSensorMachine() {
  switch(LightState){
    case WAITING:
      //Serial.println("Waiting to read");
      //Serial.println(lightDelay);
      if (!lightDelay) {
        LightState = READING;
      }
    break;
    case READING:
      Serial.println("Reading sensor");
      light_value = analogRead(LIGHT_PIN);
      samples[sampleNumber++] = light_value;

      if (sampleNumber >= numSamples) {sampleNumber = 0;}

      ave_light_value = 0;

      for(int i=0; i< numSamples; ++i){ave_light_value += samples[i];}

      ave_light_value /= numSamples;

      Serial.println("Ave light sensor value: ");
      Serial.println(ave_light_value);
      Serial.println();

      if(ave_light_value <= 600) lightFlag = false;
        
      else lightFlag = true;

      Serial.println("last light flag");
      Serial.println(lastLightFlag);
      Serial.println("light flag");
      Serial.println(lightFlag);

      lightDelay = 10;
      LightState = WAITING;
    break;
    default:
    break;
  }

}

//====================================================
// end light sensor machine
//====================================================


//====================================================
// stepper machine
//====================================================
void StepperMachine(void) {
  switch(StepperState) {
    case OFF:
      digitalWrite(9, LOW);
      digitalWrite(10,LOW);
    break;
    case CW:
      digitalWrite(9,HIGH);
      digitalWrite(10,HIGH);
      myStepper.step(stepsPerRevolution*revs);
      Serial.println("CW");
      StepperState = OFF;
    break;
    case CCW:
      digitalWrite(9,HIGH);
      digitalWrite(10,HIGH);
      myStepper.step(-stepsPerRevolution*revs);
      Serial.println("CCW");
      StepperState = OFF;

    break;
    default:
    break;
  }

}
//====================================================
// end stepper machine
//====================================================


//====================================================
// door machine
//====================================================
void DoorMachine(void) {
  switch(DoorState) {
    case UP:
      lastLightFlag = lightFlag;
      if(!doorDelay){
        Serial.println("Door going down");
        DoorState = IDLE;
        StepperState = CW;
        doorDelay = 15;
        indicator = 8;
      }
    break;

    case DOWN:
      lastLightFlag = lightFlag;
      if(!doorDelay){
        Serial.println("Door going up");
        DoorState = IDLE;
        StepperState = CCW;
        doorDelay = 15;
        indicator = 2;
      }
    break;
    case IDLE:
      if(lastLightFlag != lightFlag && lightFlag && !doorDelay) DoorState = DOWN;

      if(lastLightFlag != lightFlag && !lightFlag && !doorDelay) DoorState = UP;
    break;
    default:
    break;
  }
}


//====================================================
// end door machine
//====================================================

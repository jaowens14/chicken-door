
const byte LED = 13;

ISR(TIMER1_COMPA_vect)
{
static boolean state = false;
  state = !state;  // toggle
  digitalWrite (LED, state ? HIGH : LOW);
}

void setup() {
  pinMode (LED, OUTPUT);
  
  // set up Timer 1
  TCCR1A = 0;          // normal operation
  TCCR1B = bit(WGM12) | bit(CS10) | bit (CS12);   // CTC, scale to clock / 1024
  OCR1A =  999;       // compare A register value (1000 * clock speed / 1024)
  TIMSK1 = bit (OCIE1A);             // interrupt on Compare A Match
} // end of setup

void loop() { }
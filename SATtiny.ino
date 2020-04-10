#include <avr/sleep.h>
#include <avr/wdt.h>

/*

 The connections to the ATTiny are as follows:
 ATTiny    Arduino    Info
 Pin  1  - 5          RESET / Rx (Not receiving any data)
 Pin  2  - 3          Tx for serial conenction
 Pin  3  - 4          FET driver (PWM)
 Pin  4  -            GND
 Pin  5  - 0          RED LED (PWM)
 Pin  6  - 1          GREEN LED
 Pin  7  - 2 / A1     Vsensor (Analog)
 Pin  8  -   +Vcc
 
*/
// Utility macro
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define fadeTablesize 17
#define prefadeTablesize 6
int led = 0;           // the PWM pin the LED is attached to
int prefadeTable[]={0,121,184,218,235,245};
int fadeTable[]={255,217,185,158,134,115,98,83,71,60,51,44,37,32,27,23,20};

volatile boolean f_wdt = 1;
int counter = 0;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
 
  setup_watchdog(8); // approximately 2 seconds sleep

}

void system_sleep(void) 
{
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  sleep_mode();                        // System sleeps here
  sleep_disable();                     // System continues execution here when watchdog timed out 
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON

}

// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms

// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec

void setup_watchdog(int ii) {
  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;
  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}

// Watchdog Interrupt Service / is executed when watchdog timed out

ISR(WDT_vect) {
  counter++;
  f_wdt=1;  // set global flag
}

void flashLed (void){
for(int i=0;i<prefadeTablesize;i++){
     analogWrite(led, prefadeTable[i]);
     delay(50);
  }
  for(int i=0;i<fadeTablesize;i++){
     analogWrite(led, fadeTable[i]);
     delay(50);
  }
  analogWrite(led, 0);
  
}

// the loop routine runs over and over again forever:
void loop() {
  if (f_wdt==1) {  // wait for timed out watchdog / flag is set when a watchdog timeout occurs
    f_wdt=0;       // reset flag
    flashLed ();
    pinMode(led,INPUT); // set all used port to intput to save power
    system_sleep();
    pinMode(led,OUTPUT); // set all ports into state before sleep
  }

}

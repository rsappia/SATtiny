#include <avr/sleep.h>
#include <avr/wdt.h>

/*
 * 
 * Witho no light:
 * 4,5uA/6,5uA
 * With light
 * 4,5uA/1,2mA for 0,7seg

 The connections to the ATTiny are as follows:
 ATTiny    Arduino    Info
 Pin  1  - 5          RESET 
 Pin  2  - 3          Solar cell
 Pin  3  - 4         
 Pin  4  -            GND
 Pin  5  - 0          RED LED (PWM)
 Pin  6  - 1          RED LED 2 (PWM)
 Pin  7  - 2 / A1     
 Pin  8  -   +Vcc
 
*/
// Utility macro
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define PATTERN_1 0
#define PATTERN_2 1
#define PATTERN_3 2
#define PATTERN_BOTH 3

#define led_1  0           // the PWM pin the LED is attached to
#define led_2  1           // the PWM pin the LED is attached to
#define solar_cell 3
#define fadeTablesize 17
#define fadeTablesize_2 13
#define prefadeTablesize 6



int prefadeTable[]={0,121,184,218,235,245};
int fadeTable[]={255,217,185,158,134,115,98,83,71,60,51,44,37,32,27,23,20};
int fadeTable_2[]={255,255,255,134,98,83,71,51,44,37,32,23,20};
int wdt_delay = 9;
volatile boolean f_wdt = 1;
int tick_counter=0;
int swap_led= led_1;

void setup() {
  pinMode(led_1, OUTPUT);
  pinMode(led_2,OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT);
  pinMode(4, INPUT_PULLUP);
 
  setup_watchdog(9); // approximately 8 seconds sleep

}

void system_sleep(void) 
{
  setup_watchdog(wdt_delay); 
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
  f_wdt=1;  // set global flag
}

void flashLed (int pattern){
  

  switch (pattern) {

    case PATTERN_1:
       pinMode(led_1,OUTPUT); // set all ports into state before sleep
       pattern_1(led_1);
       analogWrite(led_1, 0);
    break;

    case PATTERN_2:
       pinMode(led_2,OUTPUT); // set all ports into state before sleep
       pattern_2(led_2);
       analogWrite(led_2, 0);
    break;

     case PATTERN_3:
       pinMode(led_1,OUTPUT); // set all ports into state before sleep
       pattern_3(led_1);
       analogWrite(led_1, 0);
    break;

    case PATTERN_BOTH:
        pinMode(led_2,OUTPUT); // set all ports into state before sleep
         pattern_1(led_2);
         analogWrite(led_2, 0);

         pinMode(led_1,OUTPUT); // set all ports into state before sleep
         pattern_1(led_1);
         analogWrite(led_1, 0);
    break;
  }
  
  pinMode(led_1,INPUT); // set all used port to intput to save power
  pinMode(led_2,INPUT); // set all used port to intput to save power
  
}

// the loop routine runs over and over again forever:
void loop() {
  if (f_wdt==1 ) {  // wait for timed out watchdog / flag is set when a watchdog timeout occurs
    f_wdt=0;       // reset flag
    
    if(digitalRead(solar_cell)==0){
      
     wdt_delay=8;

     flashLed (tick_counter);
          
      tick_counter++;
      if(tick_counter>3){
        tick_counter=0;
      }
      
     }
    else{
     wdt_delay=9; //minimize the cycles if light is detected
    }

    system_sleep();
    
  }

}

void pattern_1(int pin){

  for(int i=0;i<prefadeTablesize;i++){
     analogWrite(pin, prefadeTable[i]);
     delay(30);
  }
  analogWrite(pin,255);
  delay(100);
  for(int i=0;i<fadeTablesize;i++){
     analogWrite(pin, fadeTable[i]);
     delay(30);
  }

 } 

void pattern_2(int pin){

for (int j=0;j<2;j++){
    for(int i=0;i<prefadeTablesize;i++){
       analogWrite(pin, prefadeTable[i]);
       delay(60);
    }
    for(int i=0;i<fadeTablesize;i++){
       analogWrite(pin, fadeTable[i]);
       delay(10);
    }
  }
 }

 void pattern_3(int pin){

    for(int i=0;i<prefadeTablesize;i++){
       analogWrite(pin, prefadeTable[i]);
       delay(30);
    }

    for(int i=0;i<fadeTablesize_2;i++){
       analogWrite(pin, fadeTable_2[i]);
       delay(35);
    }

    for(int i=0;i<prefadeTablesize;i++){
       analogWrite(pin, prefadeTable[i]/2);
       delay(20);
    }
    for(int i=0;i<fadeTablesize_2;i++){
       analogWrite(pin, fadeTable_2[i]/2);
       delay(25);
    }
  
 }

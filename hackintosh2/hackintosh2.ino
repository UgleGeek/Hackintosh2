//
// Hackintosh2
//
// Description of the project
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author 		Erik Steen Sørensen
// 				Erik Steen Sørensen
//
// Date			25/03/17 12:35
// Version		<#version#>
//
// Copyright	© Erik Steen Sørensen, 2017
// Licence		<#licence#>
//
// See         ReadMe.txt for references
//


// Core library for code-sense - IDE-based
#if defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(ESP8266) // ESP8266 specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.8 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif // end IDE

// Set parameters


// Include application, user and local libraries


// Define structures and classes
enum pc_state {OFF, ON, SLEEP};

// Define variables and constants
#define POWER_LED 3
#define POWER_IN 4
#define PCB_LED 13
#define INCREMENT 10
#define INPULSE 1000
#define OFF_VAL 0
#define ON_VAL 25500

pc_state inState, outState;
// Prototypes


// Utilities


// Functions
void scheduler(void);
void one_ms(unsigned long time_stamp);
void ten_ms(unsigned long time_stamp);
void hun_ms(unsigned long time_stamp);
void handle_power_led(int taskdur);

// ----------------------------------------------------------------------------

void setup()
{
inState = OFF;
pinMode(POWER_LED, OUTPUT);
pinMode(POWER_IN, INPUT);
pinMode(PCB_LED, OUTPUT);
pinMode(2, INPUT); // temporary
}

void one_ms(unsigned long time_stamp)
{
;
}

// ----------------------------------------------------------------------------

void ten_ms(unsigned long time_stamp)
{
handle_power_led(10);
}

// ----------------------------------------------------------------------------

void hun_ms(unsigned long time_stamp)
{
;
}

// ----------------------------------------------------------------------------

void handle_power_led(int taskdur)
{
static int out = 0;
static int increment_sign = 1;
static int cntr = 0;
static int fcntr = 0;
static bool prev_inp = false;
bool inp;

inp = digitalRead(POWER_IN);
digitalWrite(PCB_LED, inp);  // debug

if(inp != prev_inp)
{
fcntr++;
prev_inp = inp;
}

if(cntr++ >= INPULSE/taskdur)
{
if(fcntr > 1)       {  inState = SLEEP; }
else  {
if(inp == true) {  inState = ON;    }
else            {  inState = OFF;   }
}
fcntr = 0;
cntr = 0;
}

switch(inState)
{
case SLEEP:
if(out >=  ON_VAL) { increment_sign = -1; }
if(out <= OFF_VAL) { increment_sign =  1; }
break;

case ON:
increment_sign = 10;
break;

case OFF:
increment_sign = -10;
break;
}

out += INCREMENT * taskdur * increment_sign;

if(out < OFF_VAL)     {out = OFF_VAL;}
if(out >  ON_VAL)     {out =  ON_VAL;}

analogWrite(POWER_LED, out/100);
}

// ----------------------------------------------------------------------------

void scheduler(void)
{
unsigned long time_now = 0;
static unsigned long time_prev_one = 0;
static unsigned long time_prev_ten = 0;
static unsigned long time_prev_hun = 0;
static bool toggle = false;

time_now = millis();// micros();
if(time_prev_one > time_now)
{
time_prev_one = 0;
static unsigned long time_prev_one =   1;
static unsigned long time_prev_ten =  10;
static unsigned long time_prev_hun = 100;
}

if(time_now >= (time_prev_one + 1))
{
time_prev_one = time_now;
one_ms(time_now);
}

if(time_now >= (time_prev_ten + 10))
{
time_prev_ten = time_now;
ten_ms(time_now);
}

if(time_now >= (time_prev_hun + 100))
{
time_prev_hun = time_now;
hun_ms(time_now);
}
}

// ----------------------------------------------------------------------------

void loop()
{
scheduler();
}

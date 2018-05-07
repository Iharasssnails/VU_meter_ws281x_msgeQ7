/*
 * greetings,
 * This is actually a work in progress. i meant to write this code for left and right sepperatly but haven't come to the completion yet
 * right now it works of just one signal.
 * I already have some new ideas on how it could be done better, but it will take a while before i actually proceed with these changes.
 * if you want to connect a ws281x strip on this, you totally can but please keep this in mind.
 * 164 leds like here, are quite the maxmimum of leds you can connect.
 * Your cycletime trough the strip will eventually get so slow that your strip will just look laggy and bad
 * Code written by I_harass_snails
 * Hit me up on reddit if you've got any questions or improvements
 */


int strobePin_R  = 22;    // Strobe Pin on the MSGEQ7
int resetPin_R   = 24;    // Reset Pin on the MSGEQ7
int outPin_R     = A0;   // Output Pin on the MSGEQ7
float Volume_level_R[7];          // An array to hold the values from the 7 frequency bands
int last_Volume_level_R[7];       // An array to remember all the previous vallue's of the bands
int vol = 0;

byte brightness = 75;

int strobePin_L  = 26;    // Strobe Pin on the MSGEQ7
int resetPin_L   = 28;    // Reset Pin on the MSGEQ7
int outPin_L     = A1;   // Output Pin on the MSGEQ7

#include <Adafruit_NeoPixel.h>

#define PIN            50
#define NUMPIXELS   164
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  Serial.begin (9600);
  pixels.begin();
  pinMode      (strobePin_R, OUTPUT);
  pinMode      (resetPin_R,  OUTPUT);         // inisiation of your msgeq7 chip
  pinMode      (outPin_R,    INPUT);
  digitalWrite (resetPin_R,  LOW);
  digitalWrite (strobePin_R, LOW);          
  delay        (1);
  digitalWrite (resetPin_R,  HIGH);
  delay        (1);
  digitalWrite (resetPin_R,  LOW);
  digitalWrite (strobePin_R, HIGH);
  delay        (1);
}
int i = 0;

void loop() {
  for (i = 0; i < 7; i++) { //loops 7 times to do the same work for 7 different freequency's
    digitalWrite       (strobePin_R, LOW);
    delayMicroseconds  (100);                    // Delay necessary due to timing diagram
    Volume_level_R[i] = analogRead (outPin_R);
    digitalWrite       (strobePin_R, HIGH);
    delayMicroseconds  (100);                    // Delay necessary due to timing diagram

    Volume_level_R[i] = map(Volume_level_R[i] - 75, 0, 948, 0, NUMPIXELS / 14); //the "Volume_level_R[i] - 75" is meant to put in a treshhold. On aux calbe's there is quite a bit of noise that will distrub your results and look ugly 
// after the value goes over your treshhold, you sould remap it to value's that make sence. so from 0 - (1023-75) to the amount of leds that you want the maximum amplitude to have. since you have 7 bands and 2 side's you take "amout of leds"/14

    if (last_Volume_level_R[i] > Volume_level_R[i]) { // if the vallue is below it's last value, go down by 1. you have a smooth decreasing amplitude now
      Volume_level_R[i]  = last_Volume_level_R[i] - 1;
    }
    last_Volume_level_R[i] =  Volume_level_R[i]; 
    
    if (Volume_level_R[i] < 0) {// if it's below 0 what can happen because of the noise that is lower then 75, then just be 0.
      Volume_level_R[i] = 0;
    }
    frequentie_analyzer_band();
  }
}


void frequentie_analyzer_band() {

  for (vol = 0; vol < NUMPIXELS; vol++) { // it starts counting all the led's in your strip\
    if (i == 0) { // for when i ==0 so this means the bass line
      if (vol < Volume_level_R[i]) { // at this point "Volume_level_R[i]" already has the exact amount of leds in amplitude that it wants to be. if your vol is < then this. you sould make these collors
        pixels.setPixelColor((NUMPIXELS / 2)-1 + vol, 0, 0, brightness); // code colors written in variable's, so that you can easily adjust brightness of the whole strip at once.
        pixels.setPixelColor((NUMPIXELS / 2) - vol, 0, 0, brightness);
      }
    }

    else if (i == 1) {
// now you'll have to count the results of this band onto the results of the previous band
      if (vol < Volume_level_R[i] + Volume_level_R[0] && vol > Volume_level_R[0] ) { // so if your vol is in between the value's of less then the amplitude of band 0 + band 1. and more then the value of band 0.
        pixels.setPixelColor((NUMPIXELS / 2)-1 + vol, brightness, 0, brightness);
        pixels.setPixelColor((NUMPIXELS / 2) - vol, brightness, 0, brightness);
      }
    }

    else if (i == 2) { // it's just the same from here on
      if (vol < Volume_level_R[i] + Volume_level_R[1] + Volume_level_R[0] && vol > Volume_level_R[0] + Volume_level_R[1] ) { // les then band 1+2+3. more then band 1+2 and so on.
        pixels.setPixelColor((NUMPIXELS / 2)-1 + vol, 0, brightness,brightness);
        pixels.setPixelColor((NUMPIXELS / 2) - vol, 0,brightness, brightness);
      }
    }

   else if (i == 3) {
      if (vol < Volume_level_R[i] + Volume_level_R[2] + Volume_level_R[1] + Volume_level_R[0] && vol > Volume_level_R[0] + Volume_level_R[1] + Volume_level_R[2] ) {
        pixels.setPixelColor((NUMPIXELS / 2)-1 + vol, brightness, brightness/4, 0);
        pixels.setPixelColor((NUMPIXELS / 2 ) - vol, brightness, brightness/4, 0);
      }
    }

   else if (i == 4) {
      if (vol < Volume_level_R[i] + Volume_level_R[3] + Volume_level_R[2] + Volume_level_R[1] + Volume_level_R[0] && vol > Volume_level_R[0] + Volume_level_R[1] + Volume_level_R[2] + Volume_level_R[3] ) {
        pixels.setPixelColor((NUMPIXELS / 2)-1 + vol, brightness, brightness, 0);
        pixels.setPixelColor((NUMPIXELS / 2) - vol, brightness, brightness, 0);
      }
    }

   else if (i == 5) {
      if (vol < Volume_level_R[i]  + Volume_level_R[4]+ Volume_level_R[3] + Volume_level_R[2] + Volume_level_R[1] + Volume_level_R[0] && vol > Volume_level_R[0] + Volume_level_R[1] + Volume_level_R[2] + Volume_level_R[3]  + Volume_level_R[4]) {
        pixels.setPixelColor((NUMPIXELS / 2)-1 + vol, 0, brightness, 0);
        pixels.setPixelColor((NUMPIXELS / 2) - vol, 0, brightness, 0);
      }
    }

  else  if (i == 6) {
      if (vol < Volume_level_R[i] + Volume_level_R[5]  + Volume_level_R[4]+ Volume_level_R[3] + Volume_level_R[2] + Volume_level_R[1] + Volume_level_R[0] && vol > Volume_level_R[0] + Volume_level_R[1] + Volume_level_R[2] + Volume_level_R[3]  + Volume_level_R[4] + Volume_level_R[5]) {
        pixels.setPixelColor((NUMPIXELS / 2)-1 + vol, brightness, 0, 0);
        pixels.setPixelColor((NUMPIXELS / 2) - vol, brightness,0 , 0);
      }

      else if ( vol > Volume_level_R[i] + Volume_level_R[5]  + Volume_level_R[4]+ Volume_level_R[3] + Volume_level_R[2] + Volume_level_R[1] + Volume_level_R[0]) { // so when it has done everything. now you should turn off ever led that is more then band 1+2+3+4+5+6
        pixels.setPixelColor(( (NUMPIXELS / 2) - vol), 0, 0, 0);
        pixels.setPixelColor(((NUMPIXELS / 2)-1 + vol), 0, 0, 0);
      }
    }
  }
  pixels.show(); //  show the results
}



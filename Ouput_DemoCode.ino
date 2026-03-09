// sample LED code from: https://github.com/hendog993/Youtube_arduino/blob/master/LEDStrip.ino
// code for running user tests. 

// written by Yinying Miao, v3 adjustments made by Yinying Miao.

#include <FastLED.h>

#define LED_PIN 10
#define NUM_LEDS 26

CRGB leds[NUM_LEDS];

const int motorPin [] = {3, 4, 5, 6, 7};

unsigned long currentTime;
unsigned long previousTime = 0;
unsigned long newTime;
unsigned long delayTime = 10000; // for testing purpose

void setup() {

  Serial.begin (9600);

  FastLED.addLeds <WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps (5, 500);
  FastLED.clear();
  FastLED.show();

  pinMode(motorPin[0], OUTPUT);
  pinMode(motorPin[1], OUTPUT);
  pinMode(motorPin[2], OUTPUT);
  pinMode(motorPin[3], OUTPUT);
  pinMode(motorPin[4], OUTPUT);

  digitalWrite (motorPin[0], LOW);
  digitalWrite (motorPin[1], LOW);
  digitalWrite (motorPin[2], LOW);
  digitalWrite (motorPin[3], LOW);
  digitalWrite (motorPin[4], LOW);

  delay (5000);

}

void loop() {

  currentTime = millis();

  if (currentTime > previousTime + delayTime) {

    for (int i = 0; i< NUM_LEDS; i++){
      leds[i] = CRGB(180 + 3*i, 3*i, 5*i);
      //FastLED.setBrightness(1.2*i);
      FastLED.show();
      delay(400);
    }

    digitalWrite (motorPin[1], HIGH); //vibrating for 1 second (1000)
    digitalWrite (motorPin[2], HIGH);
    digitalWrite (motorPin[3], HIGH);
    digitalWrite (motorPin[4], HIGH);

    delay (3000);

    digitalWrite (motorPin[1], LOW);
    digitalWrite (motorPin[2], LOW);
    digitalWrite (motorPin[3], LOW);
    digitalWrite (motorPin[4], LOW);    

    delay (3000);

    for (int i = 0; i< NUM_LEDS; i++){
      FastLED.setBrightness(1.2*i);
      FastLED.show();
      delay(400);
    }

    // for (int i=0; i<NUM_LEDS; i--){
    //   leds[i] = CRGB(258 - 3*i, 78 - 3*i, 130 - 5*i);
    //   FastLED.setBrightness(0.8*i);
    //   FastLED.show();
    //   delay(400);
    // } 

    digitalWrite (motorPin[0], HIGH); //vibrating for 1 second (1000)
    delay (3000);
    digitalWrite (motorPin[0], LOW);

    for (int i = 0; i< NUM_LEDS; i++){
      FastLED.setBrightness(0.7*i);
      FastLED.show();
      delay(400);
    }

  // cold colours 
    for (int i = 0; i< NUM_LEDS; i++){
      leds[i] = CRGB(3*i, 5*i, 180 + 3*i);
      //FastLED.setBrightness(1.2*i);
      FastLED.show();
      delay(400);
    }

    previousTime = currentTime;

    delay (60000);

  }  





}


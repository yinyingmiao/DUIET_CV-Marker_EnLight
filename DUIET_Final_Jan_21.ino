// sample LED code from: https://github.com/hendog993/Youtube_arduino/blob/master/LEDStrip.ino
// sample code for getting data from processing: https://gist.github.com/atduskgreg/1349176

#include <FastLED.h>

#define LED_PIN 10
#define NUM_LEDS 26

CRGB leds[NUM_LEDS];

const int motorPin[] = { 3, 4, 5, 6, 7 };

//long delayTime = 60000;  // getting data once per minute
//long currentTime;
//long previousTime;

int currentValue = 0;
int values[5];

bool ledOn = false;

//finding a way to convert the temperature and humidity scales into a 0-10 scale


void setup() {

  Serial.begin(9600);

  // LED light set up 
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();

  // motor set up 
  pinMode(motorPin[0], OUTPUT);
  pinMode(motorPin[1], OUTPUT);
  pinMode(motorPin[2], OUTPUT);
  pinMode(motorPin[3], OUTPUT);
  pinMode(motorPin[4], OUTPUT);

  digitalWrite(motorPin[0], LOW);
  digitalWrite(motorPin[1], LOW);
  digitalWrite(motorPin[2], LOW);
  digitalWrite(motorPin[3], LOW);
  digitalWrite(motorPin[4], LOW);
}

void loop() {

  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');  // Read data until newline
    int index = 0;
    int lastIndex = 0;

    for (int i = 0; i < data.length(); i++) {
      if (data[i] == ',') {
        values[index] = data.substring(lastIndex, i).toInt();
        lastIndex = i + 1;
        index++;
      }
    }
    //values[index] = data.substring(lastIndex).toInt();  // Last value

    // Now `values` array holds your 5 integers. Use them as needed.
    // For example:
    Serial.print("Received values: ");
    for (int i = 0; i < 5; i++) {
      Serial.print(values[i]);
      Serial.print(" ");
    }

    Serial.println();
    
    // for the light condition setting with a direct brightness mapping function: 

    for (int i = 0; i < NUM_LEDS; i++) {
      FastLED.setBrightness(values[0]*0.04*255);
      FastLED.show();
      delay(40);
    }

  } 

  // for the temperature and humidity:
  if (values[1] <= -136 & values[2] <= -136) {
    Serial.print ("Temperature and Humidity are both low");
    for (int i = 0; i < NUM_LEDS; i++) { // this is cold colours
      leds[i] = CRGB(3*i, 5*i, 180 + 3*i);
      FastLED.show();
      delay(400);
    }
  }

  if (values[1] > -136 & values[2] > -136) {
    Serial.print ("Temperature and Humdity are both high");
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(180 + 3*i, 3*i, 5*i);
      //FastLED.setBrightness(1.2*i);
      FastLED.show();
      delay(400);
    }
  }
  
  //for the air quality: 
  
    if (values[4] <= 3) { //air quality bad
      Serial.print ("Air quality bad");
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(400);
        leds[i] = CRGB(180 + 3*i, 3*i, 5*i);
        FastLED.show();
        delay(400);
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(400);
        leds[i] = CRGB(180 + 3*i, 3*i, 5*i);
        FastLED.show();
        delay(400); 
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(400);
        leds[i] = CRGB(180 + 3*i, 3*i, 5*i);
        FastLED.show();
        delay(400);                   
        }
    }

  //for the noise level: 

  if (values[3] < 2.5) { // very noisy

    Serial.print ("It is very noisy");

    digitalWrite (motorPin[0], HIGH); 
    digitalWrite (motorPin[1], HIGH); 
    digitalWrite (motorPin[2], HIGH);
    digitalWrite (motorPin[3], HIGH);
    digitalWrite (motorPin[4], HIGH);
    
    delay (4000);

    digitalWrite (motorPin[0], LOW); 
    digitalWrite (motorPin[1], LOW); 
    digitalWrite (motorPin[2], LOW);
    digitalWrite (motorPin[3], LOW);
    digitalWrite (motorPin[4], LOW);
  }

  if (values[3] >= 2.5 & values[3] < 2.8) { //a bit noisy

    Serial.print ("It is a bit noisy");

    digitalWrite (motorPin[0], HIGH); 
    digitalWrite (motorPin[2], HIGH);
    digitalWrite (motorPin[4], HIGH);
    
    delay (2500);

    digitalWrite (motorPin[0], LOW); 
    digitalWrite (motorPin[2], LOW);
    digitalWrite (motorPin[4], LOW);
  }

  if (values[3] >= 2.8) { // quiet

    Serial.print ("It is quiet");
    digitalWrite (motorPin[2], HIGH);
    delay (2000);
    digitalWrite (motorPin[2], LOW);
  }

}

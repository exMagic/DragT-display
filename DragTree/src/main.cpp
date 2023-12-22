#include <Arduino.h>
#include "TFT_eSPI.h"

unsigned colour = 0xFFFF;
TFT_eSPI tft = TFT_eSPI();
#define topbutton 0
#define lowerbutton 1
#define PIN_POWER_ON 15  // LCD and battery Power Enable
#define PIN_LCD_BL 38    // BackLight enable pin (see Dimming.txt)
int progress = 0;
int x;
int blocks = 0;
int i = 0;
bool ticktock = true;
long counter = 0;
char mystr[40];
char mystr2[40];
int BulbX = 80;
int BulbR = 20;
int BulbMargin = 8;

int BlueBulbY = 20;
int Y1BulbY = BlueBulbY + BulbMargin + BulbR * 2;
int Y2BulbY = Y1BulbY + BulbMargin + BulbR * 2;
int Y3BulbY = Y2BulbY + BulbMargin + BulbR * 2;
int GreenBulbY = Y3BulbY + BulbMargin + BulbR * 2;
int RedBulbY = GreenBulbY + BulbMargin + BulbR * 2;

int state = 0;

unsigned long previousMillis = 0;  
unsigned long prm = 0;             

const long interval = 1000;  // interval at which to blink (milliseconds)

void setup() {

  pinMode(PIN_POWER_ON, OUTPUT);  //enables the LCD and to run on battery
  pinMode(PIN_LCD_BL, OUTPUT);    // BackLight enable pin
  pinMode(lowerbutton, INPUT_PULLUP);    //Right button pulled up, push = 0
  pinMode(topbutton, INPUT);      //Left button  pulled up, push = 0
  delay(100);
  digitalWrite(PIN_POWER_ON, HIGH);
  digitalWrite(PIN_LCD_BL, HIGH);
  tft.init();
  tft.setRotation(4);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);  //horiz / vert<> position/dimension
  tft.setTextSize(1);
  tft.setTextDatum(4);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  delay(200);
}

void loop() {
  counter++;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    sprintf(mystr, "Millis: %u", counter);
    counter = 0;
    tft.drawString(mystr, 5, 5, 3);
    previousMillis = currentMillis;
  }

  if (state == 0) {
    if (currentMillis - prm >= 1000) {
      state++;
      tft.fillSmoothCircle(BulbX, BlueBulbY, BulbR, TFT_BLUE, TFT_BLACK);
      prm = currentMillis;
    }
  } else if (state == 1) {
    if (currentMillis - prm >= 2000) {
      state++;
      tft.fillSmoothCircle(BulbX, Y1BulbY, BulbR, TFT_YELLOW, TFT_BLACK);
    }
  } else if (state == 2) {
    if (currentMillis - prm >= 2500) {
      state++;
      tft.fillSmoothCircle(BulbX, Y2BulbY, BulbR, TFT_YELLOW, TFT_BLACK);
    }

  } else if (state == 3) {
    if (currentMillis - prm >= 3000) {
      state++;
      tft.fillSmoothCircle(BulbX, Y3BulbY, BulbR, TFT_YELLOW, TFT_BLACK);
    }

  } else if (state == 4) {
    if (currentMillis - prm >= 3500) {
      state++;
      tft.fillSmoothCircle(BulbX, GreenBulbY, BulbR, TFT_GREEN, TFT_BLACK);
    }
  }
  if (digitalRead(lowerbutton)== 0) {
    int result = currentMillis - prm - 3500;
    if (result >= 0) {
      float sec = (float)result/1000;
      dtostrf(sec, 6, 3, mystr2);
      tft.drawString(mystr2, 70, 280, 6);
    }else{
      tft.fillSmoothCircle(BulbX, RedBulbY, BulbR, TFT_RED, TFT_BLACK);
      tft.fillSmoothCircle(BulbX, GreenBulbY, BulbR, TFT_BLACK, TFT_BLACK);
      state = 5;
    }
    delay(500);
  }
  if (digitalRead(topbutton)== 0) {
    state = 0;
    tft.fillScreen(TFT_BLACK);
    delay(200);
  }  
}

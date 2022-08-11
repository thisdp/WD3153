#include <Wire.h>
#include "WD3153.h"
WD3153 LED;
void setup(){
  Wire.begin();
  LED.reset();
  LED.setChipEnabled(true);
  LED.setCHRGEnabled(false);
  LED.setGlobalPWMFrequencyLevel(PWM500HZ);
  LED.setLEDEnabled(0,true);
  LED.setLEDEnabled(1,true);
  LED.setLEDEnabled(2,true);
  LED.setLEDCurrentLimit(0,LED15mA);
  LED.setLEDCurrentLimit(1,LED15mA);
  LED.setLEDCurrentLimit(2,LED15mA);
}


void loop(){
  uint16_t pos;
  for (pos = 0; pos <= 255; pos++) {
    LED.setLEDPWM(0,pos);
    LED.setLEDPWM(1,pos);
    LED.setLEDPWM(2,pos);
    delay(5);
  }
  for (pos = 255; pos > 1; pos--) {
    LED.setLEDPWM(0,pos);
    LED.setLEDPWM(1,pos);
    LED.setLEDPWM(2,pos);
    delay(5);
  }
}

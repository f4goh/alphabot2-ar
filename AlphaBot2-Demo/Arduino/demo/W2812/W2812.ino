#include <Adafruit_NeoPixel.h>

#define PIN 7

Adafruit_NeoPixel RGB = Adafruit_NeoPixel(4, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  RGB.begin();
  RGB.setPixelColor(0, RGB.Color(255, 0, 0));
  RGB.setPixelColor(1, RGB.Color(0, 255, 0));
  RGB.setPixelColor(2, RGB.Color(0, 0, 255));
  RGB.setPixelColor(3, RGB.Color(255, 255, 0));
  RGB.show();
}

void loop() {
  // put your main code here, to run repeatedly:

}

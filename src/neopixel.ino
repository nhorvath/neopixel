// Simple NeoPixel test.  Lights just a few pixels at a time so a
// long strip can safely be powered from Arduino 5V pin.  Arduino
// may nonetheless hiccup when LEDs are first connected and not
// accept code.  So upload code first, unplug USB, connect pixels
// to GND FIRST, then +5V and digital pin 6, then re-plug USB.
// A working strip will show a few pixels moving down the line,
// cycling between red, green and blue.  If you get no response,
// might be connected to wrong end of strip -- look for the data
// direction arrows printed on the strip.

#include <Adafruit_NeoPixel.h>

#define PIN    6
#define N_LEDS 450 // 5 meter reel @ 30 LEDs/m = 150

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
char input = '1';   // for incoming serial data
bool run = true;

void setup() {
  Serial.begin(9600);
  strip.begin();
}

void loop() {
  char last_input;
  for(uint16_t offset=0; offset<strip.numPixels(); offset++) {
    if (run) {
      // colors are RR GG BB
      multichase(offset, 0xFF0000, 0xFFFFFF, 0x00FF00);
    } else {
      dark();
    }

    last_input = readChar();
    if (last_input == '0') {
      run = false;
    } else if (last_input == '1') {
      run = true;
    }
  }
}

static char readChar() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming char:
    input = (char) Serial.read();

    // say what you got:
//    Serial.print("I received: ");
//    Serial.println(input);

    return input;
  }
  return NULL;
}

static void chase(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
      delay(25);
  }
}

static void multichase(uint16_t offset, uint32_t c1, uint32_t c2, uint32_t c3) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    switch ((i+offset) % 24) {
      case 1:
      case 2:
      case 3:
      case 4:
        strip.setPixelColor(i, c1); // Draw new pixel
        break;
      case 9:
      case 10:
      case 11:
      case 12:
        strip.setPixelColor(i, c2); // Draw new pixel
        break;
      case 17:
      case 18:
      case 19:
      case 20:
        strip.setPixelColor(i, c3); // Draw new pixel
        break;
      default:
        strip.setPixelColor(i, 0); // Erase pixel
        break;
    }
  }
    
  strip.show();
  delay(25);
}

static void dark() {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, 0); // Erase pixel
  }
  strip.show();
  delay(25);
}


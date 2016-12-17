// Accepts serial commands to change the pattern / colors of the animation

#include <Adafruit_NeoPixel.h>

#define PIN    6
#define N_LEDS 450 // 5 meter reel @ 30 LEDs/m = 150

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
String input;   // for incoming serial data
bool run = true;
bool debug = false;

void setup() {
  Serial.begin(9600);
  strip.begin();
}

void loop() {
  uint16_t wait = 25;
  
  if (run) {
    // colors are RR GG BB
    multichase(false, wait, 0xFF0000, 0xFFFFFF, 0x00FF00);
    multichase(true, wait, 0xFF0000, 0xFFFFFF, 0x00FF00);
    colorFill(wait/2, 0xFF0000);
    colorFill(wait/2, 0x00FF00);
  } else {
    dark(wait);
  }

  processInput(readStr());
}

static void processInput(String in) {
  switch (in.charAt(0)) {
    case '0':
      Serial.println("Stopping animations");
      run = false;
      break;
    case '1':
      Serial.println("Starting animations");
      run = true;
      break;
    case 'A':
      Serial.print("Set animation sequence to: ");
      Serial.println(in);
      setAnimation(in);
      break;
    case 'C':
      Serial.print("Add color to color sequence: ");
      Serial.println(in);
      addColor(in);
      break;
    case 'R':
      Serial.println("Reset color sequence");
      resetColors();
      break;
    case 'D':
      Serial.println("Toggle debug");
      debug = !debug;
      break;
    default:
      if (in != "") {
        Serial.print("Unsupported command: ");
        Serial.println(in);
      }
      break;
  }
}
  

static String readStr() {
  // send data only when you receive data:
  if (Serial && Serial.available() > 0) {
    // read the incoming char:
    input = Serial.readStringUntil('\n');

    if (debug) {
      // say what you got:
      Serial.print("I received: ");
      Serial.println(input);
    }
    return input;
  }
  return "";
}

static void setAnimation(String in) {
  // stub for switching animations
}

static void addColor(String in) {
  // stub for adding colors
}

static void resetColors() {
  // stub for resetting colors
}

// turn off all pixels
static void dark(uint16_t wait) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, 0); // Erase pixel
  }
  strip.show();
  delay(wait);
}

// single bar of color 4 pixels long runs around the strip
static void chase(uint16_t wait, uint32_t c) {
  for(int i=0; i<strip.numPixels()+4; i++) {
    if (run) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
      delay(wait);
    }
    
    processInput(readStr());
  }
}

// continous bars of colors run around the strip
static void multichase(bool reverse, uint16_t wait, uint32_t c1, uint32_t c2, uint32_t c3) {
  int width = 4;
  int numColors = 3;
  int repeat = 10;
  int maxOffset = width * numColors*2 * repeat;
  if (reverse) {
    // reverse
    for(int offset=maxOffset; offset>=0; offset--) {
      if (run) {
        multichaseFrame(offset, wait, c1, c2, c3);
      }
   
      processInput(readStr());
    }
  } else {
    // forward
    for(int offset=0; offset<maxOffset; offset++) {
      if (run) {
        multichaseFrame(offset, wait, c1, c2, c3);
      }
   
      processInput(readStr());
    }
  }
}

static void multichaseFrame(int offset, uint16_t wait, uint32_t c1, uint32_t c2, uint32_t c3) {
  for(int i=0; i<strip.numPixels(); i++) {
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
  delay(wait);
}

// Fill the dots one after the other with a color
void colorWipe(uint16_t wait, uint32_t c) {
  for(int i=0; i<strip.numPixels(); i++) {
    if (run) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
    }
   
    processInput(readStr());
  }
}

// Fill the dots one after the other with a color from both sides
void colorFill(uint16_t wait, uint32_t c) {
  int numPixels = strip.numPixels();
  int halfway = numPixels/2;
  for(int i=0; i<=halfway; i++) {
    if (run) {
      strip.setPixelColor(i, c);
      strip.setPixelColor(numPixels-i, c);
      strip.show();
      delay(wait);
    }
   
    processInput(readStr());
  }
}

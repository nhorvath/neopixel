// Accepts serial commands to change the pattern / colors of the animation

#include <Adafruit_NeoPixel.h>

#define PIN    6
#define N_LEDS 450 // 5 meter reel @ 30 LEDs/m = 150

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
String input;   // for incoming serial data
bool run = true;
bool debug = false;
int PRESET = 3; // 0 = xmas, 1 = 4th of july, 2 = mothers day, 3 = spring

void setup() {
  Serial.begin(9600);
  strip.begin();
}

void loop() {
  uint16_t wait = 25;
  
    // colors are RR GG BB
  uint32_t RED = 0xFF0000;
  uint32_t GREEN = 0x00FF00;
  uint32_t BLUE = 0x0000FF;
  uint32_t WHITE = 0xFFFFFF;
  uint32_t PINK = 0xFFAAAA;
  uint32_t PURPLE = 0xFF00FF;
  uint32_t YELLOW = 0x00FFFF;
  
  if (run) {
    if (PRESET == 0) { // xmas
      multichase(false, wait, RED, WHITE, GREEN);
      multichase(true, wait, RED, WHITE, GREEN);
      colorFill(wait/2, RED);
      colorFill(wait/2, GREEN);
    } else if (PRESET == 1) { // 4th of july
      multichase(false, wait, RED, WHITE, BLUE);
      multichase(true, wait, RED, WHITE, BLUE);
      colorFill(wait/2, RED);
      colorFill(wait/2, BLUE);
    } else if (PRESET == 2) { // mother's day
      multichase(false, wait, PINK, WHITE, PURPLE);
      multichase(true, wait, PINK, WHITE, PURPLE);
      colorFill(wait/2, PINK);
      colorFill(wait/2, PURPLE);
    } else if (PRESET == 3) { // spring
      multichase(false, wait, PINK, YELLOW, PURPLE);
      multichase(true, wait, PINK, YELLOW, PURPLE);
      colorFill(wait/2, PINK);
      colorFill(wait/2, YELLOW);
      colorFill(wait/2, PURPLE);
    }
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

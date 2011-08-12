// Blink by Jenna Fox - testing my new ATtiny85s
#import <stdlib.h>
#import <avr/io.h>
#import <util/delay.h>
#import <math.h>
#import "nice-things.c"
#import "lamp-api.c"

// make and install with ./upload.rb glass-lamp.c 16.5mhz
#define PI 3.14159265
#define circleThird(third) ((PI * 2 / 3) * third)


void sawtoothFade(void) {
  byte counter = 0;
  while (counter < 255) {
    pixel(counter, 0, counter);
    counter++;
    delay(20);
  }
  
  while (counter > 0) {
    pixel(counter, 0, counter);
    counter--;
    delay(20);
  }
}

void spinHue(void) {
  static float angle = 0.0f;
  angle += 0.001;
  if (angle > PI * 2) angle -= PI * 2;
  
  pixel(
      64.0f * (sin(angle + circleThird(0)) + 1.0f),
      64.0f * (sin(angle + circleThird(1)) + 1.0f),
      64.0f * (sin(angle + circleThird(2)) + 1.0f)
    );
}


void testGrayRange(void) {
  pixel(255, 255, 255);
  delay(100);
  pixel(127, 127, 127);
  delay(100);
}

// setup 
inline void setup(void) {
  pixel(255, 0, 0);
  pixelSetup();
}

void loop(void) {
  //sawtoothFade();
  spinHue();
  //testGrayRange();
}

int main(void) {
  setup();
	while(1) {
	  loop();
  }
}


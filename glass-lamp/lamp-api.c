#import <stdlib.h>
#import <avr/io.h>
#import <avr/interrupt.h>
#import <util/delay.h>
#import "nice-things.c"

//#include "lamp-api-spiutil.c"
#include "binary.c"
//#define clockPin 2
//#define dataPin 1
// when we're bitbanging, lets just use these pins
#define clockPin 4
#define dataPin 3


struct color { // these LED pixels have weird wiring.
  byte red;
  byte green;
  byte blue;
};

volatile struct color currently = {0, 0, 0}; // current colour

signed char nextDitheringStep();
inline void updatePixelPayload(byte *payload);
inline void loadNextOutputByte();
void pixelSetup();
void pixel(byte red, byte green, byte blue);

// on each call, returns a number in a sequence which adds dithering to low resolution values
// the sequence it outputs is -1 2 -2 3 -3 4 -4 5 -5 6 -6 7 repeating
signed char nextDitheringStep() {
  static signed char counter = 0;
  
  counter = counter - (counter < 0); // add one if counter is currently positive
  counter = (~counter) % 8; // bitwise not (toggle negate), restrained to no more than 8 as value
  return counter + 1;
}

// current repeating command
volatile byte spiOutputBytes[8] = {0,0,  0,0,  0,0, 0,0};
volatile byte outputStep = 0;
byte outgoing = 0;
void loadNextOutputByte() {
  // load our next byte in to the USI data register
  outgoing = spiOutputBytes[outputStep]; // get the next byte and increment counter after
  outputStep = (outputStep + 1) % sizeof(spiOutputBytes); // wrap it back to 0 for next call if would overflow
  
  if (outputStep == 2) {
    updatePixelPayload( (byte *) spiOutputBytes );
  }
}

// transfer a bit to the LED Pixel, and clock it once.
void sillyClock() {
  volatile static byte bitPosition = 8;

  bitPosition--; // move down to next bit in outgoing byte
  set(dataPin, (outgoing >> bitPosition) & 1); // set dataPin to bit
  set(clockPin, on);
  set(clockPin, off);
  
  if (bitPosition == 0) {
    loadNextOutputByte();
    bitPosition = 8;
  }
}

// interrupt handler - when Timer 0 fires, clock out another bit to the LED
ISR(TIMER0_COMPA_vect) {
  sillyClock();
}


// update the current output bytes
void updatePixelPayload(byte *target) {
  int dithering = nextDitheringStep(); // get next dithering value
  unsigned int payload = 0x8000; // this pixel module is wired up weird - it's channels go Green Blue Red.
	payload |= (constrain(currently.green + dithering, 0x00, 0xFF) & 0xF8) << 7; // green is the high 5 bits
	payload |= (constrain(currently.blue + dithering, 0x00, 0xFF) & 0xF8) << 2; // blue is the middle 5 bits
	payload |= (constrain(currently.red + dithering, 0x00, 0xFF)) >> 3; // red is the low 5 bits
  target[0] = (payload >> 8) & 0xFF;
  target[1] = payload & 0xFF;
}

// setup pixel output
void pixelSetup() {
  // setup our pins as outputs
  outputs(pin(clockPin));
  outputs(pin(dataPin));
  
  // preload the first byte out to the light in to memory
  loadNextOutputByte(); // get a byte in to the USI data register, ready for transmission
  
  // Setup Timer 0 to clock out our SPI bits
  // Can tweak down the number to increase framerate and improve dithering
  // Tweaking number up leaves more CPU time for other stuff
  TCCR0A = B11110010; // CTC mode
  TCCR0B = B00000010; // speed = clock / 8
  OCR0A = 64; // number to count to before next clock toggle
  TIMSK = B00010000; // turn on Compare Match A interrupt
  
  // turn on interrupts globally
  sei();
}

// set the light to a colour
void pixel(byte red, byte green, byte blue) {
  currently.red = red;
  currently.green = green;
  currently.blue = blue;
}

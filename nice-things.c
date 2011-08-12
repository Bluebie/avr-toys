// For the niceness
typedef unsigned char byte;
typedef unsigned char boolean;

// make bit & value, eg bit(5) #=> 0b00010000
#define bit(number) _BV(number)
#define pin(number) _BV(number)

// USI serial aliases
#define USIOutputPort    PORTE
#define USIInputPort     PINE
#define USIDirectionPort DDRE
#define USIClockPin      PE4
#define USIDataInPin     PE5
#define USIDataOutPin    PE6

// booleans
#define    on 255
#define   off 0
#define  true 1
#define false 0
#define   yes true
#define    no false

// ensure a value is within the bounds of a minimum and maximum (inclusive)
#define constrainUpper(value, max) (value > max ? max : value)
#define constrainLower(value, min) (value < min ? min : value)
#define constrain(value, min, max) constrainLower(constrainUpper(value, max), min)

// set a pin on DDRB to be an input or an output - i.e. becomeOutput(pin(3));
inline void inputs(byte pinmap) { DDRB &= ~pinmap; }
inline void outputs(byte pinmap) { DDRB |= pinmap; }

// turn a pin on or off
inline void pinOn(byte pin) { PORTB |= bit(pin); }
inline void pinOff(byte pin) { PORTB &= ~bit(pin); }

// delay in milliseconds
#define delay(ms) _delay_ms(ms);

// digital read returns 0 or 1
inline boolean get(byte pin) {
  return ((PINB & bit(pin)) != 0);
}

// the previous version of set
inline void set(byte pin, boolean state) {
  if (state) {
    pinOn(pin);
  } else {
    pinOff(pin);
  }
}

inline void toggle(byte pin) {
  PORTB ^= bit(pin);
}


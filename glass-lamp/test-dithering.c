#import <stdio.h>
#import <stdlib.h>
#import "binary.c"
typedef unsigned char byte;

// on each call, returns a number in a sequence which adds dithering to low resolution values
// the sequence it outputs is -1 1 -2 2 -3 3 -4 4 -5 5 -6 6 -7 7 0 repeating
// adding this to the values adds enough jitter to counteract being converted from 8 bit color to 5 bit
signed char nextDitheringStep() {
  volatile static signed char counter = 0;
  
  counter = counter - (counter < 0); // add one if counter is currently positive
  counter = (~counter) % 8; // bitwise not (toggle negate), restrained to no more than 8 as value
  return counter;
}

int main(int argc, char *argv[]) {
  int c = 0;
  while (c < 25) {
    printf("%d ", nextDitheringStep());
    c++;
  }
  printf("\n");
  
  return EXIT_SUCCESS;
}
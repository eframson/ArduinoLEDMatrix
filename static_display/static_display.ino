#include <FastLED.h>

#define STRIP_ONE_PIN 2
#define STRIP_TWO_PIN 3
#define STRIP_THREE_PIN 4
#define STRIP_FOUR_PIN 5
#define STRIP_FIVE_PIN 6
#define STRIP_SIX_PIN 7
#define STRIP_SEVEN_PIN 8
#define STRIP_EIGHT_PIN 9

#define NUM_PIXELS_PER_STRIP 8
#define NUM_STRIPS 8

struct pair {
  int first;
  int second;
};

/*CRGB all_leds_in_parallel[NUM_PIXELS_PER_STRIP];
CRGB strip_one_leds[NUM_PIXELS_PER_STRIP];
CRGB strip_two_leds[NUM_PIXELS_PER_STRIP];
CRGB strip_three_leds[NUM_PIXELS_PER_STRIP];
CRGB strip_four_leds[NUM_PIXELS_PER_STRIP];
CRGB strip_five_leds[NUM_PIXELS_PER_STRIP];
CRGB strip_six_leds[NUM_PIXELS_PER_STRIP];
CRGB strip_seven_leds[NUM_PIXELS_PER_STRIP];
CRGB strip_eight_leds[NUM_PIXELS_PER_STRIP];*/

/*CRGB all_strips[NUM_STRIPS] = {
  strip_one_leds,
  strip_two_leds,
  strip_three_leds,
  strip_four_leds,
  strip_five_leds,
  strip_six_leds,
  strip_seven_leds,
  strip_eight_leds
};*/

CRGB leds[NUM_STRIPS][NUM_PIXELS_PER_STRIP];
int persistent_h = 0;

void setup() {
  /*
  FastLED.addLeds<NEOPIXEL, STRIP_ONE_PIN>(all_leds_in_parallel, NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_TWO_PIN>(all_leds_in_parallel, NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_THREE_PIN>(all_leds_in_parallel, NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_FOUR_PIN>(all_leds_in_parallel, NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_FIVE_PIN>(all_leds_in_parallel, NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_SIX_PIN>(all_leds_in_parallel, NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_SEVEN_PIN>(all_leds_in_parallel, NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_EIGHT_PIN>(all_leds_in_parallel, NUM_PIXELS_PER_STRIP);
  */

  /*
  FastLED.addLeds<NEOPIXEL, STRIP_ONE_PIN>(strip_one_leds, NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_TWO_PIN>(strip_two_leds, NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_THREE_PIN>(strip_three_leds, NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_FOUR_PIN>(strip_four_leds, NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_FIVE_PIN>(strip_five_leds, NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_SIX_PIN>(strip_six_leds, NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_SEVEN_PIN>(strip_seven_leds, NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_EIGHT_PIN>(strip_eight_leds, NUM_PIXELS_PER_STRIP);
  */

  FastLED.addLeds<NEOPIXEL, STRIP_ONE_PIN>(leds[0], NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_TWO_PIN>(leds[1], NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_THREE_PIN>(leds[2], NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_FOUR_PIN>(leds[3], NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_FIVE_PIN>(leds[4], NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_SIX_PIN>(leds[5], NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_SEVEN_PIN>(leds[6], NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, STRIP_EIGHT_PIN>(leds[7], NUM_PIXELS_PER_STRIP);
}

void loop() {
  //moving_blue_line();
  //delay(5000);
  //moving_red_dots_in_series();
  //delay(5000);
  //randoDot();
  bunchaRandoDots();
}

void moving_blue_line() {
  for(int dot = 0; dot < NUM_PIXELS_PER_STRIP; dot++) { 
    leds[0][dot] = CRGB::Blue;
    leds[1][dot] = CRGB::Blue;
    leds[2][dot] = CRGB::Blue;
    leds[3][dot] = CRGB::Blue;
    leds[4][dot] = CRGB::Blue;
    leds[5][dot] = CRGB::Blue;
    leds[6][dot] = CRGB::Blue;
    leds[7][dot] = CRGB::Blue;
    FastLED.show();
    // clear this led for the next time around the loop
    leds[0][dot] = CRGB::Black;
    leds[1][dot] = CRGB::Black;
    leds[2][dot] = CRGB::Black;
    leds[3][dot] = CRGB::Black;
    leds[4][dot] = CRGB::Black;
    leds[5][dot] = CRGB::Black;
    leds[6][dot] = CRGB::Black;
    leds[7][dot] = CRGB::Black;
    delay(500);
  }
}

void moving_red_dots_in_series() {
  /*
  for(int i = 0; i < NUM_STRIPS; i++ ) {
    move_red_dot(all_strips[i]);
  }
  */
  // This outer loop will go over each strip, one at a time
  for(int x = 0; x < NUM_STRIPS; x++) {
    // This inner loop will go over each led in the current strip, one at a time
    for(int i = 0; i < NUM_PIXELS_PER_STRIP; i++) {
      leds[x][i] = CHSV(persistent_h % 255, 255, 128);
      FastLED.show();
      leds[x][i] = CRGB::Black;
      persistent_h++;
      delay(500);
    }
  }
}

void bunchaRandoDots() {
  int rand_h;
  int rand_strip;
  int rand_led;
  int num_times = random(1,11);

  pair selected_leds[num_times];

  for(int i = 0; i < num_times; i++) {
    rand_strip = random(0,NUM_STRIPS);
    rand_led = random(0,NUM_PIXELS_PER_STRIP);
    rand_h = random(0, 255);
    selected_leds[i] = {rand_strip, rand_led};
    leds[rand_strip][rand_led] = CHSV(rand_h, 255, 128);
  }

  FastLED.show();
  
  for(int i = 0; i < num_times; i++) {
    leds[selected_leds[i].first][selected_leds[i].second] = CRGB::Black;
  }

  delay(5000);
}

void randoDot() {
  int rand_h = random(0, 255);
  //int rand_s = random(0, 255);
  //int rand_v = random(0, 255);
  int rand_strip = random(0,NUM_STRIPS);
  int rand_led = random(0,NUM_PIXELS_PER_STRIP);

  leds[rand_strip][rand_led] = CHSV(rand_h, 255, 128);
  FastLED.show();
  leds[rand_strip][rand_led] = CRGB::Black;
  persistent_h++;
  delay(1000);
}
/*void move_red_dot(CRGB &leds) {
  for(int dot = 0; dot < NUM_PIXELS_PER_STRIP; dot++) { 
    leds[dot] = CRGB::Red;
    FastLED.show();
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    delay(1000);
  }
}*/
/*
CRGB * generateColorSteps(CRGB starting_rgb, CRGB target_rgb, int num_desired_steps) {
  CRGB[num_desired_steps + 2] steps;
  //We know our starting and ending values, let's just figure out everything in between!
  steps[0] = starting_rgb;
  steps[num_desired_steps - 1] = target_rgb;

  int starting_r = starting_rgb.r;
  int target_r = target_rgb.r;
  int starting_g = starting_rgb.g;
  int target_g = target_rgb.g;
  int starting_b = starting_rgb.b;
  int target_b = target_rgb.b;

  //for each channel (R, G, B)
    int diff = target_r - starting_r;
    //Because these are ints and not floats we're already discarding remainders
    int delta = abs(diff) / num_desired_steps;
    if(diff > 0) {
      for(int i = 1; i <= num_desired_steps; i++) {
        steps[i]
      }
    } else if(diff < 0) {
      for(int i = 1; i <= num_desired_steps; i++) {
  
      }
    }
    
    //for each step:
      //if diff is positive, increment
      //if diff is negative, decrement
}
*/

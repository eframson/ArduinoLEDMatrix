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

struct led_meta {
  int x;
  int y;
  int h;
};

CRGB leds[NUM_STRIPS][NUM_PIXELS_PER_STRIP];
int persistent_h = 0;

//led_meta old_leds[10];
//led_meta new_leds[10];
led_meta selected_leds[10];

void setup() {
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
  int num_fade_levels = 32;
  int fadeSettings[num_fade_levels] = {4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128};

  //pair selected_leds[num_times];

  //Choose LEDs + colors
  for(int i = 0; i < num_times; i++) {
    rand_strip = random(0,NUM_STRIPS);
    rand_led = random(0,NUM_PIXELS_PER_STRIP);
    rand_h = random(0, 255);
    selected_leds[i] = {rand_strip, rand_led, rand_h};
  }

  for(int i = 0; i < num_fade_levels; i++) {
    for(int j = 0; j < num_times; j++) {
      leds[selected_leds[j].x][selected_leds[j].y] = CHSV(selected_leds[j].h, 255, fadeSettings[i]);
    }
    FastLED.show();
    delay(100);
  }
  
  delay(900);

  for(int i = (num_fade_levels - 1); i >= 0; i--) {
    for(int j = 0; j < num_times; j++) {
      leds[selected_leds[j].x][selected_leds[j].y] = CHSV(selected_leds[j].h, 255, fadeSettings[i]);
    }
    FastLED.show();
    delay(100);
  }

  for(int j = 0; j < num_times; j++) {
    leds[selected_leds[j].x][selected_leds[j].y] = CRGB::Black;
  }
  FastLED.show();
  delay(1000);
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

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define STRIP_ONE_PIN 3
#define STRIP_TWO_PIN 4
#define STRIP_THREE_PIN 5
#define STRIP_FOUR_PIN 6
#define STRIP_FIVE_PIN 7
#define STRIP_SIX_PIN 8
#define STRIP_SEVEN_PIN 9
#define STRIP_EIGHT_PIN 10

#define NUM_PIXELS_PER_STRIP 8
#define NUM_STRIPS 8

/*
 * TODO:
 * Cycle through arrays a given number of times and loop back to the start if necessary (if there's a mismatch between # colors and # arrays e.g.)
 * Allow multiple arrays with different color values apiece in tandem
 */
Adafruit_NeoPixel strip_one = Adafruit_NeoPixel(NUM_PIXELS_PER_STRIP, STRIP_ONE_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_two = Adafruit_NeoPixel(NUM_PIXELS_PER_STRIP, STRIP_TWO_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_three = Adafruit_NeoPixel(NUM_PIXELS_PER_STRIP, STRIP_THREE_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_four = Adafruit_NeoPixel(NUM_PIXELS_PER_STRIP, STRIP_FOUR_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_five = Adafruit_NeoPixel(NUM_PIXELS_PER_STRIP, STRIP_FIVE_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_six = Adafruit_NeoPixel(NUM_PIXELS_PER_STRIP, STRIP_SIX_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_seven = Adafruit_NeoPixel(NUM_PIXELS_PER_STRIP, STRIP_SEVEN_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_eight = Adafruit_NeoPixel(NUM_PIXELS_PER_STRIP, STRIP_EIGHT_PIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel all_strips[NUM_STRIPS] = {strip_one, strip_two, strip_three, strip_four, strip_five, strip_six, strip_seven, strip_eight};

struct xypair {
  int x;
  int y;
};

#define TOTAL_COLORS 12

uint32_t red = strip_one.Color(255, 0, 0);
uint32_t orange = strip_one.Color(255, 125, 0);
uint32_t yellow = strip_one.Color(255, 255, 0);
uint32_t spring_green = strip_one.Color(125, 255, 0);
uint32_t green = strip_one.Color(0, 255, 0);
uint32_t turquoise = strip_one.Color(0, 255, 125);
uint32_t cyan = strip_one.Color(0, 255, 255);
uint32_t ocean = strip_one.Color(0, 125, 255);
uint32_t blue = strip_one.Color(0, 0, 255);
uint32_t violet = strip_one.Color(125, 0, 255);
uint32_t magenta = strip_one.Color(255, 0, 255);
uint32_t raspberry = strip_one.Color(255, 0, 125);
uint32_t white = strip_one.Color(255, 255, 255);

uint32_t rainbow[TOTAL_COLORS] = {
  red,
  orange,
  yellow,
  spring_green,
  green,
  turquoise,
  cyan,
  ocean,
  blue,
  violet,
  magenta,
  raspberry
};

void setup() {
  Serial.begin(9600);
  for( unsigned int i = 0; i < 8; i++ ) {
    all_strips[i].begin();
    all_strips[i].setBrightness(16);
    all_strips[i].show();
  }
  
  //movingDiamondPattern(blue);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait, Adafruit_NeoPixel &strip) {
  for(uint16_t i=0; i < NUM_PIXELS_PER_STRIP; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void colorWipeWithoutShowOrDelay(uint32_t c, Adafruit_NeoPixel &strip) {
  for(uint16_t i=0; i < NUM_PIXELS_PER_STRIP; i++) {
    strip.setPixelColor(i, c);
  }
}

void xColorWipe() {
  unsigned int i = 0;

  for( i = 0; i < NUM_STRIPS; i++ ) {
    colorWipe(all_strips[i].Color(255, 0, 0), 50, all_strips[i]);
  }
  for( i = 0; i < NUM_STRIPS; i++ ) {
    colorWipe(all_strips[i].Color(0, 255, 0), 50, all_strips[i]);
  }
  for( i = 0; i < NUM_STRIPS; i++ ) {
    colorWipe(all_strips[i].Color(0, 0, 255), 50, all_strips[i]);
  }
}

void yColorWipe() {
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int c = 0;
  
  uint32_t colors[3] = {red, green, blue};
  for( c = 0; c < 3; c++ ) {
    for( i = 0; i < NUM_PIXELS_PER_STRIP; i++ ) {
      for( j = 0; j < NUM_STRIPS; j++ ) {
        all_strips[j].setPixelColor(i, colors[c]);
        all_strips[j].show();
        delay(50);
      }
    }
  }
}

void setPixelColorAtCoords(int x, int y, uint32_t color) {
  all_strips[x].setPixelColor(y, color);
}

void turnStripsOff() {
  unsigned int i = 0;
  for( i = 0; i < NUM_STRIPS; i++ ) {
    colorWipeWithoutShowOrDelay(all_strips[i].Color(0, 0, 0), all_strips[i]);
  }
}

void showAllStrips() {
  unsigned int i = 0;
  for( i = 0; i < NUM_STRIPS; i++ ) {
    all_strips[i].show();
  }
}

void rainbowExpandingSquarePattern() {
  //This was supposed to take a dynamic number of colors and cycle through as appropriate, maybe some modulo stuff, whatever...
  //Not going to figure this out for now, just modifying expandingSquarePattern to have the full rainbow
}

void expandingSquarePattern() {
  unsigned int i = 0;
  unsigned int j = 0;

  //I'm not sure how to deal with an array of dynamically-sized arrays, so I'm just populating
  //them all with dummy (repeated) values where appropriate. Worst case scenario is it's
  //inefficient, but I'd rather inefficient code than non-functional. Actual dummy values
  //(like {0,0} can't be used because that's an actual pixel
  xypair sm_sq[28] = {
    {3,3},{3,4},{4,3},{4,4},
    {3,3},{3,4},{4,3},{4,4},
    {3,3},{3,4},{4,3},{4,4},
    {3,3},{3,4},{4,3},{4,4},
    {3,3},{3,4},{4,3},{4,4},
    {3,3},{3,4},{4,3},{4,4},
    {3,3},{3,4},{4,3},{4,4}
  };
  xypair med_sq[28] = {
    {5,2},{5,3},{5,4},{5,5},
    {4,2},{4,5},{3,2},{3,5},
    {2,2},{2,3},{2,4},{2,5},
    {5,2},{5,3},{5,4},{5,5},
    {4,2},{4,5},{3,2},{3,5},
    {2,2},{2,3},{2,4},{2,5},
    {5,2},{5,3},{5,4},{5,5}
  };
  xypair lg_sq[28] = {
    {6,1},{6,2},{6,3},{6,4},
    {6,5},{6,6},{5,1},{5,6},
    {4,1},{4,6},{3,1},{3,6},
    {2,1},{2,6},{1,1},{1,2},
    {1,3},{1,4},{1,5},{1,6},
    {6,1},{6,2},{6,3},{6,4},
    {6,5},{6,6},{5,1},{5,6}
  };
  xypair xlg_sq[28] = {
    {7,0},{7,1},{7,2},{7,3},
    {7,4},{7,5},{7,6},{7,7},
    {6,0},{6,7},{5,0},{5,7},
    {4,0},{4,7},{3,0},{3,7},
    {2,0},{2,7},{1,0},{1,7},
    {0,0},{0,1},{0,2},{0,3},
    {0,4},{0,5},{0,6},{0,7}
  };

  xypair *all_shapes[12] = {
    sm_sq,
    med_sq,
    lg_sq,
    xlg_sq,
    lg_sq,
    med_sq,
    sm_sq,
    med_sq,
    lg_sq,
    xlg_sq,
    lg_sq,
    med_sq
  };

  for( i = 0; i < 12; i++ ) {
    turnStripsOff();
    for( j = 0; j < 28; j++ ) {
      setPixelColorAtCoords(all_shapes[i][j].x, all_shapes[i][j].y, rainbow[i]);
    }
    showAllStrips();
    delay(500);
  }
}

void movingDiamondPattern(uint32_t color) {
  
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int array_size = 16;
  unsigned int num_arrays_in_array = 8;
  
  xypair diamond_one[array_size] = {
    {0,3},{0,4},{7,4},{7,3},
    {4,7},{3,7},{3,0},{4,0},
    {5,1},{6,2},{6,5},{5,6},
    {2,6},{1,5},{1,2},{2,1}
  };
  
  xypair diamond_two[array_size] = {
    {3,0},{4,0},{4,1},{3,1},
    {5,2},{6,3},{7,4},{7,5},
    {2,2},{1,3},{0,4},{0,5},
    {1,6},{2,7},{6,6},{5,7}
  };
  
  xypair diamond_three[array_size] = {
    {5,0},{2,0},{3,1},{4,1},
    {4,2},{3,2},{2,3},{5,3},
    {6,4},{7,5},{7,6},{1,4},
    {0,5},{0,6},{1,7},{6,7}
  };
  
  xypair diamond_four[array_size] = {
    {4,2},{3,2},{3,3},{4,3},
    {5,4},{6,5},{7,6},{7,7},
    {2,4},{1,5},{0,6},{0,7},
    {2,1},{1,0},{5,1},{6,0}
  };
  
  xypair diamond_five[array_size] = {
    {4,3},{3,3},{3,4},{4,4},
    {5,5},{6,6},{7,7},{2,5},
    {1,6},{0,7},{2,2},{1,1},
    {0,0},{5,2},{6,1},{7,0}
  };
  
  xypair diamond_six[array_size] = {
    {4,4},{3,4},{3,5},{4,5},
    {5,6},{6,7},{2,6},{1,7},
    {2,3},{1,2},{0,1},{0,0},
    {5,3},{6,2},{7,1},{7,0}
  };
  
  xypair diamond_seven[array_size] = {
    {4,5},{3,5},{3,6},{4,6},
    {5,7},{2,7},{2,4},{1,3},
    {0,2},{0,1},{1,0},{5,4},
    {6,3},{7,2},{7,1},{6,0}
  };
  
  xypair diamond_eight[array_size] = {
    {3,7},{3,6},{4,6},{4,7},
    {2,5},{1,4},{0,3},{0,2},
    {1,1},{2,0},{5,5},{6,4},
    {7,3},{7,2},{6,1},{5,0}
  };

  xypair *all_shapes[num_arrays_in_array] = {
    diamond_one,
    diamond_two,
    diamond_three,
    diamond_four,
    diamond_five,
    diamond_six,
    diamond_seven,
    diamond_eight
  };

  for( i = 0; i < num_arrays_in_array; i++ ) {
    turnStripsOff();
    for( j = 0; j < array_size; j++ ) {
      setPixelColorAtCoords(all_shapes[i][j].x, all_shapes[i][j].y, color);
    }
    showAllStrips();
    delay(100);
  }
}

void movingWavePattern() {
  int num_in_array = 36;
  int num_arrays_in_array = 4;
  int i = 0;
  int j = 0;
  
  xypair waveform_one[num_in_array] = {
    {7,0},{6,0},{5,0},{4,0},
    {3,0},{2,0},{1,0},{0,0},
    {7,1},{7,2},{6,2},{5,2},
    {4,2},{3,2},{2,2},{1,2},
    {0,2},{0,3},{7,4},{6,4},
    {5,4},{4,4},{3,4},{2,4},
    {1,4},{0,4},{7,5},{7,6},
    {6,6},{5,6},{4,6},{3,6},
    {2,6},{1,6},{0,6},{0,7}
  };

  xypair waveform_two[num_in_array] = {
    {7,1},{6,1},{5,1},{4,1},
    {3,1},{2,1},{1,1},{0,1},
    {7,2},{7,3},{6,3},{5,3},
    {4,3},{3,3},{2,3},{1,3},
    {0,3},{0,4},{7,5},{6,5},
    {5,5},{4,5},{3,5},{2,5},
    {1,5},{0,5},{7,6},{7,7},
    {6,7},{5,7},{4,7},{3,7},
    {2,7},{1,7},{0,7},{0,0}
  };

  xypair waveform_three[num_in_array] = {
    {7,0},{6,0},{5,0},{4,0},
    {3,0},{2,0},{1,0},{0,0},
    {0,1},{7,2},{6,2},{5,2},
    {4,2},{3,2},{2,2},{1,2},
    {0,2},{7,3},{7,4},{6,4},
    {5,4},{4,4},{3,4},{2,4},
    {1,4},{0,4},{0,5},{7,6},
    {6,6},{5,6},{4,6},{3,6},
    {2,6},{1,6},{0,6},{7,7}
  };

  xypair waveform_four[num_in_array] = {
    {7,0},{7,1},{6,1},{5,1},
    {4,1},{3,1},{2,1},{1,1},
    {0,1},{0,2},{7,3},{6,3},
    {5,3},{4,3},{3,3},{2,3},
    {1,3},{0,3},{7,4},{7,5},
    {6,5},{5,5},{4,5},{3,5},
    {2,5},{1,5},{0,5},{0,6},
    {7,7},{6,7},{5,7},{4,7},
    {3,7},{2,7},{1,7},{0,7}
  };

  xypair *all_waves[num_arrays_in_array] = {
    waveform_one,
    waveform_two,
    waveform_three,
    waveform_four,
  };

  for( i = 0; i < num_arrays_in_array; i++ ) {
    turnStripsOff();
    for( j = 0; j < num_in_array; j++ ) {
      setPixelColorAtCoords(all_waves[i][j].x, all_waves[i][j].y, spring_green);
    }
    showAllStrips();
    delay(100);
  }

}

void checkerboardPattern(int num_times_to_do = 1) {
  int num_in_array = 32;
  int num_arrays_in_array = 2;
  int i = 0;
  int j = 0;
  uint32_t colors[2] = {ocean, orange};
  
  xypair checkerboard_one[num_in_array] = {
    {7,7},{6,6},{5,5},{4,4},
    {3,3},{2,2},{1,1},{0,0},
    {0,2},{1,3},{2,4},{3,5},
    {4,6},{5,7},{0,4},{1,5},
    {2,6},{3,7},{0,6},{1,7},
    {2,0},{3,1},{4,2},{5,3},
    {6,4},{7,5},{4,0},{5,1},
    {6,2},{7,3},{6,0},{7,1}
  };

  xypair checkerboard_two[num_in_array] = {
    {7,0},{5,0},{3,0},{1,0},
    {0,1},{2,1},{4,1},{6,1},
    {7,2},{7,4},{7,6},{6,3},
    {6,5},{6,7},{5,2},{5,4},
    {5,6},{4,3},{4,5},{4,7},
    {3,6},{3,4},{3,2},{2,3},
    {2,5},{2,7},{0,7},{1,6},
    {0,5},{1,4},{0,3},{1,2}
  };

  xypair *all_shapes[num_arrays_in_array] = {
    checkerboard_one,
    checkerboard_two,
  };

  for( i = 0; i < (num_arrays_in_array * num_times_to_do); i++ ) {
    turnStripsOff();
    for( j = 0; j < num_in_array; j++ ) {
      setPixelColorAtCoords(all_shapes[i % num_arrays_in_array][j].x, all_shapes[i % num_arrays_in_array][j].y, colors[i % num_arrays_in_array]);
    }
    showAllStrips();
    delay(1000);
  }

}

void travellingQuadrantPattern() {
  int num_in_array = 12;
  int num_arrays_in_array = 4;
  int i = 0;
  int j = 0;
  int k = 0;

  xypair x_pattern[16] = {
    {0,0},{1,1},{2,2},{3,3},
    {4,4},{5,5},{6,6},{7,7},
    {4,3},{3,4},{2,5},{5,2},
    {6,1},{7,0},{1,6},{0,7}
  };

  xypair n_tri[num_in_array] = {
    {7,1},{7,2},{7,3},{7,4},
    {7,5},{7,6},{6,2},{6,3},
    {6,4},{6,5},{5,3},{5,4}
  };

  xypair r_tri[num_in_array] = {
    {6,7},{5,7},{4,7},{3,7},
    {2,7},{1,7},{5,6},{4,6},
    {3,6},{2,6},{4,5},{3,5}
  };

  xypair b_tri[num_in_array] = {
    {0,1},{0,2},{0,3},{0,4},
    {0,5},{0,6},{1,2},{1,3},
    {1,4},{1,5},{2,3},{2,4}
  };

  xypair l_tri[num_in_array] = {
    {6,0},{5,0},{4,0},{3,0},
    {2,0},{1,0},{5,1},{4,1},
    {3,1},{2,1},{4,2},{3,2}
  };

  xypair *all_tris[num_arrays_in_array] = {
    n_tri,
    r_tri,
    b_tri,
    l_tri
  };

  for( i = 0; i < TOTAL_COLORS; i++ ) {
    turnStripsOff();
    for( k = 0; k < 16; k++ ) {
      setPixelColorAtCoords(x_pattern[k].x, x_pattern[k].y, white);
    }
    for( j = 0; j < num_in_array; j++ ) {
      setPixelColorAtCoords(all_tris[i % num_arrays_in_array][j].x, all_tris[i % num_arrays_in_array][j].y, rainbow[i]);
    }
    showAllStrips();
    delay(500);
  }

}

void loop() {
  checkerboardPattern(5);
  travellingQuadrantPattern();
  yColorWipe();
  xColorWipe();
  expandingSquarePattern();
  movingDiamondPattern(blue);
  movingDiamondPattern(red);
  movingDiamondPattern(green);
}

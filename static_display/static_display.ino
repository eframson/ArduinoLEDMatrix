#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

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
  char x;
  char y;
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

int get_free_ram() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
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
  char i = 0;

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
  char i = 0;
  char j = 0;
  char c = 0;
  
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

void setPixelColorAtCoords(char x, char y, uint32_t color) {
  all_strips[x].setPixelColor(y, color);
}

void turnStripsOff() {
  char i = 0;
  for( i = 0; i < NUM_STRIPS; i++ ) {
    colorWipeWithoutShowOrDelay(all_strips[i].Color(0, 0, 0), all_strips[i]);
  }
}

void showAllStrips() {
  char i = 0;
  for( i = 0; i < NUM_STRIPS; i++ ) {
    all_strips[i].show();
  }
}

void expandingSquarePattern() {
  char i = 0;
  char j = 0;

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

void movingDiamondPattern(int num_times_to_do = 1, uint32_t array_of_colors[] = rainbow, int num_colors = TOTAL_COLORS, int wait = 500) {

  //num_times_to_do should be the main criterion to satisfy. Colors and num_shapes should both be able to independently adapt
  char i = 0;
  char j = 0;
  char num_shapes = 8;
  char num_pixels_in_each_shape = 16;

  xypair shape_1[num_pixels_in_each_shape] = {
    {0,3},{0,4},{7,4},{7,3},
    {4,7},{3,7},{3,0},{4,0},
    {5,1},{6,2},{6,5},{5,6},
    {2,6},{1,5},{1,2},{2,1}
  };

  xypair shape_2[num_pixels_in_each_shape] = {
    {3,0},{4,0},{4,1},{3,1},
    {5,2},{6,3},{7,4},{7,5},
    {2,2},{1,3},{0,4},{0,5},
    {1,6},{2,7},{6,6},{5,7}
  };

  xypair shape_3[num_pixels_in_each_shape] = {
    {5,0},{2,0},{3,1},{4,1},
    {4,2},{3,2},{2,3},{5,3},
    {6,4},{7,5},{7,6},{1,4},
    {0,5},{0,6},{1,7},{6,7}
  };

  xypair shape_4[num_pixels_in_each_shape] = {
    {4,2},{3,2},{3,3},{4,3},
    {5,4},{6,5},{7,6},{7,7},
    {2,4},{1,5},{0,6},{0,7},
    {2,1},{1,0},{5,1},{6,0}
  };

  xypair shape_5[num_pixels_in_each_shape] = {
    {4,3},{3,3},{3,4},{4,4},
    {5,5},{6,6},{7,7},{2,5},
    {1,6},{0,7},{2,2},{1,1},
    {0,0},{5,2},{6,1},{7,0}
  };

  xypair shape_6[num_pixels_in_each_shape] = {
    {4,4},{3,4},{3,5},{4,5},
    {5,6},{6,7},{2,6},{1,7},
    {2,3},{1,2},{0,1},{0,0},
    {5,3},{6,2},{7,1},{7,0}
  };

  xypair shape_7[num_pixels_in_each_shape] = {
    {4,5},{3,5},{3,6},{4,6},
    {5,7},{2,7},{2,4},{1,3},
    {0,2},{0,1},{1,0},{5,4},
    {6,3},{7,2},{7,1},{6,0}
  };

  xypair shape_8[num_pixels_in_each_shape] = {
    {3,7},{3,6},{4,6},{4,7},
    {2,5},{1,4},{0,3},{0,2},
    {1,1},{2,0},{5,5},{6,4},
    {7,3},{7,2},{6,1},{5,0}
  };

  xypair *all_shapes[num_shapes] = {
    shape_1,
    shape_2,
    shape_3,
    shape_4,
    shape_5,
    shape_6,
    shape_7,
    shape_8
  };

  for( i = 0; i < (num_shapes * num_times_to_do); i++ ) {
    turnStripsOff();
    for( j = 0; j < num_pixels_in_each_shape; j++ ) {
      setPixelColorAtCoords(all_shapes[i % num_shapes][j].x, all_shapes[i % num_shapes][j].y, array_of_colors[i % num_colors]);
    }
    showAllStrips();
    delay(wait);
  }

}




void movingWavePattern() {
  int num_in_array = 36;
  int num_arrays_in_array = 4;
  char i = 0;
  char j = 0;
  
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
  char i = 0;
  char j = 0;
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

void travellingTriangles() {
  int num_in_array = 12;
  int num_arrays_in_array = 4;
  char i = 0;
  char j = 0;
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

void fourQuadrants(int num_times_to_do = 1, uint32_t array_of_colors[] = rainbow, int num_colors = TOTAL_COLORS, int wait = 500) {

  //num_times_to_do should be the main criterion to satisfy. Colors and num_shapes should both be able to independently adapt
  char i = 0;
  char j = 0;
  char num_shapes = 4;
  char num_pixels_in_each_shape = 17;

  xypair shape_1[num_pixels_in_each_shape] = {
    {17,17},
    {7,0},{7,1},{7,2},{7,3},
    {6,0},{6,1},{6,2},{6,3},
    {5,0},{5,1},{5,2},{5,3},
    {4,0},{4,1},{4,2},{4,3}
  };

  xypair shape_2[num_pixels_in_each_shape] = {
    {17,17},
    {7,4},{7,5},{7,6},{7,7},
    {6,4},{6,5},{6,6},{6,7},
    {5,4},{5,5},{5,6},{5,7},
    {4,4},{4,5},{4,6},{4,7}
  };

  xypair shape_3[num_pixels_in_each_shape] = {
    {17,17},
    {3,4},{3,5},{3,6},{3,7},
    {2,4},{2,5},{2,6},{2,7},
    {1,4},{1,5},{1,6},{1,7},
    {0,4},{0,5},{0,6},{0,7}
  };

  xypair shape_4[num_pixels_in_each_shape] = {
    {17,17},
    {3,0},{3,1},{3,2},{3,3},
    {2,0},{2,1},{2,2},{2,3},
    {1,0},{1,1},{1,2},{1,3},
    {0,0},{0,1},{0,2},{0,3}
  };


  xypair *all_shapes[num_shapes] = {
    shape_1,
    shape_2,
    shape_3,
    shape_4
  };

  int num_pixels_in_array;

  for( i = 0; i < (num_shapes * num_times_to_do); i++ ) {
    turnStripsOff();
    num_pixels_in_array = all_shapes[i % num_shapes][0].x;
    for( j = 1; j < num_pixels_in_array; j++ ) {
      setPixelColorAtCoords(all_shapes[i % num_shapes][j].x, all_shapes[i % num_shapes][j].y, array_of_colors[i % num_colors]);
    }
    showAllStrips();
    delay(wait);
  }

}

//void backAndForthCorners(int num_times_to_do = 1, uint32_t array_of_colors[] = &rainbow, int num_colors = TOTAL_COLORS, int wait = 500) {
void backAndForthCorners() {

  //num_times_to_do should be the main criterion to satisfy. Colors and num_shapes should both be able to independently adapt
  char i = 0;
  char j = 0;
  char num_shapes = 16;
  char num_pixels_in_each_shape = 15;
  
  int num_times_to_do = 1;
  uint32_t array_of_colors[1] = {red};
  int num_colors = 1;
  int wait = 50;

  xypair shape_1[2] = {
    {2,2},
    {0,7}
  };

  xypair shape_2[4] = {
    {4,4},
    {0,6},{1,6},{1,7}
  };

  xypair shape_3[6] = {
    {6,6},
    {0,5},{1,5},{2,5},{2,6},
    {2,7}
  };

  xypair shape_4[8] = {
    {8,8},
    {0,4},{1,4},{2,4},{3,4},
    {3,5},{3,6},{3,7}
  };

  xypair shape_5[10] = {
    {10,10},
    {0,3},{1,3},{2,3},{3,3},
    {4,3},{4,4},{4,5},{4,6},
    {4,7}
  };

  xypair shape_6[12] = {
    {12,12},
    {0,2},{1,2},{2,2},{3,2},
    {4,2},{5,2},{5,3},{5,4},
    {5,5},{5,6},{5,7}
  };

  xypair shape_7[14] = {
    {14,14},
    {0,1},{1,1},{2,1},{3,1},
    {4,1},{5,1},{6,1},{6,2},
    {6,3},{6,4},{6,5},{6,6},
    {6,7}
  };

  xypair shape_8[16] = {
    {16,16},
    {7,0},{6,0},{5,0},{4,0},
    {3,0},{2,0},{1,0},{0,0},
    {7,1},{7,2},{7,3},{7,4},
    {7,5},{7,6},{7,7}
  };

  xypair shape_9[2] = {
    {2,2},
    {7,0}
  };

  xypair shape_10[4] = {
    {4,4},
    {6,0},{6,1},{7,1}
  };

  xypair shape_11[6] = {
    {6,6},
    {5,0},{5,1},{5,2},{6,2},
    {7,2}
  };

  xypair shape_12[8] = {
    {8,8},
    {4,0},{4,1},{4,2},{4,3},
    {5,3},{6,3},{7,3}
  };

  xypair shape_13[10] = {
    {10,10},
    {3,0},{3,1},{3,2},{3,3},
    {3,4},{4,4},{5,4},{6,4},
    {7,4}
  };

  xypair shape_14[12] = {
    {12,12},
    {2,0},{2,1},{2,2},{2,3},
    {2,4},{2,5},{3,5},{4,5},
    {5,5},{6,5},{7,5}
  };

  xypair shape_15[14] = {
    {14,14},
    {1,0},{1,1},{1,2},{1,3},
    {1,4},{1,5},{1,6},{2,6},
    {3,6},{4,6},{5,6},{6,6},
    {7,6}
  };

  xypair shape_16[16] = {
    {16,16},
    {0,7},{0,6},{0,5},{0,4},
    {0,3},{0,2},{0,1},{0,0},
    {1,7},{2,7},{3,7},{4,7},
    {5,7},{6,7},{7,7}
  };

  xypair *all_shapes[num_shapes] = {
    shape_1,
    shape_2,
    shape_3,
    shape_4,
    shape_5,
    shape_6,
    shape_7,
    shape_8,
    shape_9,
    shape_10,
    shape_11,
    shape_12,
    shape_13,
    shape_14,
    shape_15,
    shape_16
  };

  int num_pixels_in_array;

  for( i = 0; i < (num_shapes * num_times_to_do); i++ ) {
    turnStripsOff();
    num_pixels_in_array = all_shapes[i % num_shapes][0].x;
    for( j = 1; j < num_pixels_in_array; j++ ) {
      setPixelColorAtCoords(all_shapes[i % num_shapes][j].x, all_shapes[i % num_shapes][j].y, array_of_colors[i % num_colors]);
    }
    showAllStrips();
    delay(wait);
  }

}

/*
void TEMPLATE_repeatAPatternXNumberOfTimesWithYColors(int num_times_to_do = 1, uint32_t array_of_colors[] = rainbow, int num_colors = TOTAL_COLORS, int wait = 500) {

  //num_times_to_do should be the main criterion to satisfy. Colors and num_shapes should both be able to independently adapt
  char i = 0;
  char j = 0;
  char num_shapes = 0;
  char num_pixels_in_each_shape = 0;

  xypair shape_1[num_pixels_in_each_shape] = {

  };

  xypair shape_2[num_pixels_in_each_shape] = {

  };

  xypair *all_shapes[num_shapes] = {
    shape_1,
    shape_2
  };

  for( i = 0; i < (num_shapes * num_times_to_do); i++ ) {
    turnStripsOff();
    for( j = 0; j < num_shapes; j++ ) {
      setPixelColorAtCoords(all_shapes[i % num_shapes][j].x, all_shapes[i % num_shapes][j].y, array_of_colors[i % num_colors]);
    }
    showAllStrips();
    delay(wait);
  }

}
*/

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  for( char i = 0; i < 8; i++ ) {
    all_strips[i].begin();
    all_strips[i].setBrightness(16);
    all_strips[i].show();
  }  
}

void loop() {
  fourQuadrants();
  backAndForthCorners();
  //checkerboardPattern(5);
  //travellingTriangles();
  //yColorWipe();
  //xColorWipe();
  //expandingSquarePattern();
  //movingDiamondPattern(6, rainbow, TOTAL_COLORS, 100);
  //backAndForthCorners(1, &red, 1, 50);
  //backAndForthCorners(1, &green, 1, 50);
  //backAndForthCorners(1, &blue, 1, 50);
}

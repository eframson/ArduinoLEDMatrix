/* Comment this out to disable prints and save space */
/* #define BLYNK_PRINT Serial */


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "FastLED.h"
#include "wifi_config.h"

#define NUM_LEDS 72
#define DATA_PIN 5
#define NUM_COLS 9
#define NUM_ROWS 8
#define NUM_FADE_LEVELS 63
#define FADE_IN_ANIM_INTERVAL_MS 25
#define FADE_OUT_ANIM_INTERVAL_MS 25
#define MAX_NUM_LEDS_LIT 72
#define NUM_ANIMS 14
#define BLYNK_TIMEOUT_THRESHOLD 4500

char auth[] = "IM0WqdsZ-YIRwxe6RBq8D_ag2fULeOTl";

// Your WiFi credentials.
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASS;

CRGB leds[NUM_LEDS];
CRGB primary_colors[] = {
  CRGB::Red,
  CRGB::Orange,
  CRGB::Yellow,
  CRGB::Green,
  CRGB::Blue,
  CRGB::Indigo,
  CRGB::Violet,
  CRGB::White,
  CRGB::FairyLight,
  CRGB::FairyLightNCC
};
int fxIdx = 0;
int displayOn = 0;
int activeColorIdx = 1;
int cursorWidth = 1;
int delayTime = 100;
int doRainbow = 0;
int led_grid_by_column[NUM_COLS][NUM_ROWS];
int clearOnNext = 0;

//TODO: Experiment with some kind of sine/logarithmic/exponential values here
//int fadeSettings[NUM_FADE_LEVELS] = {4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128};
int fadeSettings[NUM_FADE_LEVELS] = {4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,192,196,200,204,208,212,216,220,224,228,232,236,240,244,248,252};

struct led_coord {
  int x;
  int y;
  int led_idx;

  String toString() {
	  return "x: " + String(x) + ", y: " + String(y) + ", led_idx: " + String(led_idx);
  }
};

struct led_meta {
  int x;
  int y;
  int h;
};

//Should this be global or function specific? Could have uses either way...
led_meta selected_leds[MAX_NUM_LEDS_LIT];

int getLedIdxByXYCoords(int x, int y) {
  return led_grid_by_column[x][y];
}

//TODO: Add the ability to increment by more than one at a time (a "skipping around" effect)
led_coord getAdjacentLEDIdx(int x, int y, char* dir, int doLoop = 0) {
  int desired_x;
  int desired_y;
  led_coord nextLED;
  
  if(dir == "NW"){
    desired_x = x - 1;
    desired_y = y - 1;
  } else if (dir == "N") {
    desired_x = x;
    desired_y = y - 1;
  } else if (dir == "NE") {
    desired_x = x + 1;
    desired_y = y - 1;
  } else if (dir == "E") {
    desired_x = x + 1;
    desired_y = y;
  } else if (dir == "SE") {
    desired_x = x + 1;
    desired_y = y + 1;
  } else if (dir == "S") {
    desired_x = x;
    desired_y = y + 1;
  } else if (dir == "SW") {
    desired_x = x - 1;
    desired_y = y + 1;
  } else if (dir == "W") {
    desired_x = x - 1;
    desired_y = y;
  } else {
    nextLED.x = x;
    nextLED.y = y;
    nextLED.led_idx = getLedIdxByXYCoords(x, y);
    return nextLED;
  }

  if(desired_x >= NUM_COLS){
    if(doLoop == 1){
      desired_x = 0;
    } else {
      desired_x = x;
    }
  } else if (desired_x < 0) {
    if(doLoop == 1){
      desired_x = NUM_COLS - 1;
    } else {
      desired_x = x;
    }
  }

  if(desired_y >= NUM_ROWS) {
    if(doLoop == 1){
      desired_y = 0;
    } else {
      desired_y = y;
    }
  } else if (desired_y < 0) {
    if(doLoop == 1){
      desired_y = NUM_ROWS - 1;
    } else {
      desired_y = y;
    }
  }

  nextLED.x = desired_x;
  nextLED.y = desired_y;
  nextLED.led_idx = getLedIdxByXYCoords(desired_x, desired_y);
  return nextLED;
}

int _getNextYBasedOnDesiredHorizontalLoopingStrategy(int y, char* loopDir = "down") {
  if (loopDir == "down") {
    y++;
    if (y >= NUM_ROWS) {
      y = 0;
    }
  } else {
    y--;
    if (y < 0) {
      y = NUM_ROWS - 1;
    }
  }
  return y;
}

int _getNextXBasedOnDesiredVerticalLoopingStrategy(int x, char* loopDir = "right") {
  if (loopDir == "right") {
    x++;
    if (x >= NUM_COLS) {
      x = 0;
    }
  } else {
    x--;
    if (x < 0) {
      x = NUM_COLS - 1;
    }
  }
  return x;
}

led_coord getNextHorizontalLEDIdx(int x, int y, char* dir, int advancing = 0, char* loopDir = "down", int switchDirection = 0) {
  led_coord nextLED;
  if (dir == "E"){
    if (advancing == 1){
      if ( x == NUM_COLS - 1 ) {
        //At end of strip
        y = _getNextYBasedOnDesiredHorizontalLoopingStrategy(y, loopDir);

        if(!switchDirection){
          x = 0;
        }

        nextLED.x = x;
        nextLED.y = y;
        nextLED.led_idx = getLedIdxByXYCoords(x, y);
      } else {
        nextLED = getAdjacentLEDIdx(x, y, dir, 1);
      }
    } else {
      nextLED = getAdjacentLEDIdx(x, y, dir, 1);
    }
  } else if (dir == "W") {
    if (advancing == 1){
      if ( x == 0 ) {
        //At end (beginning, technically) of strip
        y = _getNextYBasedOnDesiredHorizontalLoopingStrategy(y, loopDir);

		if (!switchDirection) {
			x = NUM_COLS - 1;
		}

        nextLED.x = x;
        nextLED.y = y;
        nextLED.led_idx = getLedIdxByXYCoords(x, y);
      } else {
        nextLED = getAdjacentLEDIdx(x, y, "W", 1);
      }
    } else {
      nextLED = getAdjacentLEDIdx(x, y, "W", 1);
    }    
  } else {
    nextLED.x = x;
    nextLED.y = y;
    nextLED.led_idx = getLedIdxByXYCoords(x, y);
  }

  return nextLED;
}

led_coord getNextVerticalLEDIdx(int x, int y, char* dir, int advancing = 0, char* loopDir = "right") {
  led_coord nextLED;
  if (dir == "S"){
    if (advancing == 1){
      if ( y == NUM_ROWS - 1 ) {
        //At end of strip
        x = _getNextXBasedOnDesiredVerticalLoopingStrategy(x, loopDir);
        y = 0;

        nextLED.x = x;
        nextLED.y = y;
        nextLED.led_idx = getLedIdxByXYCoords(x, y);
      } else {
        nextLED = getAdjacentLEDIdx(x, y, "S", 1);
      }
    } else {
      nextLED = getAdjacentLEDIdx(x, y, "S", 1);
    }
  } else if (dir == "N") {
    if (advancing == 1){
      if ( y == 0 ) {
        //At end (beginning, technically) of strip
        x = _getNextXBasedOnDesiredVerticalLoopingStrategy(x, loopDir);
        y = NUM_ROWS - 1;
        
        nextLED.x = x;
        nextLED.y = y;
        nextLED.led_idx = getLedIdxByXYCoords(x, y);
      } else {
        nextLED = getAdjacentLEDIdx(x, y, "N", 1);
      }
    } else {
      nextLED = getAdjacentLEDIdx(x, y, "N", 1);
    }    
  } else {
    nextLED.x = x;
    nextLED.y = y;
    nextLED.led_idx = getLedIdxByXYCoords(x, y);
  }

  return nextLED;
}

int _calculate_blynk_should_run_frequency(){
  int blynk_run_frequency;
  float maximum_blynk_run_frequency = (BLYNK_TIMEOUT_THRESHOLD / delayTime) - 1;
  blynk_run_frequency = (int)maximum_blynk_run_frequency;
  return blynk_run_frequency;
}

void horizPattern(char* dir = "E", int advancing = 0, int fadeTrailLength = 0, char* loopDir = "down", int num_iterations = -1, int switchDirection = 0, int custom_start_x = -1, int custom_start_y = -1) { //TODO: Implement fade trail
  int starting_x = 0;
  int starting_y = 0;
  
  if(custom_start_x > -1 && custom_start_y > -1){
    starting_x = custom_start_x;
    starting_y = custom_start_y;
  } else {
    if(dir == "W"){
      starting_x = NUM_COLS - 1;
    }
  
    if(loopDir == "up"){
      starting_y = NUM_ROWS - 1;
    }
  }

  led_coord nextLED;
  nextLED.x = starting_x;
  nextLED.y = starting_y;
  nextLED.led_idx = getLedIdxByXYCoords(starting_x, starting_y);
  
  leds[nextLED.led_idx] = primary_colors[activeColorIdx - 1];
  FastLED.show();
  leds[nextLED.led_idx] = CRGB::Black;
  delay(delayTime);
  if(delayTime > 500){
    Blynk.run();
  }

  if(num_iterations == -1){
    num_iterations = (NUM_ROWS * NUM_COLS) - 1;
  }

  int blynk_run_frequency = _calculate_blynk_should_run_frequency();

  for(int i = 0; i < num_iterations; i++){
    if(i % blynk_run_frequency == 0){
      Blynk.run();
    }

    nextLED = getNextHorizontalLEDIdx(nextLED.x, nextLED.y, dir, advancing, loopDir);
    leds[nextLED.led_idx] = primary_colors[activeColorIdx - 1];
    FastLED.show();
    leds[nextLED.led_idx] = CRGB::Black;
    delay(delayTime);
  }
}

void vertPattern(char* dir = "S", int advancing = 0, int fadeTrailLength = 0, char* loopDir = "right", int num_iterations = -1, int custom_start_x = -1, int custom_start_y = -1) { //TODO: Implement fade trail
  int starting_x = 0;
  int starting_y = 0;
  
  if(custom_start_x > -1 && custom_start_y > -1){
    starting_x = custom_start_x;
    starting_y = custom_start_y;
  } else {
    if(dir == "N"){
      starting_y = NUM_ROWS - 1;
    }
  
    if(loopDir == "left"){
      starting_x = NUM_COLS - 1;
    }
  }

  led_coord nextLED;
  nextLED.x = starting_x;
  nextLED.y = starting_y;
  nextLED.led_idx = getLedIdxByXYCoords(starting_x, starting_y);

  leds[nextLED.led_idx] = primary_colors[activeColorIdx - 1];
  FastLED.show();
  leds[nextLED.led_idx] = CRGB::Black;
  delay(delayTime);
  if(delayTime > 500){
    Blynk.run();
  }

  if(num_iterations == -1){
    num_iterations = (NUM_ROWS * NUM_COLS) - 1;
  }

  int blynk_run_frequency = _calculate_blynk_should_run_frequency();

  for(int i = 0; i < num_iterations; i++){
    if(i % blynk_run_frequency == 0){
      Blynk.run();
    }

    nextLED = getNextVerticalLEDIdx(nextLED.x, nextLED.y, dir, advancing, loopDir);
    leds[nextLED.led_idx] = primary_colors[activeColorIdx - 1];
    FastLED.show();
    leds[nextLED.led_idx] = CRGB::Black;
    delay(delayTime);
  }
}

void horizSequentialPattern(char* start_dir = "E", int fadeTrailLength = 0, char* loopDir = "down", int num_iterations = -1, int custom_start_x = -1, int custom_start_y = -1) { //TODO: Implement fade trail
  int starting_x = 0;
  int starting_y = 0;
  
  if(custom_start_x > -1 && custom_start_y > -1){
    starting_x = custom_start_x;
    starting_y = custom_start_y;
  } else {
    if(start_dir == "E"){
      starting_x = 0;
    }else{
      starting_x = NUM_COLS - 1;
    }
    
    if(loopDir == "down"){
      starting_y = 0;
    } else{
      starting_y = NUM_ROWS - 1;
    }
  }

  led_coord nextLED;
  nextLED.x = starting_x;
  nextLED.y = starting_y;
  nextLED.led_idx = getLedIdxByXYCoords(starting_x, starting_y);
  
  leds[nextLED.led_idx] = primary_colors[activeColorIdx - 1];
  Serial.print(starting_x);
  Serial.print(",");
  Serial.println(starting_y);
  FastLED.show();
  leds[nextLED.led_idx] = CRGB::Black;
  delay(delayTime);
  if(delayTime > 500){
    Blynk.run();
  }

  if(num_iterations == -1){
    num_iterations = (NUM_ROWS * NUM_COLS) - 1;
  }

  int blynk_run_frequency = _calculate_blynk_should_run_frequency();
  char* dir = start_dir;

  for(int i = 0; i < num_iterations; i++){
    
    int current_y = nextLED.y;
    
    if(i % blynk_run_frequency == 0){
      Blynk.run();
    }

    nextLED = getNextHorizontalLEDIdx(nextLED.x, nextLED.y, dir, 1, loopDir, 1);
    leds[nextLED.led_idx] = primary_colors[activeColorIdx - 1];
    Serial.print(nextLED.x);
    Serial.print(",");
    Serial.println(nextLED.y);
    FastLED.show();
    leds[nextLED.led_idx] = CRGB::Black;
    delay(delayTime);

    //switch dir if we just changed lines
    if(current_y != nextLED.y){
      if(dir == "E"){
        dir = "W";
      }else if(dir == "W"){
        dir = "E";
      }
    }
  }
}
/* Patterns:
 * Horizontal (options: advancing/non-advancing, direction/origin, fading trail) 
 * --------->  --------->  <---------  <----------
 *             --------->              <----------
 *             --------->              <----------
 * 
 * Vertical (options: advancing/non-advancing, direction/origin, fading trail)
 * ^   ^ ^ ^   |   | | |
 * |   | | |   |   | | |
 * |   | | |   |   | | |
 * |   | | |   |   | | |
 * |   | | |   V   V V V
 * 
 * Sequential - Horizontal (options: fading trail, starting direction/origin--which is basically the same alternation except starting differently)
 * --------->   <---------
 * <---------   --------->
 * --------->   <---------
 * 
 * Sequential - Vertical (options: fading trail, starting direction/origin--which is basically the same alternation except starting differently)
 * ^ | ^        | ^ |
 * | | |        | | |
 * | | |        | | |
 * | | |        | | |
 * | V |        V | V
 */

void displayCursor()
{
  int desiredColorIdx;
  if(activeColorIdx > 0){
    for(int dot = 0; dot < NUM_LEDS; dot++) {
        leds[dot] = primary_colors[activeColorIdx - 1];
        if(cursorWidth > 1){
          if(cursorWidth > NUM_LEDS) {
            cursorWidth = NUM_LEDS;
          }
          for(int i = dot; i < (dot + cursorWidth); i++){
            if(i < NUM_LEDS){
              if(doRainbow){
                desiredColorIdx = (activeColorIdx - 1) + (i - dot);
                //if(desiredColorIdx > sizeof(primary_colors) - 1){
                if(desiredColorIdx > 7 - 1){
                  //desiredColorIdx = desiredColorIdx % sizeof(primary_colors);
                  desiredColorIdx = desiredColorIdx % 7;
                }
                leds[i] = primary_colors[desiredColorIdx];
              }else{
                leds[i] = primary_colors[activeColorIdx - 1];
              }
              
            } else {
              int activeLED = i % NUM_LEDS;
              if(doRainbow){
                desiredColorIdx = (activeColorIdx - 1) + (i - dot);
                //if(desiredColorIdx > sizeof(primary_colors) - 1){
                if(desiredColorIdx > 7 - 1){
                  //desiredColorIdx = desiredColorIdx % sizeof(primary_colors);
                  desiredColorIdx = desiredColorIdx % 7;
                }
                leds[activeLED] = primary_colors[desiredColorIdx];
              } else {
                leds[activeLED] = primary_colors[activeColorIdx - 1];
              }
            }
          }
        }
        FastLED.show();
        // clear this led for the next time around the loop
        leds[dot] = CRGB::Black;
        if(cursorWidth > 1){
          for(int i = dot; i < (dot + cursorWidth); i++){
            if(i < NUM_LEDS){
              leds[i] = CRGB::Black;
            }
          }
        }
        delay(delayTime);
        Blynk.run();
    }
  }
}

void clearDisplay()
{
  for(int dot = 0; dot < NUM_LEDS; dot++) { 
      leds[dot] = CRGB::Black;
      FastLED.show();
  }  
}

void switchToNextAnimation()
{
  fxIdx++;
  if(fxIdx >= NUM_ANIMS){
    fxIdx = 0;
  }
  clearOnNext = 1;
}

BLYNK_WRITE(V1)
{
  displayOn = param.asInt();
}

BLYNK_WRITE(V2)
{
  activeColorIdx = param.asInt();
}

BLYNK_WRITE(V3)
{
  cursorWidth = param.asInt();
}

BLYNK_WRITE(V4)
{
  int brightnessValue = param.asInt();
  if(brightnessValue < 0){
    brightnessValue = 0;
  } else if (brightnessValue > 255) {
    brightnessValue = 255;
  }
  FastLED.setBrightness(brightnessValue);
}

BLYNK_WRITE(V5)
{
  int delayParam = param.asInt();
  if(delayParam < 1){
    delayParam = 1;
  }
  delayTime = delayParam;
}

BLYNK_WRITE(V6)
{
  switchToNextAnimation();
  //doRainbow = param.asInt();
}

void calculate_linear_serpentine_array_indices_in_column(int indices[], int col_num)
{
  int col_end = NUM_ROWS * col_num;
  int col_start = col_end - (NUM_ROWS - 1);
  int i = 0;

  //col is even numbered
  if(col_num % 2 == 0){
    for(int j = col_end; j >= col_start; j--){
      indices[i] = j - 1;
      i++;
    }
  }else{
    for(int j = col_start; j <= col_end; j++){
      indices[i] = j - 1;
      i++;
    }
  }
}

void _set_up_2d_led_array()
{
  for(int i = 0; i < NUM_COLS; i++){
    calculate_linear_serpentine_array_indices_in_column(led_grid_by_column[i], i + 1);
  }
}

void bunchaRandoDots() {
  int rand_h;
  int rand_y;
  int rand_x;
  int num_times = random(1,cursorWidth);
  int num_fade_levels = NUM_FADE_LEVELS;

  //Choose LEDs + colors
  for(int i = 0; i < num_times; i++) {
    rand_y = random(0,NUM_COLS);
    rand_x = random(0,NUM_ROWS);
    rand_h = random(0, 255);
    selected_leds[i] = {rand_y, rand_x, rand_h};
  }

  for(int i = 0; i < num_fade_levels; i++) {
    for(int j = 0; j < num_times; j++) {
      leds[getLedIdxByXYCoords(selected_leds[j].x, selected_leds[j].y)] = CHSV(selected_leds[j].h, 255, fadeSettings[i]);
    }
    FastLED.show();
    delay(FADE_IN_ANIM_INTERVAL_MS);
  }
  
  Blynk.run();
  delay(delayTime);

  for(int i = (num_fade_levels - 1); i >= 0; i--) {
    for(int j = 0; j < num_times; j++) {
      leds[getLedIdxByXYCoords(selected_leds[j].x, selected_leds[j].y)] = CHSV(selected_leds[j].h, 255, fadeSettings[i]);
    }
    FastLED.show();
    delay(FADE_OUT_ANIM_INTERVAL_MS);
  }
  Blynk.run();

  for(int j = 0; j < num_times; j++) {
    leds[getLedIdxByXYCoords(selected_leds[j].x, selected_leds[j].y)] = CRGB::Black;
  }
  FastLED.show();
  delay(delayTime);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS).setCorrection(CRGB(255, 180, 160));

  //Turn on to show we've got power
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  _set_up_2d_led_array();

  clearDisplay();

  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  if(displayOn == 1)
  {
    if(clearOnNext == 1){
      clearDisplay();
      clearOnNext = 0;
    }

    horizSequentialPattern("E");
    horizSequentialPattern("E", 0, "up", -1, 0, (NUM_ROWS - 1));

    horizSequentialPattern("W");
    horizSequentialPattern("W", 0, "up", -1, (NUM_COLS - 1), (NUM_ROWS - 1));
    /*if(fxIdx == 0){
      horizPattern("E", 0, 0, "", NUM_COLS - 1);
    } else if (fxIdx == 1) {
      horizPattern("W", 0, 0, "", NUM_COLS - 1);
    } else if (fxIdx == 2) {
      horizPattern("E", 1, 0, "down");
    } else if (fxIdx == 3) {
      horizPattern("W", 1, 0, "down");
    } else if (fxIdx == 4) {
      horizPattern("E", 1, 0, "up");
    } else if (fxIdx == 5) {
      horizPattern("W", 1, 0, "up");
    } else if(fxIdx == 6){
      vertPattern("S", 0, 0, "", NUM_ROWS - 1);
    } else if (fxIdx == 7) {
      vertPattern("N", 0, 0, "", NUM_ROWS - 1);
    } else if (fxIdx == 8) {
      vertPattern("S", 1, 0, "right");
    } else if (fxIdx == 9) {
      vertPattern("N", 1, 0, "right");
    } else if (fxIdx == 10) {
      vertPattern("S", 1, 0, "left");
    } else if (fxIdx == 11) {
      vertPattern("N", 1, 0, "left");
    } else if (fxIdx == 12) {
      int random_strip = random(0, NUM_COLS);
      vertPattern("N", 0, 0, "right", NUM_ROWS - 1, random_strip, (NUM_ROWS - 1));
    } else if (fxIdx == 13) {
      bunchaRandoDots();
    }*/
    //TODO: Pong!!
  } else {
    clearDisplay();
  }
  Blynk.run();
}

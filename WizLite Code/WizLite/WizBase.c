// base methods are stored here... don't edit pls, unless u wanna break stuff 
#include <Arduino.h>
#include <WS2812.h>
#include <string.h>
#include "WizBase.h"


// 0-255. At only 40 to cap powerdraw,so that a USB port can handle it.
#define BRIGHTNESS 40

#define ROWS_PER_HALF 7
#define LEDS_PER_HALF (SCREEN_W * ROWS_PER_HALF)   // 63

#define TOP_PIN    33   // P3.3
#define BOTTOM_PIN 34   // P3.4

// LED buffer: 3 bytes (G, R, B) per LED, top half then bottom half.
static __xdata uint8_t ledData[SCREEN_W * SCREEN_H * 3];   // 378 bytes

void screenBegin(void) {
  pinMode(TOP_PIN, OUTPUT);
  pinMode(BOTTOM_PIN, OUTPUT);
  screenClear();
  screenShow();
}

// Applies brightness and stores the color at the pixels LED position.
void screenUpdate(int8_t x, int8_t y, uint8_t r, uint8_t g, uint8_t b) {
  if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H) return;

  // zigzag: the first row of each half runs right to left
  uint8_t row = y % ROWS_PER_HALF;
  uint8_t col = (row % 2 == 0) ? SCREEN_W - 1 - x : x;
  uint8_t led = (y / ROWS_PER_HALF) * LEDS_PER_HALF + row * SCREEN_W + col;

  r = ((uint16_t)r * BRIGHTNESS) >> 8;
  g = ((uint16_t)g * BRIGHTNESS) >> 8;
  b = ((uint16_t)b * BRIGHTNESS) >> 8;

  set_pixel_for_GRB_LED(ledData, led, r, g, b);
}

void screenClear(void) {
  memset(ledData, 0, sizeof(ledData));
}

void screenShow(void) {
  neopixel_show_P3_3(ledData, LEDS_PER_HALF * 3);
  neopixel_show_P3_4(ledData + LEDS_PER_HALF * 3, LEDS_PER_HALF * 3);
}

#define BUTTON_COUNT 8
#define DEBOUNCE_MS 10

static const uint8_t buttonPins[BUTTON_COUNT] = {
  14, 15, 16, 17,
  11, 30, 31, 32
};

static uint8_t stableState[BUTTON_COUNT];  
static uint8_t lastRaw[BUTTON_COUNT];
static uint8_t lastChange[BUTTON_COUNT];
static uint8_t justPressed[BUTTON_COUNT];

void buttonsBegin(void) {
  for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
    pinMode(buttonPins[i], INPUT);
  }
}

void buttonsUpdate(void) {
  uint8_t now = (uint8_t)millis();

  for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
    justPressed[i] = 0;

    uint8_t raw = (digitalRead(buttonPins[i]) == LOW); 

    if (raw != lastRaw[i]) {
      lastRaw[i] = raw;
      lastChange[i] = now;
    } else if (raw != stableState[i] && (uint8_t)(now - lastChange[i]) >= DEBOUNCE_MS) {
      stableState[i] = raw;
      if (raw) justPressed[i] = 1;
    }
  }
}

uint8_t buttonHeld(uint8_t button) {
  return stableState[button];
}

uint8_t buttonPressed(uint8_t button) {
  return justPressed[button];
}

uint8_t buttonsExitRequested(void) {
  static uint8_t timing = 0;
  static uint16_t startTime;

  if (stableState[P1_LEFT] && stableState[P1_RIGHT]) {
    if (!timing) {
      timing = 1;
      startTime = (uint16_t)millis();
    } else if ((uint16_t)((uint16_t)millis() - startTime) >= 2000) {
      return 1;
    }
  } else {
    timing = 0;
  }
  return 0;
}



// Each letter is 5 rows of 3 characters: #  = full brightness      o  = dim (gray)      .  = off
// The gray pixels make letters like K, M and W readable in 3 x 5. Check repo with figma sketch on it to see what I mean.
static __code const char font[36][16] = {
  ".#." "#.#" "###" "#.#" "#.#",  // A
  "##." "#.#" "##." "#.#" "##.",  // B
  ".##" "#.." "#.." "#.." ".##",  // C
  "##." "#.#" "#.#" "#.#" "##.",  // D
  "###" "#.." "##." "#.." "###",  // E
  "###" "#.." "##." "#.." "#..",  // F
  ".##" "#.." "#.#" "#.#" ".##",  // G
  "#.#" "#.#" "###" "#.#" "#.#",  // H
  "###" ".#." ".#." ".#." "###",  // I
  "..#" "..#" "..#" "#.#" ".#.",  // J
  "#.o" "#o." "##." "#o." "#.o",  // K
  "#.." "#.." "#.." "#.." "###",  // L
  "#o#" "###" "#.#" "#.#" "#.#",  // M
  "##o" "#.#" "#.#" "#.#" "#.#",  // N
  ".#." "#.#" "#.#" "#.#" ".#.",  // O
  "##." "#.#" "##o" "#.." "#..",  // P
  ".#." "#.#" "#.#" "#.o" ".##",  // Q
  "##." "#.#" "##o" "#o." "#.#",  // R
  ".##" "#.." ".#." "..#" "##.",  // S
  "###" ".#." ".#." ".#." ".#.",  // T
  "#.#" "#.#" "#.#" "#.#" "###",  // U
  "#.#" "#.#" "#.#" "o#o" ".#.",  // V
  "#.#" "#.#" "#o#" "###" "#.#",  // W
  "#.#" "o#o" ".#." "o#o" "#.#",  // X
  "#.#" "o#o" ".#." ".#." ".#.",  // Y
  "###" "..o" ".#." "o.." "###",  // Z
  "###" "#.#" "#.#" "#.#" "###",  // 0
  ".#." "o#." ".#." ".#." "###",  // 1
  "##." "..#" ".#." "#.." "###",  // 2
  "##." "..#" ".#." "..#" "##.",  // 3
  "#.#" "#.#" "###" "..#" "..#",  // 4
  "###" "#.." "##." "..#" "##.",  // 5
  ".##" "#.." "###" "#.#" "###",  // 6
  "###" "..#" ".#." ".#." ".#.",  // 7
  "###" "#.#" "###" "#.#" "###",  // 8
  "###" "#.#" "###" "..#" "##.",  // 9
};



void drawChar(char c, int8_t x, int8_t y, uint8_t r, uint8_t g, uint8_t b) {
  uint8_t index;

  if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';  

  if (c >= 'A' && c <= 'Z') {
    index = c - 'A';
  } else if (c >= '0' && c <= '9') {
    index = 26 + (c - '0');
  } else {
    return;
  }

  for (uint8_t row = 0; row < 5; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      char pixel = font[index][row * 3 + col];
      if (pixel == '#') {
        screenUpdate(x + col, y + row, r, g, b);
      } else if (pixel == 'o') {
        screenUpdate(x + col, y + row, r / 2, g / 2, b / 2);
      }
    }
  }
}

// Scrolls text across the screen like the micro:bit
void scrollText(const char *text, uint8_t r, uint8_t g, uint8_t b) {
  int len = strlen(text);
  int steps = SCREEN_W + len * 4;

  for (int step = 0; step < steps; step++) {
    screenClear();
    for (int i = 0; i < len; i++) {
      int x = SCREEN_W - step + i * 4;
      if (x >= SCREEN_W || x < -3) continue; 
      drawChar(text[i], x, 4, r, g, b);
    }
    screenShow();
    delay(SCROLL_DELAY_MS);
  }
}


//  PICTURES: A picture is made of two things (see Picture in WizBase.h):
//   colors - the list of colors (red, green, blue) this picture uses
//   pixels - 14 rows of 9 characters, one character per pixel
// Pixel characters:  '.' = off,  then 0-9, A-Z, a-z = color number 0, 1, 2 ... 61  from the colors list. 

static uint8_t pixelToColorNumber(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
  return c - 'a' + 36;
}

void drawPicture(__code const Picture *picture, int8_t xShift) {
  for (uint8_t y = 0; y < SCREEN_H; y++) {
    for (uint8_t x = 0; x < SCREEN_W; x++) {
      int8_t col = x + xShift; 
      if (col < 0 || col >= SCREEN_W) continue;

      char c = picture->pixels[y * SCREEN_W + col];
      if (c == '.') {
        screenUpdate(x, y, COLOR_OFF);
      } else {
        __code const Color *color = &picture->colors[pixelToColorNumber(c)];
        screenUpdate(x, y, color->r, color->g, color->b);
      }
    }
  }
}

void showPicture(__code const Picture *picture, uint16_t ms) {
  screenClear();
  drawPicture(picture, 0);
  screenShow();
  delay(ms);
}


//  Screens shared by all games (each game also keeps its own cover picture at the top of its own file, for example Snake.c) these are just the basic ones like start game, p1 win, etc.

__code const Color picGameStart_colors[] = { {255, 0, 0}, {0, 255, 0} };
__code const char picGameStart_pixels[] =
  "........."
  ".000.000."
  ".0...0.0."
  ".0.0.0.0."
  ".0.0.0.0."
  ".000.000."
  "........."
  "..1......"
  "..111...."
  "..11111.."
  "..111...."
  "..1......"
  "........."
  ".........";
__code const Picture picGameStart = { picGameStart_colors, picGameStart_pixels };

__code const Color picGameOver_colors[] = { {255, 255, 255} };
__code const char picGameOver_pixels[] =
  "........."
  "........."
  "........."
  "..00000.."
  ".0000000."
  ".0..0..0."
  ".0..0..0."
  ".0000000."
  "..00000.."
  "..0.0.0.."
  "........."
  "........."
  "........."
  ".........";
__code const Picture picGameOver = { picGameOver_colors, picGameOver_pixels };

__code const Color picOnePlayer_colors[] = { {255, 255, 255}, {128, 128, 128} };
__code const char picOnePlayer_pixels[] =
  "........."
  "........."
  "........."
  "........."
  "..0..00.."
  ".00..0.0."
  "..0..001."
  "..0..0..."
  ".000.0..."
  "........."
  "........."
  "........."
  "........."
  ".........";
__code const Picture picOnePlayer = { picOnePlayer_colors, picOnePlayer_pixels };

__code const Color picTwoPlayer_colors[] = { {255, 255, 255}, {128, 128, 128} };
__code const char picTwoPlayer_pixels[] =
  "........."
  "........."
  "........."
  "........."
  ".00..00.."
  "...0.0.0."
  "..0..001."
  ".0...0..."
  ".000.0..."
  "........."
  "........."
  "........."
  "........."
  ".........";
__code const Picture picTwoPlayer = { picTwoPlayer_colors, picTwoPlayer_pixels };

__code const Color picPongMock_colors[] = { {255, 255, 255}, {255, 0, 0} };
__code const char picPongMock_pixels[] =
  "...000..."
  "........."
  "........."
  "........."
  "........."
  "........."
  "........."
  "........."
  "1........"
  "........."
  "........."
  "........."
  "........."
  "...000...";
__code const Picture picPongMock = { picPongMock_colors, picPongMock_pixels };

__code const Color picP1Won_colors[] = { {255, 255, 255}, {128, 128, 128}, {255, 200, 0} };
__code const char picP1Won_pixels[] =
  "........."
  "........."
  ".00...0.."
  ".0.0.00.."
  ".001..0.."
  ".0....0.."
  ".0...000."
  "........."
  "........."
  ".2..2..2."
  ".2222222."
  ".2222222."
  "........."
  ".........";
__code const Picture picP1Won = { picP1Won_colors, picP1Won_pixels };

__code const Color picP2Won_colors[] = { {255, 255, 255}, {128, 128, 128}, {255, 200, 0} };
__code const char picP2Won_pixels[] =
  "........."
  "........."
  ".00..00.."
  ".0.0...0."
  ".001..0.."
  ".0...0..."
  ".0...000."
  "........."
  "........."
  ".2..2..2."
  ".2222222."
  ".2222222."
  "........."
  ".........";
__code const Picture picP2Won = { picP2Won_colors, picP2Won_pixels };

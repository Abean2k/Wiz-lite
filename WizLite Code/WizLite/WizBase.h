#ifndef WIZBASE_H
#define WIZBASE_H

#include <Arduino.h>
#include <stdint.h>

#define SCREEN_W 9
#define SCREEN_H 14

// Colors are red, green, blue (0-255 each). These names expand to three values: screenUpdate(x, y, COLOR_RED)
#define COLOR_OFF        0, 0, 0
#define COLOR_WHITE      255, 255, 255
#define COLOR_GRAY       128, 128, 128
#define COLOR_RED        255, 0, 0
#define COLOR_GREEN      0, 255, 0
#define COLOR_DARK_GREEN 0, 100, 0
#define COLOR_BLUE       0, 0, 255
#define COLOR_YELLOW     255, 200, 0

enum {
  P1_UP, P1_DOWN, P1_LEFT, P1_RIGHT,
  P2_UP, P2_DOWN, P2_LEFT, P2_RIGHT
};

void buttonsBegin(void);
void buttonsUpdate(void);    // call this often (every loop)
uint8_t buttonHeld(uint8_t button);  // 1 while the button is down
uint8_t buttonPressed(uint8_t button);   // 1 only on the update where it was first pressed
uint8_t buttonsExitRequested(void);   // 1 after P1 LEFT + RIGHT held for 2 seconds


void screenBegin(void);
void screenUpdate(int8_t x, int8_t y, uint8_t r, uint8_t g, uint8_t b);   // (0,0) is top left
void screenClear(void);
void screenShow(void);

#define SCROLL_DELAY_MS 80   // time per column when scrolling, lower = faster

void drawChar(char c, int8_t x, int8_t y, uint8_t r, uint8_t g, uint8_t b);   // 3 wide x 5 tall, A-Z and 0-9
void scrollText(const char *text, uint8_t r, uint8_t g, uint8_t b);      // micro:bit style 

//Full screen pictures
// A picture has its own list of colors plus 9 x 14 pixel characters ('.' = off, 0-9 A-Z a-z = color number 0-61 from the list).
// Make them with PictureMaker.html, it writes this code for you.

typedef struct {
  uint8_t r, g, b;
} Color;

typedef struct {
  __code const Color *colors;
  __code const char *pixels;
} Picture;

// Screens shared by every game. Each game keeps its own cover picture in its own file.
extern __code const Picture picGameStart;
extern __code const Picture picGameOver;
extern __code const Picture picOnePlayer;
extern __code const Picture picTwoPlayer;
extern __code const Picture picPongMock;
extern __code const Picture picP1Won;
extern __code const Picture picP2Won;

void drawPicture(__code const Picture *picture, int8_t xShift);
void showPicture(__code const Picture *picture, uint16_t ms); 

#endif

#include <Arduino.h>
#include <stdlib.h>
#include <string.h>
#include "WizBase.h"
#include "Snake.h"


//  Cover picture (shown in the game carousel) -- Made with PictureMaker.html, to change this just go to my repo and find the html file and make your edits :)

__code const Color picSnakeCover_colors[] = { {255, 255, 255}, {79, 178, 93}, {102, 213, 117}, {30, 30, 30}, {242, 72, 34} };
__code const char picSnakeCover_pixels[] =
  "000000000"
  "001111000"
  "012222100"
  "121322210"
  "121122110"
  "122221440"
  "122110044"
  "012100000"
  "012210000"
  "001221100"
  "000122210"
  "000012210"
  "000122210"
  "011222100";
__code const Picture picSnakeCover = { picSnakeCover_colors, picSnakeCover_pixels };

#define MAX_LENGTH (SCREEN_W * SCREEN_H)

#define START_DELAY 350   
#define MIN_DELAY   120 

enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };

// Each snake segment is stored as ONE byte: position = y * 9 + x. body[0] is the head. This keeps the whole snake to 126 bytes max. Note: not my idea, came from some github project I saw a long time ago...
static __xdata uint8_t body[MAX_LENGTH];
static uint8_t length;
static uint8_t food;
static uint8_t score;

static uint8_t onSnake(uint8_t position) {
  for (uint8_t i = 0; i < length; i++) {
    if (body[i] == position) return 1;
  }
  return 0;
}

static void placeFood(void) {
  do {
    food = rand() % MAX_LENGTH;
  } while (onSnake(food));
}

static void drawGame(void) {
  screenClear();

  screenUpdate(food % SCREEN_W, food / SCREEN_W, COLOR_RED);

  for (uint8_t i = 1; i < length; i++) {
    screenUpdate(body[i] % SCREEN_W, body[i] / SCREEN_W, COLOR_DARK_GREEN);
  }
  screenUpdate(body[0] % SCREEN_W, body[0] / SCREEN_W, COLOR_GREEN);

  screenShow();
}


static uint8_t waitForStep(uint8_t currentDir, uint8_t *nextDir, uint16_t ms) {
  unsigned long start = millis();

  while (millis() - start < ms) {
    buttonsUpdate();
    if (buttonsExitRequested()) return 1;

    // Can't turn straight back into yourself
    if (buttonPressed(P1_UP)    && currentDir != DIR_DOWN)  *nextDir = DIR_UP;
    if (buttonPressed(P1_DOWN)  && currentDir != DIR_UP)    *nextDir = DIR_DOWN;
    if (buttonPressed(P1_LEFT)  && currentDir != DIR_RIGHT) *nextDir = DIR_LEFT;
    if (buttonPressed(P1_RIGHT) && currentDir != DIR_LEFT)  *nextDir = DIR_RIGHT;
  }
  return 0;
}

static void showScore(void) {
  char message[12];
  uint8_t n = 6;

  strcpy(message, "SCORE ");
  if (score >= 100) message[n++] = '0' + score / 100;
  if (score >= 10)  message[n++] = '0' + (score / 10) % 10;
  message[n++] = '0' + score % 10;
  message[n] = 0;

  scrollText(message, COLOR_WHITE);
}

void playSnake(void) {
  showPicture(&picOnePlayer, 1000);
  showPicture(&picGameStart, 1000);

  length = 3;
  body[0] = 10 * SCREEN_W + 4;
  body[1] = 11 * SCREEN_W + 4;
  body[2] = 12 * SCREEN_W + 4;
  score = 0;

  uint8_t dir = DIR_UP;
  uint8_t nextDir = DIR_UP;
  uint16_t stepDelay = START_DELAY;

  placeFood();
  drawGame();

  while (1) {
    if (waitForStep(dir, &nextDir, stepDelay)) return;
    dir = nextDir;

    // Work out where the head goes next
    int8_t x = body[0] % SCREEN_W;
    int8_t y = body[0] / SCREEN_W;
    if (dir == DIR_UP)    y--;
    if (dir == DIR_DOWN)  y++;
    if (dir == DIR_LEFT)  x--;
    if (dir == DIR_RIGHT) x++;

    if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H) break;

    uint8_t newHead = y * SCREEN_W + x;

    if (onSnake(newHead)) break;

    uint8_t ate = (newHead == food);
    if (ate) length++;

    for (uint8_t i = length - 1; i > 0; i--) {
      body[i] = body[i - 1];
    }
    body[0] = newHead;

    if (ate) {
      score++;
      if (length >= MAX_LENGTH) break;   
      placeFood();
      if (stepDelay > MIN_DELAY) stepDelay -= 10;
    }

    drawGame();
  }

  showPicture(&picGameOver, 2000);
  showScore();
}

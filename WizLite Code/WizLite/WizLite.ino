// WizGameLite.ino - game carousel menu
// LEFT / RIGHT = pan between games      UP = start the selected game
// In a game: hold P1 LEFT + RIGHT for 2 seconds to return here.

#include <stdlib.h>
#include "WizBase.h"
#include "Snake.h"
// STEP 2 of adding a game: #include its header here, for example:
// #include "MyGame.h"

typedef struct {
  __code const Picture *cover;   // picture shown in the carousel
  void (*play)(void);            // function that runs the game
} Game;

//  HOW TO ADD A GAME:
//  1. Make two new files next to this one, named with .c (NOT .cpp):
//       MyGame.h   declares the cover picture and the play function:
//                    #include "WizBase.h"
//                    extern __code const Picture picMyGameCover;
//                    void playMyGame(void);
//       MyGame.c   holds the cover picture at the TOP of the file (make it with PictureMaker.html), then the game itself:
//                    void playMyGame(void) { ... }
//       Inside playMyGame(): call buttonsUpdate() often to check for the button presses, and return when the game ends or when buttonsExitRequested() is 1. 
//       Look at Snake.c for a complete example.
//  2. Add  #include "MyGame.h"  in the include list above.
//  3. Add one line to the games[] list below: { &picMyGameCover, playMyGame }

static __code const Game games[] = {
  { &picSnakeCover, playSnake },
  // { &picMyGameCover, playMyGame },
};
#define GAME_COUNT (sizeof(games) / sizeof(games[0]))

static uint8_t selected = 0;
static uint8_t needRedraw = 1;

static void slideTo(uint8_t next, int8_t direction) {
  for (int8_t s = 1; s <= SCREEN_W; s++) {
    screenClear();
    drawPicture(games[selected].cover, direction * s);
    drawPicture(games[next].cover, direction * s - direction * SCREEN_W);
    screenShow();
    delay(20);
  }
  selected = next;
}

void setup() {
  screenBegin();
  buttonsBegin();
  scrollText("WIZ GAME", COLOR_WHITE);
}

void loop() {
  if (needRedraw) {
    screenClear();
    drawPicture(games[selected].cover, 0);
    screenShow();
    needRedraw = 0;
  }

  buttonsUpdate();

  if (GAME_COUNT > 1) {
    if (buttonPressed(P1_RIGHT)) {
      slideTo((selected + 1) % GAME_COUNT, 1);
    }
    if (buttonPressed(P1_LEFT)) {
      slideTo((selected + GAME_COUNT - 1) % GAME_COUNT, -1);
    }
  }

  if (buttonPressed(P1_UP)) {
    srand(millis());
    games[selected].play();
    needRedraw = 1;
  }
}

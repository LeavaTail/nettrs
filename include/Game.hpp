#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <stack>
#include <algorithm>
#include <random>
#include "Block.hpp"

#ifdef DEBUG
#define debug_game(str)                           \
  std::cout << "(" << __FILE__ << ":"             \
            << std::to_string(__LINE__) << "): "  \
            << __func__ << ":"                    \
            << str << std::endl;
#else
#define debug_game(str) do { } while (0)
#endif

#define move_block(position, x_diff, y_diff)                    \
  do {                                                          \
    bool status = false;                                        \
    for (int i = 0; i < BLOCK_COUNT && !status; i++) {          \
      int x = block -> get_block_x(i);                          \
      int y = block -> get_block_y(i);                          \
      status = stage -> get_grid_status(x + x_diff, y + y_diff);\
    }                                                           \
    block -> move_block_##position(status);                     \
  } while (0)

#define SINGLELINE_SCORE  40
#define DOUBLELINE_SCORE  100
#define TRIPLELINE_SCORE  300
#define TETRISLINE_SCORE  1200

enum GameStatus {
  TITLE,
  RUNNING,
  PAUSING,
  GAMEOVER,
};

class Game{
  public:
    SDL_Window *window;
    SDL_Renderer *renderer;

    Game();
    ~Game();

    void init(const char *title, int xpos, int ypos, int width, int height,
              bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();

    Btype get_blocktype();
    bool runnig() {
      return isRunning;
    }
    int get_status() {
      return gamestatus;
    }

  private:
    /* FIXME: Using gamestatus instead of isRunning */
    bool isRunning;
    Uint32  gamescore;
    GameStatus gamestatus;
    Uint32 gravity;
    Uint32 BlockStart;

    int calc_score(int linenums, bool isTspin);
};



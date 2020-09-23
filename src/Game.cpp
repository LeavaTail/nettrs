#include "Game.hpp"
#include "Block.hpp"
#include "Stage.hpp"

Block *block = nullptr;
Stage *stage = nullptr;
std::stack<Btype> blocktype;

Game::Game() {}
Game::~Game() {}

void Game::init(const char *title, int xpos, int ypos, int width, int height,
                bool fullscreen) {
  int flags = 0;
  Btype type = get_blocktype();
  if (fullscreen)
    flags = SDL_WINDOW_FULLSCREEN;

  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    debug_game("Subsystem initalized");
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

    if (window) {
      debug_game("Window Created ");
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      debug_game("Renderer Created ");
    }

    isRunning = true;
    gamestatus = RUNNING;
    stage = new Stage();
    gravity = STAGE_GRAVITY;
    gamescore = 0;
    block = new Block(type, renderer);
  }
  else {
    isRunning = false;
  }
}

void Game::handleEvents() {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type){
    case SDL_QUIT: {
      isRunning = false;
      break;
    }
    case SDL_KEYDOWN: {
      switch (event.key.keysym.sym) {
        //move block to right
        case SDLK_d:
        case SDLK_RIGHT: {
          move_block(right, BLOCK_WIDTH, 0);
          break;
        }
        //move block to left
        case SDLK_a:
        case SDLK_LEFT: {
          move_block(left, - BLOCK_WIDTH, 0);
          break;
        }
        //move block down fast
        case SDLK_s:
        case SDLK_DOWN: {
          BlockStart = SDL_GetTicks();
          move_block(downfast, 0, BLOCK_HEIGHT);
          break;
        }
        //move block down fast
        case SDLK_w:
        case SDLK_UP: {
          while (!block -> Dead()) {
            BlockStart = SDL_GetTicks();
            move_block(downfast, 0, BLOCK_HEIGHT);
          }
          break;
        }
        // rotate block
        case SDLK_z: {
          bool status = false;
          int x_axis = block -> get_block_x();
          int y_axis = block -> get_block_y();
          for (int i = 1; i < BLOCK_COUNT && !status; i++) {
            int x = block -> get_block_x(i);
            int y = block -> get_block_y(i);
            status = stage -> get_grid_status(x_axis - y_axis + y, y_axis + x_axis - x);
          }
          block -> rotate_block_left(status);
          break;
        }
        case SDLK_x: {
          bool status = false;
          int x_axis = block -> get_block_x();
          int y_axis = block -> get_block_y();
          for (int i = 1; i < BLOCK_COUNT && !status; i++) {
            int x = block -> get_block_x(i);
            int y = block -> get_block_y(i);
            status = stage -> get_grid_status(x_axis - y_axis + y, y_axis + x_axis - x);
          }
          block -> rotate_block_right(status);
          break;
        }
        //do nothing
        default: {
          break;
        }
      }
    }
    default : {
      // do nothing
      break;
    }
  }
}

void Game::update(){
  bool occupied = false;
  //check gameover
  for (int i = 0; i < BLOCK_COUNT && !occupied; i++) {
    int x = block -> get_block_x(i);
    int y = block -> get_block_y(i);
    occupied = stage -> get_grid_status(x, y);
  }
  if (occupied) {
    debug("GAME OVER");
    gamestatus = GAMEOVER;
    return;
  }
  //check if block is falling or stopped
  if (block -> Dead()) {
    //change grid color if occupied
    for (int i = 0; i < BLOCK_COUNT; i++) {
      stage -> update_grid_status(block -> get_block_x(i),
                                  block -> get_block_y(i));
      stage -> copy_block_color(block);
    }
    delete block; //destroy block instance
    //check if row is occupied
    Uint32 lnums = 0;
    int y = block -> get_block_y();
    int radius = BLOCK_HEIGHT * (BLOCK_COUNT / 2);
    int upper = std::max(0, y - radius);
    int lower = std::min(STAGE_HEIGHT - BLOCK_HEIGHT, y + radius);
    for (int row = lower; row >= upper; row -= BLOCK_HEIGHT) {
      if (stage -> check_row(row)) {
        //move stage color to downwards
        stage -> move_stagecolor_down(row);
        row += BLOCK_HEIGHT;
        lnums++;
      }
    }
    if (lnums) {
      calc_score(lnums, false);
      //FIXME: LevelUP routine.
      gravity /= 2;
    }
    // make new block to fall
    Btype type = get_blocktype();
    block = new Block(type, renderer);
    BlockStart = SDL_GetTicks();
  } else {
    if ((SDL_GetTicks() - BlockStart) > gravity) {
      BlockStart = SDL_GetTicks();
      move_block(downfast, 0, BLOCK_HEIGHT);
    }
  }
}

void Game::render() {
  SDL_RenderClear(renderer);
  //render copy block
  block -> render_block(renderer);
  //render copy stage
  stage -> render_stage(renderer);
  SDL_RenderPresent(renderer);
}

void Game::clean() {
  debug_game("Game::clean()");
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  debug_game("Game Ended ....");
}

Btype Game::get_blocktype() {
  Btype btmp;
  if (!blocktype.size()) {
    Btype tmp[]{
      BLOCK_I,
      BLOCK_J,
      BLOCK_L,
      BLOCK_O,
      BLOCK_Z,
      BLOCK_S,
      BLOCK_T,
      BLOCK_SI,
      BLOCK_SL,
    };
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::shuffle(tmp, tmp + BLOCK_MAX, engine);
    for(int i = 0; i < BLOCK_MAX; i++)
      blocktype.push(tmp[i]);
  }
  btmp = blocktype.top();
  blocktype.pop();
  return btmp;
}

int Game::calc_score(int linenums, bool isTspin) {
  int err = 0;
  int bonus = 1;
  if (isTspin)
    bonus++;

  switch (linenums) {
    case 1:
      gamescore += (SINGLELINE_SCORE * bonus);
      break;
    case 2:
      gamescore += (DOUBLELINE_SCORE * bonus);
      break;
    case 3:
      gamescore += (TRIPLELINE_SCORE * bonus);
      break;
    case 4:
      gamescore += (TETRISLINE_SCORE * bonus);
      break;
    default:
      err = linenums;
  }

  std::cout << "score:" << gamescore << std::endl;
  return err;
}

#include "Stage.hpp"
#include "Block.hpp"
#include "TextureManager.hpp"

#include <iostream>

Stage::Stage() {
  debug("Stage () ");
  memset(grid, 0, sizeof(grid));
  memset(grid_color, 0, sizeof(grid_color));
  memset(grid_block, 0, sizeof(grid_block));
}

Stage::~Stage() {
 debug("~Stage() ");
}

void Stage::render_stage(SDL_Renderer *ren){
  int i = 0;
  int j = 0;
  for (i = 0; i < STAGE_WIDTH; i += BLOCK_WIDTH) {
    for (j = 0; j < STAGE_HEIGHT; j += BLOCK_HEIGHT) {
      if (grid[i][j])
        SDL_RenderCopy(ren, grid_color[i][j], NULL, &grid_block[i][j]);
    }
  }
}

void Stage::move_stagecolor_down(int row) {
  int x = 0;
  int y = 0;
  for (y = row; y >= 0; y -= BLOCK_HEIGHT) {
    for (x = 0; x < STAGE_WIDTH; x += BLOCK_WIDTH) {
      grid[x][y]       = grid[x][y - BLOCK_HEIGHT];
      grid_color[x][y] = grid_color[x][y - BLOCK_HEIGHT];
    }
  }
}

bool Stage::check_row(int row) {
  int i = 0;
  for (i = 0; i < STAGE_WIDTH; i += BLOCK_WIDTH) {
    if (!grid[i][row])
      return false;
  }
  return true;
}

void Stage::update_grid_status(int x, int y) {
  grid[x][y] = true;
}

void Stage::copy_block_color(Block *block) {
  for (int i = 0; i < BLOCK_COUNT; i++) {
    int x = block -> get_block_x(i);
    int y = block -> get_block_y(i);
    if (grid[x][y]) {
      grid_color[x][y] = block -> get_block_texture();
      grid_block[x][y] = block -> get_block_Rect(i);
    }
  }
}

bool Stage::get_grid_status(int x, int y) {
  if (x < 0 || x >= STAGE_WIDTH) return true;
  if (y >= STAGE_HEIGHT) return true;
  return grid[x][y];
}


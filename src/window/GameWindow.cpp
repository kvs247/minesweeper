#include <GameWindow.hpp>
#include <SDL.h>
#include <SpriteFactory.hpp>
#include <config.hpp>
#include <cstdint>
#include <iostream>

GameWindow::~GameWindow()
{
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void GameWindow::init(uint32_t *fb)
{
  WIDTH = config::WINDOW_PIXEL_WIDTH;
  HEIGHT = config::WINDOW_PIXEL_HEIGHT;

  window = SDL_CreateWindow(
      "Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

  if (window == nullptr)
  {
    std::cerr << "error creating game window: " << SDL_GetError() << std::endl;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (renderer == nullptr)
  {
    std::cerr << "error creating game window renderer: " << SDL_GetError() << std::endl;
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

  if (texture == nullptr)
  {
    std::cerr << "error creating game window texture" << SDL_GetError() << std::endl;
  }

  windowID = SDL_GetWindowID(window);

  frameBuffer = fb;
};

void GameWindow::updateFrameBuffer(Minesweeper &game)
{
  updateInterface(game);
  updateGameArea(game);
};

void GameWindow::updateInterface(Minesweeper &game)
{
  // remaining flags
  SpriteFactory::buffInsertRemainingFlags(
      frameBuffer,
      WIDTH,
      config::REMAINING_FLAGS_X,
      config::REMAINING_FLAGS_Y,
      config::INFO_PANEL_BUTTONS_HEIGHT * 2,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      game.getRemainingFlags());

  // reset button
  const auto resetButtonSprite = game.getIsResetButtonPressed() ? sprites->pressedButton : sprites->raisedButton;
  SpriteFactory::copySprite(
      frameBuffer, resetButtonSprite, config::INFO_PANEL_BUTTONS_HEIGHT, config::RESET_BUTTON_X, config::RESET_BUTTON_Y);

  // config button
  const auto configButtonSprite = game.getIsConfigButtonPressed() ? sprites->pressedButton : sprites->raisedButton;
  SpriteFactory::copySprite(
      frameBuffer,
      configButtonSprite,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::CONFIG_BUTTON_X,
      config::CONFIG_BUTTON_Y);

  // timer
  SpriteFactory::buffInsertRemainingFlags(
      frameBuffer,
      WIDTH,
      config::TIMER_X,
      config::TIMER_Y,
      config::INFO_PANEL_BUTTONS_HEIGHT * 2,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      game.getSecondsElapsed());
}

void GameWindow::updateGameArea(Minesweeper &game)
{
  for (int row = 0; row < config::GRID_HEIGHT; ++row)
  {
    for (int col = 0; col < config::GRID_WIDTH; ++col)
    {
      const int gameAreaX = config::FRAME_WIDTH;
      const int gameAreaY = config::INFO_PANEL_HEIGHT + 2 * config::FRAME_WIDTH;

      const int x = gameAreaX + config::GRID_AREA_X_PAD + col * config::CELL_PIXEL_SIZE;
      const int y = gameAreaY + config::GRID_AREA_Y_PAD + row * config::CELL_PIXEL_SIZE;

      const int index = row * config::GRID_WIDTH + col;
      const auto &[isMine, isHidden, isFlagged, nAdjacentMines] = game.getMinefield()[index];
      uint32_t *sprite;

      if (isHidden)
      {
        sprite = sprites->hidden;

        if (isFlagged)
        {
          sprite = sprites->flag;
        }
      }
      else
      {
        if (isMine)
        {
          sprite = sprites->mine;
        }
        else
        {
          sprite = sprites->intToSpriteMap[nAdjacentMines];
        }
      }

      SpriteFactory::copySprite(frameBuffer, sprite, config::CELL_PIXEL_SIZE, x, y);
    }
  }
};
#include <GameWindow.hpp>
#include <SDL2/SDL.h>
#include <SpriteFactory.hpp>
#include <config.hpp>
#include <cstdint>
#include <cstring>
#include <iostream>

GameWindow::GameWindow()
{
  frameBuffer = std::make_unique<uint32_t[]>(config::WINDOW_PIXEL_WIDTH * config::WINDOW_PIXEL_HEIGHT);
  if (!frameBuffer)
  {
    std::cerr << "error allocating frame buffer" << std::endl;
  }

  SpriteFactory::buffInsertInterface(
      frameBuffer.get(), config::WINDOW_PIXEL_WIDTH, config::WINDOW_PIXEL_WIDTH * config::WINDOW_PIXEL_HEIGHT);
}

void GameWindow::init()
{
  SDL_Rect bounds;
  SDL_GetDisplayBounds(0, &bounds);

  const int leftPad = (config::DISPLAY_PIXEL_WIDTH - config::WINDOW_PIXEL_WIDTH) / 2;
  const int topPad = (config::DISPLAY_PIXEL_HEIGHT - config::WINDOW_PIXEL_HEIGHT) / 2;

  const int windowX = bounds.x + leftPad;
  const int windowY = bounds.y + topPad;

  window = SDL_CreateWindow(
      "Minesweeper", windowX, windowY, config::WINDOW_PIXEL_WIDTH, config::WINDOW_PIXEL_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_SetWindowPosition(window, windowX, windowY);
  SDL_ShowWindow(window);

  if (window == nullptr)
  {
    std::cerr << "error creating game window: " << SDL_GetError() << std::endl;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (renderer == nullptr)
  {
    std::cerr << "error creating game window renderer: " << SDL_GetError() << std::endl;
  }

  texture = SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_RGBA8888,
      SDL_TEXTUREACCESS_STREAMING,
      config::WINDOW_PIXEL_WIDTH,
      config::WINDOW_PIXEL_HEIGHT);

  if (texture == nullptr)
  {
    std::cerr << "error creating game window texture" << SDL_GetError() << std::endl;
  }

  windowID = SDL_GetWindowID(window);
};

void GameWindow::update(Minesweeper &game)
{
  updateInterface(game);
  updateGameArea(game);

  void *pixels;
  int pitch;

  SDL_LockTexture(texture, nullptr, &pixels, &pitch);
  std::memcpy(pixels, frameBuffer.get(), config::WINDOW_PIXEL_WIDTH * config::WINDOW_PIXEL_HEIGHT * sizeof(uint32_t));

  SDL_UnlockTexture(texture);
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
};

void GameWindow::updateInterface(Minesweeper &game)
{
  // remaining flags
  SpriteFactory::buffInsertRemainingFlags(
      frameBuffer.get(),
      config::WINDOW_PIXEL_WIDTH,
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
      frameBuffer.get(),
      config::WINDOW_PIXEL_WIDTH,
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
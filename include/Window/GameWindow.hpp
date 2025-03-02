#pragma once

#include <Minesweeper.hpp>
#include <SDL2/SDL.h>
#include <config.hpp>
#include <cstdint>
#include <memory>
#include <vector>

#include "Window.hpp"

class GameWindow : public Window
{
public:
  GameWindow();
  ~GameWindow() override = default;

  void init() override;
  void update(Minesweeper &) override;
  void handleEvent(SDL_Event &event, Minesweeper &gameState, bool &isGameLoopRunning);

private:
  const int gameAreaX = config::GRID_AREA_X_PAD + config::FRAME_WIDTH;
  const int gameAreaY = config::GRID_AREA_Y_PAD + config::INFO_PANEL_HEIGHT + 2 * config::FRAME_WIDTH;
};
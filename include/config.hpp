#pragma once

#include <SDL2/SDL.h>
#include <cstdint>

namespace config
{
inline const char *APP_NAME = "Minesweeper";

constexpr uint32_t BLACK = 0x000000ff;
constexpr uint32_t BLUE = 0x0000ffff;
constexpr uint32_t DARK_BLUE = 0x01007aff;
constexpr uint32_t DARK_GREY = 0x7a7a7bff;
constexpr uint32_t DARK_RED = 0x840101ff;
constexpr uint32_t GREEN = 0x017a00ff;
constexpr uint32_t GREY = 0xbdbdbdff;
constexpr uint32_t LIGHT_GREY = 0xfefffeff;
constexpr uint32_t PURPLE = 0x850085ff;
constexpr uint32_t RED = 0xff0000ff;
constexpr uint32_t TURQUOISE = 0x018384ff;

// info panel
constexpr int FRAME_WIDTH = 20;
constexpr int INFO_PANEL_HEIGHT = 70;
constexpr int INFO_PANEL_BUTTONS_HEIGHT = 0.75 * INFO_PANEL_HEIGHT;

// game area cells
constexpr int CELL_PIXEL_SIZE = 50;
constexpr int CELL_BORDER_WIDTH_3D = CELL_PIXEL_SIZE / 10;
constexpr int CELL_BORDER_WIDTH_2D = 2; // even int

constexpr double GAME_WINDOW_TO_DISPLAY_RATIO = 0.7;

inline int DISPLAY_PIXEL_WIDTH;
inline int DISPLAY_PIXEL_HEIGHT;

inline int WINDOW_PIXEL_WIDTH;
inline int WINDOW_PIXEL_HEIGHT;

inline int CONFIG_WINDOW_PIXEL_WIDTH;
inline int CONFIG_WINDOW_PIXEL_HEIGHT;
inline int RESET_BUTTON_X;
inline int RESET_BUTTON_Y;
inline int REMAINING_FLAGS_X;
inline int REMAINING_FLAGS_Y;
inline int REMAINING_FLAGS_PAD;
inline int TIMER_X;
inline int TIMER_Y;
inline int CONFIG_BUTTON_X;
inline int CONFIG_BUTTON_Y;
inline int GAME_AREA_WIDTH;
inline int GAME_AREA_HEIGHT;
inline int GRID_WIDTH;
inline int GRID_HEIGHT;
inline int GRID_AREA_X_PAD;
inline int GRID_AREA_Y_PAD;

inline void update()
{
  CONFIG_WINDOW_PIXEL_WIDTH = WINDOW_PIXEL_HEIGHT / 2;
  CONFIG_WINDOW_PIXEL_HEIGHT = WINDOW_PIXEL_HEIGHT / 2;
  RESET_BUTTON_X = config::WINDOW_PIXEL_WIDTH / 2 - config::INFO_PANEL_BUTTONS_HEIGHT / 2;
  RESET_BUTTON_Y = config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT / 2 - config::INFO_PANEL_BUTTONS_HEIGHT / 2;
  REMAINING_FLAGS_X = 2 * config::FRAME_WIDTH;
  REMAINING_FLAGS_Y = config::FRAME_WIDTH + config::INFO_PANEL_HEIGHT / 2 - config::INFO_PANEL_BUTTONS_HEIGHT / 2;
  REMAINING_FLAGS_PAD = 0.05 * INFO_PANEL_HEIGHT;
  TIMER_X = WINDOW_PIXEL_WIDTH - REMAINING_FLAGS_X - 2 * config::INFO_PANEL_BUTTONS_HEIGHT;
  TIMER_Y = REMAINING_FLAGS_Y;
  CONFIG_BUTTON_X = TIMER_X - INFO_PANEL_BUTTONS_HEIGHT * 2;
  CONFIG_BUTTON_Y = RESET_BUTTON_Y;
  GAME_AREA_WIDTH = WINDOW_PIXEL_WIDTH - 2 * FRAME_WIDTH;
  GAME_AREA_HEIGHT = WINDOW_PIXEL_HEIGHT - INFO_PANEL_HEIGHT - 3 * FRAME_WIDTH;
  GRID_WIDTH = GAME_AREA_WIDTH / CELL_PIXEL_SIZE;
  GRID_HEIGHT = GAME_AREA_HEIGHT / CELL_PIXEL_SIZE;
  GRID_AREA_X_PAD = (GAME_AREA_WIDTH % CELL_PIXEL_SIZE) / 2;
  GRID_AREA_Y_PAD = (GAME_AREA_HEIGHT % CELL_PIXEL_SIZE) / 2;
}

inline const auto initialize = []()
{
  update();
  return 0;
};
} // namespace config
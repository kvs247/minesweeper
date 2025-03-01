#pragma once

#include <config.hpp>
#include <cstdint>
#include <vector>

#include "BaseArtist.hpp"

class MinefieldElementArtist : public BaseArtist
{
public:
  static int NUMERIC_SPRITE_HEIGHT;
  static int NUMERIC_SPRITE_WIDTH;
  static int NUMERIC_SPRITE_PAD;

  static void drawFlaggedCellSprite(std::vector<uint32_t> &buff, const int width);
  static void drawMineCellSprite(std::vector<uint32_t> &buff, const int width);
  static void drawMineCellRedXSprite(std::vector<uint32_t> &buff, const int width);
  static void drawClickedMineCellSprite(std::vector<uint32_t> &buff, const int width);
  static void drawNumericSprite(std::vector<uint32_t> &buff, const int width, const int n, const uint32_t c);

  static void drawMineCell(std::vector<uint32_t> &buff, const int width);
  static void drawClickedMineCell(std::vector<uint32_t> &buff, const int width);

  static void draw2DCellBase(std::vector<uint32_t> &buff, const int width);
  static void draw3DCellBase(std::vector<uint32_t> &buff, const int width);

  static void drawMine(std::vector<uint32_t> &buff, const int width);
  static void drawFlag(std::vector<uint32_t> &buff, const int width);
  static void drawOne(std::vector<uint32_t> &buff, const int width);
};

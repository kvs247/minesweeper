#include <BufferCompositor.hpp>
#include <FaceCompositor.hpp>
#include <MinefieldElementCompositor.hpp>
#include <SpriteFactory.hpp>
#include <cmath>
#include <config.hpp>
#include <memory>
#include <stdexcept>

SpriteFactory::SpriteFactory(Sprites *spriteObjs) : sprites(spriteObjs)
{
  makeRaisedResetButtonSprite();
  makePressedResetButtonSprite();
  makeRaisedButtonSprite();
  makePressedButtonSprite();
  makeWinnerResetButtonSprite();
  makeLoserResetButtonSprite();

  makeEmptyCellSprite();
  makeHiddenCellSprite();
  makeFlaggedCellSprite();
  makeMineCellSprite();
  makeClickedMineCellSprite();
  makeMineCellWithRedXSprite();

  makeOneSprite();
  makeNumericSprite(sprites->two, 2, config::GREEN);
  makeNumericSprite(sprites->three, 3, config::RED);
  makeNumericSprite(sprites->four, 4, config::DARK_BLUE);
  makeNumericSprite(sprites->five, 5, config::DARK_RED);
  makeNumericSprite(sprites->six, 6, config::TURQUOISE);
  makeNumericSprite(sprites->seven, 7, config::PURPLE);
  makeNumericSprite(sprites->eight, 8, config::DARK_GREY);

  sprites->intToSpriteMap = {
      {0, sprites->empty},
      {1, sprites->one},
      {2, sprites->two},
      {3, sprites->three},
      {4, sprites->four},
      {5, sprites->five},
      {6, sprites->six},
      {7, sprites->seven},
      {8, sprites->eight},
  };
}

std::unique_ptr<SpriteFactory::Sprites> SpriteFactory::createSprites()
{
  auto spriteObjs = std::make_unique<Sprites>();

  SpriteFactory spriteFactory(spriteObjs.get());

  return spriteObjs;
}

void SpriteFactory::copySprite(uint32_t *buff, const uint32_t *sprite, const int spriteWidth, const int x, const int y)
{
  for (int row = 0; row < spriteWidth; ++row)
  {
    const auto sourceRow = sprite + row * spriteWidth;
    const auto sourceRowEnd = sprite + row * spriteWidth + spriteWidth;
    const auto destinationRow = buff + rowColToWindowIndex(row + y, x);
    std::copy(sourceRow, sourceRowEnd, destinationRow);
  }
}

void SpriteFactory::copySprite(
    std::unique_ptr<uint32_t[]> &buff,
    const uint32_t *sprite,
    const int spriteWidth,
    const int x,
    const int y)
{
  copySprite(buff.get(), sprite, spriteWidth, x, y);
}

// private:

void SpriteFactory::makeRaisedResetButtonSprite()
{
  auto &buff = sprites->raisedResetButton;
  BufferCompositor::buffInsertRectangle(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::GREY);
  BufferCompositor::buffInsert3DBorder(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);
  FaceCompositor::buffInsertFaceBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
  FaceCompositor::buffInsertFaceSmile(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
  FaceCompositor::buffInsertFaceAliveEyes(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
};

void SpriteFactory::makePressedResetButtonSprite()
{
  auto &buff = sprites->pressedResetButton;
  BufferCompositor::buffInsertRectangle(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::GREY);
  BufferCompositor::buffInsert2DBorder(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::DARK_GREY);
  FaceCompositor::buffInsertFaceBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
  FaceCompositor::buffInsertFaceSmile(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
  FaceCompositor::buffInsertFaceAliveEyes(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
};

void SpriteFactory::makeRaisedButtonSprite()
{
  auto &buff = sprites->raisedButton;
  BufferCompositor::buffInsertRectangle(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::GREY);
  BufferCompositor::buffInsert3DBorder(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);
}

void SpriteFactory::makePressedButtonSprite()
{
  auto &buff = sprites->pressedButton;
  BufferCompositor::buffInsertRectangle(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::GREY);
  BufferCompositor::buffInsert2DBorder(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::DARK_GREY);
}

void SpriteFactory::makeWinnerResetButtonSprite()
{
  auto &buff = sprites->winnerResetButton;
  BufferCompositor::buffInsertRectangle(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::GREY);
  BufferCompositor::buffInsert3DBorder(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);
  FaceCompositor::buffInsertFaceBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
  FaceCompositor::buffInsertFaceSmile(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
  FaceCompositor::buffInsertFaceShades(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
}

void SpriteFactory::makeLoserResetButtonSprite()
{
  auto &buff = sprites->loserResetButton;
  BufferCompositor::buffInsertRectangle(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::GREY);
  BufferCompositor::buffInsert3DBorder(
      buff,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      0,
      0,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);
  FaceCompositor::buffInsertFaceBase(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
  FaceCompositor::buffInsertFaceFrown(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
  FaceCompositor::buffInsertFaceDeadEyes(buff, config::INFO_PANEL_BUTTONS_HEIGHT);
}

void SpriteFactory::makeEmptyCellSprite()
{
  auto &buff = sprites->empty;
  BufferCompositor::buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::GREY);
  BufferCompositor::buffInsert2DBorder(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::DARK_GREY);
}

void SpriteFactory::makeHiddenCellSprite()
{
  auto &buff = sprites->hidden;

  // base
  BufferCompositor::buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::GREY);

  BufferCompositor::buffInsert3DBorder(
      buff,
      config::CELL_PIXEL_SIZE,
      0,
      0,
      config::CELL_PIXEL_SIZE,
      config::CELL_PIXEL_SIZE,
      config::LIGHT_GREY,
      config::GREY,
      config::DARK_GREY);
}

void SpriteFactory::makeFlaggedCellSprite()
{
  auto &buff = sprites->flag;
  std::copy(sprites->hidden, sprites->hidden + cellSpriteSize, buff);
  MinefieldElementCompositor::buffInsertFlag(buff, config::CELL_PIXEL_SIZE);
}

void SpriteFactory::makeMineCellSprite()
{
  auto &buff = sprites->mine;
  BufferCompositor::buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::GREY);
  BufferCompositor::buffInsert2DBorder(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::DARK_GREY);
  MinefieldElementCompositor::buffInsertMine(buff, config::CELL_PIXEL_SIZE);
}

void SpriteFactory::makeClickedMineCellSprite()
{
  auto &buff = sprites->clickedMine;
  BufferCompositor::buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::RED);
  BufferCompositor::buffInsert2DBorder(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::DARK_GREY);
  MinefieldElementCompositor::buffInsertMine(buff, config::CELL_PIXEL_SIZE);
};

void SpriteFactory::makeMineCellWithRedXSprite()
{
  auto &buff = sprites->redXMine;
  BufferCompositor::buffInsertRectangle(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::GREY);
  BufferCompositor::buffInsert2DBorder(
      buff, config::CELL_PIXEL_SIZE, 0, 0, config::CELL_PIXEL_SIZE, config::CELL_PIXEL_SIZE, config::DARK_GREY);
  MinefieldElementCompositor::buffInsertMine(buff, config::CELL_PIXEL_SIZE);
  FaceCompositor::buffInsertX(
      buff,
      config::CELL_PIXEL_SIZE,
      config::RED,
      config::CELL_PIXEL_SIZE / 2,
      config::CELL_PIXEL_SIZE / 2,
      config::CELL_PIXEL_SIZE * 0.7,
      4);
}

void SpriteFactory::makeNumericSprite(uint32_t *buff, const int n, const uint32_t c)
{
  std::copy(sprites->empty, sprites->empty + cellSpriteSize, buff);
  BufferCompositor::buffInsertDigit(
      buff,
      config::CELL_PIXEL_SIZE,
      config::CELL_PIXEL_SIZE / 2 - NUMERIC_SPRITE_WIDTH / 2,
      config::CELL_PIXEL_SIZE / 2 - NUMERIC_SPRITE_HEIGHT / 2,
      NUMERIC_SPRITE_WIDTH,
      NUMERIC_SPRITE_HEIGHT,
      n,
      c);
};

void SpriteFactory::makeOneSprite()
{
  auto &buff = sprites->one;
  std::copy(sprites->empty, sprites->empty + cellSpriteSize, buff);
  MinefieldElementCompositor::buffInsertOne(buff, config::CELL_PIXEL_SIZE);
}

int SpriteFactory::rowColToWindowIndex(const int row, const int col)
{
  return row * config::GAME_WINDOW_PIXEL_WIDTH + col;
}

int SpriteFactory::rowColToCellIndex(const int row, const int col) { return row * config::CELL_PIXEL_SIZE + col; }

std::pair<int, int> SpriteFactory::rowColToPixelPoint(const int row, const int col)
{
  return {col * config::CELL_PIXEL_SIZE, row * config::CELL_PIXEL_SIZE};
};
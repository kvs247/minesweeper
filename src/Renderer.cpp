#include <Renderer.hpp>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <algorithm>
#include <config.hpp>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

Renderer::Renderer()
{
  display = XOpenDisplay(nullptr);
  if (!display)
  {
    throw std::runtime_error("unable to open X display");
  }

  screen = DefaultScreen(display);
  root = RootWindow(display, screen);
  visual = DefaultVisual(display, screen);

  XSetWindowAttributes windowAttributes = {};
  windowAttributes.background_pixel = WhitePixel(display, screen);
  windowAttributes.border_pixel = BlackPixel(display, screen);
  windowAttributes.event_mask = ExposureMask | Button1MotionMask | ButtonPressMask | ButtonReleaseMask |
                                KeyPressMask; // check these
  windowAttributes.override_redirect = true;
  window = XCreateWindow(
      display,
      root,
      0,
      0,
      config::WINDOW_PIXEL_WIDTH,
      config::WINDOW_PIXEL_HEIGHT,
      0,
      DefaultDepth(display, screen),
      InputOutput,
      visual,
      CWBackPixel | CWEventMask | CWBorderPixel,
      &windowAttributes);

  // remove header decoration
  Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", true);
  XSetWMProtocols(display, window, &wm_delete, 1);
  Atom wm_hints = XInternAtom(display, "_MOTIF_WM_HINTS", true);
  if (wm_hints != None)
  {
    struct
    {
      unsigned long flags;
      unsigned long functions;
      unsigned long decorations;
      long input_mode;
      unsigned long status;
    } hints = {2, 0, 0, 0, 0};
    XChangeProperty(display, window, wm_hints, wm_hints, 32, PropModeReplace, (unsigned char *)&hints, 5);
  }

  XMapWindow(display, window);

  XStoreName(display, window, config::APP_NAME);

  initializeGC();
  initializeBuffers();
  sprites = SpriteFactory::createSprites();
}

Renderer::~Renderer()
{
  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}

void Renderer::run(Game &game)
{
  XEvent event;
  struct timeval timeout;
  fd_set fds;
  int fdX11 = ConnectionNumber(display);
  time_t lastTimeS = time(NULL);

  updateBackBuffer(game);

  while (true)
  {
    // reset file descriptor
    FD_ZERO(&fds);       // clear fd set
    FD_SET(fdX11, &fds); // add x11 to set

    timeout.tv_sec = 0;
    timeout.tv_usec = 100000; // 100ms for responsiveness

    // check for X events or timeout
    int num_ready = select(fdX11 + 1, &fds, NULL, NULL, &timeout);

    // handle X events
    while (XPending(display))
    {
      XNextEvent(display, &event);

      if (event.type == Expose)
      {
        renderFrame();
        continue;
      }

      if (!updateGameState(game, event))
      {
        return;
      }

      updateBackBuffer(game);
      renderFrame();
    }

    time_t currentTime = time(NULL); // whole seconds
    if (time(NULL) > lastTimeS)
    {
      game.incrementTimer();
      updateBackBuffer(game);
      renderFrame();
      lastTimeS = currentTime;
    }
  }
}

void Renderer::renderFrame()
{
  image->data = reinterpret_cast<char *>(backBuffer.get());
  XPutImage(display, window, gc, image, 0, 0, 0, 0, config::WINDOW_PIXEL_WIDTH, config::WINDOW_PIXEL_HEIGHT);
  frontBuffer.swap(backBuffer);
}

bool Renderer::updateGameState(Game &game, XEvent &event)
{
  const int cursorX = event.xbutton.x;
  const int cursorY = event.xbutton.y;

  const bool inResetButton = cursorX >= config::RESET_BUTTON_X &&
                             cursorX < config::RESET_BUTTON_X + config::INFO_PANEL_BUTTONS_HEIGHT &&
                             cursorY >= config::RESET_BUTTON_Y &&
                             cursorY < config::RESET_BUTTON_Y + config::INFO_PANEL_BUTTONS_HEIGHT;

  switch (event.type)
  {
  case ButtonPress:
  {
    const int gameAreaX = config::FRAME_WIDTH + config::GRID_AREA_X_PAD;
    const int gameAreaY = config::INFO_PANEL_HEIGHT + 2 * config::FRAME_WIDTH + config::GRID_AREA_Y_PAD;

    const int row = (cursorY - gameAreaY) / config::CELL_PIXEL_SIZE;
    const int col = (cursorX - gameAreaX) / config::CELL_PIXEL_SIZE;

    const bool inGameArea = cursorX >= gameAreaX && row >= 0 && row < config::GRID_HEIGHT && cursorY >= gameAreaY &&
                            col >= 0 && col < config::GRID_WIDTH;
    if (inGameArea)
    {
      if (event.xbutton.button == Button1)
      {
        game.handleLeftClick(row, col);
      }
      else if (event.xbutton.button == Button2)
      {
        game.handleMiddleClick(row, col);
      }
      else if (event.xbutton.button == Button3)
      {
        game.handleRightClick(row, col);
      }
      break;
    }

    if (inResetButton && event.xbutton.button == Button1)
    {
      isResetButtonPressed = true;
    }

    break;
  }

  case ButtonRelease:
  {
    if (inResetButton && isResetButtonPressed)
    {
      game.reset();
    }
    isResetButtonPressed = false;
    break;
  }

  case KeyPress:
    const auto keysym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
    if (keysym == XK_q || keysym == XK_x || keysym == XK_Escape)
    {
      return false;
    }
  }

  return true;
}

void Renderer::updateBackBuffer(Game &game)
{
  // remaining flags
  SpriteFactory::buffInsertRemainingFlags(
      backBuffer.get(),
      config::WINDOW_PIXEL_WIDTH,
      config::REMAINING_FLAGS_X,
      config::REMAINING_FLAGS_Y,
      config::INFO_PANEL_BUTTONS_HEIGHT * 2,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      game.getRemainingFlags());

  // reset button
  const auto resetButtonSprite = isResetButtonPressed ? sprites->pressedButton : sprites->raisedButton;
  SpriteFactory::copySprite(
      backBuffer, resetButtonSprite, config::INFO_PANEL_BUTTONS_HEIGHT, config::RESET_BUTTON_X, config::RESET_BUTTON_Y);

  // timer
  SpriteFactory::buffInsertRemainingFlags(
      backBuffer.get(),
      config::WINDOW_PIXEL_WIDTH,
      config::TIMER_X,
      config::TIMER_Y,
      config::INFO_PANEL_BUTTONS_HEIGHT * 2,
      config::INFO_PANEL_BUTTONS_HEIGHT,
      game.getSecondsElapsed());

  // game area
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

      SpriteFactory::copySprite(backBuffer, sprite, config::CELL_PIXEL_SIZE, x, y);
    }
  }
}

void Renderer::initializeGC()
{
  XGCValues xgcv;

  xgcv.line_style = LineSolid;
  xgcv.cap_style = CapButt;
  xgcv.join_style = JoinMiter;
  xgcv.fill_style = FillSolid;
  xgcv.foreground = BlackPixel(display, screen);
  xgcv.background = WhitePixel(display, screen);

  unsigned long valueMask = GCForeground | GCBackground | GCFillStyle | GCLineStyle | GCLineWidth | GCLineWidth |
                            GCCapStyle | GCJoinStyle;

  gc = XCreateGC(display, root, valueMask, &xgcv);
}

void Renderer::initializeBuffers()
{
  const int bufferSize = config::WINDOW_PIXEL_HEIGHT * config::WINDOW_PIXEL_WIDTH;
  frontBuffer = std::make_unique<uint32_t[]>(bufferSize);
  backBuffer = std::make_unique<uint32_t[]>(bufferSize);

  SpriteFactory::buffInsertInterface(
      backBuffer.get(), config::WINDOW_PIXEL_WIDTH, config::WINDOW_PIXEL_WIDTH * config::WINDOW_PIXEL_HEIGHT);
  SpriteFactory::buffInsertInterface(
      frontBuffer.get(), config::WINDOW_PIXEL_WIDTH, config::WINDOW_PIXEL_WIDTH * config::WINDOW_PIXEL_HEIGHT);

  image = XCreateImage(
      display,
      visual,
      DefaultDepth(display, screen),
      ZPixmap,
      0,
      reinterpret_cast<char *>(backBuffer.get()),
      config::WINDOW_PIXEL_WIDTH,
      config::WINDOW_PIXEL_HEIGHT,
      32,
      config::WINDOW_PIXEL_WIDTH * sizeof(int32_t));
}
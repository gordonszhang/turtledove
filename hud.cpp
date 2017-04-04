#include "hud.h"

HUD::HUD() : rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ) {
}

void HUD::draw() const {
  SDL_SetRenderDrawBlendMode(rc->getRenderer(), SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(rc->getRenderer(), 255, 255, 255, 100);
  SDL_Rect r = { 30, 30, 250, 150 };
  SDL_RenderFillRect(rc->getRenderer(), &r);
  SDL_RenderDrawRect(rc->getRenderer(), &r);

  std::stringstream fps;
  fps << "FPS: " << clock.getAvgFps();
  SDL_Color red = { 0xff, 0, 0, 0 };
  io.writeText(fps.str(), red, 40, 40);

  std::stringstream ela;
  ela << "Elapsed: " << clock.getSeconds();
  io.writeText(ela.str(), red, 40, 90);

  std::stringstream instr;
  instr << "Arrow keys to move";
  SDL_Color white = { 0xff, 0xff, 0xff, 0 };
  io.writeText(instr.str(), white, 40, 140);
}

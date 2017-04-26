#include "hud.h"

HUD::HUD() : rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  aliveBullets(),
  freeBullets(),
  collisions() {
}

void HUD::draw() const {
  SDL_SetRenderDrawBlendMode(rc->getRenderer(), SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(rc->getRenderer(), 255, 255, 255, 100);
  SDL_Rect r = { 30, 30, 250, 300 };
  SDL_RenderFillRect(rc->getRenderer(), &r);
  SDL_RenderDrawRect(rc->getRenderer(), &r);

  std::stringstream fps;
  fps << "FPS: " << clock.getAvgFps();
  SDL_Color red = { 0xff, 0, 0, 0 };
  io.writeText(fps.str(), red, 40, 40);

  std::stringstream ela;
  ela << "Elapsed: " << clock.getSeconds();
  io.writeText(ela.str(), red, 40, 70);

  std::stringstream instr;
  instr << "Arrow Keys: Move";
  SDL_Color white = { 0xff, 0xff, 0xff, 0 };
  io.writeText(instr.str(), white, 40, 130);

  std::stringstream instr2;
  instr2 << "Z: Shoot, X: Shield";
  io.writeText(instr2.str(), white, 40, 160);

  std::stringstream a;
  a << "Enemy bullets: ";
  SDL_Color blue = { 0, 0, 0xff, 0 };
  io.writeText(a.str(), blue, 40, 220);

  std::stringstream f;
  f << "Alive: " << aliveBullets << " Free: " << freeBullets;
  SDL_Color green = { 0, 0xff, 0, 0 };
  io.writeText(f.str(), blue, 40, 250);

  std::stringstream c;
  c << "Collisions: " << collisions;
  //io.writeText(c.str(), white, 40, 280);
}

void HUD::updateCounts(int alive, int free) {
  aliveBullets = alive;
  freeBullets = free;
}

void HUD::updateCollisions(int c) {
  collisions = c;
}

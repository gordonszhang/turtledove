#include "hud.h"

HUD::HUD() : rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  aliveBullets(),
  freeBullets(),
  collisions(),
  bossHealth(200),
  deaths(0) {
}

void HUD::draw() const {
  SDL_SetRenderDrawBlendMode(rc->getRenderer(), SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(rc->getRenderer(), 255, 255, 255, 100);
  SDL_Rect r = { 30, 30, 300, 300 };
  SDL_RenderFillRect(rc->getRenderer(), &r);
  SDL_RenderDrawRect(rc->getRenderer(), &r);

  SDL_SetRenderDrawColor(rc->getRenderer(), 255, 50, 50, 200);
  SDL_Rect healthbar = { 0, 0, (int)(float(bossHealth) / 200.0 * 854.0), 10};
  SDL_RenderFillRect(rc->getRenderer(), &healthbar);
  SDL_RenderDrawRect(rc->getRenderer(), &healthbar);

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
  io.writeText(instr.str(), white, 40, 110);

  std::stringstream instr2;
  instr2 << "Z: Shoot, X: Shield";
  io.writeText(instr2.str(), white, 40, 140);

  std::stringstream instr3;
  instr3 << "G: Godmode, R: Restart";
  io.writeText(instr3.str(), white, 40, 170);

  std::stringstream a;
  a << "Enemy bullets: ";
  SDL_Color blue = { 0, 0, 0xff, 0 };
  io.writeText(a.str(), blue, 40, 210);

  std::stringstream f;
  f << "Alive: " << aliveBullets << " Free: " << freeBullets;
  //SDL_Color green = { 0, 0xff, 0, 0 };
  io.writeText(f.str(), blue, 40, 240);

  std::stringstream c;
  c << "Boss Health: " << bossHealth;
  io.writeText(c.str(), blue, 40, 270);

  std::stringstream d;
  d << "Deaths: " << deaths;
  io.writeText(d.str(), blue, 40, 300);

  if(bossHealth == 0) {
    std::stringstream w;
    w << "You won!";
    SDL_Color green = { 0, 0xff, 0, 0 };
    io.writeText(w.str(), green, 375, 200);
  }
}

void HUD::updateCounts(int alive, int free) {
  aliveBullets = alive;
  freeBullets = free;
}

void HUD::updateCollisions(int c) {
  collisions = c;
}

void HUD::updateBoss(int h) {
  bossHealth = h;
}

void HUD::updateDeaths(int d) {
  deaths = d;
}

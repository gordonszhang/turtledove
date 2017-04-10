#ifndef HUD_H
#define HUD_H

#include <sstream>
#include "ioMod.h"
#include "clock.h"
#include "renderContext.h"
#include "SDL2/SDL.h"

class HUD {
public:
  void draw() const;
  HUD();
  void updateCounts(int alive, int free);
private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;
  int aliveBullets;
  int freeBullets;
};

#endif

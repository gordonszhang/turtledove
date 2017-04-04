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
private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;
};

#endif

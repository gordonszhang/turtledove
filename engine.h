#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();
  void backSprite();
private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World world;
  World world2;
  Viewport& viewport;

  std::vector<Drawable*> sprites;

  int currentSprite;
  float radians;
  int counter;
  bool makeVideo;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
};

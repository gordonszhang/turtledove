#include <vector>
#include <SDL.h>
#include "world.h"
#include "viewport.h"
#include "hud.h"

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
  HUD hud;
  Viewport& viewport;

  std::vector<Drawable*> sprites;
  Drawable* player;

  int currentSprite;
  float radians;
  int counter;
  bool makeVideo;
  bool showHUD;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
};

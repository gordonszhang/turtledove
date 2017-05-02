#include <vector>
#include <SDL.h>
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "sound.h"
#include "bullet.h"
#include "collisionStrategy.h"
#include "explodingSprite.h"

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
  SDLSound sound;
  Viewport& viewport;

  std::vector<Drawable*> sprites;
  std::vector<Drawable*> bullets;
  std::vector<Drawable*> enemies;
	std::vector<Drawable*> playerBullets;
  std::queue<int> freeBullets;
	std::queue<int> freePlayerBullets;
  std::queue<int> freeEnemies;
  Drawable* player;
	Drawable* barrier;
  Drawable* boss;

  int currentSprite;
  float radians;
  int counter;
  bool makeVideo;
  bool showHUD;
  int deathTimer;
  int invulnTimer;
	int shootTimer;
  int lives;
  bool playerAlive;
  bool playerInvuln;
  bool godmode;
	bool playerShooting;
  bool bossAlive;

  CollisionStrategy* strategy;

  void draw() const;
  void update(Uint32);
  void checkForCollisions();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
};

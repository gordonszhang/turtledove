#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <random>
#include <algorithm>
#include "sprite.h"
#include "bullet.h"
#include "player.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"

class DrawComp {
public:
  bool operator()(const Drawable *lhs, const Drawable* rhs) const {
    return lhs->getSize() < rhs->getSize();
  }
};


Engine::~Engine() {
  std::cout << "Terminating program" << std::endl;
  for(auto* s : sprites) {
    delete s;
  }
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  world2("back2", Gamedata::getInstance().getXmlInt("back2/factor") ),
  hud(),
  viewport( Viewport::getInstance() ),
  sprites(),
  player(new Player("playership")),
  currentSprite(-1),
  radians(), counter(),
  makeVideo( false ),
  showHUD(true)
{
  player->setSize(4);
  sprites.push_back(player);
  switchSprite();
  sprites.push_back( new Sprite("crystal") );
  sprites[1]->setSize(4);

  std::random_device rd;
  std::mt19937 mt(rd());
  std::normal_distribution<float> dist(1.0, 0.5);
  int width = Gamedata::getInstance().getXmlInt("world/width");
  int height = Gamedata::getInstance().getXmlInt("world/height");

  unsigned int no_ty = 50;
  for(unsigned int i = 0; i < no_ty; ++i) {
    for(unsigned int j = 0; j < no_ty; ++j) {
      float color = dist(mt);
      Drawable* s;
      if(color > 0.5) s = new Sprite("tiny_star_1");
      else  s = new Sprite("tiny_star_2");
      float offset_x = dist(mt);
      float offset_y = dist(mt);
      int new_x = (int)(((float) i + offset_x) * (width / (float)(no_ty)));
      int new_y = (int)(((float) j + offset_y) * (height / (float)(no_ty)));
      s->setX(new_x);
      s->setY(new_y);
      s->setVelocityX(5);
      s->setSize(1);
      sprites.push_back(s);
    }
  }

  unsigned int no_sm = 20;
  for(unsigned int i = 0; i < no_sm; ++i) {
    for(unsigned int j = 0; j < no_sm; ++j) {
      float color = dist(mt);
      Drawable* s;
      if(color < 0.3) s = new Sprite("small_star_1");
      else if(color < 0.6) s = new Sprite("small_star_2");
      else s = new Sprite("small_star_3");
      float offset_x = dist(mt);
      float offset_y = dist(mt);
      int new_x = (int)(((float) i + offset_x) * (width / (float)(no_sm)));
      int new_y = (int)(((float) j + offset_y) * (height / (float)(no_sm)));
      s->setX(new_x);
      s->setY(new_y);
      float speed_mod = 1.0 + dist(mt);
      s->setVelocityX(10 * speed_mod);
      s->setSize(2);
      sprites.push_back(s);
    }
  }

  unsigned int no_big = 10;
  for(unsigned int i = 0; i < no_big; ++i) {
    for(unsigned int j = 0; j < no_big; ++j) {
      auto* s = new Sprite("big_star");
      float offset_x = dist(mt);
      float offset_y = dist(mt);
      int new_x = (int)(((float) i + offset_x) * (width / (float)(no_big)));
      int new_y = (int)(((float) j + offset_y) * (height / (float)(no_big)));
      s->setX(new_x);
      s->setY(new_y);
      float speed_mod = 1.0 + dist(mt);
      s->setVelocityX(30 * speed_mod);
      s->setSize(3);
      sprites.push_back(s);
    }
  }

  sort(sprites.begin(), sprites.end(), DrawComp());

  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  world.draw();

  for(auto* s : sprites) s->draw();
  if(showHUD) hud.draw();
  viewport.draw();

  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for(auto* s : sprites) {
    s->update(ticks);
  }

  world.update();
  world2.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}

void Engine::backSprite(){
  --currentSprite;
  currentSprite = currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_S] ) {
          clock.toggleSloMo();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if ( keystate[SDL_SCANCODE_B] ) {
          backSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
        if (keystate[SDL_SCANCODE_F1] && !showHUD) {
          std::cout << "Displaying HUD" << std::endl;
          showHUD = true;
        }
        else if (keystate[SDL_SCANCODE_F1] && showHUD) {
          std::cout << "Hiding HUD" << std::endl;
          showHUD = false;
        }
      }
      // Player control
      player->setVelocityX(0);
      player->setVelocityY(0);
      if ( keystate[SDL_SCANCODE_LEFT] && keystate[SDL_SCANCODE_RIGHT] ) {

      }
      else if ( keystate[SDL_SCANCODE_LEFT] ) {
        player->setVelocityX(player->getVelocityX() - 200);
      }
      else if ( keystate[SDL_SCANCODE_RIGHT] ) {
        player->setVelocityX(player->getVelocityX() + 200);
      }
      if ( keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_DOWN] ) {

      }
      else if ( keystate[SDL_SCANCODE_UP] ) {
        player->setVelocityY(player->getVelocityY() - 200);
      }
      else if ( keystate[SDL_SCANCODE_DOWN] ) {
        player->setVelocityY(player->getVelocityY() + 200);
      }
    }
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      if(counter % 16 == 0 && counter < 160) {
      for(int i = 0; i < 8; ++i) {
        float velX = 100.0 * cos(radians + i * (M_PI / 4.0));
        float velY = 100.0 * sin(radians + i * (M_PI / 4.0));
        sprites.push_back( new Bullet("bullet", velX, velY));
      }
      radians += M_PI / 32.0;
      }
      ++counter;
      clock.incrFrame();
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}

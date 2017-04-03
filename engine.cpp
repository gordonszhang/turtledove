#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>
#include "sprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"

Engine::~Engine() {
  std::cout << "Terminating program" << std::endl;
  std::vector<Drawable*>::iterator it = sprites.begin();
  while ( it != sprites.end() ) {
    delete *it;
    ++it;
  }
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  world2("back2", Gamedata::getInstance().getXmlInt("back2/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(-1),
  radians(), counter(),

  makeVideo( false )
{
  sprites.push_back( new MultiSprite("playership") );
  sprites.push_back( new Sprite("crystal") );
  switchSprite();
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  world.draw();
  world2.draw();
  std::stringstream strm;
  strm << "FPS: " << clock.getAvgFps();
  SDL_Color red = { 0xff, 0, 0, 0 };
  io.writeText(strm.str(), red, 30, 30);

  for(auto* s : sprites) s->draw();

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for(auto* s : sprites) s->update(ticks);
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
      }
    }
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      if(counter % 16 == 0 && counter < 160) {
      for(int i = 0; i < 8; ++i) {
        float velX = 100.0 * cos(radians + i * (M_PI / 4.0));
        float velY = 100.0 * sin(radians + i * (M_PI / 4.0));
        sprites.push_back( new Sprite("bullet", velX, velY));
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

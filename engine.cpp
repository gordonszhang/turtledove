#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <random>
#include <algorithm>
#include "sprite.h"
#include "star.h"
#include "player.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "barrier.h"

extern bool restart;

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
  bullets(),
  enemies(),
	playerBullets(),
  freeBullets(),
	freePlayerBullets(),
  freeEnemies(),
  player(new Player("playership")),
	barrier(new Barrier("barrier")),
  boss(new Sprite("crystal")),
  currentSprite(-1),
  radians(), counter(),
  makeVideo( false ),
  showHUD(true),
  deathTimer(),
  invulnTimer(),
	shootTimer(),
  lives(),
  playerAlive(true),
  playerInvuln(false),
  godmode(false),
	playerShooting(false),
  bossAlive(true),
  strategy( new PerPixelCollisionStrategy )
{
  player->setSize(4);
	((Player*)player)->attachBarrier(barrier);
  ((Player*)player)->attachEnemy(boss);
  //sprites.push_back(player);
  //switchSprite();
  Viewport::getInstance().setObjectToTrack(player);
  boss->setSize(4);

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
      if(color > 0.5) s = new Star("tiny_star_1");
      else  s = new Star("tiny_star_2");
      float offset_x = dist(mt);
      float offset_y = dist(mt);
      int new_x = (int)(((float) i + offset_x) * (width / (float)(no_ty)));
      int new_y = (int)(((float) j + offset_y) * (height / (float)(no_ty)));
      s->setX(new_x);
      s->setY(new_y);
      s->setVelocityY(5);
      s->setSize(1);
      sprites.push_back(s);
    }
  }

  unsigned int no_sm = 20;
  for(unsigned int i = 0; i < no_sm; ++i) {
    for(unsigned int j = 0; j < no_sm; ++j) {
      float color = dist(mt);
      Drawable* s;
      if(color < 0.3) s = new Star("small_star_1");
      else if(color < 0.6) s = new Star("small_star_2");
      else s = new Star("small_star_3");
      float offset_x = dist(mt);
      float offset_y = dist(mt);
      int new_x = (int)(((float) i + offset_x) * (width / (float)(no_sm)));
      int new_y = (int)(((float) j + offset_y) * (height / (float)(no_sm)));
      s->setX(new_x);
      s->setY(new_y);
      float speed_mod = 1.0 + dist(mt);
      s->setVelocityY(10 * speed_mod);
      s->setSize(2);
      sprites.push_back(s);
    }
  }

  unsigned int no_big = 10;
  for(unsigned int i = 0; i < no_big; ++i) {
    for(unsigned int j = 0; j < no_big; ++j) {
      auto* s = new Star("big_star");
      float offset_x = dist(mt);
      float offset_y = dist(mt);
      int new_x = (int)(((float) i + offset_x) * (width / (float)(no_big)));
      int new_y = (int)(((float) j + offset_y) * (height / (float)(no_big)));
      s->setX(new_x);
      s->setY(new_y);
      float speed_mod = 1.0 + dist(mt);
      s->setVelocityY(30 * speed_mod);
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
  boss->draw();
	player->draw();
  for(auto* b : bullets) {
    if(b->isAlive()) b->draw();
  }
	for(auto* p : playerBullets) {
    if(p->isAlive()) p->draw();
  }

  if(showHUD) hud.draw();
  viewport.draw();

  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for(auto* s : sprites) {
    s->update(ticks);
  }
  player->update(ticks);
  boss->update(ticks);
  if(!playerAlive) {
    if(deathTimer > 60) {
      deathTimer = 0;
      playerAlive = true;
      player = new Player("playership");
			((Player*)player)->attachBarrier(barrier);
      ((Player*)player)->attachEnemy(boss);
      Viewport::getInstance().setObjectToTrack(player);
      player->setAlive(false);
      invulnTimer = 0;
      playerInvuln = true;
    }
    else ++deathTimer;
  }
  else if(playerInvuln) {
    if(!godmode && invulnTimer > 90) {
      invulnTimer = 0;
      playerInvuln = false;
      player->setAlive(true);
    }
    else ++invulnTimer;
  }
  for(int i = 0; i < (int)bullets.size(); ++i) {
    auto* s = bullets[i];
    if(s->isAlive()) {
      s->update(ticks);
      if(!s->isAlive()) freeBullets.push(i);
    }
  }

  for(int i = 0; i < (int)playerBullets.size(); ++i) {
    auto* s = playerBullets[i];
    if(s->isAlive()) {
      s->update(ticks);
      if(!s->isAlive()) freePlayerBullets.push(i);
    }
  }

	if(playerShooting) {
		if(shootTimer == 0) {
			float velX = 0.0;
			float velY = -400.0;
			if(freePlayerBullets.size() > 1) {
				int f = freePlayerBullets.front();
				freePlayerBullets.pop();
				playerBullets[f]->reset(velX, velY);
				playerBullets[f]->setX(player->getX()+12);
				playerBullets[f]->setY(player->getY()+12);

				int f2 = freePlayerBullets.front();
				freePlayerBullets.pop();
				playerBullets[f2]->reset(velX, velY);
				playerBullets[f2]->setX(player->getX()+36);
				playerBullets[f2]->setY(player->getY()+12);
			}
			else {
				Drawable* b = new Bullet("playerBullet", velX, velY);
				b->setX(player->getX()+12);
				b->setY(player->getY()+12);
				playerBullets.push_back(b);

				Drawable* b2 = new Bullet("playerBullet", velX, velY);
				b2->setX(player->getX() + 36);
				b2->setY(player->getY() + 12);
				playerBullets.push_back(b2);
			}
		}
		shootTimer = (shootTimer + 1) % 4;
	}

  hud.updateCounts(bullets.size() - freeBullets.size(), freeBullets.size());
  world.update();
  world2.update();
  viewport.update(); // always update viewport last
}

void Engine::checkForCollisions() {

  std::vector<Drawable*>::const_iterator it = bullets.begin();
  int collisions = 0;
  while ( it != bullets.end() ) {
    if(!(*it)->isAlive()) {
      ++it;
      continue;
    }
    if(!playerAlive || !player->isAlive()) break;
    if (strategy->execute(*barrier, **it) && (((Barrier*)barrier)->isLight() == ((Bullet*)(*it))->isLight())) {
      ++collisions;
      hud.updateCollisions(collisions);
      //std::cout << sprites[10]->getName();
      //Drawable* boom = new ExplodingSprite(*static_cast<Sprite*>(player));
      //player = boom;
      //delete player;
      //player = new Player("playership");
        (*it)->setAlive(false);
        int i = it - bullets.begin();
        freeBullets.push(i);
    }

    else if ( strategy->execute(*player, **it) ) {
      //std::cout << "collision: " << ++collisions << std::endl;
      hud.updateDeaths(++lives);
      //std::cout << sprites[10]->getName();
      //Drawable* boom = new ExplodingSprite(*static_cast<Sprite*>(player));
      //player = boom;
      //delete player;
      //player = new Player("playership");

        Drawable* temp = new Sprite("playershipE");
				((Player*)player)->detachBarrier();
        ((Player*)player)->detachEnemy();
        temp->setX(player->getX());
        temp->setY(player->getY());
        Drawable* explodingSprite = new ExplodingSprite(*static_cast<Sprite*>(temp));
        player = explodingSprite;
        Viewport::getInstance().setObjectToTrack(player);
        playerAlive = false;
        deathTimer = 0;
        break;
    }
    ++it;
  }
  it = playerBullets.begin();
  while ( it != playerBullets.end() ) {
    if(!(*it)->isAlive()) {
      ++it;
      continue;
    }
    if(!bossAlive || !boss->isAlive()) break;
    if ( strategy->execute(*boss, **it) ) {
      //std::cout << "collision: " << ++collisions << std::endl;
      ((Sprite*)boss)->damage();
      hud.updateBoss(((Sprite*)boss)->getHealth());
      (*it)->setAlive(false);
      int i = it - playerBullets.begin();
      freePlayerBullets.push(i);
      if(!boss->isAlive()) {
      //std::cout << sprites[10]->getName();
      //Drawable* boom = new ExplodingSprite(*static_cast<Sprite*>(player));
      //player = boom;
      //delete player;
      //player = new Player("playership");

        Drawable* temp = new Sprite("crystal");
        temp->setX(boss->getX());
        temp->setY(boss->getY());
        Drawable* explodingSprite = new ExplodingSprite(*static_cast<Sprite*>(temp));
        boss = explodingSprite;
        bossAlive = false;
        break;
      }
    }
    ++it;
  }
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
        if ( keystate[SDL_SCANCODE_R] ) {
          restart = true;
          return;
        }
        if ( keystate[SDL_SCANCODE_G] ) {
          if(!godmode) {
            playerInvuln = true;
            player->setAlive(false);
            godmode = true;
          }
          else {
            godmode = false;
          }
        }
				if ( keystate[SDL_SCANCODE_X] && playerAlive ) {
          ((Barrier*)barrier)->switchBarrier();
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
			playerShooting = false;
			if(keystate[SDL_SCANCODE_Z] && playerAlive) {
				playerShooting = true;
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
      if(bossAlive) {
        if(counter % 24 == 0) {
        for(int i = 0; i < 8; ++i) {
          float velX = 100.0 * cos(radians + i * (M_PI / 4.0));
          float velY = 100.0 * sin(radians + i * (M_PI / 4.0));
          if(freeBullets.size()) {
            int f = freeBullets.front();
            freeBullets.pop();
            bullets[f]->reset(velX, velY);
            ((Bullet*)(bullets[f]))->setLight(true);
            bullets[f]->setPosition(boss->getPosition() + Vector2f(16, 16));
          }

          else {
            Drawable* b = new Bullet("bullet", velX, velY);
            ((Bullet*)b)->setLight(true);
            b->setPosition(boss->getPosition() + Vector2f(16, 16));
            bullets.push_back(b);
          }
          if(freeBullets.size()) {
            int f = freeBullets.front();
            freeBullets.pop();
            bullets[f]->reset(velY, velX);
            ((Bullet*)(bullets[f]))->setLight(false);
            bullets[f]->setPosition(boss->getPosition() + Vector2f(16, 16));
          }

          else {
            Drawable* b = new Bullet("bullet", velY, velX);
            ((Bullet*)b)->setLight(false);
            b->setPosition(boss->getPosition() + Vector2f(16, 16));
            bullets.push_back(b);
          }
        }
        radians += M_PI / 32.0;
        }
      }
      ++counter;
      clock.incrFrame();
      draw();

      update(ticks);
      checkForCollisions();
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}

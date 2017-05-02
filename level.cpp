#include "level.h"

Level::Level() : 	clock( Clock::getInstance() ),
									phase(),
									timer(),
									enemies() { }


Level::Level(const Level& l) : 	clock(l.clock),
									phase(l.phase),
									timer(l.timer),
									enemies(l.enemies) { }

void Level::update() {
	// TODO: Enemy generation code. Switch cased by time/phase
  //       Should probably define a Directive class plug-n-play AI at initialization...
	Uint32 ticks = clock.getElapsedTicks();


	// Call update() on each of our enemies.
	for(auto e : enemies) {
    e->update(ticks);
  }
}

/*
void Level::checkForCollisions(std::vector<Drawable*>& playerBullets) {
	// TODO: Probably should move this into engine
}
*/

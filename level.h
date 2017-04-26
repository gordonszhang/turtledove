#include "clock.h"
#include "drawable.h"
#include "bullet.h"
#include <vector>

class Level {
public:
	Level();
	Level(const Level&);
	void update();
	//void checkForCollisions(std::vector<Drawable*>& playerBullets);
private:
	Clock& clock;
	int phase;
	int timer;
	std::vector<Drawable*> enemies;

};

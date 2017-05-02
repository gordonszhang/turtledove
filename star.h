#ifndef STAR_H
#define STAR_H
#include <string>
#include "drawable.h"

class StarCompare {

};

class Star : public Drawable {
public:
  Star(const std::string&);
  Star(const std::string&, float vX, float vY);
  Star(const Star&);
  virtual ~Star() { }
  Star& operator=(const Star&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void setFrame(Frame* f) { frame = f; };
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void damage() { --health; if(!health) setAlive(false); };
  int getHealth() const { return health; };
  void resetDamage() { health = 2000; };

private:
  const Frame * frame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  int getDistance(const Star*) const;
  int health;
  Vector2f playerPos;
};
#endif

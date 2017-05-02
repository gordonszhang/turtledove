#ifndef SPRITE__H
#define SPRITE__H
#include <string>
#include "drawable.h"

class SpriteCompare {

};

class Sprite : public Drawable {
public:
  Sprite(const std::string&);
  Sprite(const std::string&, float vX, float vY);
  Sprite(const Sprite&);
  virtual ~Sprite() { }
  Sprite& operator=(const Sprite&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void setFrame(Frame* f) { frame = f; };
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void damage() { --health; if(!health) setAlive(false); };
  int getHealth() const { return health; };
  void resetDamage() { health = 2000; };
  void updatePlayerPos(Vector2f pos) { playerPos = pos; };

private:
  const Frame * frame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  int getDistance(const Sprite*) const;
  int health;
  Vector2f playerPos;
};
#endif

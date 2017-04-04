#ifndef BULLET__H
#define BULLET_H
#include <string>
#include "drawable.h"

class Bullet : public Drawable {
public:
  Bullet(const std::string&);
  Bullet(const std::string&, float vX, float vY);
  Bullet(const Bullet&);
  virtual ~Bullet() { }
  Bullet& operator=(const Bullet&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;
  virtual void update(Uint32 ticks);
private:
  const Frame * frame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  int getDistance(const Bullet*) const;
};
#endif

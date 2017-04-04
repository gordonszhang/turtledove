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
  virtual void draw() const;
  virtual void update(Uint32 ticks);

private:
  const Frame * frame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  int getDistance(const Sprite*) const;
};
#endif

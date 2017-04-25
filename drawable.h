#ifndef DRAWABLE__H
#define DRAWABLE__H
#include <SDL.h>
#include <iostream>
#include <string>
#include "vector2f.h"
#include "frame.h"
#include "gamedata.h"

// Drawable is an Abstract Base Class (ABC) that
// specifies the methods that derived classes may
// and must have.
class Drawable {
public:
  Drawable(const std::string& n, const Vector2f& pos, const Vector2f& vel):
    name(n), position(pos), velocity(vel), size(), alive(true) {}

  Drawable(const Drawable& s) :
    name(s.name), position(s.position), velocity(s.velocity), size(s.size), alive(true)
    { }

  virtual ~Drawable() {}

  virtual void draw() const = 0;
  virtual void update(Uint32 ticks) = 0;

  const std::string& getName() const { return name; }
  void setName(const std::string& n) { name = n;    }

  virtual const Frame* getFrame() const = 0;

  float getX() const  { return position[0]; }
  void  setX(float x) { position[0] = x;    }

  float getY() const  { return position[1]; }
  void  setY(float y) { position[1] = y;    }

  float getVelocityX() const   { return velocity[0]; }
  void  setVelocityX(float vx) { velocity[0] = vx;   }
  float getVelocityY() const   { return velocity[1]; }
  void  setVelocityY(float vy) { velocity[1] = vy;   }

  const Vector2f& getVelocity() const    { return velocity; }
  void  setVelocity(const Vector2f& vel) { velocity = vel;  }
  const Vector2f& getPosition() const    { return position; }
  void  setPosition(const Vector2f& pos) { position = pos;  }

  int getSize() const { return size; }
  void setSize(int s) { size = s; }

  bool isAlive() const { return alive; };
  virtual void setAlive(bool status) { alive = status; };

  void reset(float vX, float vY) {
    alive = true;
    setVelocityX(vX);
    setVelocityY(vY);
    setX(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"));
    setY(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y"));
  }

private:
  std::string name;
  Vector2f position;
  Vector2f velocity;
  int size;
  bool alive;

};
#endif

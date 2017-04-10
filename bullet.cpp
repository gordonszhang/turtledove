#include <cmath>
#include "bullet.h"
#include "gamedata.h"
#include "renderContext.h"

Bullet::Bullet(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frame( RenderContext::getInstance()->getFrame(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  alive(true)
{ }

Bullet::Bullet(const std::string& name, float vX, float vY) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(vX, vY)
           ),
  frame( RenderContext::getInstance()->getFrame(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  alive(true)
{ }

Bullet::Bullet(const Bullet& s) :
  Drawable(s),
  frame(s.frame),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  alive(s.alive)
{ }

Bullet& Bullet::operator=(const Bullet& rhs) {
  if(this == &rhs) return *this;
  Drawable::operator=( rhs );
  frame = rhs.frame;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  alive = rhs.alive;
  return *this;
}

void Bullet::draw() const {
  frame->draw(getX(), getY());
}

void Bullet::update(Uint32 ticks) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if(getX() < (-frameWidth) || getX() > worldWidth
  || getY() < (-frameHeight) || getY() > worldHeight) {
    alive = false;
  }

}

void Bullet::reset(float vX, float vY) {
  alive = true;
  setVelocityX(vX);
  setVelocityY(vY);
  setX(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"));
  setY(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y"));
}

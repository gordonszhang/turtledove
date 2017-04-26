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
  dark(RenderContext::getInstance()->getFrame("alt"+name)),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  light()
{ }

Bullet::Bullet(const std::string& name, float vX, float vY) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(vX, vY)
           ),
  frame( RenderContext::getInstance()->getFrame(name) ),
  dark(RenderContext::getInstance()->getFrame("alt"+name)),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  light()
{ }

Bullet::Bullet(const Bullet& s) :
  Drawable(s),
  frame(s.frame),
  dark(s.dark),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  light(s.light)
{ }

Bullet& Bullet::operator=(const Bullet& rhs) {
  if(this == &rhs) return *this;
  Drawable::operator=( rhs );
  frame = rhs.frame;
  dark = rhs.dark;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  light = rhs.light;
  return *this;
}

void Bullet::draw() const {
  if(light)frame->draw(getX(), getY());
  else dark->draw(getX(),getY());
}

void Bullet::update(Uint32 ticks) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if(getX() < (-frameWidth) || getX() > worldWidth
  || getY() < (-frameHeight) || getY() > worldHeight) {
    setAlive(false);
  }

}

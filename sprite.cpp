#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "renderContext.h"

Sprite::Sprite(const std::string& name) :
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
  health(200),
  playerPos()
{ }

Sprite::Sprite(const std::string& name, float vX, float vY) :
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
  health(200),
  playerPos()
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s),
  frame(s.frame),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  health(s.health),
  playerPos(s.playerPos)
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  if(this == &rhs) return *this;
  Drawable::operator=( rhs );
  frame = rhs.frame;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  health = rhs.health;
  playerPos = rhs.playerPos;
  return *this;
}

void Sprite::draw() const {
  frame->draw(getX(), getY());
}

void Sprite::update(Uint32 ticks) {
  //std::cout << "enemy pos: " << getPosition() << " player pos: " << playerPos << std::endl;
  if(getPosition()[0] < playerPos[0]) setVelocityX(50);
  else if(getPosition()[0] > playerPos[0]) setVelocityX(-50);

  if(abs(getPosition()[0] - playerPos[0]) < 10) setVelocityX(0);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  /*
  if ( getY() < 0) {
    setVelocityY( std::abs( getVelocityY() ) );
  }
  */
  if ( getX() > worldWidth-frameWidth) {
    setX(0);
  }

  if ( getY() > worldHeight-frameHeight) {
    setY(0);
  }
  /*
  if ( getX() < 0) {
    setVelocityX( std::abs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-frameWidth) {
    setVelocityX( -std::abs( getVelocityX() ) );
  }
  */
}

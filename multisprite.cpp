#include "multisprite.h"
#include "gamedata.h"
#include "renderContext.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames( RenderContext::getInstance()->getFrames(name) ),
	framesRight(RenderContext::getInstance()->getFrames(name+"R")),
	framesLeft(RenderContext::getInstance()->getFrames(name+"L")),
	framesTransitionRight(RenderContext::getInstance()->getFrames(name+"TR")),
	framesTransitionLeft(RenderContext::getInstance()->getFrames(name+"TL")),
	selectedFrames(frames),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
	state(0), movingRight(false)
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s),
  frames(s.frames),
	framesRight(s.framesRight),
	framesLeft(s.framesLeft),
	framesTransitionRight(s.framesTransitionRight),
	framesTransitionLeft(s.framesLeft),
	selectedFrames(s.selectedFrames),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
	state(s.state), movingRight(s.movingRight)
  { }

void MultiSprite::draw() const {

  selectedFrames[currentFrame]->draw(getX(), getY());
}

void MultiSprite::update(Uint32 ticks) {
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-frameHeight) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-frameWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
  }
	if(getVelocityX() > 0) {
		if(state <= 0) {
			state = 1;
			selectedFrames = framesTransitionRight;
		}
		else if(state >= 0 && state < 5) {
			++state;
		}
		else if (state == 5) {
			selectedFrames = framesRight;

			++state;
		}
	}
	if(getVelocityX() == 0) selectedFrames = frames;
	if(getVelocityX() < 0) {
		if(state >= 0) {
			state = -1;
			selectedFrames = framesTransitionLeft;
		}
		else if(state <= 0 && state > -5) {
			--state;
		}
		else if (state == -5) {
			selectedFrames = framesLeft;
			--state;
		}
	}
}

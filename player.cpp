#include "player.h"
#include "gamedata.h"
#include "renderContext.h"

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Player::Player( const std::string& name) :
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
  framesTurnRight(RenderContext::getInstance()->getFrames(name+"UR")),
	framesTurnLeft(RenderContext::getInstance()->getFrames(name+"UL")),
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

Player::Player(const Player& s) :
  Drawable(s),
  frames(s.frames),
	framesRight(s.framesRight),
	framesLeft(s.framesLeft),
	framesTransitionRight(s.framesTransitionRight),
	framesTransitionLeft(s.framesLeft),
  framesTurnRight(s.framesTurnRight),
	framesTurnLeft(s.framesTurnLeft),
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

void Player::draw() const {

  selectedFrames[currentFrame]->draw(getX(), getY());
}

void Player::update(Uint32 ticks) {
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setY(0);
  }
  if ( getY() > worldHeight-frameHeight) {
    setY(worldHeight-frameHeight);
  }

  if ( getX() < 0) {
    setX(0);
  }
  if ( getX() > worldWidth-frameWidth) {
    setX(worldWidth-frameWidth);
  }
	if(getVelocityX() > 0) {
		if(state < 0) {
			state = 1;
			selectedFrames = framesTransitionRight;
		}
    else if(state == 0) {
      state = 3;
      selectedFrames = framesTurnRight;
    }
		else if(state > 0 && state < 5) {
			++state;
		}
		else if (state == 5) {
			selectedFrames = framesRight;

			++state;
		}
	}
	if(getVelocityX() == 0) {
    state = 0;
    selectedFrames = frames;
  }
	if(getVelocityX() < 0) {
		if(state > 0) {
			state = -1;
			selectedFrames = framesTransitionLeft;
		}
    else if(state == 0) {
      state = -3;
      selectedFrames = framesTurnLeft;
    }
		else if(state < 0 && state > -5) {
			--state;
		}
		else if (state == -5) {
			selectedFrames = framesLeft;
			--state;
		}
	}
}

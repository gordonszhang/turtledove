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
           Vector2f(Gamedata::getInstance().getXmlInt(name+"I/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"I/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"I/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"I/speedY"))
           ),
  frames( RenderContext::getInstance()->getFrames(name+"I") ),
	framesRight(RenderContext::getInstance()->getFrames(name+"R")),
	framesLeft(RenderContext::getInstance()->getFrames(name+"L")),
	framesTransitionRight(RenderContext::getInstance()->getFrames(name+"TR")),
	framesTransitionLeft(RenderContext::getInstance()->getFrames(name+"TL")),
  framesTurnRight(RenderContext::getInstance()->getFrames(name+"UR")),
	framesTurnLeft(RenderContext::getInstance()->getFrames(name+"UL")),
	selectedFrames(frames),
	barrier(),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"I/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"I/frameInterval")),
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
	barrier(s.barrier),
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
	barrier[0]->draw();
  if(isAlive()) {
    selectedFrames[currentFrame]->draw(getX(), getY());
  }
  else {
    if(offFrame > 1) {
      selectedFrames[currentFrame]->draw(getX(), getY());
    }
  }
}

void Player::update(Uint32 ticks) {
  advanceFrame(ticks);

  if(!isAlive()) offFrame = (offFrame + 1) % 4;
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
      state = 5;
      selectedFrames = framesTurnRight;
    }
		else if(state > 0 && state < 9) {
			++state;
		}
		else if (state == 9) {
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
      state = -5;
      selectedFrames = framesTurnLeft;
    }
		else if(state < 0 && state > -9) {
			--state;
		}
		else if (state == -9) {
			selectedFrames = framesLeft;
			--state;
		}
	}
	barrier[0]->setX(getX());
	barrier[0]->setY(getY());
	barrier[0]->setVelocityX(getVelocityX());
	barrier[0]->setVelocityY(getVelocityY());
	barrier[0]->update(ticks);
}

void Player::attachBarrier(Drawable *b) {
	barrier.push_back((Barrier*)b);
}

void Player::detachBarrier() {
	barrier.pop_back();
}

void Player::setAlive(bool a) {
  Drawable::setAlive(a);
	barrier[0]->setAlive(a);
  if(!a) offFrame = 0;
}

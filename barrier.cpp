#include "barrier.h"
#include "gamedata.h"
#include "renderContext.h"

void Barrier::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Barrier::Barrier( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"L/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"L/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"L/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"L/speedY"))
           ),
  framesLightBarrier( RenderContext::getInstance()->getFrames(name+"L") ),
	framesDarkBarrier(RenderContext::getInstance()->getFrames(name+"D")),
	framesLightDark(RenderContext::getInstance()->getFrames(name+"LD")),
	framesDarkLight(RenderContext::getInstance()->getFrames(name+"DL")),
	selectedFrames(framesLightBarrier),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"L/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"L/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(framesLightBarrier[0]->getWidth()),
  frameHeight(framesLightBarrier[0]->getHeight()),
	state(-1),
	lightBarrier(true),
	isInvulnerable(),
	offFrame()

{ }

Barrier::Barrier(const Barrier& s) :
  Drawable(s),
  framesLightBarrier(s.framesLightBarrier),
	framesDarkBarrier(s.framesDarkBarrier),
	framesLightDark(s.framesLightDark),
	framesDarkLight(s.framesDarkLight),
	selectedFrames(s.selectedFrames),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
	state(s.state),
	lightBarrier(s.lightBarrier),
	isInvulnerable(s.isInvulnerable),
	offFrame(s.offFrame)
  { }

void Barrier::draw() const {

  if(isAlive()) {
    selectedFrames[currentFrame]->draw(getX(), getY());
  }
  else {
    if(offFrame > 1) {
      selectedFrames[currentFrame]->draw(getX(), getY());
    }
  }
}

void Barrier::update(Uint32 ticks) {
  advanceFrame(ticks);

  if(!isAlive()) offFrame = (offFrame + 1) % 4;
	//std::cout << "state: " << state << std::endl;
	if(state > -1 && state < 9) {
			state++;
	}

	else if(state >= 9) {
		if(lightBarrier) 	selectedFrames = framesLightBarrier;
		else 							selectedFrames = framesDarkBarrier;
		state = -1;
	}
}

void Barrier::switchBarrier() {
	if(state != -1) return;
	state = 0;
	if(lightBarrier) 	selectedFrames = framesLightDark;
	else 							selectedFrames = framesDarkLight;
	lightBarrier = !lightBarrier;
}

void Barrier::setAlive(bool a) {
  Drawable::setAlive(a);
  if(!a) offFrame = 0;
}

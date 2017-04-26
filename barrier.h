#ifndef BARRIER_H
#define BARRIER_H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class Barrier : public Drawable {
public:
  Barrier(const std::string&);
  Barrier(const Barrier&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const {
    return selectedFrames[currentFrame];
  }

	virtual void setAlive(bool a);
	virtual void switchBarrier();
  bool isLight() const { return lightBarrier; };

protected:
  const std::vector<Frame *> framesLightBarrier;
  const std::vector<Frame *> framesDarkBarrier;
  const std::vector<Frame *> framesLightDark;
  const std::vector<Frame *> framesDarkLight;
  std::vector<Frame *> selectedFrames;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  int state;
	bool lightBarrier;

  bool isInvulnerable;
  int offFrame;
  void advanceFrame(Uint32 ticks);
};
#endif

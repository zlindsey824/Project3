#include "multisprite.h"
#include "gameData.h"
#include "imageFactory.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    if (reversed)
      currentFrame = (currentFrame-1) % (numberOfFrames/2);
    else
      currentFrame = ((currentFrame+1) % (numberOfFrames/2)) + (numberOfFrames/2);
		timeSinceLastFrame = 0;
	}
}

Vector2f MultiSprite::makeVelocity(int vx, int vy){
  float x = Gamedata::getInstance().getRandInRange(vx-50, vx+50);
  float y = Gamedata::getInstance().getRandInRange(vy-50, vy+50);
  vx += x;
  vy += y;
  return Vector2f(vx, vy);
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  reversed(false)
{ }

void MultiSprite::draw() const {
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void MultiSprite::update(Uint32 ticks) {
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() + getScaledHeight() > worldHeight) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    reversed = false;
  }
  if ( getX() + getScaledWidth() > worldWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
    reversed = true;
  }

}

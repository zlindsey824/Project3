#include <iostream>
#include "world.h"
#include "imageFactory.h"

World::World(const std::string& name, int fact) : 
  image( ImageFactory::getInstance()->getImage(name) ),
  factor(fact),
    imageWidth( image->getWidth() ),
    imageHeight(image->getHeight()),
    viewX(0.0), viewY(0.0), 
    view(Viewport::getInstance()) 
{ }

void World::update() {
  viewX = static_cast<int>(view.getX() / factor) % imageWidth;
  viewY = static_cast<int>(view.getY() / factor) % imageHeight;
}

void World::draw() const { 
  //places the image in focus
  image->draw(0,0,-viewX,-viewY);
  //places the image to the right/left
  image->draw(0,0,imageWidth-viewX,-viewY);
  //places the image up/down
  image->draw(0,0,-viewX,imageHeight-viewY);
  //places the image diagonally
  image->draw(0,0,imageWidth-viewX,imageHeight-viewY);
}


#include <string>
#include "image.h"
#include "viewport.h"

class World {
public:
  World(const std::string& name, int fact);
  // ~World() { } // The image will be deleted by the FrameFactory
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  void update();
  void draw() const;
private:
  Image* const image;
  int factor;
  unsigned imageWidth;
  float viewX;
  float viewY;
  const Viewport & view;
};

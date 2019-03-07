#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"

int numSprites = 10;

Engine::~Engine() {
  std::vector<Drawable*>::iterator it = sprites.begin();
  while (it != sprites.end())
    it = sprites.erase(it);
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  clouds("clouds", Gamedata::getInstance().getXmlInt("clouds/factor") ),
  mountains("mountains", Gamedata::getInstance().getXmlInt("mountains/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(0),
  makeVideo( false )
{
  for (int i = 0; i < numSprites; i++){
    if (i % 3 == 0)
      sprites.push_back(new Sprite("AngelStand"));
    else
      sprites.push_back(new MultiSprite("AngelRun"));
  }
  sprites[0]->setScale(1.5);
  Viewport::getInstance().setObjectToTrack(sprites[0]);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  clouds.draw();
  mountains.draw();

  for (int i = 0; i < numSprites; i++)
    sprites[i]->draw();

  viewport.draw();

  std::stringstream fps;
  std::string fpsString;
  fps << clock.getFps();
  fps >> fpsString;
  std::cout << fpsString << std::endl;
  int y = Gamedata::getInstance().getXmlInt("view/height");
  io.writeText(fpsString, 100, y-100);
  SDL_Color color = {255,255,255,255};
  io.writeText("Zeandre Lindsey", 100, y-50, color);

  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for (int i = 0; i < numSprites; i++)
    sprites[i]->update(ticks);
  clouds.update();
  mountains.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % numSprites;

  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);

}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {

    std::stringstream fps;
    std::string fpsString;

    fps << clock.getFps();

    fps >> fpsString;

    std::cout << fpsString << std::endl;


    //int y = Gamedata::getInstance().getXmlInt("view/height");

    io.writeText(fpsString, 100, 100);

    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}

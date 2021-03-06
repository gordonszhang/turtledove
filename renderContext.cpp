#include "renderContext.h"

RenderContext::RenderContext() : factory(FrameFactory::getInstance()),
	window(nullptr), renderer(nullptr),
	WIDTH(Gamedata::getInstance().getXmlInt("view/width")),
	HEIGHT(Gamedata::getInstance().getXmlInt("view/height")) {
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
	  throw (std::string("Could not init SDL: ") + SDL_GetError());
 	}
  window = initWindow();
  renderer = initRenderer();
}

RenderContext::~RenderContext() {
  delete FrameFactory::getInstance();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow( window );
  SDL_Quit();
}

RenderContext* RenderContext::getInstance() {
  if ( instance ) return instance;
  instance = new RenderContext;
  return instance;
}

SDL_Window* RenderContext::initWindow( ) {
	window = SDL_CreateWindow( "Turtledove", SDL_WINDOWPOS_CENTERED,
             SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN );
  if( window == NULL ) {
    throw (std::string("Couldn't make a window: ")+SDL_GetError());
  }
  return window;
}

SDL_Renderer* RenderContext::initRenderer() {
  // To test the Clock class's ability to cap the frame rate, use:
  // SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Renderer* renderer =
    SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  if ( renderer == NULL ) throw std::string("No Renderer");
  return renderer;
}

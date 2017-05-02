// Brian Malloy        Data-Driven Object oriented Game Construction
#include "engine.h"

FrameFactory*  FrameFactory::instance = NULL;
RenderContext* RenderContext::instance = NULL;

bool restart = true;

int main(int, char*[]) {

   try {
      while(restart) {
        Engine engine;
        restart = false;
        engine.play();
      }
      delete RenderContext::getInstance();

   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}

#include "ui.h"
#include "render.h"
#include "gwindow.h"
#include "uiConstants.h"

UI::UI(Renderer *renderer, gWindow *window)
{
  _quit = false;
  attach(renderer);
  attach(window);

  lastx = lasty = -1.0;
}

void UI::attach(Renderer *renderer)
{
   this->renderer = renderer;
}

void UI::attach(gWindow *window)
{
   this->gwindow = window;
}

int UI::render()
{
   bool r = renderer->update();
   return r? renderer->render() : r;
}

void UI::handleKeys(int key, float x, float y )
{
   switch (key)
   {
      case CHARDEF(Q):
      case KEYDEF(ESCAPE):
            _quit = true;
            break;
      case KEYDEF(UP):
            renderer->zoom(true);
            std::cout<<"up pressed\n";
            break;
      case KEYDEF(DOWN):
            renderer->zoom(false);
            std::cout<<"down pressed\n";
            break;
      case CHARDEF(T):
            std::cout<<"t pressed\n";
            this->renderer->switch_model();
            break;
   }
}


void UI::handleMouseDown(int button, float x, float y )
{
   lastx = x; lasty = y;
   if(button != 0) return;
}

void UI::handleMouseUp(int button, float x, float y )
{
   // std::cout<<"change in x "<<x-lastx<<std::endl;
   lastx = x; lasty = y;
   if(button != 0) return;
}


void UI::handleMouseMotion(float x, float y)
{
   // std::cout<<x<<" "<<y<<"\n";
   renderer->orient((x-lastx),(y-lasty));
   lastx = x; lasty = y;
}

void UI::handleResize(int width, int height)
{
    renderer->resize(width, height);
}

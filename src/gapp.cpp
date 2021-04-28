#include <stdio.h>
#include "GL/glew.h"

#include "uiGLFW.h"
#include "gwindowGLFW.h"
#include "render.h"
#include "scene.h"
#include <cstring>

int main( int argc, char* args[] )
{
   bool mipmap_type = 0;//box
   if(argc == 2)
   {
      if(strcmp(args[1],"-t")==0)
         mipmap_type = 1; //triangle
   }

   gWindow_GLFW window("Assignment3"); 
   int M, m;
   glGetIntegerv(GL_MAJOR_VERSION, &M);
   glGetIntegerv(GL_MINOR_VERSION, &m);
   printf("OpenGL version: %d.%d\n", M, m);

   Scene scene("../meshes/meshes.txt",mipmap_type);                 // Should come from a file. Temporary scene, with default shader
   Renderer renderer(window.Width(), window.Height(), &scene); // Renderer renders scene from its camera
   UI_GLFW ui(&renderer, &window);	// User interface: intermediary between a renderer and a window
   window.renderloop(renderer);		// Keep rendering until an "End condition"
   return 0;
}
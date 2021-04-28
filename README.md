# Info

The following features have been implemented:

1. User-generated mipmaps
2. Stencils (for reflection)
3. Shadow mapping
4. Normal maps (Tiled floor)
5. Illumination models (press 't' to switch):
  a. Blinn phong
  b. Cook torrance
6. Four direction lights located right above the table corners
7. Serperate mesh creating executable (meshes are saved in the meshes/meshes.txt file)
8. User interaction
  a. User mouse to hover around
  b. Up and down arrow keys to zoom in & out
  c. Esc to exit
9. Texture mapping
10. Rendering reflections

# Sample image
![sample](https://github.com/hs2041/Rasterization-using-opengl/blob/main/sample%20images/save.png)

Both the illumination model required in the last assignment have been added. Sample images provided.

System dependencies:
1. Glut
2. opengl
3. glfw

sudo apt-get install freeglut3*
sudo apt-get install libglfw3*

Use the following commands make and compile the code:
1. ./run.sh

I am using the key 't' to switch between illumination models
use "-t" for triangle filters

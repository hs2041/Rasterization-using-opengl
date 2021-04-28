#include "geombase.h"
#include <fstream>
#include <iostream>
#include "Geometry.h"
#include <vector>
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Renderer;

class Geom : public GeomBase {
public:
   std::ifstream infile;
   int index = 0;
   int reflect_index = 0;
   unsigned int texture1, texture2 ;
   bool mipmap_type = false; //set to squares
   //shadow material
    unsigned int depthMapFBO1, depthMapFBO2, depthMapFBO3, depthMapFBO4;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMap1, depthMap2, depthMap3, depthMap4;
   unsigned int planeVAO;

   int read(const char *filename);
   int render(Renderer *renderer, glm::mat4 rendermat,glm::vec3 cam_pos, bool model_type, float width, float height) const;
   int renderscene(Renderer *renderer, glm::mat4 rendermat,glm::vec3 cam_pos, bool model_type, float width, float height) const;
   // void renderScene(Shader *shader, unsigned int planeVAO);

   Geom(const char *filename=NULL, bool mip_type = false):mipmap_type(mip_type) { read(filename); }
   bool add_texture(const char* filename, GLuint &id);
   void add_shadowbuffer(unsigned int &mapfbo,unsigned int &map);
   glm::mat4 shadow_map(const glm::vec3 &lightpos, const unsigned int &depthfbo, Renderer *renderer, float &width, float &height) const;
   unsigned char* convolve(unsigned char* data, int height, int width,int o_h, int o_w);
};


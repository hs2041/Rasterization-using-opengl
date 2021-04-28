#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scenebase.h"
#include "camera.h"
#include "geombase.h"
#include "render.h"

int SceneBase::render(Renderer *renderer, const Camera &camera, bool model_type,float width, float height, const float *mat)
{
   if(mat == NULL) 
      return render(renderer, camera, model_type, width, height, glm::mat4(1.0f));
   else
      return render(renderer, camera, model_type, width, height, glm::make_mat4(mat));
}

int SceneBase::render(Renderer *renderer, const Camera &camera, bool model_type,float width, float height, glm::mat4 mat) {
   glm::mat4 nextmat = mat * objmat; // objmat must not be NULL
   auto cam = camera.getposition();
   object->render(renderer, camera.matrix(nextmat),cam,model_type, width, height);
   bool result = true;
   for(SceneBase* cur = firstchild; cur != NULL; cur = cur->sibling) {
      result &= cur->render(renderer, camera, model_type,width, height, nextmat);
   }
   return result;
}

SceneBase* SceneBase::addchild(const GeomBase *obj, glm::mat4 mat) {
   SceneBase *sn = new SceneBase(obj, mat, firstchild, NULL);
   firstchild = sn;
   return sn;
}

void SceneBase::init(const GeomBase *obj, glm::mat4 mat, SceneBase *next, SceneBase *child)
{
   set(obj); set(mat);
   this->sibling = next;
   this->firstchild = child;
}

SceneBase::SceneBase(const GeomBase *obj, glm::mat4 mat, SceneBase *next, SceneBase *child)
{
   init(obj, mat, next, child);
}

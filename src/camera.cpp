#include "camera.h"

Camera::Camera(int width, int height, float fov, float near, float far)
{
    // projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
    projection(width, height, fov, near, far); 
    position(60.0, 50.0, 30.0, false);
    lookat(0.0, 0.0, 0.0, false);
    upward(0.0, 0.0, 1.0, true);

    // orientation = glm::quat(up);
}

void Camera::calcProjView()
{
   viewmat = glm::lookAt(pos, at, up);
   pvmat = projmat * viewmat;
}

void Camera::view(glm::vec3 pos, glm::vec3 at, glm::vec3 up)
{
   viewmat = glm::lookAt(pos, at, up);
}

void Camera::projection(int width, int height, float fovd, float near, float far, bool update) 
{
   projmat = glm::perspective(glm::radians(fovd), (float) width / (float)height, near, far);
   if(update) calcProjView();
}

void Camera::position(float x, float y, float z, bool update)
{
   pos = glm::vec3(x, y, z);
   if(update) calcProjView();
}

glm::vec3 Camera::getposition() const
{
   return pos;
}

void Camera::zoom(bool input, bool update)
{
   float mul;
   if(input)
      mul = 0.9;
   else
      mul = 1.1;
   pos.x = pos.x*mul;
   pos.y = pos.y*mul;
   pos.z = pos.z*mul;
   if(update) calcProjView();
}  

void Camera::orient(float a, float b, bool update)
{
   //Latitude
   pos.x = pos.x*cos(a) + pos.y*sin(a);
   pos.y = -pos.x*sin(a) + pos.y*cos(a);
   
   //Longitude
   //change direction of upward and update position
   glm::quat temp(cos(b/100.0),sin(b/100.0)*pos.x,sin(b/100.0)*(-pos.y),0.0);
   pos = temp*pos;

   if(update) calcProjView();
}

void Camera::lookat(float x, float y, float z, bool update)
{
   at = glm::vec3(x, y, z);
   if(update) calcProjView();
}

void Camera::upward(float x, float y, float z, bool update)
{
   up = glm::vec3(x, y, z);
   if(update) calcProjView();
}

const glm::mat4 Camera::viewmatrix()
{
   return viewmat; 
}

const glm::mat4 Camera::projmatrix()
{
   return projmat; 
}

const glm::mat4 Camera::pvmatrix()
{
   return pvmat;
}

const glm::mat4 Camera::matrix() const
{
    // return glm::mat4(1.0f); // Test identity
    // return projmat * viewmat; // Remember, matrix multiplication is the other way around
    return pvmat; // Remember, matrix multiplication is the other way around
}

const glm::mat4 Camera::matrix(const glm::mat4 modelmat) const
{
    // ModelViewProjection
    return pvmat * modelmat; // Remember, matrix multiplication is the other way around
}

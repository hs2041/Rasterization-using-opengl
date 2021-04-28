// #include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#pragma once

class Camera {

public:

    Camera(int width, int height, float fov, float near, float far);

    void calcProjView();
    void view(glm::vec3 pos, glm::vec3 at, glm::vec3 up);
    void projection(int width, int height, float fovd, float near, float far, bool update=true) ;
    void position(float x, float y, float z, bool update=true);
    glm::vec3 getposition() const;

    void zoom(bool input, bool update=true);
    void orient(float a, float b, bool update=true);
    
    void lookat(float x, float y, float z, bool update=true);
    void upward(float x, float y, float z, bool update=true);
    const glm::mat4 viewmatrix();
    const glm::mat4 projmatrix();
    const glm::mat4 pvmatrix();
    const glm::mat4 matrix(const glm::mat4 modelmat) const;
    const glm::mat4 matrix() const;

private:
    glm::mat4 projmat;
    glm::mat4 viewmat;
    glm::mat4 pvmat;
    glm::vec3 pos, at, up;
};

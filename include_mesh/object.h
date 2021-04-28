#ifndef OBJECT_H
#define OBJECT_H

#include "Geometry.h"
#include <fstream>
#include <vector>
#include <cmath>
#include <iostream>
#define circle_res 20
#define s_res 20
#define s_h 20

class Object
{
    public:
        Vector3f pose_origin;
        virtual void save_mesh(std::ofstream &input_file) = 0;
        int vertex_count;
        float reflect = 1; //change to -1 for reflect
        // static std::ofstream *my_file;
};

class Cuboid: public Object
{
    public:
    // Cuboid(Cuboid,float ref):reflect(ref),;
    Vector3f sides;
    void save_mesh(std::ofstream &input_file);
};

class Cylinder: public Object
{
    public:
    float radius;
    float height;
    void save_mesh(std::ofstream &input_file);
};
class Tumbler: public Object
{
    public:
    // Tumbler();
    float inner_r;
    float outer_r;
    float height;
    void save_mesh(std::ofstream &input_file);
};
class Bowl: public Object
{
    public:
    // Bowl();
    float inner_r;
    float outer_r;
    void save_mesh(std::ofstream &input_file);
};


#endif
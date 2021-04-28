#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <fstream>

template <typename T>
class Vector3{
    public:
        T p[3];
    Vector3(T a, T b, T c)
    {
        p[0] = a;
        p[1] = b;
        p[2] = c;
    }
    Vector3()
    {
        p[0] = 0.0;
        p[1] = 0.0;
        p[2] = 0.0;
    }
    // Vector3f(std::ifstream in_file)
    // {
    //     in_file>>p[0];
    //     in_file>>p[1];
    //     in_file>>p[2];
    // }
   
    T operator [](int index) {
        return p[index];
    }    
    void show()
    {
        std::cout<<"x = "<<p[0]<<"y = "<<p[1]<<"z = "<<p[2]<<std::endl;
    }
};

using Vector3f = Vector3<float>;
using Vector3i = Vector3<int>;

#endif
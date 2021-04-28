#include "object.h"
#include <vector>
#include <fstream>

using namespace std;

int main()
{
    std::vector<Object*> list_obj;

    std::ofstream myfile;

    Cuboid base;
    Cuboid table_base;
    Cuboid table_legs[4];
    Cylinder disk;
    Cylinder tumbler_fluid[4];
    Tumbler tumbler[4];
    Bowl bowl;

    base.pose_origin = Vector3f(0.0,0.0,-33.0);
    base.sides = Vector3f(100.0,100.0,2);
    table_base.pose_origin = Vector3f(0.0,0.0,-1.0);
    table_base.sides = Vector3f(75.,75.,2.);

    std::vector<float> signs{1, 1, -1, -1, 1};
    for(int i = 0;i<4;i++)
    {
        table_legs[i].pose_origin = Vector3f(signs.at(i)*35.5,signs.at(i+1)*35.5,-17.);
        table_legs[i].sides = Vector3f(2.,2.,30.);
        tumbler_fluid[i].pose_origin = Vector3f(signs.at(i)*10.,signs.at(i+1)*10.,0.1);
        tumbler_fluid[i].radius = 1.9;
        tumbler_fluid[i].height = 5.0;

        tumbler[i].pose_origin = Vector3f(signs.at(i)*10.,signs.at(i+1)*10.,0.);
        tumbler[i].inner_r = 2.0;
        tumbler[i].outer_r = 3.0;
        tumbler[i].height = 10.0;
    }

    disk.pose_origin = Vector3f(0.,0.,0.);
    disk.radius = 10.0;
    disk.height = 1.0;

    bowl.pose_origin = Vector3f(0.,0.,7.0);
    bowl.inner_r = 4.0;
    bowl.outer_r = 5.0;
    


    myfile.open("../meshes/meshes.txt");
    
    //added Base
    list_obj.push_back(new Cuboid(base));
    //added Table Base
    list_obj.push_back(new Cuboid(table_base));
    // //added Table legs
    for(int i=0;i<4;i++){
        list_obj.push_back(new Cuboid(table_legs[i]));
    }
    //Added Disk
    list_obj.push_back(new Cylinder(disk));
    // //Added Bowl
    list_obj.push_back(new Bowl(bowl));

    for(int i=0;i<4;i++)
        list_obj.push_back(new Cylinder(tumbler_fluid[i]));
    for(int i=0;i<4;i++)
        list_obj.push_back(new Tumbler(tumbler[i]));
    
    // disk.reflect = -1.0;
    // list_obj.push_back(new Cylinder(disk)); 
    // base.reflect =  -1.0;
    // list_obj.push_back(new Cuboid(base));
    myfile<<std::to_string(list_obj.size())<<"\n";
    for(int i=0;i<list_obj.size();i++)
    {
        Vector3f tf = list_obj.at(i)->pose_origin;
        tf.show();
        
        myfile<<tf.p[0]<<" "<<tf.p[1]<<" "<<tf.p[2]<<"\n";
        list_obj.at(i)->save_mesh(myfile);
    }
    for(auto temp:list_obj)
    {
        delete temp;
    }

    return 0;
}
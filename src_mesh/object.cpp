#include "object.h"

void add_uv(std::ofstream &input_file,bool use=true,int mod=0)
{
    // return;
        input_file<<std::to_string(-1.0)<<" "
                <<std::to_string(-1.0)<<" "
                <<std::to_string(-1.0)<<" "<<"\n";
        
}

void Cuboid::save_mesh(std::ofstream &input_file)
{
    //Vertex count
    input_file<<"8"<<"\n";

    for(int i=1;i>-2;i-=2)
        for(int j=1;j>-2;j-=2)
            for(int k=1;k>-2;k-=2)
                input_file<<std::to_string(i*sides[0]/2)<<" "
                          <<std::to_string(j*sides[1]/2)<<" "
                          <<std::to_string(k*sides[2]/2)<<" "<<"\n";
    //Triangle count
    input_file<<"12"<<"\n";
    std::vector<int> steps{4,2,1};
    std::vector<std::vector<int>> indices{{0,1,2,3},{0,1,4,5},{0,2,4,6}};
    float norm[3] = {0.0,0.0,0.0};

    int k = 1;
    for(int k=0;k<indices.size();k++)
    {
        auto v = indices.at(k);
        int j=0;
        for(int j=0;j<2;j++)
            for(int i=0;i<2;i++){
                for(int m=0;m<3;m++)
                    {
                        input_file<<std::to_string(v.at(m+i)+j*steps.at(k))<<" ";
                    }
                input_file<<"\n";
                    add_uv(input_file);
                    norm[k] = 1.0;
                    input_file<<std::to_string((float)(1-j*2)*norm[0])<<" "<<std::to_string((float)(1-j*2)*norm[1])<<" "<<std::to_string((float)(1-j*2)*norm[2])<<"\n";
                    norm[k] = 0.0;
                
                }
    }
}
//mod 0 means outward normal for cylinders
//mod 0 dir -1 means inward normals for cylinders 
//mod 1 is for spheres
//mod 2 is upward phasing surface
void curve_handler(std::ofstream &input_file, int s1, int s2, int mod = 0,int dir = 1, float full_r = 0.0,int h_i=0)
{
    float norm[3] = {0.0,0.0,0.0};

    for(int j=0;j<circle_res;j++)
    {   
        if(mod == 0)
        {
            norm[0] = cos(2*M_PI*((float)j/(float)circle_res));
            norm[1] = sin(2*M_PI*((float)j/(float)circle_res));
        }
        if(mod == 1)
        {
            norm[0] = cos(2*M_PI*((float)j/(float)circle_res));
            norm[1] = sin(2*M_PI*((float)j/(float)circle_res));
            norm[2] = -(float)h_i/(float)s_h;
        }
        if(mod == 2)
        {
            norm[2] = dir;
        }

        if(j==circle_res-1){
            input_file<<std::to_string(j+s1)<<" "
            <<std::to_string(s1)<<" "
            <<std::to_string(j+s2)<<" "<<"\n";
            add_uv(input_file);
            input_file<<std::to_string((float)dir*norm[0])<<" "<<std::to_string((float)dir*norm[1])<<" "<<std::to_string((float)dir*norm[2])<<"\n";

            input_file<<std::to_string(s1)<<" "
            <<std::to_string(j+s2)<<" "
            <<std::to_string(s2)<<" "<<"\n";
            add_uv(input_file);
            input_file<<std::to_string((float)dir*norm[0])<<" "<<std::to_string((float)dir*norm[1])<<" "<<std::to_string((float)dir*norm[2])<<"\n";
        }
        else{
            input_file<<std::to_string(j+s1)<<" "
            <<std::to_string(j+1+s1)<<" "
            <<std::to_string(j+s2)<<" "<<"\n";
            add_uv(input_file);
            input_file<<std::to_string((float)dir*norm[0])<<" "<<std::to_string((float)dir*norm[1])<<" "<<std::to_string((float)dir*norm[2])<<"\n";

            input_file<<std::to_string(j+1+s1)<<" "
            <<std::to_string(j+s2)<<" "
            <<std::to_string(j+1+s2)<<" "<<"\n";
            add_uv(input_file);
            input_file<<std::to_string((float)dir*norm[0])<<" "<<std::to_string((float)dir*norm[1])<<" "<<std::to_string((float)dir*norm[2])<<"\n";
        }
    }
}
void Cylinder::save_mesh(std::ofstream &input_file)
{
    //Vertex count
    input_file<<std::to_string(circle_res*2+2)<<"\n";
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<circle_res;j++)
        {
            input_file<<std::to_string(radius*cos(2*M_PI*(float)j/circle_res))<<" "
                      <<std::to_string(radius*sin(2*M_PI*(float)j/circle_res))<<" "
                      <<std::to_string(i*height)<<" "<<"\n";
        }    
    }
    for(int i=0;i<2;i++)
        input_file<<std::to_string(0.0)<<" "
        <<std::to_string(0.0)<<" "
        <<std::to_string(i*height)<<" "<<"\n";
    //Triangle count
    input_file<<std::to_string(circle_res*4)<<"\n";
    //circular area
    for(int i=2*circle_res;i<2*circle_res+2;i++)
    {
        for(int j=0;j<circle_res;j++)
        {  
            if(j==circle_res-1){
             input_file<<std::to_string(i)<<" "
                      <<std::to_string(j+circle_res*(i%(2*circle_res)))<<" "
                      <<std::to_string(j+1-circle_res+circle_res*(i%(2*circle_res)))<<" "<<"\n";
             add_uv(input_file);
            input_file<<std::to_string((1-(i%2)*2)*1.0)<<" "<<std::to_string((1-(i%2)*2)*0.0)<<" "<<std::to_string((1-(i%2)*2)*0.0)<<"\n";

            }
            else{
             input_file<<std::to_string(i)<<" "
                      <<std::to_string(j+circle_res*(i%(2*circle_res)))<<" "
                      <<std::to_string(j+1+circle_res*(i%(2*circle_res)))<<" "<<"\n";
             add_uv(input_file);
            input_file<<std::to_string((1-(i%2)*2)*1.0)<<" "<<std::to_string((1-(i%2)*2)*0.0)<<" "<<std::to_string((1-(i%2)*2)*0.0)<<"\n";
            }
        }
    }
    //curved surface
    curve_handler(input_file, 0, circle_res,0);

}


void Tumbler::save_mesh(std::ofstream &input_file)
{
   //Vertex count
    input_file<<std::to_string(circle_res*4)<<"\n";  
    
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<circle_res;j++)
        {
            input_file<<std::to_string(inner_r*cos(2*M_PI*(float)j/circle_res))<<" "
                      <<std::to_string(inner_r*sin(2*M_PI*(float)j/circle_res))<<" "
                      <<std::to_string(i*height)<<" "<<"\n";
        }
        for(int j=0;j<circle_res;j++)
        {    
            input_file<<std::to_string(outer_r*cos(2*M_PI*(float)j/circle_res))<<" "
                      <<std::to_string(outer_r*sin(2*M_PI*(float)j/circle_res))<<" "
                      <<std::to_string(i*height)<<" "<<"\n";
        }
    }

    //Triangle count
    input_file<<std::to_string(circle_res*8)<<"\n";

    curve_handler(input_file,0,circle_res,2,-1.0);
    curve_handler(input_file,circle_res,3*circle_res,0);
    curve_handler(input_file,0,2*circle_res,0,-1.0);
    curve_handler(input_file,2*circle_res,3*circle_res,2);
}

void Bowl::save_mesh(std::ofstream &input_file)
{
    //Vertex count
    const int inner_z = 2*s_res*s_h;
    input_file<<std::to_string(2*s_res*s_h+2)<<"\n";
    float current_height = 0.0;
    float current_radius = inner_r;
    for(int layer = 0;layer < 2;layer++)
    {   float local_radius;
        switch(layer){
            case 0: local_radius = inner_r;break;
            case 1: local_radius = outer_r;break;
            default: std::cout<<"how is this possible\n";
        }

        current_radius = local_radius;

        for(int i=0; i<s_h;i++)
        {
            current_height = local_radius*((float)i/(float)(s_h-1));
            current_radius = std::sqrt(std::pow(local_radius,2)-std::pow(current_height,2));
            for(int j=0;j<s_res;j++)
            {
                input_file<<std::to_string(current_radius*cos(2*M_PI*(float)j/s_res))<<" "
                        <<std::to_string(current_radius*sin(2*M_PI*(float)j/s_res))<<" "
                        <<std::to_string(-current_height)<<" "<<"\n";
            }    
        }
    }
    input_file<<std::to_string(0.0)<<" "<<std::to_string(0.0)<<" "<<std::to_string(-inner_r)<<" "<<"\n";
    input_file<<std::to_string(0.0)<<" "<<std::to_string(0.0)<<" "<<std::to_string(-outer_r)<<" "<<"\n";
    //Triangle count
    // input_file<<std::to_string(2*s_res+2*(s_h-1)*s_res+ 2*s_res)<<"\n";
    input_file<<std::to_string(2*s_res+(s_h-1)*s_res*2*2)<<"\n";
    curve_handler(input_file,0,circle_res*s_h,2);
    
    // for(int k=0;k<s_h-1;k++)
    // {
    //     curve_handler(input_file,k*s_res, (k+1)*s_res);

    // }

    for(int layer = 0;layer < 2;layer++)
    {
        switch(layer){
            case 0: current_radius = inner_r;break;
            case 1: current_radius = outer_r;break;
            default: std::cout<<"how is this possible\n";
        }

        for(int k=0;k<s_h-1;k++)
        {
            curve_handler(input_file,k*s_res+layer*s_h*s_res, (k+1)*s_res+layer*s_h*s_res,1,(float)layer*2-1.0,current_radius,k);
        }
    }
    // {
    //     for(int i=0;i<s_h-1;i++)
    //     {
    //         curve_handler(input_file,i+layer*s_h*s_res,(i+1)*s_res+layer*s_h*s_res);
    //     }
    //     // for(int i=0;i<s_res;i++)
    //     // {
    //     //     if(i==s_res-1)
    //     //     input_file<<std::to_string(s_h*s_res+i)<<" "<<std::to_string(s_h*s_res+i+1-s_res)<<" "<<std::to_string(inner_z+layer)<<" "<<"\n";
    //     //     else
    //     //     input_file<<std::to_string(s_h*s_res+i)<<" "<<std::to_string(s_h*s_res+i+1)<<" "<<std::to_string(inner_z+layer*s_h*s_res)<<" "<<"\n";
            
    //     // }
    // }
}


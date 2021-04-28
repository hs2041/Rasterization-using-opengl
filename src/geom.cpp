#include "geom.h"
#include "render.h"
#include "shader.h"
#include <stb_image.h>
// #include <learnopengl/filesystem.h>
using namespace std;

void renderScene(Shader *shader, unsigned int planeVAO, int reflect_index);
void renderCube();
void renderQuad();

// void generate_mippy()
unsigned char* image_corrector(unsigned char* data1, int &width, int &height){
            
    int o_w = width;
    int o_h = height;
    width = 2048;
    height = 2048;
    unsigned char *new_data = (unsigned char*)malloc(width*height*3*sizeof(unsigned char));  

    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
                for(int layer=0;layer<3;layer++){
                    new_data[i*width*3+j*3+layer] = data1[i*o_w*3+j*3+layer];

            }
        }
    }

    return new_data;        
}
unsigned char* Geom::convolve(unsigned char* data, int height, int width,int o_h, int o_w)
{
    //sqaure filter
    // unsigned char *kernel = (unsigned char*)malloc(5*5*sizeof(unsigned char));
    unsigned char *new_data = (unsigned char*)malloc(width*height*3*sizeof(unsigned char));  
    
    if(mipmap_type == false)
    {

        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                for(int layer=0;layer<3;layer++)
                {
                // int index = 3*(width*(2*i)+2*j)+layer;
                new_data[3*(width*(i)+j)+layer] = 
                    (data[3*(o_w*(2*i)+2*j)+layer]+data[3*(o_w*(2*i)+2*j)+layer+3]+data[3*(o_w*(2*i)+2*j)+3*o_w+layer]+data[3*(o_w*(2*i)+2*j)+3*o_w+layer+3])/4;
                }
            }
        }
    }
    else
    {
        std::cout<<"Triangle filter in action\n";
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                for(int layer=0;layer<3;layer++)
                {
                // int index = 3*(width*(2*i)+2*j)+layer;
                if(i<=1 || i>= (height-2) || j<=1 || j>= (width-2)){
                new_data[3*(width*(i)+j)+layer] = 
                    (data[3*(o_w*(2*i)+2*j)+layer]+data[3*(o_w*(2*i)+2*j)+layer+3]+data[3*(o_w*(2*i)+2*j)+3*o_w+layer]+data[3*(o_w*(2*i)+2*j)+3*o_w+layer+3])/4;
                }
                else{
                    // std::cout<<"ever here\n";
                    new_data[3*(width*(i)+j)+layer] = (
                    4*(data[3*(o_w*(2*i)+2*j)+layer]+data[3*(o_w*(2*i)+2*j)+layer+3]+data[3*(o_w*(2*i)+2*j)+3*o_w+layer]+data[3*(o_w*(2*i)+2*j)+3*o_w+layer+3])+
                    (data[3*(o_w*(2*i)+2*j)+layer-3]+data[3*(o_w*(2*i)+2*j)+layer+6]+data[3*(o_w*(2*i)+2*j)+layer+3*o_w-3]+data[3*(o_w*(2*i)+2*j)+layer+3*o_w+6])+
                    (data[3*(o_w*(2*i)+2*j)+layer-3*o_w-3]+data[3*(o_w*(2*i)+2*j)+layer-3*o_w]+data[3*(o_w*(2*i)+2*j)+layer-3*o_w+3]+data[3*(o_w*(2*i)+2*j)+layer-3*o_w+6])+
                    (data[3*(o_w*(2*i)+2*j)+layer+6*o_w-3]+data[3*(o_w*(2*i)+2*j)+layer+6*o_w]+data[3*(o_w*(2*i)+2*j)+layer+6*o_w+3]+data[3*(o_w*(2*i)+2*j)+layer+6*o_w+6])
                    )/28;
                }
                }
            }
        }
    }

    // for(int layer = 0;layer<3;layer++)
    // {
          
    // }
    // std::cout<<"data convolved\n";

    return new_data;
}
bool Geom::add_texture(const char* filename, GLuint &id)
{
    bool is=0;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	int width, height, nrChannels;
    unsigned char *data1 = stbi_load(filename, &width, &height, &nrChannels, 0);
	// unsigned char *data = stbi_load("data/wood.jpg", &width, &height, &nrChannels, 0);
    int o_h = height;
    int o_w = width;
    std::cout<<"image resolution: "<<height<<" "<<width<<"\n";
	if (data1)
	{   int level = 0;
        // width = 1024*2;
        // height = 1024*2;
        if(width != height)
            data1 = image_corrector(data1,width,height);
        do{
    	    glTexImage2D(GL_TEXTURE_2D, level++, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
            o_w = width;
            o_h = height;
            height =height/2;
            width = width/2;
            if(width == 0)
                width = 1;
            if(height == 0)
                break;
            data1 = convolve(data1,height,width,o_h,o_w);
        }while(height>0);

        // glGenerateMipmap(GL_TEXTURE_2D);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10);
        is = 1;
	}
	else
	{
	    std::cout << "Failed to load wood texture" << std::endl;
        is = 0;
    }
	stbi_image_free(data1);
    return is;
}

void Geom::add_shadowbuffer(unsigned int &mapfbo,unsigned int &map)
{
    glGenFramebuffers(1, &mapfbo);  

        glGenTextures(1, &map);
        glBindTexture(GL_TEXTURE_2D, map);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                    SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  

        glBindFramebuffer(GL_FRAMEBUFFER, mapfbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, map, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

int Geom::read(const char *filename)
{
   infile.open(filename,std::ifstream::in);
   
   int object_count;
   int index2 = 0;
   infile>>object_count;
    Vector3f tf,temp,tempf;
    Vector3i tempi;
    int temp_uv_size = 0;

    float points[400000];
    cout<<"object_count"<<object_count<<"\n";
    for(int i=0;i<object_count;i++)
    {     
        int vertex_count,triangle_count;
        std::vector<Vector3f> vertex_list;
        std::vector<Vector3i> triangle_list;
        std::vector<Vector3f> uv_list;
        std::vector<Vector3f> normal_list;
        
        infile>>tf.p[0]>>tf.p[1]>>tf.p[2];
        // cout<<"transform: ";
        // tf.show();
        infile>>vertex_count;
        // cout<<"vertex_count"<<vertex_count<<"\n";
        for(int j=0;j<vertex_count;j++)
        {
            for(int n=0;n<3;n++)
                infile>>temp.p[n];
            vertex_list.push_back(temp);
        }

        infile>>triangle_count;
        // cout<<"triangle_count"<<triangle_count<<"\n";
        for(int j=0;j<triangle_count;j++)
        {
            for(int n=0;n<3;n++)
                infile>>tempi.p[n];
            triangle_list.push_back(tempi);
            // tempi.show();
            for(int n=0;n<3;n++)
                infile>>tempf.p[n];
            uv_list.push_back(tempf);
            for(int n=0;n<3;n++)
                infile>>tempf.p[n];
            normal_list.push_back(tempf);
                
        }
        // std::cout<<"present uv size = "<<triangle_list.size()-uv_list.size()<<"\n";
        for(int j=0;j<triangle_list.size();j++){
            for(int k=0;k<3;k++)
            {   
                // std::cout<<"triangle count "<<j<<" ";
                temp = vertex_list.at(triangle_list.at(j).p[k]);
                // temp.show();
                for(int l=0;l<3;l++)
                {
                    points[index++] = temp.p[l]+tf.p[l]; 
                }
                //uv elements
                for(int l=0;l<3;l++)
                {   
                    points[index++] = uv_list.at(j).p[l];//temp_uv.at(index2++);
                }
                //normals
                for(int l=0;l<3;l++)
                {   
                    points[index++] = normal_list.at(j).p[l];
                }
            }
        }
    }
    //base and table uv values
    for (int l=0;l<6;l++)
    {
        for(int k=0;k<6;k++)
        {
            points[l*36*9+k*9*6+3] =     0.0;
            points[l*36*9+k*9*6+4] =     0.0;
            points[l*36*9+k*9*6+3+9] =   0.0;
            points[l*36*9+k*9*6+4+9] =   1.0;
            points[l*36*9+k*9*6+3+9*2] = 1.0;
            points[l*36*9+k*9*6+4+9*2] = 0.0;

            points[l*36*9+k*9*6+3+9*3] = 0.0;
            points[l*36*9+k*9*6+4+9*3] = 1.0;
            points[l*36*9+k*9*6+3+9*4] = 1.0;
            points[l*36*9+k*9*6+4+9*4] = 0.0;
            points[l*36*9+k*9*6+3+9*5] = 1.0;
            points[l*36*9+k*9*6+4+9*5] = 1.0;
        }
    }

    reflect_index = index;
    // std::cout<<"original index: "<<reflect_index<<"\n";
    for(int i = 36*6*9; i <reflect_index;i++)
    {
        if((i+1)%9 == 3)
        {
            points[index++] = -points[i]-1.0;    
        }
        else
            points[index++] = points[i];
    }
   
    // std::cout<<"new index: "<<index<<"\n";
    add_texture("../data/wood.jpg",texture1);
    add_texture("../data/png2jpg/tile.jpg",texture2);
	
   shader = new Shader("../data/vs.glsl", "../data/fs.glsl");
   depthshader = new Shader("../data/vs_shadow.glsl", "../data/fs_shadow.glsl");
   debugdepth = new Shader("../data/vs_shadowDebug.glsl", "../data/fs_shadowDebug.glsl");
   

     

//    GLuint VBO = 0;
//    glGenBuffers(1, &VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);

//     //cartesian coordinates
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//     //barycentric coordinates
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//     //normals
//    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)(6 * sizeof(float)));
//    glEnableVertexAttribArray(2);    

    //shadow mapping

    add_shadowbuffer(depthMapFBO1, depthMap1);
    add_shadowbuffer(depthMapFBO2, depthMap2);
    add_shadowbuffer(depthMapFBO3, depthMap3);
    add_shadowbuffer(depthMapFBO4, depthMap4);
    
        // plane VAO

        unsigned int planeVBO;
        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindVertexArray(0);

   return vao;

}

glm::mat4 Geom::shadow_map(const glm::vec3 &lightpos, const unsigned int &depthfbo, Renderer *renderer, float &width, float &height) const
{

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 0.1f, far_plane = 250.0f;
        lightProjection = glm::ortho(-60.0f, 60.0f, -60.0f, 60.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightpos, glm::vec3(0.0f), glm::vec3(0.0,0.0, 1.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        renderer->useShader(depthshader);
        // depthshader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glUniformMatrix4fv(glGetUniformLocation(depthshader->id(), "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
        
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthfbo);
            glClear(GL_DEPTH_BUFFER_BIT);
                glm::mat4 model = glm::mat4(1.0f);
                glUniformMatrix4fv(glGetUniformLocation(depthshader->id(), "model"), 1, GL_FALSE, &model[0][0]);
                glBindVertexArray(planeVAO);
                glCullFace(GL_FRONT);
                glDrawArrays(GL_TRIANGLES, 0, reflect_index/9);
                glCullFace(GL_BACK);            
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return lightSpaceMatrix;
}

int Geom::render(Renderer *renderer, glm::mat4 rendermat,glm::vec3 cam_pos, bool model_type, float width, float height) const
{
    //shadow magic
    float dist = 75.0f;
        glm::vec3 lightPos1(-dist, dist, dist);
        glm::vec3 lightPos2(dist, dist, dist);
        glm::vec3 lightPos3(dist, -dist, dist);
        glm::vec3 lightPos4(-dist, -dist, dist);
        
        glm::mat4 lightSpaceMatrix1 =shadow_map(lightPos1,depthMapFBO1, renderer, width, height);      
        glm::mat4 lightSpaceMatrix2 =shadow_map(lightPos2,depthMapFBO2, renderer, width, height);      
        glm::mat4 lightSpaceMatrix3 =shadow_map(lightPos3,depthMapFBO3, renderer, width, height);      
        glm::mat4 lightSpaceMatrix4 =shadow_map(lightPos4,depthMapFBO4, renderer, width, height);      

        // render Depth map to quad for visual debugging
        // ---------------------------------------------
     
        renderer->useShader(shader);
        
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        glUniform1i(glGetUniformLocation(shader->id(), "texture0"), 0);
        glUniform1i(glGetUniformLocation(shader->id(), "texture1"), 1);
        glUniform1i(glGetUniformLocation(shader->id(), "depthmap1"), 2);
        glUniform1i(glGetUniformLocation(shader->id(), "depthmap2"), 3);
        glUniform1i(glGetUniformLocation(shader->id(), "depthmap3"), 4);
        glUniform1i(glGetUniformLocation(shader->id(), "depthmap4"), 5);



        glm::mat4 projection = renderer->camera.projmatrix();
        glm::mat4 view = renderer->camera.viewmatrix();

        glUniformMatrix4fv(glGetUniformLocation(shader->id(), "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader->id(), "view"), 1, GL_FALSE, &view[0][0]);
        glm::vec3 camera_pos = renderer->camera.getposition();
        glUniform3fv(glGetUniformLocation(shader->id(), "viewPos"), 1, &camera_pos[0]);

        // glUniform3fv(glGetUniformLocation(shader->id(), "lightPos1"), 1, &lightPos1[0]); 
        glUniformMatrix4fv(glGetUniformLocation(shader->id(), "lightSpaceMatrix1"), 1, GL_FALSE, &lightSpaceMatrix1[0][0]);
        
        // glUniform3fv(glGetUniformLocation(shader->id(), "lightPos2"), 1, &lightPos2[0]); 
        glUniformMatrix4fv(glGetUniformLocation(shader->id(), "lightSpaceMatrix2"), 1, GL_FALSE, &lightSpaceMatrix2[0][0]);
                
        // glUniform3fv(glGetUniformLocation(shader->id(), "lightPos3"), 1, &lightPos3[0]); 
        glUniformMatrix4fv(glGetUniformLocation(shader->id(), "lightSpaceMatrix3"), 1, GL_FALSE, &lightSpaceMatrix3[0][0]);
        
        // glUniform3fv(glGetUniformLocation(shader->id(), "lightPos4"), 1, &lightPos4[0]); 
        glUniformMatrix4fv(glGetUniformLocation(shader->id(), "lightSpaceMatrix4"), 1, GL_FALSE, &lightSpaceMatrix4[0][0]);
        
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader->id(), "model"), 1, GL_FALSE, &model[0][0]);



        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthMap1);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, depthMap2);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, depthMap3);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, depthMap4);
     
        // renderer->useShader(debugdepth);
        // renderQuad();

    renderscene(renderer,rendermat,cam_pos,model_type,width,height);

    return glGetError();
}

int Geom::renderscene(Renderer *renderer, glm::mat4 rendermat,glm::vec3 cam_pos, bool model_type, float width, float height) const
{

        shader->setXform((const GLfloat*)glm::value_ptr(rendermat));
        GLint myLoc = glGetUniformLocation(shader->id(), "camera");
        glProgramUniform3f(shader->id(), myLoc, cam_pos[0],cam_pos[1],cam_pos[2]);

        GLint myLoc2 = glGetUniformLocation(shader->id(), "light_model");
        glProgramUniform1i(shader->id(), myLoc2, model_type);

        glBindVertexArray(planeVAO);
        //floor
        glDrawArrays(GL_TRIANGLES, 0, 6*6);
        //table top
        glDrawArrays(GL_TRIANGLES, 6*6, 6*6);
        //table_legs
        glDrawArrays(GL_TRIANGLES, 6*6*2, 6*6*4);
        glDisable(GL_DEPTH_TEST);

        glStencilMask(0xFF);
        
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 
        glEnable(GL_STENCIL_TEST); 
        glStencilFunc(GL_ALWAYS, 1, 0xFF); 
        glStencilMask(0xFF);
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); 
        glDrawArrays(GL_TRIANGLES, 6*6, 6*6);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilFunc(GL_EQUAL, 1, 0xFF); 
        //reflected objects
        glDrawArrays(GL_TRIANGLES, reflect_index/9, ((index-reflect_index)/9));
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        //original objects
        glDrawArrays(GL_TRIANGLES, 6*6*6, ((reflect_index)/9));

        return glGetError();
}


// renders the 3D scene
// --------------------
void renderScene(Shader *shader, unsigned int planeVAO, int reflect_index)
{
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader->id(), "model"), 1, GL_FALSE, &model[0][0]);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, reflect_index/9);
    
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
void renderQuad()
{
    unsigned int quadVAO = 0;
    unsigned int quadVBO;

    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}


#version 410
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vertexUV;
layout(location = 2) in vec3 norm;


uniform vec3 camera;
uniform mat4 MVP;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix1;
uniform mat4 lightSpaceMatrix2;
uniform mat4 lightSpaceMatrix3;
uniform mat4 lightSpaceMatrix4;

out vec4 vcolor;
flat out int is_texture;
flat out int is_reflect;
out vec3 position;
out vec3 UV;
out vec3 normal;
out vec3 viewer;
out vec4 FragPosLightSpace1;
out vec4 FragPosLightSpace2;
out vec4 FragPosLightSpace3;
out vec4 FragPosLightSpace4;

void main() {

    position = vp;
    UV = vertexUV;
    normal = norm;
    
    FragPosLightSpace1 = lightSpaceMatrix1 * vec4(vp, 1.0);
    FragPosLightSpace2 = lightSpaceMatrix2 * vec4(vp, 1.0);
    FragPosLightSpace3 = lightSpaceMatrix3 * vec4(vp, 1.0);
    FragPosLightSpace4 = lightSpaceMatrix4 * vec4(vp, 1.0);

    gl_Position = projection * view * model * vec4(vp, 1.0);

  viewer = camera - vp;
   gl_Position =  MVP * vec4(vp,1.0);
//God mode hardcoded color assignment
  is_texture = 0;
  is_reflect = 0;

  vcolor = vec4(0.,0.,0.,1.);
//base
  if(gl_VertexID<36)
      { 
        vcolor = vec4(0.9, 0.9, 0.9, 1.0);
        is_texture = 1;
      }
//table
  else if(gl_VertexID>=36 && gl_VertexID<36+36*5)
      { 
        vcolor = vec4(0.64, 0.16, 0.16, 0.9);
        is_texture = 2;
 
      }
//disk
  else if(gl_VertexID>=36*6 && gl_VertexID<36*6+80*3)
      vcolor = vec4(0.5451, 0.5451, 0.5804, 1.0);
//bowl
  else if(gl_VertexID>=36*6+80*3 && gl_VertexID<36*6+80*3+40*(19*2*2+41))
      vcolor = vec4(0.6, 0.4902, 0.2118, 1.0);
//lemon yellow fluid
  else if(gl_VertexID>=36*6+80*3+40*(19*2*2+41) && gl_VertexID<36*6+80*3*2+40*(19*2*2+41))
      vcolor = vec4(1.0, 0.95, 0.31, 1.0);
//orange fluid
  else if(gl_VertexID>=36*6+80*3*2+40*(19*2*2+41) && gl_VertexID<36*6+80*3*3+40*(19*2*2+41))
      vcolor = vec4(1.0, 0.62, 0.47, 1.0);
//wine red fluid
  else if(gl_VertexID>=36*6+80*3*3+40*(19*2*2+41) && gl_VertexID<36*6+80*3*4+40*(19*2*2+41))
      vcolor = vec4(0.64, 0.18, 0.21, 1.0);
//colorless fluid
  else if(gl_VertexID>=36*6+80*3*4+40*(19*2*2+41) && gl_VertexID<36*6+80*3*5+40*(19*2*2+41))
      vcolor = vec4(0.3, 0.3, 0.3, 0.5);
//tumbler
  else if(gl_VertexID>=36*6+80*3*5+40*(19*2*2+41) && gl_VertexID<36*6+80*3*5+40*(19*2*2+41)+160*3*4)
      vcolor = vec4(0.6, 0.6, 0.6, 0.4);
//REFLECTED BOYS
  int breflect = 36*6+80*3*5+40*(19*2*2+41)+160*3*4;
//disk
  if(gl_VertexID>=breflect)
      is_reflect = 1;

  if(gl_VertexID>=breflect && gl_VertexID<breflect+80*3)
      vcolor = vec4(0.5451, 0.5451, 0.5804, 0.4);
//bowl
  else if(gl_VertexID>=breflect+80*3 && gl_VertexID<breflect+80*3+40*(19*2*2+41))
      vcolor = vec4(0.6, 0.4902, 0.2118, 0.6);
//lemon yellow fluid
  else if(gl_VertexID>=breflect+80*3+40*(19*2*2+41) && gl_VertexID<breflect+80*3*2+40*(19*2*2+41))
      vcolor = vec4(1.0, 0.95, 0.31, 0.6);
//orange fluid
  else if(gl_VertexID>=breflect+80*3*2+40*(19*2*2+41) && gl_VertexID<breflect+80*3*3+40*(19*2*2+41))
      vcolor = vec4(1.0, 0.62, 0.47, 0.6);
//wine red fluid
  else if(gl_VertexID>=breflect+80*3*3+40*(19*2*2+41) && gl_VertexID<breflect+80*3*4+40*(19*2*2+41))
      vcolor = vec4(0.64, 0.18, 0.21, 0.6);
//colorless fluid
  else if(gl_VertexID>=breflect+80*3*4+40*(19*2*2+41) && gl_VertexID<breflect+80*3*5+40*(19*2*2+41))
      vcolor = vec4(0.3, 0.3, 0.3, 0.5);
//tumbler
  else if(gl_VertexID>=breflect+80*3*5+40*(19*2*2+41) && gl_VertexID<breflect+80*3*5+40*(19*2*2+41)+160*3*4)
      vcolor = vec4(0.6, 0.6, 0.6, 0.4);
//  //else 
    // vcolor = vec4(0.85, 0.64, 0.12, 0.0);

}

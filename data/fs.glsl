
#version 410

in vec4 vcolor;
flat in int is_texture;
flat in int is_reflect;
in vec3 position;
in vec3 UV;
in vec3 normal;
in vec3 viewer;
in vec4 FragPosLightSpace1;
in vec4 FragPosLightSpace2;
in vec4 FragPosLightSpace3;
in vec4 FragPosLightSpace4;

out vec4 fcolor;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform sampler2D depthmap1;
uniform sampler2D depthmap2;
uniform sampler2D depthmap3;
uniform sampler2D depthmap4;


uniform vec3 l1 = vec3(0.57, 0.57, 0.57);
uniform vec3 l2 = vec3(0.57, -0.57, 0.57);
uniform vec3 l3 = vec3(-0.57, 0.57, 0.57);
uniform vec3 l4 = vec3(-0.57, -0.57, 0.57);

uniform vec3 l1_c = vec3(1.0, 1.0, 0.0);
uniform vec3 l2_c = vec3(1.0, 1.0, 0.0);
uniform vec3 l3_c = vec3(1.0, 1.0, 1.0);
uniform vec3 l4_c = vec3(1.0, 1.0, 1.0);

uniform int light_model;

// struct DirLight {
//     vec3 direction;
  
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };  
// uniform DirLight dirLight;
 
float ShadowCalculation(vec4 fragPosLightSpace, sampler2D depthmap)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthmap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 0.0 : 1.0;  
    // float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
} 

void main() {  
    
    fcolor = vec4(0.0, 0.0, 0.0,1.0);
    //base
    // viewer = cam/era - pos;
	// FragColor = mix(texture(texture1, TexCoord), texture(texture0, TexCoord), 0.2);

    vec3 norm;
    vec4 tempcolor;

    if(is_texture == 1)
    {
        norm = (texture(texture1, UV.rg)).rgb;
        norm = normalize(norm * 2.0 - 1.0);
        tempcolor = vec4(0.5529, 0.5529, 0.5529, vcolor.a);
    }
    if(is_texture == 2)
    {
        tempcolor = texture(texture0, UV.rg);
        norm = normal;
    }
    else if(is_texture == 0)
    {
        norm = normalize(normal);
        tempcolor = vcolor;
    }

    //Ambient
    float diff1 = max(dot(norm, l1), 0.0);
    vec3 diffuse1 = diff1 * l1_c;
    float diff2 = max(dot(norm, l2), 0.0);
    vec3 diffuse2 = diff2 * l2_c;
    float diff3 = max(dot(norm, l3), 0.0);
    vec3 diffuse3 = diff3 * l3_c;
    float diff4 = max(dot(norm, l4), 0.0);
    vec3 diffuse4 = diff4 * l4_c;
    //Blinn Phong's light_model
    vec4 temp2;
    if(light_model == 0){            
            vec3 h1 = normalize((viewer)+normalize(l1));
            vec3 h2 = normalize((viewer)+normalize(l2));
            vec3 h3 = normalize((viewer)+normalize(l3));
            vec3 h4 = normalize((viewer)+normalize(l4));

            temp2 = vec4((tempcolor.rgb+diffuse1*0.03+diffuse2*0.03+diffuse3*0.03+diffuse4*0.03)+
                          0.03*(dot(norm,h1)+dot(norm,h2)+dot(norm,h3)+dot(norm,h4)),
                          tempcolor.a);
    }
    else if(light_model == 1)
    {
        temp2 = vec4((tempcolor.rgb+diffuse1*0.03+diffuse2*0.03+diffuse3*0.03+diffuse4*0.03)+
                       l1_c*dot(norm,l1)*(0.4/(dot(norm,l1)*dot(norm,viewer)))+
                       l2_c*dot(norm,l2)*(0.4/(dot(norm,l2)*dot(norm,viewer)))+
                       l3_c*dot(norm,l3)*(0.4/(dot(norm,l3)*dot(norm,viewer)))+
                       l4_c*dot(norm,l4)*(0.4/(dot(norm,l4)*dot(norm,viewer))),
                        tempcolor.a);
    }

    //Shadowcraft
    if(is_reflect == 0)
    {
        float shadow1 = ShadowCalculation(FragPosLightSpace1, depthmap1);
        float shadow2 = ShadowCalculation(FragPosLightSpace2, depthmap2);
        float shadow3 = ShadowCalculation(FragPosLightSpace3, depthmap3);
        float shadow4 = ShadowCalculation(FragPosLightSpace4, depthmap4);

        fcolor = vec4(vec3(temp2.rgb)*(1.0+0.1*shadow1*l1_c+0.1*shadow2*l2_c+0.1*shadow3*l3_c+0.1*shadow4*l4_c),temp2.a);
    }
    else 
        fcolor = temp2;
}

#shader vertex
#version 430 core
layout (location = 0) in vec3 aPos;

/* Global information sent by the engine */
layout (std140, binding = 0) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
};

void main()
{
    gl_Position = ubo_Projection * ubo_View * ubo_Model * vec4(aPos, 1.0);
}  

#shader fragment
#version 430 core

layout(location = 0) out float fragmentdepth;
void main()
{             
     fragmentdepth  = gl_FragCoord.z;
}  
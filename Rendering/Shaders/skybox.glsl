#shader vertex
#version 460 core
layout (location = 0) in vec3 aPos;

/* Global information sent by the engine */
layout (std140, binding = 0) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
};

out vec3 WorldPos;

void main()
{
    WorldPos = aPos;

	mat4 rotView = mat4(mat3(ubo_View));
	vec4 clipPos = ubo_Projection * rotView * vec4(WorldPos, 1.0);

	gl_Position = clipPos.xyww;
}

#shader fragment
#version 460 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;

void main()
{		
    vec3 envColor = texture(environmentMap, WorldPos).rgb;
    
    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
    
    FragColor = vec4(envColor, 1.0);
}
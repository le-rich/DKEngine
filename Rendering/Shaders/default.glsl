#shader vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;

/* Global information sent by the engine */
layout (std140, binding = 0) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
};

out vec2 v_TexCoord;
out vec3 v_WorldPos;

void main()
{
    vec4 worldPosition = ubo_Model * vec4(position, 1.0);
    v_WorldPos = worldPosition.xyz;

    gl_Position = ubo_Projection * ubo_View * worldPosition;
    v_TexCoord = texCoord;
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;
in vec3 v_WorldPos;

layout (std140, binding = 0) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
};

uniform sampler2D u_Texture;

void main()
{
    vec4 texColour = texture(u_Texture, v_TexCoord);
    colour = texColour;
}
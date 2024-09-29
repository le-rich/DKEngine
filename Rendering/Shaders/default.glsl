#shader vertex
#version 410 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoords;

layout(location = 0) out vec2 v_TexCoords;

void main()
{
    v_TexCoords = texCoords;
    gl_Position = position;
}

#shader fragment
#version 410 core

layout(location = 0) in vec2 v_TexCoords;

layout(location = 0) out vec4 colour;

uniform sampler2D u_Texture;

void main()
{
    colour = texture(u_Texture, v_TexCoords);
}
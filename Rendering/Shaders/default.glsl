#shader vertex
#version 460 core

layout(location = 0) in vec3 position;

out vec3 fragPos;

void main()
{
    fragPos = position;
    gl_Position = vec4(position, 1.0);
}

#shader fragment
#version 460 core

in vec3 fragPos;

layout(std140, binding = 0) buffer LightSSBO
{
    mat4 ssboLights[];
};

// Uniforms
uniform float uShininess = 100.0;

// Globals
vec3 gNormal = vec3(1.0f, 0.5f, 0.5f);
vec3 gViewDir;

out vec4 FragColor;

/* Color unpack as defined in Light.cpp */
vec3 UnPackColor(float pTarget)
{
    return vec3
    (
      /* '& 0xff' to get the last 8 bits of the value */
      /* Then divide by 255 to get a value between 0 - 1 */
      float((uint(pTarget) >> 24) & 0xff) * 0.0039215686274509803921568627451,
      float((uint(pTarget) >> 16) & 0xff) * 0.0039215686274509803921568627451,
      float((uint(pTarget) >> 8)  & 0xff) * 0.0039215686274509803921568627451
    );
}

vec3 BlinnPhong(vec3 plightDir, vec3 plightColor, float pluminosity)
{
    const vec3 lightDir             = normalize(plightDir);
    const vec3 halfwayDir           = normalize(plightDir + gViewDir);
    const float diffuseCoefficient  = max(dot(lightDir, gNormal), 0.0);
    const float specularCoefficient = pow(max(dot(halfwayDir, gNormal), 0.0), uShininess * 2.0);

    vec3 diffuse  = vec3(0.2f,0.2f,1.0f) * diffuseCoefficient * plightColor * pluminosity;
    //vec3 specular = gSpecularTexel.rgb * specularCoefficient * plightColor * pluminosity;

    return diffuse;// + specular;
}

vec3 CalculateDirectionalLight(mat4 plight)
{
    return BlinnPhong(-plight[1].rgb, UnPackColor(plight[2][0]), plight[3][3]);
}

void main()
{
    gNormal = normalize(gNormal);
    gViewDir = normalize(fragPos);
    vec3 lightSum = vec3(0.0);
    for (int i = 0; i < ssboLights.length(); ++i)
    {
        lightSum += CalculateDirectionalLight(ssboLights[i]);
    }

    FragColor = vec4(lightSum, 1.f);
}
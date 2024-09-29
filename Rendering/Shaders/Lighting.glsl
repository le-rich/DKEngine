/*
    This file exists as a reference for adding BlinnPhong lighting into a shader.
    Since include directives are not natively supported in OpenGL,
    the relevent functions will need to be copied into the Fragment Shader using it.

*/
#version 430 core

/* Lighting information sent by the engine */
layout(std140, binding = 0) buffer LightSSBO
{
    mat4 ssboLights[];
};

/* Fragment shader input, may not be inputted as interface block */
in VSOUT
{
    vec3        FragPos;
    vec3        Normal;
    vec2        TexCoords;
//    vec3        TangentFragPos;
//    flat vec3   TangentViewPos;
//    mat3        TBN;
} fsin;

/* Texutre and lighting Uniforms */
uniform sampler2D uDiffuseMap;
uniform sampler2D uSpecularMap;
uniform vec4 uDiffuse    = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 uSpecular   = vec4(1.0, 1.0, 1.0, 1.0);
uniform float uShininess = 100.0;

/* Local global variables */
vec3 gNormal;
vec2 gTexCoords;
vec3 gViewDir;
vec4 gDiffuseTexel;
vec4 gSpecularTexel;

/* Output definition */
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

    vec3 diffuse  = gDiffuseTexel.rgb * diffuseCoefficient * plightColor * pluminosity;
    vec3 specular = gSpecularTexel.rgb * specularCoefficient * plightColor * pluminosity;

    return diffuse + specular;
}

vec3 Phong(vec3 plightDir, vec3 plightColor, float pluminosity)
{
    const vec3 lightDir             = normalize(plightDir);
    const vec3 reflectDir           = reflect(-lightDir, gNormal);
    const float diffuseCoefficient  = max(dot(lightDir, gNormal), 0.0);
    const float specularCoefficient = pow(max(dot(reflectDir, gViewDir), 0.0), uShininess);

    vec3 diffuse  = gDiffuseTexel.rgb * diffuseCoefficient * plightColor * pluminosity;
    vec3 specular = gSpecularTexel.rgb * specularCoefficient * plightColor * pluminosity;

    return diffuse + specular;
}

/* Calculate luminosity from attenuation values in Light matrix */
float LuminosityFromAttenuation(mat4 pLight)
{
    const vec3  lightPosition   = pLight[0].rgb;
    const float constant        = pLight[0][3];
    const float linear          = pLight[1][3];
    const float quadratic       = pLight[2][3];

    const float distanceToLight = length(lightPosition - fsin.FragPos);
    const float attenuation     = (constant + linear * distanceToLight + quadratic * (distanceToLight * distanceToLight));
    return 1.0 / attenuation;
}

vec3 CalculateAmbientLight(mat4 plight)
{
    return BlinnPhong(-plight[1].rgb, UnPackColor(plight[2][0]), plight[3][3]);
}

vec3 CalcPointLight(mat4 pLight)
{
    const vec3 lightPosition  = pLight[0].rgb;
    const vec3 lightColor     = UnPackColor(pLight[2][0]);
    const float intensity     = pLight[3][3];

    const vec3  lightDirection  = normalize(lightPosition - fsin.FragPos);
    const float luminosity      = LuminosityFromAttenuation(pLight);

    return BlinnPhong(lightDirection, lightColor, intensity * luminosity);
}

vec3 CalculateDirectionalLight(mat4 plight)
{
    return BlinnPhong(-plight[1].rgb, UnPackColor(plight[2][0]), plight[3][3]);
}

//vec3 CalculateDirectionalLight(mat4 plight)
//{
//    return BlinnPhong(-plight[1].rgb, UnPackColor(plight[2][0]), plight[3][3]);
//}
//
//vec3 CalculateDirectionalLight(mat4 plight)
//{
//    return BlinnPhong(-plight[1].rgb, UnPackColor(plight[2][0]), plight[3][3]);
//}

vec3 CalculateLightSum()
{
    vec3 lightSum = vec3(0.0);
    for (int i = 0; i < ssboLights.length(); ++i)
    {
        switch(int(ssboLights[i][3][0]))
        {
            case 0: // Ambient Light
                break;
            case 1: // Point Light
                lightSum += CalcPointLight(ssboLights[i]); 
                break;
            case 2: // Directional Light
                lightSum += CalculateDirectionalLight(ssboLights[i]);  
                break;
            case 3: // Spot Light
                break;
            case 4: // Area Light
                break;
            default: // No Light
                lightSum += vec3(gDiffuseTexel.rgb * UnPackColor(ssboLights[i][2][0]) * 0.1); 
      }
  }
    return lightSum;
}

void main()
{
  gNormal        = normalize(fsin.Normal);
  gTexCoords     = vec2(fsin.TexCoords);
  gViewDir       = normalize(fsin.FragPos);
  gDiffuseTexel  = texture(uDiffuseMap,  gTexCoords) * uDiffuse;
  gSpecularTexel = texture(uSpecularMap, gTexCoords) * uSpecular;


  FragColor = vec4(CalculateLightSum(), gDiffuseTexel.a);
}
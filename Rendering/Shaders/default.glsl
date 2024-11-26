#shader vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

/* Global information sent by the engine */
layout (std140, binding = 0) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
};

out VS_OUT
{
    vec3 v_WorldPos;
    vec3 v_Normal;
    vec2 v_TexCoord;
    vec4 v_ShadowPos;
}vs_out;

void main()
{
    vs_out.v_Normal = normal;

    vec4 worldPosition = ubo_Model * vec4(position, 1.0);
    vs_out.v_WorldPos = worldPosition.xyz;
    vs_out.v_ShadowPos = worldPosition;

    gl_Position = ubo_Projection * ubo_View * worldPosition;
    vs_out.v_TexCoord = texCoord;
}

#shader fragment
#version 460 core

in VS_OUT
{
    vec3 v_WorldPos;
    vec3 v_Normal;
    vec2 v_TexCoord;
    vec4 v_ShadowPos;
}fs_in;

layout(std140, binding = 0) buffer LightMatrixSSBO
{
    mat4 ssboLights[];
};

layout(std140, binding = 1) buffer LightViewSSBO
{
    mat4 lightSpaceMatricies[];
};

layout(std140, binding = 2) buffer LightEnabledSSBO
{
    mat4 lightsEnable[];
};

layout (std140, binding = 0) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
};

uniform sampler2D uDiffuseMap;
layout(binding = 31) uniform sampler2DArray uShadowsMap;

// Uniforms
uniform vec4 uDiffuse    = vec4(1.0, 1.0, 1.0, 1.0);
uniform float uShininess = 100.0;

// Globals
vec3 gNormal;
vec2 gTexCoords;
vec3 gViewDir;
vec4 gDiffuseTexel;
//vec4 gSpecularTexel;

layout (location = 0) out vec4 FragColor;

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

/* Calculate luminosity from attenuation values in Light matrix */
float LuminosityFromAttenuation(mat4 pLight)
{
    const vec3  lightPosition   = pLight[0].rgb;
    const float constant        = pLight[0][3];
    const float linear          = pLight[1][3];
    const float quadratic       = pLight[2][3];

    const float distanceToLight = length(fs_in.v_WorldPos - lightPosition);
    const float attenuation     = (constant + linear * distanceToLight + quadratic * (distanceToLight * distanceToLight));
    return 1.0 / attenuation;
}

vec3 BlinnPhong(vec3 plightDir, vec3 plightColor, float pluminosity)
{
    const vec3 lightDir             = plightDir;
    const vec3 halfwayDir           = normalize(plightDir + gViewDir);
    const float diffuseCoefficient  = max(dot(gNormal, -lightDir), 0.0);
    const float specularCoefficient = pow(max(dot(halfwayDir, gNormal), 0.0), uShininess * 2.0);

    vec3 diffuse  = gDiffuseTexel.rgb * diffuseCoefficient * plightColor * pluminosity;
    //vec3 specular = gSpecularTexel.rgb * specularCoefficient * plightColor * pluminosity;

    return diffuse;// + specular;
}

vec3 CalculateDirectionalLight(mat4 plight, float shadow)
{
    return BlinnPhong(-plight[1].rgb, UnPackColor(plight[2][0]), plight[3][3] * shadow);
}

vec3 CalcPointLight(mat4 pLight, float shadow)
{
    const vec3 lightPosition  = pLight[0].rgb;
    const vec3 lightColor     = UnPackColor(pLight[2][0]);
    const float intensity     = pLight[3][3];

    const vec3  lightDirection  = normalize(fs_in.v_WorldPos - lightPosition);
    const float luminosity      = LuminosityFromAttenuation(pLight);

    return BlinnPhong(lightDirection, lightColor, intensity * luminosity * shadow);
}

vec3 CalculateSpotLight(mat4 pLight, float shadow)
{
    const vec3 lightPosition  = pLight[0].rgb;
    const vec3 lightColor     = UnPackColor(pLight[2][0]);
    const float intensity     = pLight[3][3];

    const vec3  lightDirection  = normalize(fs_in.v_WorldPos - lightPosition);
    const float luminosity      = LuminosityFromAttenuation(pLight);

    vec3 lightsum = BlinnPhong(lightDirection, lightColor, intensity * luminosity * shadow);

    float theta = dot(lightDirection, -pLight[1].rgb); 
    const float epsilon = (pLight[3][1] -  pLight[3][2]);
    const float spotIntensity = clamp((theta - pLight[3][2]) / epsilon, 0.0, 1.0);

    return lightsum * spotIntensity;
}

float ShadowCalculation(int i, float bias)
{
    vec4 fragPosLightSpace = lightSpaceMatricies[i] * vec4(fs_in.v_WorldPos, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(uShadowsMap, vec3(projCoords.xy, i)).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  

    return 1 - shadow;
}

vec3 CalculateLightSum()
{
    vec3 lightSum = vec3(0.0);
    
    for (int i = 0; i < ssboLights.length(); ++i)
    {
        float shadow = 1;
        if(lightsEnable[i][0].x == 1)
        {
            const vec3  lightDirection  = normalize(ssboLights[i][0].rgb - fs_in.v_WorldPos);
            float bias = max(0.05 * (1.0 - dot(gNormal, lightDirection)), 0.005);  
            shadow = ShadowCalculation(i, bias);
        }
        switch(int(ssboLights[i][3][0]))
        {
            case 0: // Ambient Light
                lightSum += gDiffuseTexel.rgb * UnPackColor(ssboLights[i][2][0]) * ssboLights[i][3][3];
                break;
            case 1: // Point Light
                lightSum += CalcPointLight(ssboLights[i], shadow); 
                break;
            case 2: // Directional Light
                lightSum += CalculateDirectionalLight(ssboLights[i], shadow);  
                break;
            case 3: // Spot Light
                lightSum += CalculateSpotLight(ssboLights[i], shadow);  
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
    gNormal = normalize(fs_in.v_Normal);
    gTexCoords = vec2(fs_in.v_TexCoord);
    gViewDir = normalize(ubo_ViewPos - fs_in.v_WorldPos);
    gDiffuseTexel  = texture(uDiffuseMap,  gTexCoords) * uDiffuse;

    FragColor = vec4(CalculateLightSum(), gDiffuseTexel.a);
}
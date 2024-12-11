#shader vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent; // Add tangent input

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
    mat3 v_TBN;
}vs_out;

void main()
{
    vec3 worldTangent = normalize(vec3(ubo_Model * vec4(tangent, 0)));
    vec3 worldNormal = normalize(vec3(ubo_Model * vec4(normal, 0)));
    worldTangent = normalize(worldTangent - dot(worldTangent, worldNormal) * worldNormal);
    vec3 worldBitangent = cross(worldNormal, worldTangent);

    vs_out.v_TBN = transpose(mat3(worldTangent, worldBitangent, worldNormal));

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
    mat3 v_TBN;
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

layout (binding = 0) uniform sampler2D uDiffuseMap;
layout (binding = 1) uniform sampler2D uMetallicMap;          // Metallic map
layout (binding = 2) uniform sampler2D uNormalMap;            // Normal map
layout (binding = 3) uniform sampler2D uHeightMap;            // Height map
layout (binding = 4) uniform sampler2D uAmbientOcclusionMap;  // AO map
layout (binding = 5) uniform sampler2D uRoughnessMap;         // Roughness map

layout(binding = 31) uniform sampler2DArray uShadowsMap;

// Uniforms
uniform vec4 uDiffuse    = vec4(1.0, 1.0, 1.0, 1.0);

// Globals
vec3 gNormal;
vec2 gTexCoords;
vec3 gViewDir;
vec4 gDiffuseTexel;
float gMetallic;
float gRoughness;
float gAmbientOcclusion;

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

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 BlinnPhong(vec3 plightDir, vec3 plightColor, float pluminosity, vec3 F0, float roughness, float metallic)
{
    plightDir = fs_in.v_TBN * plightDir;
    float NdotL = max(dot(gNormal, plightDir), 0.0);

    vec3 halfwayDir = normalize(plightDir + gViewDir);
    float NdotH = max(dot(halfwayDir, gNormal), 0.0);

    float glossiness = 1.0 - roughness;
    float specularExponent = glossiness * 128.0; 
    float specularCoefficient = pow(NdotH, specularExponent);

    float VdotH = max(dot(gViewDir, halfwayDir), 0.0);
    vec3 fresnel = FresnelSchlick(VdotH, F0);

    vec3 albedo = gDiffuseTexel.rgb;
    vec3 diffuseColor = albedo * (1.0 - metallic);
    vec3 diffuse  = diffuseColor * NdotL * plightColor * pluminosity;

    vec3 specular = fresnel * specularCoefficient * NdotL * plightColor * pluminosity;

    return diffuse + specular;
}

vec3 CalculateDirectionalLight(mat4 plight, float shadow, vec3 F0)
{
    // Directional light: plight[1].rgb = direction
    return BlinnPhong(-plight[1].rgb, UnPackColor(plight[2][0]), plight[3][3] * shadow, F0, gRoughness, gMetallic) ;
}

vec3 CalcPointLight(mat4 pLight, float shadow, vec3 F0)
{
    const vec3 lightPosition  = pLight[0].rgb;
    const vec3 lightColor     = UnPackColor(pLight[2][0]);
    const float intensity     = pLight[3][3];

    const vec3 lightDirection = normalize(fs_in.v_WorldPos - lightPosition);
    const float luminosity    = LuminosityFromAttenuation(pLight);

    return BlinnPhong(lightDirection, lightColor, intensity * luminosity  * shadow, F0, gRoughness, gMetallic);
}

vec3 CalculateSpotLight(mat4 pLight, float shadow, vec3 F0)
{
    const vec3 lightPosition  = pLight[0].rgb;
    const vec3 lightColor     = UnPackColor(pLight[2][0]);
    const float intensity     = pLight[3][3];

    const vec3 lightDirection = normalize(fs_in.v_WorldPos - lightPosition);  
    const float luminosity    = LuminosityFromAttenuation(pLight);

    float theta = dot(lightDirection, normalize(-pLight[1].rgb)); 
    float epsilon = (pLight[3][1] -  pLight[3][2]);
    float spotIntensity = clamp((theta - pLight[3][2]) / epsilon, 0.0, 1.0);

    return BlinnPhong(lightDirection, lightColor, intensity * luminosity * shadow * spotIntensity, F0, gRoughness, gMetallic) ;
}

float ShadowCalculation(int i, float bias)
{
    vec4 fragPosLightSpace = lightSpaceMatricies[i] * vec4(fs_in.v_WorldPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;
    float shadow = 0.0;
    
    vec2 texelSize = 1 / textureSize(uShadowsMap, 0).xy;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(uShadowsMap, vec3(projCoords.xy + vec2(x, y) * texelSize, i)).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return 1 - shadow;
}

vec3 CalculateLightSum()
{
    vec3 lightSum = vec3(0.0);

    // Compute F0 for Fresnel: 
    // For dielectric: F0 ~ 0.04. For metals, F0 from base color.
    vec3 dielectricF0 = vec3(0.04);
    vec3 F0 = mix(dielectricF0, gDiffuseTexel.rgb, gMetallic);

    for (int i = 0; i < ssboLights.length(); ++i)
    {
        float shadow = 1.0;
        if(lightsEnable[i][0].x == 1)
        {
            vec3 lightDirection  = normalize(ssboLights[i][0].rgb - fs_in.v_WorldPos);
            float bias = max(0.05 * (1.0 - dot(gNormal, lightDirection)), 0.005);  
            shadow = ShadowCalculation(i, bias);
        } else 
        {
            shadow = 1.0;
        }

        switch(int(ssboLights[i][3][0]))
        {
            case 0: // Ambient Light
                // Multiply ambient by AO to darken indirect light in occluded areas
                lightSum += gDiffuseTexel.rgb * UnPackColor(ssboLights[i][2][0]) * ssboLights[i][3][3] * gAmbientOcclusion;
                break;
            case 1: // Point Light
                lightSum += CalcPointLight(ssboLights[i], shadow, F0); 
                break;
            case 2: // Directional Light
                lightSum += CalculateDirectionalLight(ssboLights[i], shadow, F0);  
                break;
            case 3: // Spot Light
                lightSum += CalculateSpotLight(ssboLights[i], shadow, F0);  
                break;
            case 4: // Area Light (Not implemented, but could be added similarly)
                break;
            default: // No Light
                lightSum += gDiffuseTexel.rgb * UnPackColor(ssboLights[i][2][0]) * 0.1; 
        }
    }

    // Apply AO again to final result if desired (if not already done per light)
    // But since we did it in ambient, we can also do a global multiplier:
    // lightSum *= gAmbientOcclusion;

    return lightSum;
}

void main()
{
    gTexCoords = vec2(fs_in.v_TexCoord);
    gDiffuseTexel  = texture(uDiffuseMap,  gTexCoords) * uDiffuse;

	if (textureSize(uNormalMap, 0).x > 0) {
		vec3 normalMap = -texture(uNormalMap, gTexCoords).rgb * 2.0 - 1.0;
	    gNormal = normalize(normalMap);
	} else {
        gNormal = normalize(fs_in.v_TBN * fs_in.v_Normal);
	}

    // GLTF Schema dictates metallic and roughness are the same texture
    // metallic on B channel
    // roughness on G channel
	gMetallic = (textureSize(uMetallicMap, 0).x > 0) ? texture(uMetallicMap, gTexCoords).b : 0.0;
	gRoughness = (textureSize(uRoughnessMap, 0).x > 0) ? texture(uRoughnessMap, gTexCoords).g : 1.0;
	gAmbientOcclusion = (textureSize(uAmbientOcclusionMap, 0).x > 0) ? texture(uAmbientOcclusionMap, gTexCoords).r : 0.1;

//    gMetallic = 0;
//    gRoughness = 1;
//    gAmbientOcclusion = 1;

    gViewDir = fs_in.v_TBN * normalize(ubo_ViewPos - fs_in.v_WorldPos);

    vec3 color = CalculateLightSum();
    FragColor = vec4(color, gDiffuseTexel.a);
}
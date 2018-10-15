//This is for a preview of the shader composition, but in time we must use more specific Light Shader
#include "DefaultLight.glsl"
const float Pi = 3.14159265;

uniform sampler2D uShadowMap;

out vec4 fragColor;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_texcoord;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec3 in_tangent;
layout (location = 4) in vec3 in_viewVector;
layout (location = 5) in vec3 in_lightVector;


struct Material
{
    vec4 kd;
    vec4 ks;
    float ns;
    float alpha;
};

uniform Material material;



vec3 getNormal(Material material, vec2 texCoord, vec3 N, vec3 T, vec3 B)
{
    return normalize(N);
}



vec3 computeMaterialInternal(Material material, vec2 texC, vec3 L, vec3 V, vec3 N, vec3 X, vec3 Y) {
    vec3 Kd = material.kd.xyz / Pi;

    float intensity = dot(normalize(L),N);

    if (intensity > 0.85)      intensity = 0.8;
    else if (intensity > 0.45) intensity = 0.45;
    else if (intensity > 0.25) intensity = 0.25;
    else                       intensity = 0.05;

    return intensity * Kd;
}

void main() {
    vec3 binormal = normalize(cross(in_normal, in_tangent));

    vec3 normalLocal   = getNormal(material, in_texcoord.xy, in_normal, in_tangent, binormal);
    vec3 binormalLocal = normalize(cross(normalLocal, in_tangent));
    vec3 tangentLocal  = normalize(cross(binormalLocal, normalLocal));

    vec3 materialColor = computeMaterialInternal(material, in_texcoord.xy, in_lightVector, in_viewVector,
                                                 normalLocal, tangentLocal, binormalLocal);
    vec3 lightContribution = lightContributionFrom(light, in_position);

    fragColor = vec4(materialColor * lightContribution, 1.0);
}

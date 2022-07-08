/* Currently rendering mesh material data. */
layout(std140, binding = BINDING_POINT_MATERIAL_DATA) uniform ubo_Material
{
    vec3 Specular;
    float Shininess;
    vec3 Diffuse;
    int IsSpecularMap;
    int IsDiffuseMap;
    int IsNormalMap;
};

layout(binding  = TEXTURE_SLOT_MATERIAL_DIFFUSE)    uniform sampler2D u_DiffuseMap;
layout(binding  = TEXTURE_SLOT_MATERIAL_SPECULAR)   uniform sampler2D u_SpecularMap;
layout(binding  = TEXTURE_SLOT_MATERIAL_NORMAL_MAP) uniform sampler2D u_NormalMap;
layout(binding  = TEXTURE_SLOT_SHADOW_MAP)          uniform sampler2D u_ShadowMap;
/* Currently rendering mesh material data. */
layout(std140, binding = BINDING_POINT_MATERIAL_DATA) uniform ubo_Material
{
    vec3    Specular;      /* Material specular lighting coefficient. */
    float   Shininess;     /* Material shiness exponent lighting coefficient. */
    vec3    Diffuse;       /* Material diffuse lighting coefficient. */
    bool    IsSpecularMap; /* Flag, showing whether specular map passing to shader. */
    bool    IsDiffuseMap;  /* Flag, showing whether diffuse map passing to shader. */
    bool    IsEmissionMap; /* Flag, showing whether normal map passing to shader. */
    bool    IsNormalMap;   /* Flag, showing whether normal map passing to shader. */
};

layout(binding  = TEXTURE_SLOT_MATERIAL_DIFFUSE)      uniform sampler2D u_DiffuseMap;
layout(binding  = TEXTURE_SLOT_MATERIAL_SPECULAR)     uniform sampler2D u_SpecularMap;
layout(binding  = TEXTURE_SLOT_MATERIAL_EMISSION_MAP) uniform sampler2D u_EmissionMap;
layout(binding  = TEXTURE_SLOT_MATERIAL_NORMAL_MAP)   uniform sampler2D u_NormalMap;

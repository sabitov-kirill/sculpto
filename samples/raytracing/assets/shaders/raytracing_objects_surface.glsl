struct surface
{
    vec3  Emmitance;
    vec3  Reflectance;
    float Roughness;
    float Opacity;
};

struct intersection
{
    vec3 Position;
    vec3 Normal;
    float Distance;
    surface Surface;
};
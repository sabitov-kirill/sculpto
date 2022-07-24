struct surface
{
    vec3  Emmitance;
    float Roughness;
    vec3  Reflectance;
    float Opacity;
};

struct intersection
{
    vec3 Position;
    vec3 Normal;
    float Distance;
    surface Surface;
};
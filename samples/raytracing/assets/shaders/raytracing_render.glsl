#define FAR_CLIP 100000000

#define SPHERE_COUNT 3
// #define PLANES_COUNT 1
#define BOX_COUNT    9

sphere  Spheres[SPHERE_COUNT];
box     Boxes[BOX_COUNT];
// plane   Planes[PLANES_COUNT];

void InitializeScene()
{
    Spheres[0].Center = vec3(2.5, -1.0, -1.0);
    Spheres[1].Center = vec3(-3.5, -4.25, 2.25);
    Spheres[2].Center = vec3(2.75, -2, 1.25);
    Spheres[0].Radius = 1.0;
    Spheres[1].Radius = 0.75;
    Spheres[2].Radius = 1.5;
    Spheres[0].Surface.Roughness = 1.0;
    Spheres[1].Surface.Roughness = 0.7;
    Spheres[2].Surface.Roughness = 1.0;
    Spheres[0].Surface.Opacity = 0.0;
    Spheres[1].Surface.Opacity = 0.0;
    Spheres[2].Surface.Opacity = 0.75;
    Spheres[0].Surface.Reflectance = vec3(0.95, 0.012, 0.032);
    Spheres[1].Surface.Reflectance = vec3(0.97, 0.43, 0.012);
    Spheres[2].Surface.Reflectance = vec3(1);
    Spheres[0].Surface.Emmitance = vec3(0.0);
    Spheres[1].Surface.Emmitance = vec3(0.0);
    Spheres[2].Surface.Emmitance = vec3(0.0);

    // up
    Boxes[1].Surface.Roughness = 0.0;
    Boxes[0].Surface.Emmitance = vec3(0.0);
    Boxes[0].Surface.Reflectance = vec3(1.0, 1.0, 1.0);
    Boxes[0].Size = vec3(5.0, 0.5, 5.0);
    Boxes[0].Position = vec3(0.0, 5.5, 0.0);
    Boxes[0].Rotation = mat3(
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    );

    // down
    Boxes[1].Surface.Roughness = 0.0;
    Boxes[1].Surface.Opacity = 0.0;
    Boxes[1].Surface.Emmitance = vec3(0.0);
    Boxes[1].Surface.Reflectance = vec3(1.0, 1.0, 1.0);
    Boxes[1].Size = vec3(5.0, 0.5, 5.0);
    Boxes[1].Position = vec3(0.0, -5.5, 0.0);
    Boxes[1].Rotation = mat3(
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    );

    // right
    Boxes[2].Surface.Roughness = 0.0;
    Boxes[2].Surface.Opacity = 0.0;
    Boxes[2].Surface.Emmitance = vec3(0.0);
    Boxes[2].Surface.Reflectance = vec3(0.09, 0.09, 0.91);
    Boxes[2].Size = vec3(5.0, 0.5, 5.0);
    Boxes[2].Position = vec3(5.5, 0.0, 0.0);
    Boxes[2].Rotation = mat3(
        0.0, 1.0, 0.0,
       -1.0, 0.0, 0.0,
        0.0, 0.0, 1.0
    );

    // left
    Boxes[3].Surface.Roughness = 0.0;
    Boxes[3].Surface.Opacity = 0.0;
    Boxes[3].Surface.Emmitance = vec3(0.0);
    Boxes[3].Surface.Reflectance = vec3(0.94, 0.05, 0.05);
    Boxes[3].Size = vec3(5.0, 0.5, 5.0);
    Boxes[3].Position = vec3(-5.5, 0.0, 0.0);
    Boxes[3].Rotation = mat3(
        0.0, 1.0, 0.0,
       -1.0, 0.0, 0.0,
        0.0, 0.0, 1.0
    );

    // back
    Boxes[4].Surface.Roughness = 0.0;
    Boxes[4].Surface.Opacity = 0.0;
    Boxes[4].Surface.Emmitance = vec3(0.0);
    Boxes[4].Surface.Reflectance = vec3(1.0, 1.0, 1.0);
    Boxes[4].Size = vec3(5.0, 0.5, 5.0);
    Boxes[4].Position = vec3(0.0, 0.0, -5.5);
    Boxes[4].Rotation = mat3(
        1.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        0.0, 1.0, 0.0
    );

    // light source
    Boxes[5].Surface.Roughness = 0.0;
    Boxes[5].Surface.Opacity = 0.0;
    Boxes[5].Surface.Emmitance = vec3(16);
    Boxes[5].Surface.Reflectance = vec3(0.0);
    Boxes[5].Size = vec3(2.5, 0.025, 2.5);
    Boxes[5].Position = vec3(0, 4.9, 0);
    Boxes[5].Rotation = mat3(
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    );

    // Boxes
    Boxes[6].Surface.Roughness = 0;
    Boxes[6].Surface.Opacity = 0.0;
    Boxes[6].Surface.Emmitance = vec3(0.0);
    Boxes[6].Surface.Reflectance = vec3(0.3);
    Boxes[6].Size = vec3(1.5, 3.0, 1.5);
    Boxes[6].Position = vec3(-2.0, -2.0, -2.0);
    Boxes[6].Rotation = mat3(
        0.7, 0.0, 0.7,
        0.0, 1.0, 0.0,
        -0.7, 0.0, 0.7
    );

    // Boxes
    Boxes[7].Surface.Roughness = 0.0;
    Boxes[7].Surface.Opacity = 0.0;
    Boxes[7].Surface.Emmitance = vec3(0.0);
    Boxes[7].Surface.Reflectance = vec3(1.0);
    Boxes[7].Size = vec3(1.0, 1.5, 1.0);
    Boxes[7].Position = vec3(2.5, -3.5, -0.0);
    Boxes[7].Rotation = mat3(
        0.7, 0.0, 0.7,
        0.0, 1.0, 0.0,
        -0.7, 0.0, 0.7
    );

    // Mirror
    Boxes[8].Surface.Roughness = 1;
    Boxes[8].Surface.Opacity = 0.0;
    Boxes[8].Surface.Emmitance = vec3(0.0);
    Boxes[8].Surface.Reflectance = vec3(0.85);
    Boxes[8].Size = vec3(1.75, 4.75, 0.25);
    Boxes[8].Position = vec3(1.5, 0, -5.2);
    Boxes[8].Rotation = mat3(
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    );
}

bool Intersect(ray Ray, inout intersection Intersection)
{
    intersection current_inter;
    current_inter.Distance = FAR_CLIP;

    for (uint i = 0; i < SPHERE_COUNT; ++i) RaySphereIntersect(Ray, Spheres[i], current_inter);
    for (uint i = 0; i < BOX_COUNT; ++i) RayBoxIntersect(Ray, Boxes[i], current_inter);
 // for (uint i = 0; i < PLANES_COUNT; ++i) RayPlaneIntersect(Ray, Planes[i], current_inter);

    if (current_inter.Distance != FAR_CLIP)
    {
        Intersection = current_inter;
        return current_inter.Distance != FAR_CLIP;
    }
    return false;
}

vec3 RandomHemispherePoint(vec2 RandomVector)
{
    float cos_theta = sqrt(1.0 - RandomVector.x);
    float sin_theta = sqrt(RandomVector.x);
    float phi = 2.0 * PI * RandomVector.y;
    return vec3(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);
}

vec3 NormalOrientedHemispherePoint(vec2 RandomVector, vec3 Normal)
{
    vec3 v = RandomHemispherePoint(RandomVector);
    return dot(v, Normal) < 0.0 ? -v : v;
}

float FresnelSchlick(float In, float Out, vec3 Dirction, vec3 Normal)
{
    float F0 = ((Out - In) * (Out - In)) / ((Out + In) * (Out + In));
    float fresnel = F0 + (1.0 - F0) * pow((1.0 - abs(dot(Dirction, Normal))), 5.0);
    return fresnel;
}

bool IsRefracted(float Rand, vec3 Direction, vec3 Normal, float Opacity, float In, float Out)
{
    float fresnel = FresnelSchlick(In, Out, Direction, Normal);
    return Opacity > Rand && fresnel < Rand;
}

#define RECURSION_DEPTH 8

vec3 TracePath(ray Ray, float Seed)
{
    vec3 total_light = vec3(0);
    vec3 light_weight = vec3(1);

    for (uint i = 0; i < RECURSION_DEPTH; ++i)
    {
        intersection inter;
        if (Intersect(Ray, inter))
        {
            vec2 rand = vec2(RandomNoise(Seed * TexCoords.xy), Seed * RandomNoise(TexCoords.yx));
            vec3 hemisphere_direction = NormalOrientedHemispherePoint(rand, inter.Normal);
            vec3 random_vec = normalize(vec3(RandomNoise(sin(Seed * TexCoords.xy)),
                                             RandomNoise(cos(Seed * TexCoords.xy)),
                                             RandomNoise(sin(Seed * TexCoords.yx))) * 2.0 - 1.0);
            vec3 tangent   = cross(random_vec, inter.Normal);
            vec3 bitangent = cross(inter.Normal, tangent);
            mat3 transform = mat3(tangent, bitangent, inter.Normal);

            vec3 new_ray_direction = transform * hemisphere_direction;
            vec3 new_ray_origin = inter.Position;

            const float n_in  = 0.96;
            const float n_out = 1.00;
            float refraction_rand = RandomNoise(cos(Seed * TexCoords.yx));
            if (IsRefracted(refraction_rand, Ray.Direction, inter.Normal, inter.Surface.Opacity, n_in, n_out))
            {
                bool outside = dot(Ray.Direction, inter.Normal) < 0;
                float nu = outside ? n_out / n_in : n_in / n_out;

                vec3 ideal_refraction = outside ?
                    refract(Ray.Direction, inter.Normal, nu) :
                    refract(Ray.Direction, -inter.Normal, nu);

                new_ray_direction = normalize(mix(-new_ray_direction, ideal_refraction, inter.Surface.Roughness));
                if (outside) new_ray_origin -= inter.Normal * 0.01;
                else         new_ray_origin += inter.Normal * 0.01;
            }
            else
            {
                vec3 ideal_reflection = reflect(Ray.Direction, inter.Normal);
                new_ray_direction = normalize(mix(new_ray_direction, ideal_reflection, inter.Surface.Roughness));
                new_ray_origin += inter.Normal * 0.01;
            }

            Ray.Origin = new_ray_origin;
            Ray.Direction = new_ray_direction;

            total_light += light_weight * inter.Surface.Emmitance;
            light_weight *= inter.Surface.Reflectance;
        }
        else break;
    }
    return total_light;
}
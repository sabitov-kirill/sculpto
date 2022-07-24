#define FAR_CLIP 100000000

bool Intersect(ray Ray, inout intersection Intersection)
{
    intersection current_inter;
    current_inter.Distance = FAR_CLIP;

    for (uint i = 0; i < u_SpheresCount; ++i) RaySphereIntersect(Ray, u_Spheres[i], current_inter);
    for (uint i = 0; i < u_BoxesCount;   ++i) RayBoxIntersect(Ray,    u_Boxes[i], current_inter);
    for (uint i = 0; i < u_PlanesCount;  ++i) RayPlaneIntersect(Ray,  u_Planes[i], current_inter);

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

            const float n_in  = 1.00;
            const float n_out = 0.9;
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
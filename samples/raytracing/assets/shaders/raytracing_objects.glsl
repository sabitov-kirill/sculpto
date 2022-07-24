#include "raytracing_objects_surface.glsl"

/******************************************************************
 * SPHERE
 ******************************************************************/

struct sphere
{
    vec3 Center;
    float Radius;
    surface Surface;
};

bool RaySphereIntersect(ray Ray, sphere Sphere, inout intersection BestIntersection)
{
    vec3 L = Ray.Origin - Sphere.Center;
    float a = dot(Ray.Direction, Ray.Direction);
    float b = 2.0 * dot(L, Ray.Direction);
    float c = dot(L, L) - Sphere.Radius * Sphere.Radius;
    float D = b * b - 4 * a * c;

    if (D < 0.0) return false;

    float r1 = (-b - sqrt(D)) / (2.0 * a);
    float r2 = (-b + sqrt(D)) / (2.0 * a);

    float dist;
    if (r1 > 0.0)      dist = r1;
    else if (r2 > 0.0) dist = r2;
    else               return false;

    if (dist > BestIntersection.Distance)
        return false;

    BestIntersection.Position = RayGetPoint(Ray, dist);
    BestIntersection.Normal = normalize(Ray.Direction * dist + L);
    BestIntersection.Distance = dist;
    BestIntersection.Surface = Sphere.Surface;
    return true;
}

/******************************************************************
 * PLANE
 ******************************************************************/

struct plane
{
    vec3 Normal;
    float Dist;
    surface Surface;
};

#define THRESHOLD 0.001

bool RayPlaneIntersect(ray Ray, plane Plane, inout intersection BestIntersection)
{
    float nd = dot(Ray.Direction, Plane.Normal);
    if (abs(nd) + THRESHOLD * 2 < 0)
        return false;

    float ray_dist = -(dot(Ray.Origin, Plane.Normal) - Plane.Dist) / nd;
    if (ray_dist < THRESHOLD || ray_dist > BestIntersection.Distance) return false;

    BestIntersection.Distance = ray_dist;
    BestIntersection.Position = RayGetPoint(Ray, ray_dist);
    BestIntersection.Normal = Plane.Normal;
    BestIntersection.Surface = Plane.Surface;
    return true;
}

/******************************************************************
 * BOX
 ******************************************************************/

struct box
{
    vec3 Size;
    float dummy1;
    mat3 Rotation;
    vec4 dummy2;
    vec3 Position;
    float dummy3;
    surface Surface;
};

bool RayBoxIntersect(ray Ray, box Box, inout intersection BestIntersection)
{
    vec3 rd = Box.Rotation * Ray.Direction;
    vec3 ro = Box.Rotation * (Ray.Origin - Box.Position);

    vec3 m = vec3(1.0) / rd; 

    vec3 s = vec3((rd.x < 0.0) ? 1.0 : -1.0,
                  (rd.y < 0.0) ? 1.0 : -1.0,
                  (rd.z < 0.0) ? 1.0 : -1.0);
    vec3 t1 = m * (-ro + s * Box.Size);
    vec3 t2 = m * (-ro - s * Box.Size);

    float tN = max(max(t1.x, t1.y), t1.z);
    float tF = min(min(t2.x, t2.y), t2.z);

    if (tN > tF || tF < 0.0) return false;

    mat3 txi = transpose(Box.Rotation);

    if (tN > BestIntersection.Distance)
        return false;

    vec3 normal;
    if (t1.x > t1.y && t1.x > t1.z) normal = txi[0] * s.x;
    else if (t1.y > t1.z)           normal = txi[1] * s.y;
    else                            normal = txi[2] * s.z;

    BestIntersection.Distance = tN;
    BestIntersection.Position = RayGetPoint(Ray, tN);
    BestIntersection.Normal = normal;
    BestIntersection.Surface = Box.Surface;
    return true;
}
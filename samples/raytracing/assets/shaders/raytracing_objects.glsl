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
    vec3 a = Sphere.Center - Ray.Origin;
    float OC2 = dot(a, a);
    float OK = dot(a, Ray.Direction);
    float OK2 = OK * OK;

    float R2 = Sphere.Radius * Sphere.Radius;
    float H2 = R2 - (OC2 - OK2);
    if (H2 < 0) return false;

    float dist = OK - sqrt(H2);
    if (dist >= BestIntersection.Distance) return false;

    if (OC2 < R2)
    {
        BestIntersection.Distance = dist;
        BestIntersection.Position = RayGetPoint(Ray, dist);
        BestIntersection.Normal = normalize(BestIntersection.Position - Sphere.Center);
        BestIntersection.Surface = Sphere.Surface;
        return true;
    }

    if (OK < 0) return false;

    BestIntersection.Distance = dist;
    BestIntersection.Position = RayGetPoint(Ray, dist);
    BestIntersection.Normal = normalize(BestIntersection.Position - Sphere.Center);
    BestIntersection.Surface = Sphere.Surface;
    return true;
}

/******************************************************************
 * PLANE
 ******************************************************************/

struct plane
{
    float Dist;
    vec3 Normal;
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
    vec3 Position;
    mat3 Rotation;
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
struct ray
{
    vec3 Origin;
    vec3 Direction;
};

ray RayFromCamera(float X, float Y,
                  vec3 CameraPosition, vec3 CameraDirection, vec3 CameraRightDirection, vec3 CameraUpDirection, float CameraProjectionDistance,
                  float ViewportWidth, float ViewportHeight, float ViewportProjectionWidth, float ViewportProjectionHeight)
{
    vec3 A = CameraDirection * CameraProjectionDistance;
    vec3 B = CameraRightDirection * (X - ViewportWidth / 2) * ViewportProjectionWidth / ViewportWidth;
    vec3 C = CameraUpDirection * (-Y + ViewportHeight / 2) * ViewportProjectionHeight / ViewportHeight;
    vec3 Q = A + B + C;

    return ray(CameraPosition, normalize(Q));
}

vec3 RayGetPoint(ray Ray, float Distance)
{
    return Ray.Origin + Ray.Direction * Distance;
}
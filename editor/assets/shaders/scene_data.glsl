/* Scene rendering data. */
layout(std140, binding = BINDING_POINT_MATERIAL_DATA) uniform ubo_Scene {
    vec3       CameraPosition;     /* Submission camera direction vector. */
    float      Time;               /* Currently rendering frame time since porgram time. */
    vec3       CameraLocation;     /* Submission camer location vector. */
    int        ViewportWidth;      /* Currently rendering frame viewport width. */
    int        ViewportHeight;     /* Currently rendering frame viewport height. */
};
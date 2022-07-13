/* Scene rendering data. */
layout(std140, binding = BINDING_POINT_SCENE_DATA) uniform ubo_PipelineData {
    vec3  CameraPosition;    /* Submission camera direction vector. */
    float Time;              /* Currently rendering frame time since porgram time. */
    vec3  CameraDirection;   /* Submission camer location vector. */
    int   ViewportWidth;     /* Currently rendering frame viewport width. */
    vec3  EnviromentAmbient; /* Scene enviroment apbient color. */
    int   ViewportHeight;    /* Currently rendering frame viewport height. */
    bool  IsHDR;             /* Flag, showing wheather HDR frame buffer, tone mapped to destination frame buffer is active or not. */
    float Exposure;          /* Exposure level for exposure tone mapping algoritm (applyed only if HDR is active). */
    bool  IsBloom;           /* Flag, showing wheather bloom effect is active or not. */
    int   BloomAmount;       /* Iteration of blur while applying bloom effect. */
};
/* Scene rendering data. */
layout(std140, binding = BINDING_POINT_SCENE_DATA) uniform ubo_PipelineData {
    vec3  u_CameraPosition;       /* Submission camera direction vector. */
    float u_Time;                 /* Currently rendering frame time since porgram time. */
    vec3  u_CameraDirection;      /* Submission camer location vector. */
    int   u_ViewportWidth;        /* Currently rendering frame viewport width. */
    vec3  u_CameraUpDirection;    /* Submission camer location vector. */
    int   u_ViewportHeight;       /* Currently rendering frame viewport height. */
    vec3  u_CameraRightDirection; /* Submission camer location vector. */
    bool  u_IsHDR;                /* Flag, showing wheather HDR frame buffer, tone mapped to destination frame buffer is active or not. */
    vec3  u_EnviromentAmbient;    /* Scene enviroment apbient color. */
    float u_Exposure;             /* Exposure level for exposure tone mapping algoritm (applyed only if HDR is active). */
    bool  u_IsBloom;              /* Flag, showing wheather bloom effect is active or not. */
    int   u_BloomAmount;          /* Iteration of blur while applying bloom effect. */
};
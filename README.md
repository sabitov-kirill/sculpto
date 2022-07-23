# About [sculpto]
Sculpto - full featured render engine with non-programmer friendly API.
Distributing with scene editor forms platform for creativity & design & code learning & game engine.
Allows implement your ideas through computer graphics via native C++ code/scripts/GUI.

## Features
- [x] possibility to create cross-platform applications
- [x] high-level render engine abstracted from specific render APIs
- [x] render pipeline that allows you to create photorealistic images (Blin-Phong shading, shadows, bloom, HDR, exposure tone mapping), it is also possible to create your own pipelines
- [x] scene system, using ECS pattern (9 diffrent components)
- [x] ability to create windows with custom GUI Widgets

## Roadmap
    - [x] so much things                                           | 18.06 - 06.07

    - [x] blin-phong                                               | 06.07
    - [x] transfer frame buffers to store textures instead ids     |
    - [x] add get id funtion to all render primitives classes      |

    - [x] shadow caster component                                  | 07.06
    - [x] GLSL shaders preprocessor                                |

    - [x] normal mapping                                           | 08.07
    - [x] models loading                                           |
    - [x] assets manager                                           |
    - [x] single color material and shader (color/texture)         |

    - [x] UI panels:                                               | 09.07
        - [x] Scene hierarchy                                      |
        - [x] Objects components configuration                     |
            - [x] Name component                                   |
            - [x] Transform component                              |
            - [x] Native script component                          |
            - [x] Mesh component                                   |
            - [x] Camera controller component                      |
            - [x] Point light component                            |
            - [x] Directional light component                      |
            - [x] Spot light component                             |
        - [x] Add/delete components                                |
        - [x] Create/delete objects                                |
        - [x] Renderer configuration                               |
        - [x] Profiling (FPS)                                      |

    - [x] HDR                                                      | 10.07
    - [x] bloom                                                    |

    - [x] render architecture redisgn                              | 12.07
    - [x] fix memory leaks while window resize                     |
    - [x] deffered rendering                                       |

    - [x] propeper mesh component configuration                    | till 25.07
          (problems with meshes in whole application)
    - [x] scene cameras system                                     |
    - [x] GPU ray tracing (client app example)                     |
    - [x] scene serialization (save/load)                          |
    - [x] embeded C++/Py scriping                                  |

    - [ ] enter KT ITMO                                            | 01.08

    - [ ] create guide for learning code, using sculpto            | future steps
    - [ ] intrinsic functions based optimized math module          |
    - [ ] mesh saving                                              |
    - [ ] mesh editor using marching cubes                         |
    - [ ] optimized models render in Path Tracer using voxel tree  |
    - [ ] sceletal animation                                       |
    - [ ] PBR render pipeline                                      |
    - [ ] sound module                                             |
    - [ ] 3D collision system                                      |
    - [ ] physics module                                           |
    - [ ] screen space SSAO                                        |
    - [ ] global illumination based on surfels                     |
    - [ ] render context implementation for DirectX, Vulkan, Metal |
    - [ ] order independent transparensy                           |
    - [ ] full featured 2D renderer                                |
    - [ ] android/ios support                                      |
    - [ ] USE SCULPTO!!!                                           |

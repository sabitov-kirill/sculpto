// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint Hash(uint x) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}

// Compound versions of the hashing algorithm I whipped together.
uint Hash(uvec2 v) { return Hash(v.x ^ Hash(v.y)                         ); }
uint Hash(uvec3 v) { return Hash(v.x ^ Hash(v.y) ^ Hash(v.z)             ); }
uint Hash(uvec4 v) { return Hash(v.x ^ Hash(v.y) ^ Hash(v.z) ^ Hash(v.w) ); }



// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float FloatConstruct(uint m) {
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}

// Pseudo-random value in half-open range [0:1].
float RandomNoise(float x) { return FloatConstruct(Hash(floatBitsToUint(x))); }
float RandomNoise(vec2  v) { return FloatConstruct(Hash(floatBitsToUint(v))); }
float RandomNoise(vec3  v) { return FloatConstruct(Hash(floatBitsToUint(v))); }
float RandomNoise(vec4  v) { return FloatConstruct(Hash(floatBitsToUint(v))); }

// Fraction noise generation
// float RandomNoise(vec2 co)
// {
//     return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
// }

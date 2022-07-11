bool IsColorBright(vec3 Color)
{
    float brightness = dot(Color, vec3(0.2126, 0.7152, 0.0722));
    return brightness > 1.2;
}
#version 430

layout(location = 0) in vec2 texture_coord;
layout(location = 1) in float geom_lifetime;
layout(location = 2) in float geom_iLifetime;

uniform sampler2D texture_1;
uniform vec3 color;

layout(location = 0) out vec4 out_color;


void main()
{
    vec4 texColor = texture(texture_1, texture_coord);

    vec3 finalColor = color;

    float alpha = texColor.a * (1.0 - (geom_lifetime / geom_iLifetime));
    alpha = clamp(alpha, 0.0, 1.0);

    out_color = vec4(finalColor, alpha);
}

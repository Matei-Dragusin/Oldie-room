#version 330

// Input
in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;
uniform int has_texture;
uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    vec4 color;

    if (has_texture == 1)
    {
        color = texture(texture_1, texture_coord);
        if (color.a < 0.5)
        {
            discard;
        }
    }
    else
    {
        color = vec4(object_color, 1.0);
    }

    out_color = color;
}

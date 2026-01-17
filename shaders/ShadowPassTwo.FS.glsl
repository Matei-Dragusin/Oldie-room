#version 330

// Input
in vec2 texture_coord;
in vec3 world_position;
in vec3 world_normal;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D depth_texture;
uniform int has_texture;
uniform vec3 object_color;

uniform vec3 light_position;
uniform vec3 light_direction;

uniform mat4 light_space_view;
uniform mat4 light_space_projection;
uniform float light_intensity;

// Output
layout(location = 0) out vec4 out_color;

// Lighting constants
const float spot_angle = 45.0;
const float ambient_light = 0.15;

float ShadowFactor()
{
    vec4 light_space_pos = light_space_projection * light_space_view * vec4(world_position, 1.0);
    light_space_pos = light_space_pos / light_space_pos.w;

    float light_space_depth = light_space_pos.z * 0.5 + 0.5;
    vec2 depth_map_pos = light_space_pos.xy * 0.5 + 0.5;

    float depth = texture(depth_texture, depth_map_pos).x;
    float bias = 0.0001;

    return light_space_depth - bias < depth ? 1.0 : 0.0;
}

vec3 SpotLight()
{
    vec3 L = normalize(light_position - world_position);

    float cut_off = radians(spot_angle);
    float spot_light = dot(-L, light_direction);

    if (spot_light > cos(cut_off)) {
        float shadow = ShadowFactor();

        // Spotlight attenuation (brighter in center, darker at edges)
        float spot_light_limit = cos(cut_off);
        float linear_att = (spot_light - spot_light_limit) / (1.0 - spot_light_limit);
        float spot_att = pow(linear_att, 2);

        // ambient + shadow * spotlight_brightness * intensity
        float normalized_intensity = light_intensity / 10.0;
        float lit = spot_att * 0.85 * normalized_intensity;
        return vec3(ambient_light + shadow * lit);
    }

    return vec3(ambient_light);
}

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

    vec3 lit_color = vec3(color) * SpotLight();

    out_color = vec4(lit_color, color.a);
}

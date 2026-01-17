#version 430
layout(lines_adjacency) in;
layout(triangle_strip, max_vertices = 256) out;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform int num_instances;
uniform int num_generated_points;
uniform float max_translate;
uniform vec3 translate_dir;

in int instance[];

out vec3 world_pos;
out vec3 world_normal;
out vec2 texcoord;

// Bezier curve calculation
vec3 bezier(float t, vec3 p0, vec3 p1, vec3 p2, vec3 p3)
{
    float u = 1.0 - t;
    return p0 * u * u * u +
           p1 * 3.0 * t * u * u +
           p2 * 3.0 * t * t * u +
           p3 * t * t * t;
}

// Bezier derivative for tangent calculation
vec3 bezierDerivative(float t, vec3 p0, vec3 p1, vec3 p2, vec3 p3)
{
    float u = 1.0 - t;
    return 3.0 * u * u * (p1 - p0) +
           6.0 * u * t * (p2 - p1) +
           3.0 * t * t * (p3 - p2);
}

void main()
{
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;
    vec3 p3 = gl_in[3].gl_Position.xyz;

    int current_instance = instance[0];

    float translation_amount = max_translate * float(current_instance) / float(num_instances - 1);
    float next_translation = max_translate * float(current_instance + 1) / float(num_instances - 1);

    vec3 current_offset = translate_dir * translation_amount;
    vec3 next_offset = translate_dir * next_translation;

    for (int i = 0; i < num_generated_points; i++)
    {
        float t = float(i) / float(num_generated_points - 1);
        vec3 point = bezier(t, p0, p1, p2, p3);

        // Calculate tangent along the curve (V direction)
        vec3 tangent_v = normalize(bezierDerivative(t, p0, p1, p2, p3));

        // Tangent along translation direction (U direction)
        vec3 tangent_u = normalize(translate_dir);

        // Normal is perpendicular to both tangents
        vec3 normal = normalize(cross(tangent_v, tangent_u));

        // UV coordinates
        float u_current = float(current_instance) / float(num_instances - 1);
        float u_next = float(current_instance + 1) / float(num_instances - 1);
        float v = t;

        // Emit first vertex (current translation)
        vec3 translated_current = point + current_offset;
        vec4 worldPos1 = Model * vec4(translated_current, 1.0);
        world_pos = worldPos1.xyz;
        world_normal = mat3(transpose(inverse(Model))) * normal;
        texcoord = vec2(u_current, v);
        gl_Position = Projection * View * worldPos1;
        EmitVertex();

        // Emit second vertex (next translation)
        vec3 translated_next = point + next_offset;
        vec4 worldPos2 = Model * vec4(translated_next, 1.0);
        world_pos = worldPos2.xyz;
        world_normal = mat3(transpose(inverse(Model))) * normal;
        texcoord = vec2(u_next, v);
        gl_Position = Projection * View * worldPos2;
        EmitVertex();
    }

    EndPrimitive();
}

#version 430

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texcoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 world_pos;
out vec3 world_normal;
out vec2 texcoord;

void main()
{
    world_pos = vec3(Model * vec4(v_position, 1.0));
    world_normal = mat3(transpose(inverse(Model))) * v_normal;
    texcoord = v_texcoord;
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}

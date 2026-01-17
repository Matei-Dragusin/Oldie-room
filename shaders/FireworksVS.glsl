#version 430

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 generator_position;
uniform float deltaTime;
uniform float time;
uniform float lifetime;
uniform int hasExploded;

out float vert_lifetime;
out float vert_iLifetime;

struct Particle
{
    vec4 position;
    vec4 speed;
    vec4 iposition;
    vec4 ispeed;
    float delay;
    float iDelay;
    float particleLifetime;
    float iLifetime;
};


layout(std430, binding = 0) buffer particles {
    Particle data[];
};


// Random number generator using vertex ID as seed
float rand(float seed)
{
    return fract(sin(seed * 12.9898) * 43758.5453);
}

vec3 randomDirection(int id)
{
    float r1 = rand(float(id) * 0.1);
    float r2 = rand(float(id) * 0.2);
    float r3 = rand(float(id) * 0.3);

    vec3 dir = vec3(r1 * 2.0 - 1.0, r2 * 2.0 - 1.0, r3 * 2.0 - 1.0);
    return normalize(dir);
}


void main()
{
    vec3 pos = data[gl_VertexID].position.xyz;
    vec3 spd = data[gl_VertexID].speed.xyz;

    float gravity = 2.0;
    float dt = deltaTime;

    // Check if we need to reset (lifetime expired)
    if (time > lifetime)
    {
        // Reset to initial position and velocity
        pos = data[gl_VertexID].iposition.xyz;
        spd = data[gl_VertexID].ispeed.xyz;
    }
    else
    {
        // Apply gravity
        spd += vec3(0, -gravity, 0) * dt;
        pos += spd * dt;

        // Check if particle should explode (when vertical velocity becomes 0 or negative)
        // and hasn't exploded yet (we check if speed is still pointing only upward)
        if (spd.y <= 0.0 && abs(spd.x) < 0.01 && abs(spd.z) < 0.01)
        {
            // Explode: set random direction
            vec3 explosionDir = randomDirection(gl_VertexID);
            float explosionSpeed = rand(float(gl_VertexID) * 0.5) * 2.0 + 1.0; // Random speed between 1 and 3
            spd = explosionDir * explosionSpeed;
        }
    }

    data[gl_VertexID].position.xyz = pos;
    data[gl_VertexID].speed.xyz = spd;

    vert_lifetime = time;
    vert_iLifetime = lifetime;

    gl_Position = Projection * View * Model * vec4(pos + generator_position, 1);
}

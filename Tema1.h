#pragma once

#include "components/simple_scene.h"
#include "core/gpu/particle_effect.h"

namespace m2
{
    struct Particle
    {
        glm::vec4 position;
        glm::vec4 speed;
        glm::vec4 initialPos;
        glm::vec4 initialSpeed;
        float delay;
        float initialDelay;
        float lifetime;
        float initialLifetime;

        Particle() {}

        Particle(const glm::vec4 &pos, const glm::vec4 &speed)
        {
            SetInitial(pos, speed);
        }

        void SetInitial(const glm::vec4 &pos, const glm::vec4 &speed,
            float delay = 0, float lifetime = 0)
        {
            position = pos;
            initialPos = pos;

            this->speed = speed;
            initialSpeed = speed;

            this->delay = delay;
            initialDelay = delay;

            this->lifetime = lifetime;
            initialLifetime = lifetime;
        }
    };

    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        // Object creation helpers
        void CreateBezierProfile(const std::string& name, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
        void CreateRectangle(const std::string& name, float width, float height);
        void CreateBox(const std::string& name, float width, float height, float depth);
        void CreateCircle(const std::string& name, float radius, int segments);

        void ResetParticlesFireworks(int xSize, int ySize, int zSize);

        // Shadow mapping
        void CreateShadowFramebuffer(int width, int height);
        void DrawSimpleObjects(Shader* shader, gfxc::Camera* camera);

        // Surface generation params
        int num_instances;
        int num_generated_points;

        // Table dimensions
        float tableWidth = 5.0f;
        float tableDepth = 3.0f;
        float tableEdgeHeight = 0.16f;
        float legInset = 0.2f;

        // Room dimensions
        float roomWidth = 20.0f;
        float roomHeight = 8.0f;
        float roomDepth = 20.0f;

        float tvRotationAngle = 0.0f;

        std::unordered_map<std::string, Texture2D> mapTextures;

        // Fireworks system
        ParticleEffect<Particle> *particleEffect;
        gfxc::Camera *fireworksCamera;  // Fixed camera for framebuffer rendering
        unsigned int framebuffer;
        unsigned int fireworksTexture;
        int fireworksWidth = 1024;
        int fireworksHeight = 1024;
        glm::vec3 generator_position;
        float offset;
        float fireworksTime;
        float fireworksLifetime;
        glm::vec3 fireworksColor;
        bool hasExploded;

        // Shadow mapping system
        unsigned int shadow_framebuffer;
        unsigned int shadow_color_texture;
        unsigned int shadow_depth_texture;
        int shadow_map_width = 2048;
        int shadow_map_height = 2048;

        glm::vec3 light_position;
        glm::vec3 light_direction;
        glm::mat4 light_space_view;
        glm::mat4 light_space_projection;
        float light_space_near_plane = 0.1f;
        float light_space_far_plane = 30.0f;

        bool draw_shadow_debug = false;
        float base_light_intensity = 10.0f;

        // Flickering light effect
        float flicker_time = 0.15f;
        float current_light_intensity = 10.0f;
    };
}
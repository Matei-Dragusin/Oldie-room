#include "lab_m2/Tema1/Tema1.h"

#include <vector>
#include <string>
#include <iostream>
#include <limits>

using namespace std;
using namespace m2;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, 2, 4), glm::quatLookAt(glm::vec3(0, -0.3f, -1), glm::vec3(0, 1, 0)));
    camera->Update();

    glDisable(GL_CULL_FACE);

    num_instances = 100;
    num_generated_points = 50;

    // Load shaders for rotation surface
    {
        Shader *shader = new Shader("RotationSurface");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "RotationSurfaceVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "RotationSurfaceGS.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "RotationSurfaceFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Load shaders for translation surface
    {
        Shader *shader = new Shader("TranslationSurface");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "TranslationSurfaceVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "TranslationSurfaceGS.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "TranslationSurfaceFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Load simple shader for caps/rectangles
    {
        Shader *shader = new Shader("Simple");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "SimpleVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "SimpleFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Load view texture shaders for debug
    {
        Shader *shader = new Shader("ViewColorTexture");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "ViewColorTexture.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "ViewColorTexture.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader *shader = new Shader("ViewDepthTexture");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "ViewDepthTexture.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "ViewDepthTexture.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Load sphere mesh for light indicator
    {
        Mesh *mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Load shadow mapping shaders
    {
        Shader *shader = new Shader("ShadowPassOne");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "ShadowPassOne.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "ShadowPassOne.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader *shader = new Shader("ShadowPassTwo");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "ShadowPassTwo.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "ShadowPassTwo.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Depth-only shaders for rotation/translation surfaces in shadow pass
    {
        Shader *shader = new Shader("RotationSurfaceDepth");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "RotationSurfaceVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "RotationSurfaceGS.glsl"), GL_GEOMETRY_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader *shader = new Shader("TranslationSurfaceDepth");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "TranslationSurfaceVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "TranslationSurfaceGS.glsl"), GL_GEOMETRY_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Load shaders for particles
    {
        Shader *shader = new Shader("ParticleShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FireworksVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FireworksGS.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FireworksFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Vase
    CreateBezierProfile("vase1",
                        glm::vec3(0.15, 0.0, 0), // Bottom
                        glm::vec3(0.1, 0.4, 0),  // Control point
                        glm::vec3(0.5, 0.6, 0),  // Control point
                        glm::vec3(0.2, 1.0, 0)   // Top
    );
    // Table leg
    CreateBezierProfile("table_leg",
                        glm::vec3(0.05, 0.0, 0), // Bottom
                        glm::vec3(0.04, 0.2, 0), // Taper
                        glm::vec3(0.04, 0.5, 0), // Keep
                        glm::vec3(0.05, 0.7, 0)  // Top
    );

    // Table top edge
    CreateBezierProfile("table_edge",
                        glm::vec3(0, 0, 0),       // Bottom
                        glm::vec3(0.03, 0.04, 0), // Bevel
                        glm::vec3(0.03, 0.12, 0), // Keep
                        glm::vec3(0, 0.16, 0)     // Top
    );

    // Table top and bottom caps (rectangles)
    CreateRectangle("table_cap", tableWidth, tableDepth); // width x depth

    // TV screen
    CreateBezierProfile("tv_screen",
                        glm::vec3(0.55, 0, 0),  // Left edge
                        glm::vec3(0.6, 0.3, 0), // Control point - slight curve
                        glm::vec3(0.6, 0.7, 0), // Control point - slight curve
                        glm::vec3(0.55, 1.0, 0) // Right edge
    );

    // TV carcass (box)
    CreateBox("tv_carcass", 1.5f, 1.3f, 1.5f); // width x height x depth

    // Lamp base
    CreateBezierProfile("lamp_base",
                        glm::vec3(0.3, 0.0, 0),  // Bottom
                        glm::vec3(0.15, 0.2, 0), // Lower
                        glm::vec3(0.08, 0.7, 0), // Upper
                        glm::vec3(0.02, 1.2, 0)  // Top
    );

    // Lamp shade
    CreateBezierProfile("lamp_shade",
                        glm::vec3(0.0, 0.0, 0),  // Bottom
                        glm::vec3(0.1, 0.1, 0),  // Lower
                        glm::vec3(0.25, 0.4, 0), // Middle
                        glm::vec3(0.45, 0.6, 0)  // Top
    );

    // Room walls, floor and ceiling
    CreateBox("room_floor", roomWidth, 0.1f, roomDepth);       // Thin floor
    CreateBox("room_ceiling", roomWidth, 0.1f, roomDepth);     // Thin ceiling
    CreateBox("room_wall_back", roomWidth, roomHeight, 0.1f);  // Back wall (thin on Z)
    CreateBox("room_wall_front", roomWidth, roomHeight, 0.1f); // Front wall (thin on Z)
    CreateBox("room_wall_left", 0.1f, roomHeight, roomDepth);  // Left wall (thin on X)
    CreateBox("room_wall_right", 0.1f, roomHeight, roomDepth); // Right wall (thin on X)

    {
        Texture2D *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "vase.jpg").c_str(), GL_REPEAT);
        mapTextures["vase_texture"] = *texture;
    }

    {
        Texture2D *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "wood.jpg").c_str(), GL_REPEAT);
        mapTextures["wood_texture"] = *texture;
    }

    {
        Texture2D *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "grunge-wall-texture.jpg").c_str(), GL_REPEAT);
        mapTextures["grey_texture"] = *texture;
    }

    {
        Texture2D *texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "particle.png").c_str(), GL_REPEAT);
        mapTextures["particle_texture"] = *texture;
    }

    // Create framebuffer for fireworks
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Create texture for framebuffer
    glGenTextures(1, &fireworksTexture);
    glBindTexture(GL_TEXTURE_2D, fireworksTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fireworksWidth, fireworksHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fireworksTexture, 0);

    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create fixed camera for fireworks framebuffer
    fireworksCamera = new gfxc::Camera();
    fireworksCamera->SetOrthographic(4.0f, 4.0f, 0.1f, 100.0f); // Orthographic projection
    fireworksCamera->SetPositionAndRotation(
        glm::vec3(0, 0, 5),
        glm::quatLookAt(glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)));
    fireworksCamera->Update();

    ResetParticlesFireworks(100, 100, 100);

    generator_position = glm::vec3(0, -3.0f, 0); // Start from bottom of view
    offset = 0.02f;
    fireworksTime = 0.0f;
    fireworksLifetime = 4.0f; // 4 seconds per cycle
    hasExploded = false;

    // Random initial color
    fireworksColor = glm::vec3(
        (rand() % 100) / 100.0f,
        (rand() % 100) / 100.0f,
        (rand() % 100) / 100.0f);

    // Initialize shadow mapping
    glm::vec3 table2Position = glm::vec3(0, 0, 2.0f);
    float tableTopHeight = 0.86f; // tblHeight (0.7) + tableEdgeHeight (0.16)
    glm::vec3 lampBasePosition = table2Position + glm::vec3(-1.5f, tableTopHeight, 0);
    float lampHeight = 1.2f;

    // Position light at lamp shade
    light_position = lampBasePosition + glm::vec3(0, lampHeight - 0.2f, -0.2f);
    light_direction = glm::normalize(glm::vec3(0, -0.5f, -0.866f));

    CreateShadowFramebuffer(shadow_map_width, shadow_map_height);
}

void Tema1::CreateShadowFramebuffer(int width, int height)
{
    // Generate and bind framebuffer
    glGenFramebuffers(1, &shadow_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_framebuffer);

    // Create color texture
    glGenTextures(1, &shadow_color_texture);
    glBindTexture(GL_TEXTURE_2D, shadow_color_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shadow_color_texture, 0);

    // Create depth texture
    glGenTextures(1, &shadow_depth_texture);
    glBindTexture(GL_TEXTURE_2D, shadow_depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_depth_texture, 0);

    // Set draw buffers
    std::vector<GLenum> draw_textures;
    draw_textures.push_back(GL_COLOR_ATTACHMENT0);
    glDrawBuffers(draw_textures.size(), &draw_textures[0]);

    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Tema1::ResetParticlesFireworks(int xSize, int ySize, int zSize)
{
    unsigned int nrParticles = 500;

    particleEffect = new ParticleEffect<Particle>();
    particleEffect->Generate(nrParticles, true);

    auto particleSSBO = particleEffect->GetParticleBuffer();
    Particle *data = const_cast<Particle *>(particleSSBO->GetBuffer());

    // All particles start from same position (bottom center)
    glm::vec4 startPos(0.0f, 0.0f, 0.0f, 1.0f);

    // All particles have same initial upward velocity
    glm::vec4 initialSpeed(0.0f, 4.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < nrParticles; i++)
    {
        data[i].SetInitial(startPos, initialSpeed);
    }

    particleSSBO->SetBufferData(data);
}

void Tema1::DrawSimpleObjects(Shader *shader, gfxc::Camera *camera)
{
    if (!shader || !shader->GetProgramID())
        return;

    shader->Use();

    // Set common uniforms
    glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

    // Scene positions
    glm::vec3 table1Position = glm::vec3(0, 0, -4.0f);
    float tableTopHeight = 0.86f;

    // Uniform name for texture usage
    const char *texUniform = "has_texture";

    glUniform3f(shader->GetUniformLocation("object_color"), 1.0f, 0.0f, 0.0f);
    glUniform1i(shader->GetUniformLocation(texUniform), 0);

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glBindVertexArray(meshes["room_floor"]->GetBuffers()->m_VAO);
    glDrawElements(meshes["room_floor"]->GetDrawMode(), meshes["room_floor"]->indices.size(), GL_UNSIGNED_INT, 0);

    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, roomHeight, 0));
    glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glBindVertexArray(meshes["room_ceiling"]->GetBuffers()->m_VAO);
    glDrawElements(meshes["room_ceiling"]->GetDrawMode(), meshes["room_ceiling"]->indices.size(), GL_UNSIGNED_INT, 0);

    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, roomHeight / 2, -roomDepth / 2));
    glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glBindVertexArray(meshes["room_wall_back"]->GetBuffers()->m_VAO);
    glDrawElements(meshes["room_wall_back"]->GetDrawMode(), meshes["room_wall_back"]->indices.size(), GL_UNSIGNED_INT, 0);

    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, roomHeight / 2, roomDepth / 2));
    glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glBindVertexArray(meshes["room_wall_front"]->GetBuffers()->m_VAO);
    glDrawElements(meshes["room_wall_front"]->GetDrawMode(), meshes["room_wall_front"]->indices.size(), GL_UNSIGNED_INT, 0);

    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-roomWidth / 2, roomHeight / 2, 0));
    glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glBindVertexArray(meshes["room_wall_left"]->GetBuffers()->m_VAO);
    glDrawElements(meshes["room_wall_left"]->GetDrawMode(), meshes["room_wall_left"]->indices.size(), GL_UNSIGNED_INT, 0);

    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(roomWidth / 2, roomHeight / 2, 0));
    glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glBindVertexArray(meshes["room_wall_right"]->GetBuffers()->m_VAO);
    glDrawElements(meshes["room_wall_right"]->GetDrawMode(), meshes["room_wall_right"]->indices.size(), GL_UNSIGNED_INT, 0);

    glUniform3f(shader->GetUniformLocation("object_color"), 0.5f, 0.5f, 0.5f);
    glUniform1i(shader->GetUniformLocation(texUniform), 0);

    float tvHeight = 1.3f;
    glm::vec3 tvPosition = table1Position + glm::vec3(0, tableTopHeight + tvHeight / 2.0f, 0);
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, tvPosition);
    modelMatrix = glm::rotate(modelMatrix, tvRotationAngle, glm::vec3(0, 1, 0));
    glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glBindVertexArray(meshes["tv_carcass"]->GetBuffers()->m_VAO);
    glDrawElements(meshes["tv_carcass"]->GetDrawMode(), meshes["tv_carcass"]->indices.size(), GL_UNSIGNED_INT, 0);

    glm::vec3 table2Position = glm::vec3(0, 0, 2.0f);
    float tblHeight = 0.7f;
    float tblEdgeHeight = this->tableEdgeHeight;

    Mesh *capMesh = meshes["table_cap"];
    glBindVertexArray(capMesh->GetBuffers()->m_VAO);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, table1Position + glm::vec3(0, tblHeight + tblEdgeHeight, 0));
    glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glDrawElements(capMesh->GetDrawMode(), capMesh->indices.size(), GL_UNSIGNED_INT, 0);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, table1Position + glm::vec3(0, tblHeight, 0));
    glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glDrawElements(capMesh->GetDrawMode(), capMesh->indices.size(), GL_UNSIGNED_INT, 0);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, table2Position + glm::vec3(0, tblHeight + tblEdgeHeight, 0));
    glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glDrawElements(capMesh->GetDrawMode(), capMesh->indices.size(), GL_UNSIGNED_INT, 0);

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, table2Position + glm::vec3(0, tblHeight, 0));
    glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glDrawElements(capMesh->GetDrawMode(), capMesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Tema1::CreateBezierProfile(const std::string &name, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    vector<VertexFormat> vertices = {
        VertexFormat(p0),
        VertexFormat(p1),
        VertexFormat(p2),
        VertexFormat(p3)};

    vector<unsigned int> indices = {0, 1, 2, 3};

    Mesh *mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    mesh->SetDrawMode(GL_LINES_ADJACENCY);
    AddMeshToList(mesh);
}

void Tema1::CreateRectangle(const std::string &name, float width, float height)
{
    glm::vec3 corner = glm::vec3(-width / 2, 0, -height / 2);

    vector<VertexFormat> vertices = {
        VertexFormat(corner, glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(0, 0)),
        VertexFormat(corner + glm::vec3(width, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(1, 0)),
        VertexFormat(corner + glm::vec3(0, 0, height), glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(0, 1)),
        VertexFormat(corner + glm::vec3(width, 0, height), glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(1, 1))};

    vector<unsigned int> indices = {0, 1, 2, 1, 3, 2};

    Mesh *mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    AddMeshToList(mesh);
}

void Tema1::CreateBox(const std::string &name, float width, float height, float depth)
{
    glm::vec3 minCorner = glm::vec3(-width / 2, -height / 2, -depth / 2);
    glm::vec3 maxCorner = glm::vec3(width / 2, height / 2, depth / 2);

    vector<VertexFormat> vertices = {
        // Front face (Z+)
        VertexFormat(glm::vec3(minCorner.x, minCorner.y, maxCorner.z), glm::vec3(0, 0, 1), glm::vec3(0), glm::vec2(0, 0)),
        VertexFormat(glm::vec3(maxCorner.x, minCorner.y, maxCorner.z), glm::vec3(0, 0, 1), glm::vec3(0), glm::vec2(1, 0)),
        VertexFormat(glm::vec3(maxCorner.x, maxCorner.y, maxCorner.z), glm::vec3(0, 0, 1), glm::vec3(0), glm::vec2(1, 1)),
        VertexFormat(glm::vec3(minCorner.x, maxCorner.y, maxCorner.z), glm::vec3(0, 0, 1), glm::vec3(0), glm::vec2(0, 1)),

        // Back face (Z-)
        VertexFormat(glm::vec3(maxCorner.x, minCorner.y, minCorner.z), glm::vec3(0, 0, -1), glm::vec3(0), glm::vec2(0, 0)),
        VertexFormat(glm::vec3(minCorner.x, minCorner.y, minCorner.z), glm::vec3(0, 0, -1), glm::vec3(0), glm::vec2(1, 0)),
        VertexFormat(glm::vec3(minCorner.x, maxCorner.y, minCorner.z), glm::vec3(0, 0, -1), glm::vec3(0), glm::vec2(1, 1)),
        VertexFormat(glm::vec3(maxCorner.x, maxCorner.y, minCorner.z), glm::vec3(0, 0, -1), glm::vec3(0), glm::vec2(0, 1)),

        // Left face (X-)
        VertexFormat(glm::vec3(minCorner.x, minCorner.y, minCorner.z), glm::vec3(-1, 0, 0), glm::vec3(0), glm::vec2(0, 0)),
        VertexFormat(glm::vec3(minCorner.x, minCorner.y, maxCorner.z), glm::vec3(-1, 0, 0), glm::vec3(0), glm::vec2(1, 0)),
        VertexFormat(glm::vec3(minCorner.x, maxCorner.y, maxCorner.z), glm::vec3(-1, 0, 0), glm::vec3(0), glm::vec2(1, 1)),
        VertexFormat(glm::vec3(minCorner.x, maxCorner.y, minCorner.z), glm::vec3(-1, 0, 0), glm::vec3(0), glm::vec2(0, 1)),

        // Right face (X+)
        VertexFormat(glm::vec3(maxCorner.x, minCorner.y, maxCorner.z), glm::vec3(1, 0, 0), glm::vec3(0), glm::vec2(0, 0)),
        VertexFormat(glm::vec3(maxCorner.x, minCorner.y, minCorner.z), glm::vec3(1, 0, 0), glm::vec3(0), glm::vec2(1, 0)),
        VertexFormat(glm::vec3(maxCorner.x, maxCorner.y, minCorner.z), glm::vec3(1, 0, 0), glm::vec3(0), glm::vec2(1, 1)),
        VertexFormat(glm::vec3(maxCorner.x, maxCorner.y, maxCorner.z), glm::vec3(1, 0, 0), glm::vec3(0), glm::vec2(0, 1)),

        // Top face (Y+)
        VertexFormat(glm::vec3(minCorner.x, maxCorner.y, maxCorner.z), glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(0, 0)),
        VertexFormat(glm::vec3(maxCorner.x, maxCorner.y, maxCorner.z), glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(1, 0)),
        VertexFormat(glm::vec3(maxCorner.x, maxCorner.y, minCorner.z), glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(1, 1)),
        VertexFormat(glm::vec3(minCorner.x, maxCorner.y, minCorner.z), glm::vec3(0, 1, 0), glm::vec3(0), glm::vec2(0, 1)),

        // Bottom face (Y-)
        VertexFormat(glm::vec3(minCorner.x, minCorner.y, minCorner.z), glm::vec3(0, -1, 0), glm::vec3(0), glm::vec2(0, 0)),
        VertexFormat(glm::vec3(maxCorner.x, minCorner.y, minCorner.z), glm::vec3(0, -1, 0), glm::vec3(0), glm::vec2(1, 0)),
        VertexFormat(glm::vec3(maxCorner.x, minCorner.y, maxCorner.z), glm::vec3(0, -1, 0), glm::vec3(0), glm::vec2(1, 1)),
        VertexFormat(glm::vec3(minCorner.x, minCorner.y, maxCorner.z), glm::vec3(0, -1, 0), glm::vec3(0), glm::vec2(0, 1))};

    vector<unsigned int> indices;
    for (int i = 0; i < 6; i++)
    {
        int offset = i * 4;
        indices.push_back(offset + 0);
        indices.push_back(offset + 1);
        indices.push_back(offset + 2);
        indices.push_back(offset + 0);
        indices.push_back(offset + 2);
        indices.push_back(offset + 3);
    }

    Mesh *mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    AddMeshToList(mesh);
}

void Tema1::FrameStart()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
    auto camera = GetSceneCamera();

    flicker_time += deltaTimeSeconds;

    float base = sin(flicker_time * 10.0f) * 0.2f;
    float flicker1 = base * 0.5f;
    float flicker2 = base * 0.3f;
    float flicker3 = base * 0.2f;

    float interrupt = sin(flicker_time * 3.0f) > 0.7f ? 0.3f : 0.0f;

    float flicker_amount = flicker1 + flicker2 + flicker3 - interrupt;
    current_light_intensity = base_light_intensity * (1.0f + flicker_amount);

    current_light_intensity = glm::max(current_light_intensity, 0.5f);

    {
        glm::vec3 camPosition = camera->m_transform->GetWorldPosition();
        glm::quat camRotation = camera->m_transform->GetWorldRotation();
        auto projectionInfo = camera->GetProjectionInfo();

        camera->SetPosition(light_position);
        camera->SetRotation(glm::quatLookAt(light_direction, glm::vec3(0, 1, 0)));
        camera->SetPerspective(90, 1, light_space_near_plane, light_space_far_plane);
        camera->Update();

        light_space_view = camera->GetViewMatrix();
        light_space_projection = camera->GetProjectionMatrix();

        glBindFramebuffer(GL_FRAMEBUFFER, shadow_framebuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, shadow_map_width, shadow_map_height);

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        DrawSimpleObjects(shaders["ShadowPassOne"], camera);

        glm::vec3 table1Position = glm::vec3(0, 0, -4.0f);
        glm::vec3 table2Position = glm::vec3(0, 0, 2.0f);
        float tableTopHeight = 0.86f;

        {
            Shader *shader = shaders["RotationSurfaceDepth"];
            shader->Use();

            glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
            glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
            glUniform1i(shader->GetUniformLocation("num_instances"), num_instances);
            glUniform1i(shader->GetUniformLocation("num_generated_points"), num_generated_points);
            glUniform1f(shader->GetUniformLocation("max_rotate"), 2.0f * M_PI);

            // Vase 1 on table 2
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, table2Position + glm::vec3(0, tableTopHeight, 0));
            glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
            Mesh *vaseMesh = meshes["vase1"];
            glBindVertexArray(vaseMesh->GetBuffers()->m_VAO);
            glDrawElementsInstanced(vaseMesh->GetDrawMode(), vaseMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

            // Vase 2 on table 1
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, table1Position + glm::vec3(1.5f, tableTopHeight, 0));
            glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glDrawElementsInstanced(vaseMesh->GetDrawMode(), vaseMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

            // Table legs (both tables)
            Mesh *legMesh = meshes["table_leg"];
            glBindVertexArray(legMesh->GetBuffers()->m_VAO);
            float halfW = tableWidth / 2;
            float halfD = tableDepth / 2;

            glm::vec3 legPositions[8] = {
                // Table 2 legs
                table2Position + glm::vec3(-halfW + legInset, 0, -halfD + legInset),
                table2Position + glm::vec3(halfW - legInset, 0, -halfD + legInset),
                table2Position + glm::vec3(-halfW + legInset, 0, halfD - legInset),
                table2Position + glm::vec3(halfW - legInset, 0, halfD - legInset),
                // Table 1 legs
                table1Position + glm::vec3(-halfW + legInset, 0, -halfD + legInset),
                table1Position + glm::vec3(halfW - legInset, 0, -halfD + legInset),
                table1Position + glm::vec3(-halfW + legInset, 0, halfD - legInset),
                table1Position + glm::vec3(halfW - legInset, 0, halfD - legInset)};

            for (int i = 0; i < 8; i++)
            {
                modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, legPositions[i]);
                glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
                glDrawElementsInstanced(legMesh->GetDrawMode(), legMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);
            }

            // Lamp base
            glm::vec3 lampBasePosition = table2Position + glm::vec3(-1.5f, tableTopHeight, 0);
            Mesh *lampBaseMesh = meshes["lamp_base"];
            glBindVertexArray(lampBaseMesh->GetBuffers()->m_VAO);
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, lampBasePosition);
            glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glDrawElementsInstanced(lampBaseMesh->GetDrawMode(), lampBaseMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

            // Lamp shade
            Mesh *lampShadeMesh = meshes["lamp_shade"];
            glBindVertexArray(lampShadeMesh->GetBuffers()->m_VAO);
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, lampBasePosition + glm::vec3(0, 1.2f, 0));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(60.0f), glm::vec3(1, 0, 0));
            glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glDrawElementsInstanced(lampShadeMesh->GetDrawMode(), lampShadeMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);
        }

        {
            Shader *shader = shaders["TranslationSurfaceDepth"];
            shader->Use();

            glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
            glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
            glUniform1i(shader->GetUniformLocation("num_instances"), num_instances);
            glUniform1i(shader->GetUniformLocation("num_generated_points"), num_generated_points);

            Mesh *edgeMesh = meshes["table_edge"];
            glBindVertexArray(edgeMesh->GetBuffers()->m_VAO);

            float tblHeight = 0.7f;

            glm::vec3 tablePositions[2] = {table1Position, table2Position};
            for (int t = 0; t < 2; t++)
            {
                glm::vec3 currentTablePos = tablePositions[t];
                glm::mat4 modelMatrix;

                // Front edge: Start at front-left corner, translate along +X
                modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(-tableWidth / 2.0f, tblHeight, -tableDepth / 2.0f));
                glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
                glUniform1f(shader->GetUniformLocation("max_translate"), tableWidth - 0.05f);
                glUniform3f(shader->GetUniformLocation("translate_dir"), 1.0f, 0.0f, 0.0f);
                glDrawElementsInstanced(edgeMesh->GetDrawMode(), edgeMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

                // Left edge: Start at front-left corner, translate along +Z
                modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(-tableWidth / 2.0f, tblHeight, -tableDepth / 2.0f));
                glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
                glUniform1f(shader->GetUniformLocation("max_translate"), tableDepth - 0.02f);
                glUniform3f(shader->GetUniformLocation("translate_dir"), 0.0f, 0.0f, 1.0f);
                glDrawElementsInstanced(edgeMesh->GetDrawMode(), edgeMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

                // Right edge: Start at front-right corner, translate along +Z
                modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(tableWidth / 2.0f, tblHeight, -tableDepth / 2.0f));
                glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
                glUniform1f(shader->GetUniformLocation("max_translate"), tableDepth - 0.02f);
                glUniform3f(shader->GetUniformLocation("translate_dir"), 0.0f, 0.0f, 1.0f);
                glDrawElementsInstanced(edgeMesh->GetDrawMode(), edgeMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

                // Back edge: Start at back-left corner, translate along +X
                modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(-tableWidth / 2.0f, tblHeight, tableDepth / 2.0f));
                glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
                glUniform1f(shader->GetUniformLocation("max_translate"), tableWidth - 0.05f);
                glUniform3f(shader->GetUniformLocation("translate_dir"), 1.0f, 0.0f, 0.0f);
                glDrawElementsInstanced(edgeMesh->GetDrawMode(), edgeMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);
            }
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        camera->SetPosition(camPosition);
        camera->SetRotation(camRotation);
        camera->SetProjection(projectionInfo);
        camera->Update();
    }

    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::ivec2 resolution = window->GetResolution();
        glViewport(0, 0, resolution.x, resolution.y);
    }

    fireworksTime += deltaTimeSeconds;

    if (fireworksTime > fireworksLifetime)
    {
        fireworksTime = 0.0f;
        hasExploded = false;

        // Random Color
        fireworksColor = glm::vec3(
            (rand() % 100) / 100.0f,
            (rand() % 100) / 100.0f,
            (rand() % 100) / 100.0f);

        // Reset particles
        ResetParticlesFireworks(100, 100, 100);
    }

    // Render fireworks to framebuffer for TV display
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, fireworksWidth, fireworksHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLineWidth(1);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);

    {
        Shader *shader = shaders["ParticleShader"];
        if (shader && shader->GetProgramID())
        {
            shader->Use();

            glUniform3fv(shader->GetUniformLocation("generator_position"), 1, glm::value_ptr(generator_position));
            glUniform1f(shader->GetUniformLocation("deltaTime"), deltaTimeSeconds);
            glUniform1f(shader->GetUniformLocation("time"), fireworksTime);
            glUniform1f(shader->GetUniformLocation("lifetime"), fireworksLifetime);
            glUniform1i(shader->GetUniformLocation("hasExploded"), hasExploded ? 1 : 0);
            glUniform1f(shader->GetUniformLocation("offset"), offset);
            glUniform3fv(shader->GetUniformLocation("color"), 1, glm::value_ptr(fireworksColor));

            glm::vec3 eyePosition = glm::vec3(0, 0, 5); // Fixed eye position for framebuffer
            glUniform3fv(shader->GetUniformLocation("eye_position"), 1, glm::value_ptr(eyePosition));

            // Bind particle texture
            if (mapTextures.find("particle_texture") != mapTextures.end())
            {
                mapTextures["particle_texture"].BindToTextureUnit(GL_TEXTURE0);
                glUniform1i(shader->GetUniformLocation("texture_1"), 0);
            }

            particleEffect->Render(fireworksCamera, shader);
        }
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Restore viewport for main scene
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    // Scene layout positions - tables facing each other
    glm::vec3 table1Position = glm::vec3(0, 0, -4.0f); // Left side - with TV
    glm::vec3 table2Position = glm::vec3(0, 0, 2.0f);  // Right side - with lamp and vase
    float tableHeight = 0.7f;
    float tableTopHeight = 0.86f; // tableHeight + tableEdgeHeight

    // Draw room (walls, floor, ceiling) with shadow mapping
    {
        Shader *shader = shaders["ShadowPassTwo"];
        shader->Use();

        // Set common uniforms
        glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

        // Camera and light uniforms
        glm::vec3 eyePosition = camera->m_transform->GetWorldPosition();
        glUniform3fv(shader->GetUniformLocation("eye_position"), 1, glm::value_ptr(eyePosition));
        glUniform3fv(shader->GetUniformLocation("light_position"), 1, glm::value_ptr(light_position));
        glUniform3fv(shader->GetUniformLocation("light_direction"), 1, glm::value_ptr(light_direction));
        glUniform1f(shader->GetUniformLocation("light_intensity"), current_light_intensity);

        // Light space matrices for shadow mapping
        glUniformMatrix4fv(shader->GetUniformLocation("light_space_view"), 1, GL_FALSE, glm::value_ptr(light_space_view));
        glUniformMatrix4fv(shader->GetUniformLocation("light_space_projection"), 1, GL_FALSE, glm::value_ptr(light_space_projection));
        glUniform1f(shader->GetUniformLocation("light_space_far_plane"), light_space_far_plane);

        // Bind shadow depth texture to texture unit 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadow_depth_texture);
        glUniform1i(shader->GetUniformLocation("depth_texture"), 1);

        // Room settings (no texture)
        glUniform3f(shader->GetUniformLocation("object_color"), 0.6f, 0.6f, 0.6f); // Grey color
        glUniform1i(shader->GetUniformLocation("has_texture"), 0);                 // No texture

        // Floor at y = 0
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        Mesh *floorMesh = meshes["room_floor"];
        glBindVertexArray(floorMesh->GetBuffers()->m_VAO);
        glDrawElements(floorMesh->GetDrawMode(), floorMesh->indices.size(), GL_UNSIGNED_INT, 0);

        // Ceiling at y = roomHeight
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, roomHeight, 0));
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        Mesh *ceilingMesh = meshes["room_ceiling"];
        glBindVertexArray(ceilingMesh->GetBuffers()->m_VAO);
        glDrawElements(ceilingMesh->GetDrawMode(), ceilingMesh->indices.size(), GL_UNSIGNED_INT, 0);

        // Back wall (Z = -roomDepth/2)
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, roomHeight / 2, -roomDepth / 2));
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        Mesh *backWallMesh = meshes["room_wall_back"];
        glBindVertexArray(backWallMesh->GetBuffers()->m_VAO);
        glDrawElements(backWallMesh->GetDrawMode(), backWallMesh->indices.size(), GL_UNSIGNED_INT, 0);

        // Front wall (Z = roomDepth/2)
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, roomHeight / 2, roomDepth / 2));
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        Mesh *frontWallMesh = meshes["room_wall_front"];
        glBindVertexArray(frontWallMesh->GetBuffers()->m_VAO);
        glDrawElements(frontWallMesh->GetDrawMode(), frontWallMesh->indices.size(), GL_UNSIGNED_INT, 0);

        // Left wall (X = -roomWidth/2)
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-roomWidth / 2, roomHeight / 2, 0));
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        Mesh *leftWallMesh = meshes["room_wall_left"];
        glBindVertexArray(leftWallMesh->GetBuffers()->m_VAO);
        glDrawElements(leftWallMesh->GetDrawMode(), leftWallMesh->indices.size(), GL_UNSIGNED_INT, 0);

        // Right wall (X = roomWidth/2)
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(roomWidth / 2, roomHeight / 2, 0));
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        Mesh *rightWallMesh = meshes["room_wall_right"];
        glBindVertexArray(rightWallMesh->GetBuffers()->m_VAO);
        glDrawElements(rightWallMesh->GetDrawMode(), rightWallMesh->indices.size(), GL_UNSIGNED_INT, 0);
    }

    {
        Shader *shader = shaders["RotationSurface"];
        shader->Use();

        // Shadow mapping uniforms
        glUniform3fv(shader->GetUniformLocation("light_position"), 1, glm::value_ptr(light_position));
        glUniform3fv(shader->GetUniformLocation("light_direction"), 1, glm::value_ptr(light_direction));
        glUniform1f(shader->GetUniformLocation("light_intensity"), current_light_intensity);
        glUniformMatrix4fv(shader->GetUniformLocation("light_space_view"), 1, GL_FALSE, glm::value_ptr(light_space_view));
        glUniformMatrix4fv(shader->GetUniformLocation("light_space_projection"), 1, GL_FALSE, glm::value_ptr(light_space_projection));

        // Bind shadow depth texture to texture unit 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadow_depth_texture);
        glUniform1i(shader->GetUniformLocation("depth_texture"), 1);
    }

    // Draw vase 1 ON table 2 (rotation surface)
    {
        Shader *shader = shaders["RotationSurface"];
        shader->Use();

        glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
        glUniform1i(shader->GetUniformLocation("num_instances"), num_instances);
        glUniform1i(shader->GetUniformLocation("num_generated_points"), num_generated_points);
        glUniform1f(shader->GetUniformLocation("max_rotate"), 2.0f * M_PI);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        // Place vase on top of table 2
        modelMatrix = glm::translate(modelMatrix, table2Position + glm::vec3(0, tableTopHeight, 0));

        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        // glUniform3f(shader->GetUniformLocation("object_color"), 0.2f, 0.6f, 0.8f); // Blue-ish color

        glUniform1i(shader->GetUniformLocation("use_texture"), 1);
        mapTextures["vase_texture"].BindToTextureUnit(GL_TEXTURE0);
        glUniform1i(shader->GetUniformLocation("texture_1"), 0);

        Mesh *mesh = meshes["vase1"];
        glBindVertexArray(mesh->GetBuffers()->m_VAO);
        glDrawElementsInstanced(mesh->GetDrawMode(), mesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);
    }

    // Draw vase 2 ON table 1 (next to TV) (rotation surface)
    {
        Shader *shader = shaders["RotationSurface"];
        shader->Use();

        glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
        glUniform1i(shader->GetUniformLocation("num_instances"), num_instances);
        glUniform1i(shader->GetUniformLocation("num_generated_points"), num_generated_points);
        glUniform1f(shader->GetUniformLocation("max_rotate"), 2.0f * M_PI);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        // Place vase on top of table 1, to the right of TV
        modelMatrix = glm::translate(modelMatrix, table1Position + glm::vec3(1.5f, tableTopHeight, 0));

        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        // glUniform3f(shader->GetUniformLocation("object_color"), 0.8f, 0.4f, 0.2f);

        glUniform1i(shader->GetUniformLocation("use_texture"), 1);
        mapTextures["vase_texture"].BindToTextureUnit(GL_TEXTURE0);
        glUniform1i(shader->GetUniformLocation("texture_1"), 0);

        Mesh *mesh = meshes["vase1"];
        glBindVertexArray(mesh->GetBuffers()->m_VAO);
        glDrawElementsInstanced(mesh->GetDrawMode(), mesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);
    }

    // Draw TABLE 2 (with vase and lamp) - 4 legs
    {
        glm::vec3 currentTablePos = table2Position; // Table 2
        Shader *shader = shaders["RotationSurface"];
        shader->Use();

        glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
        glUniform1i(shader->GetUniformLocation("num_instances"), num_instances);
        glUniform1i(shader->GetUniformLocation("num_generated_points"), num_generated_points);
        glUniform1f(shader->GetUniformLocation("max_rotate"), 2.0f * M_PI);

        glUniform1i(shader->GetUniformLocation("use_texture"), 1);
        mapTextures["wood_texture"].BindToTextureUnit(GL_TEXTURE0);
        glUniform1i(shader->GetUniformLocation("texture_1"), 0);

        Mesh *legMesh = meshes["table_leg"];
        glBindVertexArray(legMesh->GetBuffers()->m_VAO);

        // Table dimensions
        float halfW = tableWidth / 2;
        float halfD = tableDepth / 2;

        // Position table legs relative to table position
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(-halfW + legInset, 0, -halfD + legInset)); // Front-left
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawElementsInstanced(legMesh->GetDrawMode(), legMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

        // Front-right
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(halfW - legInset, 0, -halfD + legInset));
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawElementsInstanced(legMesh->GetDrawMode(), legMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

        // Back-left
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(-halfW + legInset, 0, halfD - legInset));
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawElementsInstanced(legMesh->GetDrawMode(), legMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

        // Back-right
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(halfW - legInset, 0, halfD - legInset));
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawElementsInstanced(legMesh->GetDrawMode(), legMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);
    }

    // Setup shadow mapping uniforms for TranslationSurface shader
    {
        Shader *shader = shaders["TranslationSurface"];
        shader->Use();

        // Shadow mapping uniforms
        glUniform3fv(shader->GetUniformLocation("light_position"), 1, glm::value_ptr(light_position));
        glUniform3fv(shader->GetUniformLocation("light_direction"), 1, glm::value_ptr(light_direction));
        glUniform1f(shader->GetUniformLocation("light_intensity"), current_light_intensity);
        glUniformMatrix4fv(shader->GetUniformLocation("light_space_view"), 1, GL_FALSE, glm::value_ptr(light_space_view));
        glUniformMatrix4fv(shader->GetUniformLocation("light_space_projection"), 1, GL_FALSE, glm::value_ptr(light_space_projection));

        // Bind shadow depth texture to texture unit 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadow_depth_texture);
        glUniform1i(shader->GetUniformLocation("depth_texture"), 1);
    }

    // Draw table 2 top - 4 sides with translation surfaces
    {
        glm::vec3 currentTablePos = table2Position; // Table 2
        Shader *transShader = shaders["TranslationSurface"];
        transShader->Use();

        glUniformMatrix4fv(transShader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(transShader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
        glUniform1i(transShader->GetUniformLocation("num_instances"), num_instances);
        glUniform1i(transShader->GetUniformLocation("num_generated_points"), num_generated_points);

        glUniform1i(transShader->GetUniformLocation("use_texture"), 1);
        glActiveTexture(GL_TEXTURE0);
        mapTextures["wood_texture"].BindToTextureUnit(GL_TEXTURE0);
        glUniform1i(transShader->GetUniformLocation("texture_1"), 0);

        Mesh *edgeMesh = meshes["table_edge"];
        glBindVertexArray(edgeMesh->GetBuffers()->m_VAO);

        glm::mat4 modelMatrix = glm::mat4(1.0f);

        // Front edge: Start at front-left corner, translate along +X
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(-tableWidth / 2.0f, tableHeight, -tableDepth / 2.0f));
        glUniformMatrix4fv(transShader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform1f(transShader->GetUniformLocation("max_translate"), tableWidth - 0.05f);
        glUniform3f(transShader->GetUniformLocation("translate_dir"), 1.0f, 0.0f, 0.0f); // RED arrow (X)
        glDrawElementsInstanced(edgeMesh->GetDrawMode(), edgeMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

        // Left edge: Start at front-left corner, translate along +Z
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(-tableWidth / 2.0f, tableHeight, -tableDepth / 2.0f));
        glUniformMatrix4fv(transShader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform1f(transShader->GetUniformLocation("max_translate"), tableDepth - 0.02f);
        glUniform3f(transShader->GetUniformLocation("translate_dir"), 0.0f, 0.0f, 1.0f); // GREEN arrow (Z)
        glDrawElementsInstanced(edgeMesh->GetDrawMode(), edgeMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

        // SYMMETRICAL: Right and Back edges
        // Right edge: Start at front-right corner, translate along +Z
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(tableWidth / 2.0f, tableHeight, -tableDepth / 2.0f));
        glUniformMatrix4fv(transShader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform1f(transShader->GetUniformLocation("max_translate"), tableDepth - 0.02f);
        glUniform3f(transShader->GetUniformLocation("translate_dir"), 0.0, 0.0f, 1.0f); // GREEN arrow (Z)
        glDrawElementsInstanced(edgeMesh->GetDrawMode(), edgeMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

        // Back edge: Start at back-left corner, translate along +X
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(-tableWidth / 2.0f, tableHeight, tableDepth / 2.0f));
        glUniformMatrix4fv(transShader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform1f(transShader->GetUniformLocation("max_translate"), tableWidth - 0.05f); // Slightly shorter to avoid z-fighting at corners
        glUniform3f(transShader->GetUniformLocation("translate_dir"), 1.0, 0.0f, 0.0f);    // RED arrow (X)
        glDrawElementsInstanced(edgeMesh->GetDrawMode(), edgeMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);
    }

    // Setup shadow mapping uniforms for Simple shader
    {
        Shader *shader = shaders["Simple"];
        shader->Use();

        // Shadow mapping uniforms
        glUniform3fv(shader->GetUniformLocation("light_position"), 1, glm::value_ptr(light_position));
        glUniform3fv(shader->GetUniformLocation("light_direction"), 1, glm::value_ptr(light_direction));
        glUniform1f(shader->GetUniformLocation("light_intensity"), current_light_intensity);
        glUniformMatrix4fv(shader->GetUniformLocation("light_space_view"), 1, GL_FALSE, glm::value_ptr(light_space_view));
        glUniformMatrix4fv(shader->GetUniformLocation("light_space_projection"), 1, GL_FALSE, glm::value_ptr(light_space_projection));

        // Bind shadow depth texture to texture unit 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadow_depth_texture);
        glUniform1i(shader->GetUniformLocation("depth_texture"), 1);
    }

    // Draw table 2 top and bottom caps (simple rectangles)
    {
        glm::vec3 currentTablePos = table2Position; // Table 2
        Shader *simpleShader = shaders["Simple"];
        simpleShader->Use();

        glUniformMatrix4fv(simpleShader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(simpleShader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

        // Bind depth texture to unit 1 for shadow mapping
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadow_depth_texture);
        glUniform1i(simpleShader->GetUniformLocation("depth_texture"), 1);

        // Add wood texture to unit 0
        glUniform1i(simpleShader->GetUniformLocation("use_texture"), 1);
        glActiveTexture(GL_TEXTURE0);
        mapTextures["wood_texture"].BindToTextureUnit(GL_TEXTURE0);
        glUniform1i(simpleShader->GetUniformLocation("texture_1"), 0);

        Mesh *capMesh = meshes["table_cap"];
        glBindVertexArray(capMesh->GetBuffers()->m_VAO);

        float tableEdgeHeight = this->tableEdgeHeight;

        // Top cap
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(0, tableHeight + tableEdgeHeight, 0));
        glUniformMatrix4fv(simpleShader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawElements(capMesh->GetDrawMode(), capMesh->indices.size(), GL_UNSIGNED_INT, 0);

        // Bottom cap
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(0, tableHeight, 0));
        glUniformMatrix4fv(simpleShader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawElements(capMesh->GetDrawMode(), capMesh->indices.size(), GL_UNSIGNED_INT, 0);
    }

    // Draw TABLE 1 - 4 legs
    {
        glm::vec3 currentTablePos = table1Position; // Table 1
        Shader *shader = shaders["RotationSurface"];
        shader->Use();

        glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
        glUniform1i(shader->GetUniformLocation("num_instances"), num_instances);
        glUniform1i(shader->GetUniformLocation("num_generated_points"), num_generated_points);
        glUniform1f(shader->GetUniformLocation("max_rotate"), 2.0f * M_PI);

        glUniform1i(shader->GetUniformLocation("use_texture"), 1);
        mapTextures["wood_texture"].BindToTextureUnit(GL_TEXTURE0);
        glUniform1i(shader->GetUniformLocation("texture_1"), 0);

        Mesh *legMesh = meshes["table_leg"];
        glBindVertexArray(legMesh->GetBuffers()->m_VAO);

        // Table dimensions
        float halfW = tableWidth / 2;
        float halfD = tableDepth / 2;

        // Position table legs relative to table position
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(-halfW + legInset, 0, -halfD + legInset)); // Front-left
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawElementsInstanced(legMesh->GetDrawMode(), legMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

        // Front-right
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(halfW - legInset, 0, -halfD + legInset));
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawElementsInstanced(legMesh->GetDrawMode(), legMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

        // Back-left
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(-halfW + legInset, 0, halfD - legInset));
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawElementsInstanced(legMesh->GetDrawMode(), legMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

        // Back-right
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(halfW - legInset, 0, halfD - legInset));
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawElementsInstanced(legMesh->GetDrawMode(), legMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);
    }

    // Draw table 1 top - 4 sides with translation surfaces
    {
        glm::vec3 currentTablePos = table1Position; // Table 1
        Shader *transShader = shaders["TranslationSurface"];
        transShader->Use();

        glUniformMatrix4fv(transShader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(transShader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
        glUniform1i(transShader->GetUniformLocation("num_instances"), num_instances);
        glUniform1i(transShader->GetUniformLocation("num_generated_points"), num_generated_points);

        glUniform3f(transShader->GetUniformLocation("object_color"), 0.6f, 0.35f, 0.2f); // Wood color
        glUniform1i(transShader->GetUniformLocation("use_texture"), 1);
        mapTextures["wood_texture"].BindToTextureUnit(GL_TEXTURE0);
        glUniform1i(transShader->GetUniformLocation("texture_1"), 0);

        Mesh *edgeMesh = meshes["table_edge"];
        glBindVertexArray(edgeMesh->GetBuffers()->m_VAO);

        glm::mat4 modelMatrix = glm::mat4(1.0f);

        // Front edge
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(-tableWidth / 2.0f, tableHeight, -tableDepth / 2.0f));
        glUniformMatrix4fv(transShader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform1f(transShader->GetUniformLocation("max_translate"), tableWidth - 0.05f);
        glUniform3f(transShader->GetUniformLocation("translate_dir"), 1.0f, 0.0f, 0.0f);
        glDrawElementsInstanced(edgeMesh->GetDrawMode(), edgeMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

        // Left edge
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(-tableWidth / 2.0f, tableHeight, -tableDepth / 2.0f));
        glUniformMatrix4fv(transShader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform1f(transShader->GetUniformLocation("max_translate"), tableDepth - 0.02f);
        glUniform3f(transShader->GetUniformLocation("translate_dir"), 0.0f, 0.0f, 1.0f);
        glDrawElementsInstanced(edgeMesh->GetDrawMode(), edgeMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

        // Right edge
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(tableWidth / 2.0f, tableHeight, -tableDepth / 2.0f));
        glUniformMatrix4fv(transShader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform1f(transShader->GetUniformLocation("max_translate"), tableDepth - 0.02f);
        glUniform3f(transShader->GetUniformLocation("translate_dir"), 0.0, 0.0f, 1.0f);
        glDrawElementsInstanced(edgeMesh->GetDrawMode(), edgeMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);

        // Back edge
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(-tableWidth / 2.0f, tableHeight, tableDepth / 2.0f));
        glUniformMatrix4fv(transShader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform1f(transShader->GetUniformLocation("max_translate"), tableWidth - 0.05f);
        glUniform3f(transShader->GetUniformLocation("translate_dir"), 1.0, 0.0f, 0.0f);
        glDrawElementsInstanced(edgeMesh->GetDrawMode(), edgeMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);
    }

    // Draw table 1 top and bottom caps
    {
        glm::vec3 currentTablePos = table1Position; // Table 1
        Shader *simpleShader = shaders["Simple"];
        simpleShader->Use();

        glUniformMatrix4fv(simpleShader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(simpleShader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

        // Bind depth texture to unit 1 for shadow mapping
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadow_depth_texture);
        glUniform1i(simpleShader->GetUniformLocation("depth_texture"), 1);

        // Use wood texture
        glUniform1i(simpleShader->GetUniformLocation("use_texture"), 1);
        glActiveTexture(GL_TEXTURE0);
        mapTextures["wood_texture"].BindToTextureUnit(GL_TEXTURE0);
        glUniform1i(simpleShader->GetUniformLocation("texture_1"), 0);

        Mesh *capMesh = meshes["table_cap"];
        glBindVertexArray(capMesh->GetBuffers()->m_VAO);

        float tableEdgeHeight = this->tableEdgeHeight;

        // Top cap
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(0, tableHeight + tableEdgeHeight, 0));
        glUniformMatrix4fv(simpleShader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawElements(capMesh->GetDrawMode(), capMesh->indices.size(), GL_UNSIGNED_INT, 0);

        // Bottom cap
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, currentTablePos + glm::vec3(0, tableHeight, 0));
        glUniformMatrix4fv(simpleShader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawElements(capMesh->GetDrawMode(), capMesh->indices.size(), GL_UNSIGNED_INT, 0);
    }

    // Draw TV on TABLE 1
    float tvHeight = 1.3f; // TV box height
    glm::vec3 tvPosition = table1Position + glm::vec3(0, tableTopHeight + tvHeight / 2.0f, 0);

    // Draw TV carcass (box) with shadows
    {
        Shader *shader = shaders["ShadowPassTwo"];
        shader->Use();

        // Set common uniforms
        glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

        // Camera and light uniforms
        glm::vec3 eyePosition = camera->m_transform->GetWorldPosition();
        glUniform3fv(shader->GetUniformLocation("eye_position"), 1, glm::value_ptr(eyePosition));
        glUniform3fv(shader->GetUniformLocation("light_position"), 1, glm::value_ptr(light_position));
        glUniform3fv(shader->GetUniformLocation("light_direction"), 1, glm::value_ptr(light_direction));
        glUniform1f(shader->GetUniformLocation("light_intensity"), current_light_intensity);

        // Light space matrices for shadow mapping
        glUniformMatrix4fv(shader->GetUniformLocation("light_space_view"), 1, GL_FALSE, glm::value_ptr(light_space_view));
        glUniformMatrix4fv(shader->GetUniformLocation("light_space_projection"), 1, GL_FALSE, glm::value_ptr(light_space_projection));
        glUniform1f(shader->GetUniformLocation("light_space_far_plane"), light_space_far_plane);

        // Bind shadow depth texture to texture unit 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadow_depth_texture);
        glUniform1i(shader->GetUniformLocation("depth_texture"), 1);

        // Enable texture for carcass
        glUniform1i(shader->GetUniformLocation("has_texture"), 1);
        glActiveTexture(GL_TEXTURE0);
        mapTextures["grey_texture"].BindToTextureUnit(GL_TEXTURE0);
        glUniform1i(shader->GetUniformLocation("texture_1"), 0);

        Mesh *carcassMesh = meshes["tv_carcass"];
        glBindVertexArray(carcassMesh->GetBuffers()->m_VAO);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, tvPosition);
        // Apply rotation around Y axis (vertical axis)
        modelMatrix = glm::rotate(modelMatrix, tvRotationAngle, glm::vec3(0, 1, 0));
        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawElements(carcassMesh->GetDrawMode(), carcassMesh->indices.size(), GL_UNSIGNED_INT, 0);
    }

    {
        Shader *shader = shaders["RotationSurface"];
        shader->Use();

        glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
        glUniform1i(shader->GetUniformLocation("num_instances"), num_instances);
        glUniform1i(shader->GetUniformLocation("num_generated_points"), num_generated_points);
        glUniform1f(shader->GetUniformLocation("max_rotate"), M_PI);

        float carcassDepth = 1.5f;
        glm::vec3 screenOffsetLocal = glm::vec3(0, -0.5f, -carcassDepth / 2.0f);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, tvPosition);
        modelMatrix = glm::rotate(modelMatrix, tvRotationAngle, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, screenOffsetLocal);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 0.2f));

        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform3f(shader->GetUniformLocation("object_color"), 0.0f, 0.0f, 0.0f); 

        // Enable texture and bind the fireworks framebuffer texture
        glUniform1i(shader->GetUniformLocation("use_texture"), 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fireworksTexture);
        glUniform1i(shader->GetUniformLocation("texture_1"), 0);

        Mesh *screenMesh = meshes["tv_screen"];
        glBindVertexArray(screenMesh->GetBuffers()->m_VAO);
        glDrawElementsInstanced(screenMesh->GetDrawMode(), screenMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);
    }

    // Draw lamp on TABLE 2
    glm::vec3 lampBasePosition = table2Position + glm::vec3(-1.5f, tableTopHeight, 0);

    // Draw lamp base (rotation surface)
    {
        Shader *shader = shaders["RotationSurface"];
        shader->Use();

        glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
        glUniform1i(shader->GetUniformLocation("num_instances"), num_instances);
        glUniform1i(shader->GetUniformLocation("num_generated_points"), num_generated_points); 
        glUniform1f(shader->GetUniformLocation("max_rotate"), 2.0f * M_PI); 

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, lampBasePosition);

        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform3f(shader->GetUniformLocation("object_color"), 0.7f, 0.6f, 0.4f);

        glUniform1i(shader->GetUniformLocation("use_texture"), 0);

        Mesh *baseMesh = meshes["lamp_base"];
        glBindVertexArray(baseMesh->GetBuffers()->m_VAO);
        glDrawElementsInstanced(baseMesh->GetDrawMode(), baseMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);
    }

    // Draw lamp shade
    {
        Shader *shader = shaders["RotationSurface"];
        shader->Use();

        glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
        glUniform1i(shader->GetUniformLocation("num_instances"), num_instances);
        glUniform1i(shader->GetUniformLocation("num_generated_points"), num_generated_points);
        glUniform1f(shader->GetUniformLocation("max_rotate"), 2.0f * M_PI);

        // Position shade on top of base - adjust to connect better
        float baseHeight = 1.2f;                                                       // Top of lamp base profile
        glm::vec3 shadePosition = lampBasePosition + glm::vec3(0, baseHeight, +0.04f); // Slightly lower to connect

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, shadePosition);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(60.0f), glm::vec3(1, 0, 0));

        glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform3f(shader->GetUniformLocation("object_color"), 0.9f, 0.9f, 0.8f); // Light cream/white color

        glUniform1i(shader->GetUniformLocation("use_texture"), 0);

        Mesh *shadeMesh = meshes["lamp_shade"];
        glBindVertexArray(shadeMesh->GetBuffers()->m_VAO);
        glDrawElementsInstanced(shadeMesh->GetDrawMode(), shadeMesh->indices.size(), GL_UNSIGNED_INT, 0, num_instances);
    }
}

void Tema1::FrameEnd()
{
#if 0
    DrawCoordinateSystem();
#endif
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float rotationSpeed = 1.5f; // radians per second
        if (window->KeyHold(GLFW_KEY_1))
        {
            tvRotationAngle += rotationSpeed * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_2))
        {
            tvRotationAngle -= rotationSpeed * deltaTime;
        }
    }
}

void Tema1::OnKeyPress(int key, int mods)
{
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
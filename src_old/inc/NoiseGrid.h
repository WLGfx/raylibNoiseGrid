#ifndef NOISEGRID_H
#define NOISEGRID_H

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <thread>
#include <mutex>

#include "FastNoise.h"
#include "rlights.h"

struct vec2i { int x, y; };

struct vec3i { int x, y, z; };

class NoiseGrid;    // forward declaration

class NoiseChunk {
public:
    NoiseChunk();
    ~NoiseChunk();

    NoiseGrid *grid;

    vec3i position; // grid coords
    std::vector<float> data;
    std::vector<vec3i> blocks;
    std::vector<Matrix> transforms;

    void process();

    void initialise(NoiseGrid *grid);
    void set_block(int x, int y, int z, float value);
    float get_block(int x, int y, int z);
    bool is_block(int x, int y, int z);

    void generate_noise_data();
    void generate_block_data();
    void generate_instance_data();

    bool skip_noise = false;
    bool ready = false; // for the render pass
};

struct NoiseConfig {
    FastNoise noise;
    char name[16] = "Default";
    int operation = 0;
    float range_min = 0.0f;
    float range_max = 0.1f;
    float range_width = 0.1f;
};

class NoiseGrid
{
public:
    NoiseGrid();
    ~NoiseGrid();

    void update(Vector3 &follow_position);
    void render();
    void update_noise_range();
    void update_new_noise();

    void stop();

    Mesh mesh;
    Material material;

    vec3i grid_size = {3, 3, 3};
    vec3i grid_size_old = {0, 0, 0};
    vec3i chunk_size = {8, 8, 8};
    vec3i chunk_size_old = {0, 0, 0};
    vec3i grid_origin = {0, 0, 0};
    vec3i grid_origin_old = {999, 999, 999};

    vec3i bounds_start, bounds_end;

    Vector3 block_size = {1.0f, 1.0f, 1.0f};

    //FastNoise noise;
    std::vector<NoiseConfig> noise;
    //Vector2 noise_range = {0.70f, 1.0f};

    std::vector<NoiseChunk> chunks;
    std::vector<NoiseChunk *> chunks_free;
    std::vector<NoiseChunk *> chunks_used;

    std::vector<NoiseChunk *> chunks_process_queue;
    std::mutex chunks_process_mutex;
    std::vector<std::thread> proccess_threads;
    bool exit_threads = false;
    bool abandon_flag = false;

    // i want to include the text file directly into the binary
    // so that it can be read at runtime
    const char* shader_source_lighting_instancing_vs = R"(
#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
//in vec4 vertexColor;      // Not required

in mat4 instanceTransform;
in vec4 instanceDiffuse;

// Input uniform values
uniform mat4 mvp;
uniform mat4 matNormal;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;
out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;

// NOTE: Add your custom variables here

void main()
{
    // Send vertex attributes to fragment shader
    fragPosition = vec3(instanceTransform*vec4(vertexPosition, 1.0));
    fragTexCoord = vertexTexCoord;
    fragColor = vec4(1.0);
    //fragColor = instanceDiffuse;
    fragNormal = normalize(vec3(matNormal*vec4(vertexNormal, 1.0)));

    // Calculate final vertex position, note that we multiply mvp by instanceTransform
    gl_Position = mvp*instanceTransform*vec4(vertexPosition, 1.0);
}

    )";
    const char* shader_source_lighting_instancing_fs = R"(
#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add your custom variables here

#define     MAX_LIGHTS              4
#define     LIGHT_DIRECTIONAL       0
#define     LIGHT_POINT             1

struct Light {
    int enabled;
    int type;
    vec3 position;
    vec3 target;
    vec4 color;
};

// Input lighting values
uniform Light lights[MAX_LIGHTS];
uniform vec4 ambient;
uniform vec3 viewPos;

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec3 lightDot = vec3(0.0);
    vec3 normal = normalize(fragNormal);
    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 specular = vec3(0.0);

    vec4 tint = colDiffuse*fragColor;

    // NOTE: Implement here your fragment shader code

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        if (lights[i].enabled == 1)
        {
            vec3 light = vec3(0.0);

            if (lights[i].type == LIGHT_DIRECTIONAL)
            {
                light = -normalize(lights[i].target - lights[i].position);
            }

            if (lights[i].type == LIGHT_POINT)
            {
                light = normalize(lights[i].position - fragPosition);
            }

            float NdotL = max(dot(normal, light), 0.0);
            lightDot += lights[i].color.rgb*NdotL;

            float specCo = 0.0;
            if (NdotL > 0.0) specCo = pow(max(0.0, dot(viewD, reflect(-(light), normal))), 16.0); // 16 refers to shine
            specular += specCo;
        }
    }

    finalColor = (texelColor*((tint + vec4(specular, 1.0))*vec4(lightDot, 1.0)));
    finalColor += texelColor*(ambient/10.0)*tint;

    // Gamma correction
    finalColor = pow(finalColor, vec4(1.0/2.2));
}

    )";

    Light light_sun;
    Light light_point;
private:
    bool active = false;

    static void process_thread(NoiseGrid *grid);

    bool chunks_changed();
    bool grid_changed();
    //void rebuild_grid();
    bool check_bounds();
    void rebuild_chunks();
    void fill_bounds();
};

#endif // NOISEGRID_H

#include "rlshaders.h"
#include "raylib.h"

void shader_lighting_instancing::init() {
    shader = LoadShaderFromMemory(vs, fs);
    shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(shader, "mvp");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shader, "instanceTransform");

    int ambientLoc = GetShaderLocation(shader, "ambient");
    float ambientCol[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    SetShaderValue(
        shader,
        ambientLoc,
        ambientCol,
        SHADER_UNIFORM_VEC4);
    
    lights.sun = CreateLight(LIGHT_DIRECTIONAL, Vector3{0, 10, 0}, Vector3{0, -1, 0}, DARKGRAY, shader);
    lights.point = CreateLight(LIGHT_POINT, Vector3{0, 5, 0}, Vector3{0, 0, 0}, {144, 70, 32,255}, shader);
}

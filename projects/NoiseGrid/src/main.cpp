#include "raylib.h"

#include "inc/raygui+.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#include "inc/myui.h"

#define RLIGHTS_IMPLEMENTATION
#include "inc/rlights.h"
#undef RLIGHTS_IMPLEMENTATION

// had to undef rlights as it is include again in noisegrid
// it's implementation is built here...

#include "rcamera.h"

#include "inc/NoiseGrid.h"

// New title for this - FastNoise x3 Playground

NoiseGrid grid;

Camera camera = {
    .position = {0.0f, 140.0f, -280.0f},
    .target = {0.0f, 0.0f, 0.0f},
    .up = {0.0f, 1.0f, 0.0f},
    .fovy = 45.0f,
    .projection = CAMERA_PERSPECTIVE
};

void init_ui()
{
    // Setup raygui default style for text size to 12
    GuiLoadStyleDefault();

    GuiSetStyle(DEFAULT, TEXT_SIZE, 32);
    GuiSetStyle(DEFAULT, TEXT_PADDING, 8);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xccccccff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0x888888ff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, 0xddddddff);

    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x222222ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0x444444ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x888888ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, 0x222222ff);

    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0x000000ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, 0x444444ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, 0x888888ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_DISABLED, 0x222222ff);

    GuiSetStyle(DEFAULT, BORDER_WIDTH, 4);

    GuiSetStyle(VALUEBOX, SPINNER_BUTTON_WIDTH, 50);
    //GuiSetStyle(DEFAULT, SPINNER_BUTTON_SPACING, 20);
}

void init_grid_mesh_and_material() {
    grid.mesh = GenMeshCube(1, 1, 1);
    grid.material = LoadMaterialDefault();
    grid.material.shader = LoadShaderFromMemory(
        grid.shader_source_lighting_instancing_vs, 
        grid.shader_source_lighting_instancing_fs);
    
    grid.material.shader.locs[SHADER_LOC_MATRIX_MVP] = 
        GetShaderLocation(grid.material.shader, 
        "mvp");
    grid.material.shader.locs[SHADER_LOC_VECTOR_VIEW] = 
        GetShaderLocation(grid.material.shader, 
        "viewPos");

    int ambientLoc = GetShaderLocation(grid.material.shader, "ambient");
    float ambientCol[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    SetShaderValue(
        grid.material.shader,
        ambientLoc,
        ambientCol,
        SHADER_UNIFORM_VEC4);

    grid.light_sun = CreateLight(LIGHT_DIRECTIONAL,
                {50, 750, 0},
                {0, 0, 0},
                DARKGRAY,
                grid.material.shader);

    grid.light_point = CreateLight(LIGHT_POINT,
                {-50, -50, 0},
                {0, 0, 0},
                {144, 154, 148,255},
                grid.material.shader);

    // this is the line missing from the online examples and docs
    grid.material.shader.locs[SHADER_LOC_MATRIX_MODEL] = 
        GetShaderLocationAttrib(
            grid.material.shader, 
            "instanceTransform");

}

void update_noise_grid() {
    grid.update(camera.target);
    grid.render();
}

void draw_3d()
{
    grid.light_point.position = camera.position;
    UpdateLightValues(grid.material.shader, grid.light_point);

    BeginMode3D(camera);
    DrawGrid(100, 8.0f);

    update_noise_grid();

    EndMode3D();
}

// ######################### MAIN #########################

int main() {
    MYUI myui(&camera, &grid);

    // Initialization
    int screenWidth = 1280;
    int screenHeight = 720;
    
    InitWindow(screenWidth, screenHeight, "FastNoise x3 Playground (C)WLGfx Carl S Norwood 2026"); // corrected the year (Wed 27 Jan 26-12:47GMT)
    SetTargetFPS(60);

    SetWindowState(
        FLAG_WINDOW_RESIZABLE |
        FLAG_WINDOW_HIGHDPI |
        FLAG_VSYNC_HINT
    );

    init_ui();
    init_grid_mesh_and_material();

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        
        // Draw
        BeginDrawing();
            ClearBackground(DARKBROWN);
            
            draw_3d();
            
            //draw_ui_grid();
            //draw_camera_ui();

            myui.draw();
            
            DrawFPS(10, screenHeight - 30);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

/*

https://raylibtech.itch.io/rguiicons Online icons list and preview



*/
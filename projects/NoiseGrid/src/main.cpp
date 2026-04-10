#include "raylib.h"

#include "inc/r/rCamera/rCamera.h"

#include "inc/r/raygui+/raygui+.h"
#include "inc/ui2.h"
#include "inc/myui.h"
#include "inc/NoiseGrid/NoiseGrid.h"

// New title for this - FastNoise x3 Playground

rlCamera camera_main = {{0.0f, 140.0f, -280.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f, CAMERA_PERSPECTIVE};

void init_ui_style()
{
    GuiLoadStyleDefault();

    GuiSetStyle(DEFAULT, TEXT_SIZE, 32);
    GuiSetStyle(DEFAULT, TEXT_PADDING, 8);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xccccccff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0x888888ff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, 0xddddddff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, 0x767666ff);

    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x222222ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0x444444ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x888888ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, 0x000000ff);

    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0x000000ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, 0x444444ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, 0x888888ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_DISABLED, 0x323222ff);

    GuiSetStyle(DEFAULT, BORDER_WIDTH, 4);

    GuiSetStyle(VALUEBOX, SPINNER_BUTTON_WIDTH, 50);
    GuiSetStyle(VALUEBOX, SPINNER_BUTTON_SPACING, 2);

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x101010b0);
    GuiSetStyle(DROPDOWNBOX, ARROW_PADDING, 20);
    GuiSetStyle(TOGGLE, GROUP_PADDING, 10);
    GuiSetStyle(SLIDER, SLIDER_WIDTH, 50);
    GuiSetStyle(SLIDER, SLIDER_PADDING, 2);
}

void init_mesh_and_material(NoiseGrid& grid) {
    grid.mesh = GenMeshCube(1, 1, 1);
    grid.material = LoadMaterialDefault();
    
    grid.shader.init();
    grid.material.shader = grid.shader.shader;
}

void update_noise_grid(NoiseGrid& grid) {
    grid.update(camera_main.target);
    grid.render();
}

void draw_3d(NoiseGrid& grid)
{
    grid.shader.lights.point.position = camera_main.position;
    UpdateLightValues(grid.material.shader, grid.shader.lights.point);

    BeginMode3D(camera_main);

    DrawGrid(100, 8.0f);
    update_noise_grid(grid);

    EndMode3D();
}

// ######################### MAIN #########################

int main() {
    
    // Initialization
    int screenWidth = 1280;
    int screenHeight = 720;
    
    InitWindow(screenWidth, screenHeight, "FastNoise x3 Playground (C)WLGfx Carl S Norwood 2026"); // corrected the year (Wed 27 Jan 26-12:47GMT)
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    
    init_ui_style();
    
    NoiseGrid grid;
    
    MYUI myui(&camera_main, &grid);
    UI2 ui2;

    init_mesh_and_material(grid);
    //myui.update(); // initial update
    //ui2.update(); // initial update

    // Main game loop
    while (!WindowShouldClose()) {
        myui.update();
        ui2.update();

        // Draw everything
        BeginDrawing();
            ClearBackground(DARKBROWN);
            
            draw_3d(grid);
            myui.draw();
            ui2.hbox.draw();

            DrawFPS(10, screenHeight - 30);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

/*

https://raylibtech.itch.io/rguiicons Online icons list and preview
 
#embed "file.bin" - C++ 23 GCC 14 Clang 18

    static const std::span<const unsigned char> data = {
        #embed "input.bin"
    };

*/
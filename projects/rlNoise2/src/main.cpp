#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "New Raylib Project");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            DrawText("New Raylib Project", 190, 200, 20, LIGHTGRAY);
            DrawText("This is your new project workspace!", 160, 240, 20, LIGHTGRAY);
            
            // Example GUI element
            if (GuiButton((Rectangle){ 350, 300, 100, 50 }, "Click Me")) {
                // Button clicked
            }
            
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

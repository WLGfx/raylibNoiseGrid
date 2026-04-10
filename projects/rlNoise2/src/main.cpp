#include "Game.h"

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "WLGfx's new Super Dooper Project");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    Game game;
    game.run();

    CloseWindow();
    return 0;
}

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

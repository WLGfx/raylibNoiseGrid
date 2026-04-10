/*******************************************************************************************
*
*   LayoutName v1.0.0 - Tool Description
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 raylib technologies. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "layout_name");

    // layout_name: controls initialization
    //----------------------------------------------------------------------------------
    float target_slider_xValue = 0.0f;
    float target_slider_yValue = 0.0f;
    float target_slider_zValue = 0.0f;
    bool target_reset_buttonPressed = false;
    bool range_checkChecked = false;
    float position_slider_xValue = 0.0f;
    float position_slider_yValue = 0.0f;
    float position_slider_zValue = 0.0f;
    bool debug_grid_checkChecked = false;
    bool map_bounds_checkChecked = false;
    bool orbit_checkChecked = false;
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Implement required update logic
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            // raygui: controls drawing
            //----------------------------------------------------------------------------------
            GuiGroupBox((Rectangle){ 8, 24, 1096, 312 }, "View");

            GuiSlider((Rectangle){ 24, 104, 408, 48 }, NULL, NULL, &target_slider_xValue, 0, 100);
            GuiSlider((Rectangle){ 24, 160, 408, 48 }, NULL, NULL, &target_slider_yValue, 0, 100);
            GuiSlider((Rectangle){ 24, 216, 408, 48 }, NULL, NULL, &target_slider_zValue, 0, 100);

            target_reset_buttonPressed = GuiButton((Rectangle){ 24, 272, 120, 48 }, "Reset");
            GuiLine((Rectangle){ 24, 48, 408, 48 }, "Target");

            GuiCheckBox((Rectangle){ 872, 216, 48, 48 }, "View range", &range_checkChecked);

            GuiLine((Rectangle){ 448, 48, 400, 48 }, "Position");
            GuiSlider((Rectangle){ 448, 104, 400, 48 }, NULL, NULL, &position_slider_xValue, 0, 100);
            GuiSlider((Rectangle){ 448, 160, 400, 48 }, NULL, NULL, &position_slider_yValue, 0, 100);
            GuiSlider((Rectangle){ 448, 216, 400, 48 }, NULL, NULL, &position_slider_zValue, 0, 100);

            GuiCheckBox((Rectangle){ 872, 104, 48, 48 }, "View grid", &debug_grid_checkChecked);

            GuiLine((Rectangle){ 864, 48, 216, 48 }, "Debug");
            GuiCheckBox((Rectangle){ 872, 160, 48, 48 }, "Map bounds", &map_bounds_checkChecked);
            GuiCheckBox((Rectangle){ 872, 272, 48, 48 }, "Orbit", &orbit_checkChecked);
            //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------


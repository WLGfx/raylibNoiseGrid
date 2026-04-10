#include "Game.h"

#include "raymath.h"

Game::Game() {
    _init_ui();
}

Game::~Game() {

}

void Game::run() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKBROWN);

        BeginMode3D(camera);
        _debug();
        EndMode3D();

        _ui_handler();

        EndDrawing();
    }
}

void Game::_debug() {

    float chunk_size = ui.dimensions.chunk_sizes[ui.dimensions.chunk_sizeActive];
    float view_range = ui.dimensions.view_ranges[ui.dimensions.visible_rangeActive];

    // draw ground grid based on chunk_size and view range

    if (ui.view.debug_grid_checkChecked)
    {
        DrawGrid(chunk_size * view_range, chunk_size);
    }

    // show the maps bounds

    if (ui.view.map_bounds_checkChecked)
    {
        float w = ui.dimensions.map_wid_sliderValue * chunk_size;
        float h = ui.dimensions.map_hgt_sliderValue * chunk_size;
        float d = ui.dimensions.map_dep_sliderValue * chunk_size;

        DrawCubeWires({0,0,0}, w, h, d, BLUE);
    }

    // show the current view range bounds

    if (ui.view.range_checkChecked)
    {
        float x = int(camera.target.x / chunk_size) - view_range;
        float y = int(camera.target.y / chunk_size) - view_range;
        float z = int(camera.target.z / chunk_size) - view_range;

        float w = (view_range * 2 + 1) * chunk_size;
        float h = (view_range * 2 + 1) * chunk_size;
        float d = (view_range * 2 + 1) * chunk_size;

        x = (x * chunk_size) + w / 2;
        y = (y * chunk_size) + h / 2;
        z = (z * chunk_size) + d / 2;

        DrawCubeWires({x, y, z}, w, h, d, RED);
    }
}

void Game::_init_ui() {
    GuiSetStyle(DEFAULT, TEXT_SIZE,             32);
    GuiSetStyle(DEFAULT, TEXT_PADDING,          8);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL,     0xb8b8b0ff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED,    0x888888ff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED,    0xc8c8ccff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED,   0x767666ff);

    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL,     0x222222ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED,    0x444444ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED,    0x888888ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED,   0x000000ff);

    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL,   0x000000ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED,  0x444444ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED,  0x888888ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_DISABLED, 0x323222ff);

    GuiSetStyle(DEFAULT, BORDER_WIDTH,          4);

    GuiSetStyle(VALUEBOX, SPINNER_BUTTON_WIDTH, 50);
    GuiSetStyle(VALUEBOX, SPINNER_BUTTON_SPACING, 2);

    GuiSetStyle(DEFAULT, BACKGROUND_COLOR,      0x101010b0);
    GuiSetStyle(DROPDOWNBOX, ARROW_PADDING,     20);
    GuiSetStyle(TOGGLE, GROUP_PADDING,          10);
    GuiSetStyle(SLIDER, SLIDER_WIDTH,           32);
    GuiSetStyle(SLIDER, SLIDER_PADDING,         2);
    GuiSetStyle(CHECKBOX, CHECK_PADDING,        2);
}

void Game::_ui_handler()
{
    float chunk_size = ui.dimensions.chunk_sizes[ui.dimensions.chunk_sizeActive];

    // update ui.view values

    ui.view.position_slider_xValue = camera.position.x;
    ui.view.position_slider_yValue = camera.position.y;
    ui.view.position_slider_zValue = camera.position.z;

    ui.view.target_slider_xValue = camera.target.x;
    ui.view.target_slider_yValue = camera.target.y;
    ui.view.target_slider_zValue = camera.target.z;

    // set ranges of sliders according to distance from target

    float distance_from_target = Vector3Distance(camera.position, camera.target);

    ui.view.position_x_min = ui.view.position_y_min = ui.view.position_z_min = -distance_from_target;
    ui.view.position_x_max = ui.view.position_y_max = ui.view.position_z_max =  distance_from_target;

    // set targets min and max based on map dimensions

    ui.view.target_x_max = ui.dimensions.map_wid_sliderValue * chunk_size * 0.5f;
    ui.view.target_x_min = -ui.view.target_x_max;

    ui.view.target_y_max = ui.dimensions.map_hgt_sliderValue * chunk_size * 0.5f;
    ui.view.target_y_min = -ui.view.target_y_max;

    ui.view.target_z_max = ui.dimensions.map_dep_sliderValue * chunk_size * 0.5f;
    ui.view.target_z_min = -ui.view.target_z_max;

    ui.menu.draw();

    if (ui.menu.active == 0) ui.dimensions.draw();
    if (ui.menu.active == 1) ui.noise.draw();
    if (ui.menu.active == 2) ui.view.draw();

    if (ui.view.target_x_result)
    {
        // move cameras target and position x to new value

        float difference = ui.view.target_slider_xValue - camera.target.x;

        camera.target.x = ui.view.position_slider_xValue;
        camera.position.x += difference;
    }

    if (ui.view.orbit_checkChecked) {
        UpdateCamera(&camera, CAMERA_ORBITAL);
    }
}
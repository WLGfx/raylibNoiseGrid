#ifndef GUI_VIEW_H
#define GUI_VIEW_H

#include "raygui.h"

struct GuiView {
    float target_slider_xValue = 0.0f, target_x_min = 0, target_x_max = 0;
    float target_slider_yValue = 0.0f, target_y_min = 0, target_y_max = 0;
    float target_slider_zValue = 0.0f, target_z_min = 0, target_z_max = 0;

    float position_slider_xValue = 0.0f, position_x_min = 0, position_x_max = 0;
    float position_slider_yValue = 0.0f, position_y_min = 0, position_y_max = 0;
    float position_slider_zValue = 0.0f, position_z_min = 0, position_z_max = 0;

    bool debug_grid_checkChecked = true;
    bool map_bounds_checkChecked = true;
    bool orbit_checkChecked      = true;
    bool range_checkChecked      = true;

    int target_x_result, target_y_result, target_z_result;

    bool target_reset_buttonPressed = false;

    void draw() {
        float h = GetScreenHeight() - 312 - 88;

        GuiDummyRec({ 16,h+32,1104-16,296 },nullptr);

        GuiGroupBox({ 8, h+24, 1104, 312 }, "View");

        GuiLine({ 24, h+48, 408, 48 }, "Target");
        target_x_result = GuiSlider({ 24, h+104, 408, 48 }, nullptr, nullptr, &target_slider_xValue, target_x_min, target_x_max);
        target_y_result = GuiSlider({ 24, h+160, 408, 48 }, nullptr, nullptr, &target_slider_yValue, target_y_min, target_y_max);
        target_z_result = GuiSlider({ 24, h+216, 408, 48 }, nullptr, nullptr, &target_slider_zValue, target_z_min, target_z_max);

        GuiLine(  { 448, h+48, 408, 48 }, "Position");
        GuiSlider({ 448, h+104, 408, 48 }, nullptr, nullptr, &position_slider_xValue, position_x_min, position_x_max);
        GuiSlider({ 448, h+160, 408, 48 }, nullptr, nullptr, &position_slider_yValue, position_y_min, position_y_max);
        GuiSlider({ 448, h+216, 408, 48 }, nullptr, nullptr, &position_slider_zValue, position_z_min, position_z_max);

        GuiLine(    { 872, h+48, 216, 48 }, "Debug");
        GuiCheckBox({ 872, h+104, 48, 48 }, "View grid",  &debug_grid_checkChecked);
        GuiCheckBox({ 872, h+160, 48, 48 }, "Map bounds", &map_bounds_checkChecked);
        GuiCheckBox({ 872, h+216, 48, 48 }, "Orbit",      &orbit_checkChecked);
        GuiCheckBox({ 872, h+272, 48, 48 }, "View range", &range_checkChecked);

        target_reset_buttonPressed = GuiButton({ 24, h+272, 120, 48 }, "Reset");
    }
};

#endif

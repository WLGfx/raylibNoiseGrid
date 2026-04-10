/*******************************************************************************************
*
*   LayoutName v1.0.0 - Tool Description
*
*   MODULE USAGE:
*       #define GUI_LAYOUT_NAME_IMPLEMENTATION
*       #include "gui_layout_name.h"
*
*       INIT: GuiLayoutNameState state = InitGuiLayoutName();
*       DRAW: GuiLayoutName(&state);
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

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#ifndef GUI_DIMENSIONS_H
#define GUI_DIMENSIONS_H

struct GuiDimensions {
    
    bool  chunk_sizeEditMode    = false;
    bool  map_wid_valueEditMode = false;
    bool  map_hgt_valueEditMode = false;
    bool  map_dep_valueEditMode = false;
    bool  visible_rangeEditMode = false;
    float map_wid_sliderValue   = 4;
    float map_hgt_sliderValue   = 4;
    float map_dep_sliderValue   = 4;
    int   chunk_sizeActive      = 0;
    int   map_wid_valueValue    = 4;
    int   map_hgt_valueValue    = 4;
    int   map_dep_valueValue    = 4;
    int   visible_rangeActive   = 0;

    int chunk_sizes[9] = {8,16,24,32,48,56,64};
    int view_ranges[10] = {1,2,3,4,5,6,7,8,12,14};

    void draw()
    {
        if (chunk_sizeEditMode || visible_rangeEditMode) GuiLock();

        float h = GetScreenHeight() - 328 - 80;

        GuiGroupBox({ 0, h+8, 832, 328 }, "Dimensions");
        GuiDummyRec({ 8, h+24, 816, 64 }, nullptr);
        GuiDummyRec({ 8, h+104, 816, 224 }, nullptr);

        GuiLine({ 16, h+32, 208, 48 }, "Chunk size");
        GuiLine({ 408, h+32, 240, 48 }, "Visible range");
        GuiLine({ 16, h+104, 800, 48 }, "Map Size");

        if (GuiSliderBar({ 16, h+152, 624, 48 }, nullptr, nullptr, &map_wid_sliderValue, 4, 64))  map_wid_sliderValue = map_wid_valueValue = (int)(map_wid_sliderValue / 2) * 2;
        if (GuiSliderBar({ 16, h+208, 624, 48 }, nullptr, nullptr, &map_hgt_sliderValue, 4, 64))  map_hgt_sliderValue = map_hgt_valueValue = (int)(map_hgt_sliderValue / 2) * 2;
        if (GuiSliderBar({ 16, h+264, 624, 48 }, nullptr, nullptr, &map_dep_sliderValue, 4, 128)) map_dep_sliderValue = map_dep_valueValue = (int)(map_dep_sliderValue / 2) * 2;
        
        GuiDisable();
        GuiValueBox({ 712, h+152, 104, 48 }, "Wid", &map_wid_valueValue, 0, 1024, map_wid_valueEditMode);
        GuiValueBox({ 712, h+208, 104, 48 }, "Hgt", &map_hgt_valueValue, 0, 1024, map_hgt_valueEditMode);
        GuiValueBox({ 712, h+264, 104, 48 }, "Dep", &map_dep_valueValue, 0, 1024, map_dep_valueEditMode);
        GuiEnable();

        GuiSetStyle(DROPDOWNBOX, DROPDOWN_ROLL_UP, 1);
        if (GuiDropdownBox({ 240, h+32, 152, 48 }, "8;16;24;32;48;56;64", &chunk_sizeActive, chunk_sizeEditMode)) chunk_sizeEditMode = !chunk_sizeEditMode;
        if (GuiDropdownBox({ 664, h+32, 152, 48 }, "1;2;3;4;5;6;7;8;12;16;24", &visible_rangeActive, visible_rangeEditMode)) visible_rangeEditMode = !visible_rangeEditMode;
        GuiSetStyle(DROPDOWNBOX, DROPDOWN_ROLL_UP, 0);

        GuiUnlock();
    }
};


#endif // GUI_LAYOUT_NAME_IMPLEMENTATION

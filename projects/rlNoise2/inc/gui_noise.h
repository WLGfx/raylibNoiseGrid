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

//#include "FastNoise/FastNoise.h"


#ifndef GUI_LAYOUT_NOISE_H
#define GUI_LAYOUT_NOISE_H

struct GuiNoise 
{
    GuiNoise() = default;

    float noise_fromValue = 0.0f;
    float noise_toValue   = 0.1f;

    float noise_frequency_sliderValue  = 0.02f;
    float noise_lacunarity_sliderValue = 0.10f;
    float noise_gain_sliderValue       = 0.10f;
    
    bool noise_to_valueEditMode         = false;    int noise_to_valueValue          = 0;
    bool noise_from_valueEditMode       = false;    int noise_from_valueValue        = 0;
    bool noise_typeEditMode             = false;    int noise_typeActive             = 0;
    bool noise_interpEditMode           = false;    int noise_interpActive           = 0;
    bool noise_fractalEditMode          = false;    int noise_fractalActive          = 0;
    bool noise_seedEditMode             = false;    int noise_seedValue              = 0;
    bool noise_frequency_valueEditMode  = false;    int noise_frequency_valueValue   = 0;
    bool noise_octavesEditMode          = false;    int noise_octavesValue           = 0;
    bool noise_lacunarity_valueEditMode = false;    int noise_lacunarity_valueValue  = 0;
    bool noise_gain_valueEditMode       = false;    int noise_gain_valueValue        = 0;
    
    bool cell_distance_functionEditMode = false;    int cell_distance_functionActive = 0;
    bool cell_return_typeEditMode       = false;    int cell_return_typeActive       = 0;
    bool cell_indice0EditMode           = false;    int cell_indice0Value            = 0;
    bool cell_indice1EditMode           = false;    int cell_indice1Value            = 0;
    bool cell_jitter_valueEditMode      = false;    int cell_jitter_valueValue       = 0;
    
    float cell_jitterValue = 0;
    
    bool look_seedEditMode              = false;    int look_seedValue             = 0;
    bool look_noise_typeEditMode        = false;    int look_noise_typeActive      = 0;
    bool look_interpEditMode            = false;    int look_interpActive          = 0;
    bool look_fractalEditMode           = false;    int look_fractalActive         = 0;
    bool look_octavesEditMode           = false;    int look_octavesValue          = 0;
    bool look_frequency_valueEditMode   = false;    int look_frequency_valueValue  = 0;
    bool look_lacunarity_valueEditMode  = false;    int look_lacunarity_valueValue = 0;
    bool look_gain_valueEditMode        = false;    int look_gain_valueValue       = 0;

    float look_frequencyValue  = 0;
    float look_lacunarityValue = 0;
    float look_gainValue       = 0;

    //FastNoise noise, lookup;

    void draw()
    {
        // determine height of UI based on options set

        bool cell_active   = noise_typeActive == 6;         // Cellular selected
        bool lookup_active = cell_return_typeActive == 1;   // Noise lookup selected

        float h = GetScreenHeight() - 720 - 80;

        if (noise_typeEditMode || noise_interpEditMode || noise_fractalEditMode || cell_distance_functionEditMode || cell_return_typeEditMode || look_noise_typeEditMode || look_interpEditMode || look_fractalEditMode) GuiLock();
    
        GuiGroupBox({ 8,  h+16, 1216, 720 }, "Noise");
        GuiDummyRec({ 16, h+32, 1200, 696 }, nullptr);
        
        GuiLine(        { 32,   h+48, 256, 48 },  "Noise range");
        if (GuiSpinner( { 1048, h+104, 160, 48 }, "Octaves", &noise_octavesValue, 1, 10,    noise_octavesEditMode)) noise_octavesEditMode = !noise_octavesEditMode;
        if (GuiValueBox({ 112,  h+104, 128, 48 }, "Seed",    &noise_seedValue,    0, 99999, noise_seedEditMode)) noise_seedEditMode = !noise_seedEditMode;
        GuiDisable();

        if (GuiValueBox({ 184, h+160, 176, 48 }, "Frequency",  &noise_frequency_valueValue,  0, 2, noise_frequency_valueEditMode)) noise_frequency_valueEditMode = !noise_frequency_valueEditMode;
        if (GuiValueBox({ 184, h+216, 176, 48 }, "Lacunarity", &noise_lacunarity_valueValue, 0, 2, noise_lacunarity_valueEditMode)) noise_lacunarity_valueEditMode = !noise_lacunarity_valueEditMode;
        if (GuiValueBox({ 184, h+272, 176, 48 }, "Gain",       &noise_gain_valueValue,       0, 2, noise_gain_valueEditMode)) noise_gain_valueEditMode = !noise_gain_valueEditMode;
        if (GuiValueBox({ 640, h+48, 96, 48 },   nullptr,      &noise_from_valueValue,      -2, 2, noise_from_valueEditMode)) noise_from_valueEditMode = !noise_from_valueEditMode;
        if (GuiValueBox({ 768, h+48, 96, 48 },   "<",          &noise_to_valueValue,        -2, 2, noise_to_valueEditMode)) noise_to_valueEditMode = !noise_to_valueEditMode;

        GuiEnable();
        GuiSliderBar({ 368, h+160, 840, 48 }, nullptr, nullptr, &noise_frequency_sliderValue,  0, 2);
        GuiSliderBar({ 368, h+216, 840, 48 }, nullptr, nullptr, &noise_lacunarity_sliderValue, 0, 2);
        GuiSliderBar({ 368, h+272, 840, 48 }, nullptr, nullptr, &noise_gain_sliderValue,       0, 2);
        GuiSlider(   { 296, h+48, 336, 48 },  nullptr, nullptr, &noise_fromValue,             -2, 2);
        GuiSlider(   { 872, h+48, 336, 48 },  nullptr, nullptr, &noise_toValue,               -2, 2);

        // CELLULAR settings
        
        GuiLine(       { 32,   h+328, 256, 48 }, "Cellular");
        if (GuiSpinner({ 840,  h+328, 176, 48 }, "Index 0,1", &cell_indice0Value, 0, 4, cell_indice0EditMode)) cell_indice0EditMode = !cell_indice0EditMode;
        if (GuiSpinner({ 1032, h+328, 176, 48 }, nullptr,     &cell_indice1Value, 0, 4, cell_indice1EditMode)) cell_indice1EditMode = !cell_indice1EditMode;

        GuiDisable();
        if (GuiValueBox({ 184, h+384, 176, 48 }, "Jitter", &cell_jitter_valueValue, 0, 2, cell_jitter_valueEditMode)) cell_jitter_valueEditMode = !cell_jitter_valueEditMode;
        GuiEnable();

        GuiSliderBar({ 368, h+384, 840, 48 }, nullptr, nullptr, &cell_jitterValue, 0, 2);
        
        // NOISE LOOKUP settings

        GuiLine(        { 32,   h+440, 256, 48 }, "NoiseLookup");
        if (GuiValueBox({ 112,  h+496, 128, 48 }, "Seed",    &look_seedValue,    0, 100, look_seedEditMode)) look_seedEditMode = !look_seedEditMode;
        if (GuiSpinner( { 1048, h+496, 160, 48 }, "Octaves", &look_octavesValue, 0, 100, look_octavesEditMode)) look_octavesEditMode = !look_octavesEditMode;

        GuiDisable();
        if (GuiValueBox({ 184, h+552, 176, 48 }, "Frequency",  &look_frequency_valueValue,  0, 100, look_frequency_valueEditMode)) look_frequency_valueEditMode = !look_frequency_valueEditMode;
        if (GuiValueBox({ 184, h+608, 176, 48 }, "Lacunarity", &look_lacunarity_valueValue, 0, 100, look_lacunarity_valueEditMode)) look_lacunarity_valueEditMode = !look_lacunarity_valueEditMode;
        if (GuiValueBox({ 184, h+664, 176, 48 }, "Gain",       &look_gain_valueValue,       0, 100, look_gain_valueEditMode)) look_gain_valueEditMode = !look_gain_valueEditMode;
        GuiEnable();

        GuiSliderBar({ 368, h+552, 840, 48 }, nullptr, nullptr, &look_frequencyValue,  0, 100);
        GuiSliderBar({ 368, h+608, 840, 48 }, nullptr, nullptr, &look_lacunarityValue, 0, 100);
        GuiSliderBar({ 368, h+664, 840, 48 }, nullptr, nullptr, &look_gainValue,       0, 100);
        
        // Dropdown boxes last
        
        GuiSetStyle(DROPDOWNBOX, DROPDOWN_ROLL_UP, 1);
        if (GuiDropdownBox({ 256, h+496, 240, 48 }, "Value;ValueFractal;Perlin;PerlinFractal;Simplex;SimplexFractal;WhiteNoise;Cubic;CubicFractal", &look_noise_typeActive, look_noise_typeEditMode)) look_noise_typeEditMode = !look_noise_typeEditMode;
        GuiSetStyle(DROPDOWNBOX, DROPDOWN_ROLL_UP, 0);

        if (GuiDropdownBox({ 512, h+496, 192, 48 }, "Linear;Hermite;Quintic", &look_interpActive, look_interpEditMode)) look_interpEditMode = !look_interpEditMode;
        if (GuiDropdownBox({ 720, h+496, 192, 48 }, "FBM;Billow;RigidMulti", &look_fractalActive, look_fractalEditMode)) look_fractalEditMode = !look_fractalEditMode;
        
        if (GuiDropdownBox({ 504, h+328, 192, 48 }, "Euclidean;Manhattan;Natural", &cell_distance_functionActive, cell_distance_functionEditMode)) cell_distance_functionEditMode = !cell_distance_functionEditMode;
        if (GuiDropdownBox({ 296, h+328, 192, 48 }, "CellValue;NoiseLookup;Distance;Distance2;Distance2Add;Distance2Sub;Distance2Mul;Distance2Div", &cell_return_typeActive, cell_return_typeEditMode)) cell_return_typeEditMode = !cell_return_typeEditMode;
        
        if (GuiDropdownBox({ 256, h+104, 240, 48 }, "Value;ValueFractal;Perlin;PerlinFractal;Simplex;SimplexFractal;Cellular;WhiteNoise;Cubic;CubicFractal", &noise_typeActive, noise_typeEditMode)) noise_typeEditMode = !noise_typeEditMode;
        if (GuiDropdownBox({ 512, h+104, 192, 48 }, "Linear;Hermite;Quintic", &noise_interpActive, noise_interpEditMode)) noise_interpEditMode = !noise_interpEditMode;
        if (GuiDropdownBox({ 720, h+104, 192, 48 }, "FBM;Billow;RigidMulti", &noise_fractalActive, noise_fractalEditMode)) noise_fractalEditMode = !noise_fractalEditMode;
        
        GuiUnlock();
    }
};



#endif // GUI_LAYOUT_NAME_IMPLEMENTATION

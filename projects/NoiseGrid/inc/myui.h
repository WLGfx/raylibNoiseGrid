#ifndef MYUI_H
#define MYUI_H

#include "raygui+.h"
#include "NoiseGrid.h"
#include "raygui.h"
#include "raylib.h"

struct MYUI {
    #define RUI_HEIGHT 56

    MYUI(Camera3D* cam, NoiseGrid* noise) : camera(cam), noise_grid(noise) {}

    Camera3D *camera;
    NoiseGrid* noise_grid;
    struct {float width = 0, height = 0;} window_size;

    void update();

    // Menu

    rToggleGroup menu_select = {{0, 0, 120, RUI_HEIGHT}, "Grid;Noise;Camera", 0};

    // Grid and Chunk

    rHBox grid = {{0, 1*RUI_HEIGHT, 1280, RUI_HEIGHT}, {10, 4, 10, 4}, 
                  {&grid_chunk_label, 
                    &grid_chunk_size, 
                    &grid_label, 
                    &grid_width, 
                    &grid_height, 
                    &grid_depth}, 
                  10.0f, ANCHOR_TOP_LEFT, FIT_BOTH};
    rLine grid_chunk_label = { "Chunk" };
    rDropdownBox grid_chunk_size = {"16;24;32;48;64;96", 0, std::bind(&MYUI::on_grid_chunk_size_changed, this) };
    rLine grid_label = { "Grid" };
    rSpinner grid_width = {3, 1, 100, std::bind(&MYUI::on_grid_size_changed, this)};
    rSpinner grid_height = {3, 1, 100, std::bind(&MYUI::on_grid_size_changed, this)};
    rSpinner grid_depth = {3, 1, 100, std::bind(&MYUI::on_grid_size_changed, this)};
    void on_grid_chunk_size_changed();
    void on_grid_size_changed();

    // Noise

    rVBox noise_type = {{0, 1*RUI_HEIGHT, 280, 3*RUI_HEIGHT}, {10, 4, 10, 4}, 
                   {&noise_type_label, &noise_type_dropdown, &noise_interp_dropdown}, 
                   2.0f, ANCHOR_TOP_LEFT, FIT_BOTH};
    rLine noise_type_label = { "Noise" };
    rDropdownBox noise_type_dropdown = {"Value;ValueFractal;Perlin;PerlinFractal;Simplex;SimplexFractal;Cellular;WhiteNoise;Cubic;CubicFractal", 1, std::bind(&MYUI::on_noise_type_changed, this) };
    rDropdownBox noise_interp_dropdown = {"Linear;Hermite;Quintic", 1, std::bind(&MYUI::on_noise_interp_changed, this) };
    void on_noise_type_changed();
    void on_noise_interp_changed();

    // sliders

    rVBox noise_sliders = {{550, 1*RUI_HEIGHT, 730, 7*RUI_HEIGHT}, {10, 4, 10, 4}, 
                           {&seed,
                           &octaves,
                           &frequency,
                           &gain,
                           &lacunarity,
                           &greater_than,
                           &less_than}, 
                           2.0f, ANCHOR_TOP_LEFT, FIT_BOTH};
    rSlider    seed =         { 0, 0, 9999,          std::bind(&MYUI::on_seed_changed, this),         "Seed" };
    rSlider    octaves =      { 1, 1, 10,            std::bind(&MYUI::on_octaves_changed, this),      "Octav" };
    rSliderBar frequency =    { 0.01f, 0.0001f, 0.2f, std::bind(&MYUI::on_frequency_changed, this),    "Frequ" };
    rSliderBar gain =         { 0.5f, 0.0f, 5.0f,    std::bind(&MYUI::on_gain_changed, this),         "Gain" };
    rSliderBar lacunarity =   { 0.1f, 0.0f, 2.0f,    std::bind(&MYUI::on_lacunarity_changed, this),   "Lacun" };
    rSliderBar greater_than = { 0.0f, -2.0f, 2.0f,   std::bind(&MYUI::on_greater_than_changed, this), "Great" };
    rSliderBar less_than =    { 0.1f, -2.0f, 2.0f,   std::bind(&MYUI::on_less_than_changed, this),    "Less" };
    void on_seed_changed();
    void on_octaves_changed();
    void on_frequency_changed();
    void on_gain_changed();
    void on_lacunarity_changed();
    void on_greater_than_changed();
    void on_less_than_changed();

    rVBox noise_values = {{280, 1*RUI_HEIGHT, 180, 7*RUI_HEIGHT}, {10, 4, 10, 4}, 
                          {
                            &seed_box,
                            &octaves_box,
                            &frequency_box,
                            &gain_box,
                            &lacunarity_box,
                            &greater_than_box,
                            &less_than_box
                          }, 
                          2.0f, ANCHOR_TOP_LEFT, FIT_BOTH};
    rValueBox seed_box = {0, 0, 9999};
    rValueBox octaves_box = {1, 1, 10};
    rValueBoxFloat frequency_box = {0.01f};
    rValueBoxFloat gain_box = {0.0f};
    rValueBoxFloat lacunarity_box = {0.0f};
    rValueBoxFloat greater_than_box = {0.0f};
    rValueBoxFloat less_than_box = {0.0f};

    rVBox cellular = {{0, 4*RUI_HEIGHT, 280, 4*RUI_HEIGHT}, {10, 4, 10, 4}, 
                      {
                        &cellular_line,
                        &return_type,
                        &fractal,
                        &distance_function,
                      }, 
                      2.0f, ANCHOR_TOP_LEFT, FIT_BOTH};
    rLine cellular_line = {"Cellular"};
    rDropdownBox fractal = {"Fbm;RigidMulti;Billow", 0, std::bind(&MYUI::on_fractal_changed, this)};
    rDropdownBox distance_function = {"Euclidean;Manhattan;Natural", 0, std::bind(&MYUI::on_distance_function_changed, this)};
    rDropdownBox return_type = {"CellValue;Distance;Distance2;Distance2Add;Distance2Sub;Distance2Mul;Distance2Div", 0, std::bind(&MYUI::on_return_type_changed, this)};
    void on_fractal_changed();
    void on_distance_function_changed();
    void on_return_type_changed();

    rVBox jitter = {{550, 8*RUI_HEIGHT, 730, 1*RUI_HEIGHT}, {10, 4, 10, 4}, 
                           {&jitter_slider}, 
                           2.0f, ANCHOR_TOP_LEFT, FIT_BOTH};
    rSliderBar jitter_slider = {0.2f, 0.0f, 2.0f, std::bind(&MYUI::on_jitter_changed, this), "Jitter"};
    void on_jitter_changed();

    rVBox jitter_box = {{280, 8*RUI_HEIGHT, 180, 1*RUI_HEIGHT}, {10, 4, 10, 4}, 
                          {&jitter_value}, 
                          2.0f, ANCHOR_TOP_LEFT, FIT_BOTH};
    rValueBoxFloat jitter_value = {0.2f};

    rVBox indexes = {{0, 8*RUI_HEIGHT, 280, 3*RUI_HEIGHT}, {10, 4, 10, 4}, 
                      {&index_line, &index0, &index1}, 
                      2.0f, ANCHOR_TOP_LEFT, FIT_BOTH};
    rLine index_line = {"Indexes"};
    rSpinner index0 = {0, 0, 4, std::bind(&MYUI::on_index_changed, this)};
    rSpinner index1 = {1, 0, 4, std::bind(&MYUI::on_index_changed, this)};
    void on_index_changed();
    
/*
    void draw();
    void update_grid();
    void set_noise_from_ui();

    Camera3D *camera;
    NoiseGrid* noise_grid;

    void on_effect_1_pressed();
    
    Rectangle padding = {10, 10, 10, 10};
    
    rToggle m_grid = {{}}, 
            m_noise = {{}}, 
            m_camera = {{}};
    rButton m_effect_1 = {{}, "1", std::bind(&MYUI::on_effect_1_pressed, this) };
    std::vector<rBounds*> widgets = { &m_effect_1, &m_camera, &m_noise, &m_grid };
    rVBox vbox = { {10, 10, 50, 4 * 60}, padding, ANCHOR_BOTTOM_RIGHT, &widgets };

    // Grid and chunk size
    
    void on_chunk_size_changed();
    void on_grid_size_changed();

    rDropdownBox chunk_size = { {0}, "16;24;32;48;64;96", 1, false, std::bind(&MYUI::on_chunk_size_changed, this) };
    rLine chunk_size_line = { {0}, "Chunk" };
    rSpinner grid_wid = { {0}, "w", 3, 1, 100, false, std::bind(&MYUI::on_grid_size_changed, this) };
    rSpinner grid_hgt = { {0}, "h", 3, 1, 100, false, std::bind(&MYUI::on_grid_size_changed, this) };
    rSpinner grid_dep = { {0}, "d", 3, 1, 100, false, std::bind(&MYUI::on_grid_size_changed, this) };
    rLine grid_label = { {0}, "Grid" };
    
    Rectangle grid_padding = {10, 10, 100, 10};
    std::vector<rBounds*> grid_widgets = { &chunk_size, &chunk_size_line, &grid_wid, &grid_hgt, &grid_dep, &grid_label };
    rVBox grid_vbox = { {10, 10, 200, 6 * 60}, grid_padding, ANCHOR_BOTTOM_RIGHT, &grid_widgets };

    // Noise menu

    // range and width

    void on_seed_changed();
    void on_octaves_changed();
    void on_frequency_changed();
    void on_gain_changed();
    void on_lacunarity_changed();
    void on_greater_than_changed();
    void on_less_than_changed();

    rSliderBar seed_slider =      { {0}, "Seed", "", 0, 0, 10, std::bind(&MYUI::on_seed_changed, this) };
    rSliderBar octaves_slider =   { {0}, "Oct's", "", 1, 1, 8, std::bind(&MYUI::on_octaves_changed, this) };
    rSliderBar frequency_slider = { {0}, "Freq", "", 0.02f, 0.000001f, 0.25f, std::bind(&MYUI::on_frequency_changed, this) };
    rSliderBar gain_slider =      { {0}, "Gain", "", 0.5f, 0.0f, 2.0f, std::bind(&MYUI::on_gain_changed, this) };
    rSliderBar lacunarity_slider = { {0}, "Lac", "", 2.0f, 0.000001f, 6.0f, std::bind(&MYUI::on_lacunarity_changed, this) };
    rSliderBar from_slider =      { {0}, "From", "", 0.0f, -2.0f, 2.0f, std::bind(&MYUI::on_from_changed, this) };
    rSliderBar to_slider =        { {0}, "To", "", 0.1f, -2.0f, 2.0f, std::bind(&MYUI::on_to_changed, this) };
    rSliderBar width_slider =     { {0}, "Width", "", 0.1f, 0.0f, 1.0f, std::bind(&MYUI::on_width_changed, this) };

    Rectangle range_padding = {240, 10, 100, 10};
    std::vector<rBounds*> range_widgets = { &seed_slider, &octaves_slider, &frequency_slider, &gain_slider, &lacunarity_slider, &from_slider, &to_slider, &width_slider };
    rVBox range_vbox = { {0, 0, 820, 8 * 60}, range_padding, ANCHOR_BOTTOM_RIGHT, &range_widgets };

    rValueBox seed_value =           { {0, 0, 100, 40}, "", 0, 0, 99999999 };
    rValueBox octaves_value =        { {0, 0, 100, 40}, "", 1 , 1, 8};
    rValueBoxFloat frequency_value = { {0, 0, 100, 40}, "", 0.02f };
    rValueBoxFloat gain_value =      { {0, 0, 100, 40}, "", 0.5f };
    rValueBoxFloat lacunarity_value = { {0, 0, 100, 40}, "", 2.0f };
    rValueBoxFloat from_value =      { {0, 0, 100, 40}, "", 0.0f };
    rValueBoxFloat to_value =        { {0, 0, 100, 40}, "", 0.1f };
    rValueBoxFloat width_value =     { {0, 0, 100, 40}, "", 0.1f };

    Rectangle range_value_padding = {10, 10, 100, 10};
    std::vector<rBounds*> range_value_widgets = { &seed_value, &octaves_value, &frequency_value, &gain_value, &lacunarity_value, &from_value, &to_value, &width_value };
    rVBox range_value_vbox = { {0, 0, 200, 8 * 60}, range_value_padding, ANCHOR_BOTTOM_RIGHT, &range_value_widgets };

    // Dropdowns

    void on_noise_type_changed();
    void on_interp_changed();
    
    rDropdownBox noise_type = { {0}, "Value;ValueFractal;Perlin;PerlinFractal;Simplex;SimplexFractal;Cellular;WhiteNoise;Cubic;CubicFractal", 0,false,  std::bind(&MYUI::on_noise_type_changed, this) };
    rDropdownBox interp = { {0}, "Linear;Hermite;Quintic", 0,false, std::bind(&MYUI::on_interp_changed, this) };
    
    Rectangle dropdowns_noise_padding = {10, 10, 90, 490};
    std::vector<rBounds*> dropdowns_noise_widgets = { &noise_type, &interp };
    rHBox dropdowns_noise_hbox = { {0, 0, 210 * 2, 60}, dropdowns_noise_padding, ANCHOR_BOTTOM_RIGHT, &dropdowns_noise_widgets };
    
    void on_fractal_type_changed();
    void on_distance_changed();
    void on_return_type_changed();
    void on_indexes_changed();
    
    rDropdownBox fractal_type = { {0}, "FBM;RigidMulti;Billow", 0,false, std::bind(&MYUI::on_fractal_type_changed, this) };
    rDropdownBox return_type = { {0}, "Value;Distance;Distance2;Distance2Add;Distance2Sub;Distance2Mul;Distance2Div", 1,false, std::bind(&MYUI::on_return_type_changed, this) };
    rDropdownBox distance = { {0}, "Euclidean;Manhattan;Natural", 0,false, std::bind(&MYUI::on_distance_changed, this) };
    rSpinner index0 = { {0}, "", 0, 0, 4,false, std::bind(&MYUI::on_indexes_changed, this) };
    rSpinner index1 = { {0}, "", 1, 0, 4,false, std::bind(&MYUI::on_indexes_changed, this) };
    
    Rectangle fractal_params_padding = {10, 10, 90, 550};
    std::vector<rBounds*> fractal_params_widgets = { &fractal_type, &distance, &return_type, &index0, &index1 };
    rHBox fractal_params_hbox = { {0, 0, 1060, 60}, fractal_params_padding, ANCHOR_BOTTOM_RIGHT, &fractal_params_widgets };

    void on_jitter_value_changed();

    rValueBoxFloat jitter = { {0}, "", 2.0f };
    
    std::vector<rBounds*> jitter_widget = { &jitter };
    Rectangle jitter_padding = {10, 10, 100, 610};
    rVBox jitter_vbox = { {0, 0, 200, 60}, jitter_padding, ANCHOR_BOTTOM_RIGHT, &jitter_widget };
    
    void on_jitter_slider_changed();

    rSliderBar jitter_slider = { {0}, "jitter", "", 2.0f, 0.0f, 3.0f, std::bind(&MYUI::on_jitter_slider_changed, this) };

    std::vector<rBounds*> jitter_slider_widget = { &jitter_slider };
    Rectangle jitter_slider_padding = {10, 10, 330, 610};
    rVBox jitter_slider_vbox = { {0, 0, 820, 1 * 60}, jitter_slider_padding, ANCHOR_BOTTOM_RIGHT, &jitter_slider_widget };

    // camera_vbox plus camera widgets
    void on_orbital_speed_changed();
    rSliderBar orbital_speed = { {}, "Orbital Speed", "", 5.0f, -10.0f, 10.0f, std::bind(&MYUI::on_orbital_speed_changed, this) };
    Rectangle camera_padding = {10, 10, 100, 10};
    rVBox camera_vbox = { {0, 0, 200, 60}, camera_padding, ANCHOR_BOTTOM_RIGHT, &camera_widgets };
    std::vector<rBounds*> camera_widgets = { &orbital_speed };
*/
    const FastNoise::NoiseType noise_types[10] = { 
        FastNoise::NoiseType::Value, 
        FastNoise::NoiseType::ValueFractal, 
        FastNoise::NoiseType::Perlin,
        FastNoise::NoiseType::PerlinFractal,
        FastNoise::NoiseType::Simplex,
        FastNoise::NoiseType::SimplexFractal,
        FastNoise::NoiseType::Cellular,
        FastNoise::NoiseType::WhiteNoise,
        FastNoise::NoiseType::Cubic,
        FastNoise::NoiseType::CubicFractal
    };
    const FastNoise::Interp interp_types[3] = { 
        FastNoise::Interp::Linear,
        FastNoise::Interp::Hermite,
        FastNoise::Interp::Quintic
    };
    const FastNoise::FractalType fractal_types[3] = { 
        FastNoise::FractalType::FBM,
        FastNoise::FractalType::RigidMulti,
        FastNoise::FractalType::Billow
    };
    const FastNoise::CellularDistanceFunction distance_functions[3] = { 
        FastNoise::CellularDistanceFunction::Euclidean,
        FastNoise::CellularDistanceFunction::Manhattan,
        FastNoise::CellularDistanceFunction::Natural
    };
    const FastNoise::CellularReturnType return_types[7] = { 
        FastNoise::CellularReturnType::CellValue,
        FastNoise::CellularReturnType::Distance,
        FastNoise::CellularReturnType::Distance2,
        FastNoise::CellularReturnType::Distance2Add,
        FastNoise::CellularReturnType::Distance2Sub,
        FastNoise::CellularReturnType::Distance2Mul,
        FastNoise::CellularReturnType::Distance2Div
    };

    const int chunk_sizes[6] = {16, 24, 32, 48, 64, 96};
};

#endif
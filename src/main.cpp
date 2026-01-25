#include "inc/FastNoise.h"
#include "raylib.h"

#include "inc/gui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#define RLIGHTS_IMPLEMENTATION
#include "inc/rlights.h"
#undef RLIGHTS_IMPLEMENTATION
// had to undef rlights as it is include again in noisegrid
// it's implementation is done here...

#include "inc/NoiseGrid.h"

// New title for this - FastNoise x3 Playground

NoiseGrid grid;

struct 
{
    // ################ GRID MENU ################

    rToggle grid_menu = {
        Rectangle{ 10, 10, 60, 60 },
        "",
        false
    };

    rLabel grid_size_label = {
        Rectangle{ 10, 80, 200, 50 },
        "Grid Size"
    };

    rDropdownBox grid_size_defaults = {
        {270, 80, 120, 50},
        "3x;5x;7x;9x;11x;13x;15x;17x;19x",
        0,
        false
    };

    struct {
        rSpinner w = { {10, 130, 120, 50},
            "", 9, 3, 128, false};
        rSpinner h = { {140, 130, 120, 50},
            "", 3, 3, 128, false};
        rSpinner d = { {270, 130, 120, 50},
            "", 9, 3, 128, false};
    } grid_size;

    rLabel chunk_size_label = {
        Rectangle{ 10, 190, 200, 50 },
        "Chunk Size"
    };

    rDropdownBox chunk_size_defaults = {
        {270, 190, 120, 50},
        "8;16;32;64;128",
        0,
        false
    };

    struct {
        rSpinner w = { {10, 240, 120, 50},
            "", 16, 3, 512, false};
        rSpinner h = { {140, 240, 120, 50},
            "", 16, 3, 512, false};
        rSpinner d = { {270, 240, 120, 50},
            "", 16, 3, 512, false};
    } chunk_size;

    rLabel noise_label = {
        Rectangle{ 10, 290, 200, 50 },
        "Noise"
    };

    struct {
        rSlider start = { 
            {10, 340, 280, 50},
            "", 
            "", 
            0.1f, 
            -2.0f, 
            2.0f
        };
        rSlider end = { 
            {10, 400, 280, 50},
            "", 
            "", 
            0.15f, 
            -2.0f, 
            2.0f
        };
        rValueBoxFloat start_val = {
            {300, 340, 100, 50},
            "", 
            0.1f, 
            false
        };
        rValueBoxFloat end_val = {
            {300, 400, 100, 50},
            "",
            0.15f,
            false
        };

        rSlider max_width = { 
            {10, 500, 280, 50},
            "", 
            "", 
            0.1f, 
            0.0f, 
            0.5f
        };
        rValueBoxFloat max_width_val = {
            {300, 500, 100, 50},
            "",
            0.1f,
            false
        };
        rLabel max_width_label = {
            Rectangle{ 10, 450, 280, 50 },
            "Max Width"
        };
    } noise_range;

    // ################ NOISE MENU ################

    rToggle noise_menu = {
        Rectangle{ 80, 10, 60, 60 },
        "",
        false
    };
    
    // Noise Type

    rDropdownBox noise_types_box = {
        {10, 80, 300, 50},
        "Value;ValueFractal;Perlin;PerlinFractal;Simplex;SimplexFractal;Cellular;WhiteNoise;Cubic;CubicFractal",
        1,
        false
    };

    // Seed

    rValueBox seed = { 
        {110, 140, 170, 50},
        "Seed", 
        1337, 
        0,
        65535,
        false
    };

    // Frequency

    rValueBoxFloat frequency = { 
        {110, 200, 170, 50},
        "Freq", 
        0.03f, 
        false
    };

    // Interpolation

    rDropdownBox interpolation_box = {
        {10, 260, 300, 50},
        "Linear;Hermite;Quintic",
        1,
        false
    };

    // Octaves

    rValueBox octaves_box = {
        {110, 320, 170, 50},
        "Octa", 
        3,
        1,
        10,
        false
    };

    // Lacunarity

    rValueBoxFloat lacunarity_box = {
        {110, 380, 170, 50},
        "Lacu", 
        2.0f, 
        false
    };

    // Gain

    rValueBoxFloat gain_box = {
        {110, 440, 170, 50},
        "Gain", 
        0.5f, 
        false
    };
    
    // ################ CELLULAR MENU ################

    rToggle cellular_menu = {
        Rectangle{ 150, 10, 60, 60 },
        "",
        false
    };

    rDropdownBox cfractal = {
        {10, 80, 300, 50},
        "FBm;Ridge;PingPong",
        0,
        false
    };

    rDropdownBox cdistance = {
        {10, 140, 300, 50},
        "Euclidean;Manhattan;Hybrid",
        0,
        false
    };

    rDropdownBox cReturnType = {
        {10, 200, 300, 50},
        "Value;Distance;Distance2;Distance2Add;Distance2Sub;Distance2Mul;Distance2Div",
        1,
        false
    };

    // jitter

    rValueBoxFloat jitter = {
        {110, 260, 170, 50},
        "Jitt", 
        1.0f, 
        false
    };

    rValueBoxFloat jitter_from = {
        {110, 320, 170, 50},
        "From", 
        0.0f, 
        false
    };

    rValueBoxFloat jitter_to = {
        {110, 380, 170, 50},
        "To", 
        1.0f, 
        false
    };

    // ################ CAMERA CONTROLS MENU ################

    rToggle camera_menu = {
        Rectangle{ 220, 10, 60, 60 },
        "",
        false
    };

    rDropdownBox camera_type= {
        {10, 80, 300, 50},
        "Manual;Free;Oribital",
        2,
        false
    };

} ui;

//std::cout << "Result=" << result 
//<< "  editMode=" << ui.noise_types_box.editMode 
//<< "  active=" << ui.noise_types_box.active << "\n";

// NB: before reversing the 'editMode' flag, if true then
//     a selected value is in 'active'.

// NB: also, specifically for checking after the UI is drawn
// checking the editMode for false means the value/selection
// has changed.

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

    write_float_to_char_buffer(ui.noise_range.start_val.textValue, ui.noise_range.start.value);
    write_float_to_char_buffer(ui.noise_range.end_val.textValue, ui.noise_range.end.value);
    write_float_to_char_buffer(ui.noise_range.max_width_val.textValue, ui.noise_range.max_width.value);
    write_float_to_char_buffer(ui.frequency.textValue, ui.frequency.value);
    write_float_to_char_buffer(ui.lacunarity_box.textValue, ui.lacunarity_box.value);
    write_float_to_char_buffer(ui.gain_box.textValue, ui.gain_box.value);
    write_float_to_char_buffer(ui.jitter.textValue, ui.jitter.value);
    write_float_to_char_buffer(ui.jitter_from.textValue, ui.jitter_from.value);
    write_float_to_char_buffer(ui.jitter_to.textValue, ui.jitter_to.value);
}

void draw_ui()
{
    // ############################ Grid Menu ############################

    ui.grid_menu.draw();
    GuiDrawIcon(ICON_RESIZE, ui.grid_menu.bounds.x + 6, ui.grid_menu.bounds.y + 6, 3, RAYWHITE);
    
    if (ui.grid_menu.active)
    {
        ui.noise_menu.active = false;
        ui.cellular_menu.active = false;
        ui.camera_menu.active = false;
        
        ui.grid_size_label.draw();
        ui.grid_size.w.draw();
        ui.grid_size.h.draw();
        ui.grid_size.d.draw();

        ui.chunk_size_label.draw();
        ui.chunk_size.w.draw();
        ui.chunk_size.h.draw();
        ui.chunk_size.d.draw();

        ui.noise_label.draw(); // Changing to slider ranges from -2.0 to +2.0 and adding max width
        if (ui.noise_range.start.draw()) {
            if (ui.noise_range.start.value > ui.noise_range.end.value) {
                ui.noise_range.end.value = ui.noise_range.start.value;
            }
            float width = ui.noise_range.end.value - ui.noise_range.start.value;
            if (width > ui.noise_range.max_width.value) {
                ui.noise_range.end.value = ui.noise_range.start.value + ui.noise_range.max_width.value;
            }
            ui.noise_range.start_val.value = ui.noise_range.start.value;
            ui.noise_range.end_val.value = ui.noise_range.end.value;
            init_ui();
            grid.update_noise_range();
        }
        ui.noise_range.start_val.draw();
        ui.noise_range.end_val.draw();
        if (ui.noise_range.end.draw()){
            float width = ui.noise_range.end.value - ui.noise_range.start.value;
            if (width > ui.noise_range.max_width.value) {
                ui.noise_range.start.value = ui.noise_range.end.value - ui.noise_range.max_width.value;
            }
            if (ui.noise_range.end.value < ui.noise_range.start.value) {
                ui.noise_range.start.value = ui.noise_range.end.value;
            }
            ui.noise_range.start_val.value = ui.noise_range.start.value;
            ui.noise_range.end_val.value = ui.noise_range.end.value;
            init_ui();
            grid.update_noise_range();
        }
        ui.noise_range.max_width_val.draw();
        if (ui.noise_range.max_width.draw()) {
                
                float width = ui.noise_range.end.value - ui.noise_range.start.value;
                if (width > ui.noise_range.max_width.value) {
                    ui.noise_range.end.value = ui.noise_range.start.value + ui.noise_range.max_width.value;
                }
                ui.noise_range.max_width_val.value = ui.noise_range.max_width.value;
                init_ui();
                grid.update_noise_range();
        }
        ui.noise_range.max_width_label.draw();
        if (ui.chunk_size_defaults.draw()) 
        {
            const int sizes[] = {8, 16, 32, 64, 128, 256, 512, 1024};
            if (!ui.chunk_size_defaults.editMode)
            {
                int v = sizes[ui.chunk_size_defaults.active];
                
                ui.chunk_size.w.value = v;
                ui.chunk_size.h.value = v;
                ui.chunk_size.d.value = v;
                
            }
        }
        if (ui.grid_size_defaults.draw()) 
        {
            const int sizes[] = {3, 5, 7, 9, 11, 13, 15, 17, 19};
            if (!ui.grid_size_defaults.editMode)
            {
                int v = sizes[ui.grid_size_defaults.active];
                
                ui.grid_size.w.value = v;
                ui.grid_size.h.value = v;
                ui.grid_size.d.value = v;
                
            }
        }
    }

    // ############################ Noise Menu ############################

    ui.noise_menu.draw();
    GuiDrawIcon(ICON_WAVE_SINUS, ui.noise_menu.bounds.x + 6, ui.noise_menu.bounds.y + 6, 3, RAYWHITE);

    if (ui.noise_menu.active)
    {
        ui.grid_menu.active = false;
        ui.cellular_menu.active = false;
        ui.camera_menu.active = false;

        // fractal options
        if (ui.seed.draw() ||
            ui.frequency.draw() ||
            ui.octaves_box.draw() ||
            ui.lacunarity_box.draw() ||
            ui.gain_box.draw()) {
                grid.update_new_noise();
            }

        if (ui.interpolation_box.draw() ||
            ui.noise_types_box.draw()) {
                grid.update_new_noise();
            }
    }

    // ############################ Cellular Menu ############################

    if (ui.noise_types_box.active && ui.noise_types_box.active == 6) // CELLULAR
    {
        ui.cellular_menu.draw();
        GuiDrawIcon(ICON_DITHERING, ui.cellular_menu.bounds.x + 6, ui.cellular_menu.bounds.y + 6, 3, RAYWHITE);    

        if (ui.cellular_menu.active)
        {
            ui.grid_menu.active = false;
            ui.noise_menu.active = false;
            ui.camera_menu.active = false;

            // value boxes drawn first
            if (ui.jitter.draw() ||
                ui.jitter_from.draw() ||
                ui.jitter_to.draw()) {
                    grid.update_new_noise();
                }

            if (ui.cReturnType.draw() ||
                ui.cdistance.draw() ||
                ui.cfractal.draw()) {
                    grid.update_new_noise();
                }
        }
    }

    ui.camera_menu.draw();
    GuiDrawIcon(ICON_CAMERA, ui.camera_menu.bounds.x + 6, ui.camera_menu.bounds.y + 6, 3, RAYWHITE);

    if (ui.camera_menu.active)
    {
        ui.grid_menu.active = false;
        ui.noise_menu.active = false;
        ui.cellular_menu.active = false;

        ui.camera_type.draw();

    }
}



// ####### VARIABLES #######

Camera camera = {
    .position = {0.0f, 40.0f, -80.0f},
    .target = {0.0f, 0.0f, 0.0f},
    .up = {0.0f, 1.0f, 0.0f},
    .fovy = 45.0f,
    .projection = CAMERA_PERSPECTIVE
};


FastNoise::NoiseType fnNoiseTypes[] = {
    FastNoise::NoiseType::Value,
    FastNoise::NoiseType::ValueFractal,
    FastNoise::NoiseType::Perlin,
    FastNoise::NoiseType::PerlinFractal,
    FastNoise::NoiseType::Simplex,
    FastNoise::NoiseType::SimplexFractal,
    FastNoise::NoiseType::Cellular,
    FastNoise::NoiseType::WhiteNoise,
    FastNoise::NoiseType::Cubic,
    FastNoise::NoiseType::CubicFractal,
};

FastNoise::Interp fnInterp[] = {
    FastNoise::Interp::Linear,
    FastNoise::Interp::Hermite,
    FastNoise::Interp::Quintic,
};

FastNoise::FractalType fnFractalType[] = {
    FastNoise::FractalType::FBM,
    FastNoise::FractalType::Billow,
    FastNoise::FractalType::RigidMulti
};

FastNoise::CellularDistanceFunction fnCellularDistanceFunction[] = {
    FastNoise::CellularDistanceFunction::Euclidean,
    FastNoise::CellularDistanceFunction::Manhattan,
    FastNoise::CellularDistanceFunction::Natural
};

FastNoise::CellularReturnType fnCellularReturnType[] = {
    FastNoise::CellularReturnType::CellValue,
    FastNoise::CellularReturnType::Distance,
    FastNoise::CellularReturnType::Distance2,
    FastNoise::CellularReturnType::Distance2Add,
    FastNoise::CellularReturnType::Distance2Sub,
    FastNoise::CellularReturnType::Distance2Mul,
    FastNoise::CellularReturnType::Distance2Div,
};

CameraMode fnCameraMode[] = {
    CAMERA_CUSTOM,
    CAMERA_FREE,
    CAMERA_ORBITAL,
};


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
                {50, 50, 0},
                {0, 0, 0},
                DARKGRAY,
                grid.material.shader);

    grid.light_point = CreateLight(LIGHT_POINT,
                {-50, -50, 0},
                {0, 0, 0},
                {44, 54, 48,255},
                grid.material.shader);

    // this is the line missing from the online examples and docs
    grid.material.shader.locs[SHADER_LOC_MATRIX_MODEL] = 
        GetShaderLocationAttrib(
            grid.material.shader, 
            "instanceTransform");

}

void set_noise_grid_from_ui() {
    grid.noise_range = {ui.noise_range.start.value, ui.noise_range.end.value};
    
    grid.noise.SetNoiseType(fnNoiseTypes[ui.noise_types_box.active]); // indexed array
    grid.noise.SetSeed(ui.seed.value);
    grid.noise.SetFrequency(ui.frequency.value);
    
    grid.noise.SetInterp(fnInterp[ui.interpolation_box.active]); // indexed array
    grid.noise.SetFractalOctaves(ui.octaves_box.value);
    grid.noise.SetFractalLacunarity(ui.lacunarity_box.value);
    grid.noise.SetFractalGain(ui.gain_box.value);

    grid.noise.SetFractalType(fnFractalType[ui.cfractal.active]); // indexed array
    grid.noise.SetCellularDistanceFunction(fnCellularDistanceFunction[ui.cdistance.active]); // indexed array
    grid.noise.SetCellularReturnType(fnCellularReturnType[ui.cReturnType.active]); // indexed array
    grid.noise.SetCellularJitter(ui.jitter.value);
    grid.noise.SetCellularDistance2Indices(ui.jitter_from.value, ui.jitter_to.value);

    // Uh?
    // grid.noise.SetCellularDistance2Indices(int cellularDistanceIndex0, int cellularDistanceIndex1)

    grid.grid_size = {
        ui.grid_size.w.value,
        ui.grid_size.h.value,
        ui.grid_size.d.value
    };

    grid.chunk_size = {
        ui.chunk_size.w.value,
        ui.chunk_size.h.value,
        ui.chunk_size.d.value
    };
}

void draw_3d()
{
    switch (ui.camera_type.active) {
        case 0:
            break;
        case 1:
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) ||
                IsKeyDown(KEY_LEFT_SHIFT)) {
                UpdateCamera(&camera, CAMERA_FREE);
            }
            break;
        case 2:
            UpdateCamera(&camera, CAMERA_ORBITAL);
            break;
    }

    grid.light_point.position = camera.position;
    UpdateLightValues(grid.material.shader, grid.light_point);

    //if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    //    UpdateCamera(&camera, CAMERA_FIRST_PERSON);
    //}
    //UpdateCamera(&camera, CAMERA_ORBITAL);

    BeginMode3D(camera);
    DrawGrid(100, 1.0f);

    // No optimisation here, just a test
    set_noise_grid_from_ui();

    Vector3 pos = {0, 0, 0};
    grid.update(pos);
    grid.render();

    EndMode3D();
}

// ######################### MAIN #########################

int main() {
    // Initialization
    int screenWidth = 1280;
    int screenHeight = 720;
    
    InitWindow(screenWidth, screenHeight, "FastNoise x3 Playground (C)WLGfx Carl S Norwood 2025");
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
            draw_ui();
            
            DrawFPS(10, screenHeight - 30);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


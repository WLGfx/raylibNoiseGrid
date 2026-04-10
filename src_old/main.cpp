#include "inc/FastNoise.h"
#include "raylib.h"

#include "inc/raygui+.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#define RLIGHTS_IMPLEMENTATION
#include "inc/rlights.h"
#undef RLIGHTS_IMPLEMENTATION
// had to undef rlights as it is include again in noisegrid
// it's implementation is built here...

#include "rcamera.h"

#include "inc/NoiseGrid.h"

// New title for this - FastNoise x3 Playground

#include <string>
#include <iostream>

NoiseGrid grid;

Camera camera = {
    .position = {0.0f, 140.0f, -280.0f},
    .target = {0.0f, 0.0f, 0.0f},
    .up = {0.0f, 1.0f, 0.0f},
    .fovy = 45.0f,
    .projection = CAMERA_PERSPECTIVE
};

void camera_free_flight_controls();

// callbaks for camera control

// camera ui
struct {
    Rectangle screen;
    Rectangle padding = { 10, 10, 10, 50};

    rToggle b_open = { {0, 0, 50, 40}, "", false };
    rButton b_close = { {0, 0, 50, 40}, "<" };
    rLine line = { {0, 0, 620, 40}, "Camera controls" };

    rDropdownBox dropdown = { {0, 0, 280, 40}, 
        "Orbital Auto;Orbital Free;Free Flight;FPS Gravity;FPS Float", 
        0, false };

    rValueBoxFloat orbital_speed = { {0, 0, 100, 45}, 
        "Orbital speed", 1.0f, false, nullptr };
    rValueBoxFloat move_speed = { {0, 0, 100, 45}, 
        "Move speed", 25.0f, false };
    rValueBoxFloat mouse_sensetivity = { {0, 0, 100, 45}, 
        "Mouse strength", 0.003f, false };
    rValueBoxFloat rotation_speed = { {0, 0, 100, 45}, 
        "Rotation speed", 1.0f, false };
} uicam;

struct {
    Rectangle padding = { 250, 10, 0, 80};
    
    std::vector<rBounds*> widgets = {
        &uicam.dropdown, 
        &uicam.orbital_speed, 
        &uicam.move_speed, 
        &uicam.mouse_sensetivity, 
        &uicam.rotation_speed
    };
    
    rVBox vbox = {
        {0, 0, 200, 5*50},
        padding,
        ANCHOR_BOTTOM_LEFT,
        &widgets,
        nullptr
    };
} uicam_vbox;

void draw_camera_ui_widgets()
{
    uicam_vbox.vbox.update();

    uicam.orbital_speed.draw();
    uicam.move_speed.draw();
    uicam.mouse_sensetivity.draw();
    uicam.rotation_speed.draw();
    
    uicam.dropdown.draw();
}

void draw_camera_ui()
{
    uicam.screen = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
    
    // anchor to bottom of the screen
    uicam.b_open.bounds = { uicam.screen.x + uicam.padding.x, uicam.screen.height - uicam.padding.height - uicam.b_open.bounds.height, uicam.b_open.bounds.width, uicam.b_open.bounds.height };
    uicam.b_close.bounds = { uicam.screen.x + uicam.padding.x + 640, uicam.screen.height - uicam.padding.height - uicam.b_close.bounds.height, uicam.b_close.bounds.width, uicam.b_close.bounds.height };
    uicam.line.bounds = { uicam.screen.x + uicam.padding.x, uicam.screen.height - uicam.padding.height - uicam.line.bounds.height, uicam.line.bounds.width, uicam.line.bounds.height };
    uicam.dropdown.bounds = { uicam.screen.x + uicam.padding.x, uicam.screen.height - uicam.padding.height - uicam.dropdown.bounds.height - (50 * 4), uicam.dropdown.bounds.width, uicam.dropdown.bounds.height };

    if (uicam.b_open.active) 
    {
        uicam.line.draw();
        if (uicam.b_close.draw()) 
        {
            uicam.b_open.active = false;
            return;
        }
        draw_camera_ui_widgets();
    } else 
    {
        uicam.b_open.draw();
        GuiDrawIcon(ICON_CAMERA, uicam.b_open.bounds.x + 10, uicam.b_open.bounds.y + 4, 2, RAYWHITE);
    }

    float frame_time = GetFrameTime();
    
    switch (uicam.dropdown.active)
    {
        case 0: { // Orbital Auto
            //UpdateCamera(&camera, CAMERA_ORBITAL);
            Matrix rotation = MatrixRotate(GetCameraUp(&camera), uicam.orbital_speed.value * frame_time);
            Vector3 view = Vector3Subtract(camera.position, camera.target);
            view = Vector3Transform(view, rotation);
            camera.position = Vector3Add(camera.target, view);
        } break;
        
        case 1: { // Orbital Manual
            // use mouse controls to orbit camera around target
            // up and down mouse movement to rotate around target (pitch)
            // left and right mouse movement to rotate camera (yaw)
            // mouse wheel to zoom in and out (roll)
            // space to reset camera to default position
            
            Vector2 mdelta = { GetMouseDelta().x, GetMouseDelta().y };
            float mwheel = GetMouseWheelMove();

            // orbit camera around target
        } break;
        
        case 2: { // Free Flight
            camera_free_flight_controls();
        } break;
        
        case 3: { // FPS Gravity
        } break;
        
        case 4: { // FPS Float
        } break;
    }
}


// forward function declarations
void on_grid_size_change();
void on_chunk_size_change();

struct {
    rToggle menu_open = { {10, 10, 50, 40}, ">", false};

    rLine menu_line = { {10, 10, 620, 40}, "Dimensions" };
    rButton menu_close = { {640, 10, 50, 40}, "<" };
    
    rSpinner w = { {50, 60, 120, 40}, "w", 3, 0, 100, false, on_grid_size_change};
    rSpinner h = { {220, 60, 120, 40}, "h", 3, 0, 100, false, on_grid_size_change};
    rSpinner d = { {390, 60, 120, 40}, "d", 3, 0, 100, false, on_grid_size_change};

    rLabel chunk_size_label = { {520, 60, 120, 40}, "c" };
    rDropdownBox chunk_size = { {560, 60, 120, 40}, "8;16;32;64;128", 0, false, on_chunk_size_change}; // TODO: move to end of drawing order
    
} ui_grid;

void camera_free_flight_controls()
{
    float delta = GetFrameTime();
    
    // Control speeds
    float moveSpeed = uicam.move_speed.value;
    float rotationSpeed = uicam.rotation_speed.value;
    
    // Calculate forward and right vectors from camera orientation
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right = Vector3CrossProduct(forward, camera.up);
    Vector3 up = camera.up;
    
    // Movement controls (thrust)
    if (IsKeyDown(KEY_UP)) {
        camera.position = Vector3Add(camera.position, Vector3Scale(forward, moveSpeed * delta));
        camera.target = Vector3Add(camera.target, Vector3Scale(forward, moveSpeed * delta));
    }
    if (IsKeyDown(KEY_DOWN)) {
        camera.position = Vector3Subtract(camera.position, Vector3Scale(forward, moveSpeed * delta));
        camera.target = Vector3Subtract(camera.target, Vector3Scale(forward, moveSpeed * delta));
    }
    
    // Strafe movement
    if (IsKeyDown(KEY_A)) {
        camera.position = Vector3Subtract(camera.position, Vector3Scale(right, moveSpeed * delta));
        camera.target = Vector3Subtract(camera.target, Vector3Scale(right, moveSpeed * delta));
    }
    if (IsKeyDown(KEY_D)) {
        camera.position = Vector3Add(camera.position, Vector3Scale(right, moveSpeed * delta));
        camera.target = Vector3Add(camera.target, Vector3Scale(right, moveSpeed * delta));
    }
    
    // Vertical movement
    if (IsKeyDown(KEY_W)) {
        camera.position = Vector3Add(camera.position, Vector3Scale(up, moveSpeed * delta));
        camera.target = Vector3Add(camera.target, Vector3Scale(up, moveSpeed * delta));
    }
    if (IsKeyDown(KEY_S)) {
        camera.position = Vector3Subtract(camera.position, Vector3Scale(up, moveSpeed * delta));
        camera.target = Vector3Subtract(camera.target, Vector3Scale(up, moveSpeed * delta));
    }
    
    // Rotation controls
    // Yaw (left/right turn) - using arrow keys
    if (IsKeyDown(KEY_LEFT)) {
        // Rotate around up vector (yaw)
        Matrix yawRotation = MatrixRotate(up, rotationSpeed * delta);
        Vector3 direction = Vector3Subtract(camera.target, camera.position);
        direction = Vector3Transform(direction, yawRotation);
        camera.target = Vector3Add(camera.position, direction);
    }
    if (IsKeyDown(KEY_RIGHT)) {
        // Rotate around up vector (yaw)
        Matrix yawRotation = MatrixRotate(up, -rotationSpeed * delta);
        Vector3 direction = Vector3Subtract(camera.target, camera.position);
        direction = Vector3Transform(direction, yawRotation);
        camera.target = Vector3Add(camera.position, direction);
    }
    
    // Pitch (up/down look) - using PAGE UP/DOWN keys
    if (IsKeyDown(KEY_F)) {
        // Rotate around right vector (pitch)
        Matrix pitchRotation = MatrixRotate(right, rotationSpeed * delta);
        Vector3 direction = Vector3Subtract(camera.target, camera.position);
        direction = Vector3Transform(direction, pitchRotation);
        camera.target = Vector3Add(camera.position, direction);
        camera.up = Vector3Transform(camera.up, pitchRotation);
        camera.up = Vector3Normalize(camera.up);
    }
    if (IsKeyDown(KEY_R)) {
        // Rotate around right vector (pitch)
        Matrix pitchRotation = MatrixRotate(right, -rotationSpeed * delta);
        Vector3 direction = Vector3Subtract(camera.target, camera.position);
        direction = Vector3Transform(direction, pitchRotation);
        camera.target = Vector3Add(camera.position, direction);
        camera.up = Vector3Transform(camera.up, pitchRotation);
        camera.up = Vector3Normalize(camera.up);
    }
    
    // Roll (banking) - using Q/E keys
    if (IsKeyDown(KEY_Q)) {
        // Rotate around forward vector (roll)
        Matrix rollRotation = MatrixRotate(forward, rotationSpeed * delta);
        camera.up = Vector3Transform(camera.up, rollRotation);
        camera.up = Vector3Normalize(camera.up);
    }
    if (IsKeyDown(KEY_E)) {
        // Rotate around forward vector (roll)
        Matrix rollRotation = MatrixRotate(forward, -rotationSpeed * delta);
        camera.up = Vector3Transform(camera.up, rollRotation);
        camera.up = Vector3Normalize(camera.up);
    }
    
    // Mouse look (optional - hold right mouse button)
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 mouseDelta = GetMouseDelta();
        float mouseSensitivity = uicam.mouse_sensetivity.value;
        
        // Yaw from mouse X movement
        Matrix yawRotation = MatrixRotate(up, -mouseDelta.x * mouseSensitivity);
        Vector3 direction = Vector3Subtract(camera.target, camera.position);
        direction = Vector3Transform(direction, yawRotation);
        camera.target = Vector3Add(camera.position, direction);
        
        // Pitch from mouse Y movement
        Matrix pitchRotation = MatrixRotate(right, -mouseDelta.y * mouseSensitivity);
        direction = Vector3Subtract(camera.target, camera.position);
        direction = Vector3Transform(direction, pitchRotation);
        camera.target = Vector3Add(camera.position, direction);
        camera.up = Vector3Transform(camera.up, pitchRotation);
        camera.up = Vector3Normalize(camera.up);
    }
}

// forward function declarations
void on_index_delete_button_click();

void on_noise_boundary_change_from_box();
void on_noise_boundary_change_to_box();
void on_noise_boundary_change_width_box();
void on_noise_boundary_change_from_slider();
void on_noise_boundary_change_to_slider();
void on_noise_boundary_change_width_slider();

void on_type_change();
void on_interp_change();
void on_seed_change();
void on_octaves_change();

void on_gain_change_slider();
void on_gain_change_box();
void on_frequency_change_slider();
void on_frequency_change_box();
void on_lacunarity_change_slider();
void on_lacunarity_change_box();

void on_fractal_type_change();
void on_return_type_change();
void on_distance_change();
void on_jitter_change_slider();
void on_jitter_change_box();
void on_index0_change();
void on_index1_change();

struct {
    rToggle menu_open = { {10, 120, 50, 40}, ">", false};

    rToggle grid     = { { 10, 120, 120, 40}, "Grid", true};
    rToggle noise    = { {120, 120, 120, 40}, "Noise", false};
    rToggle cellular = { {240, 120, 120, 40}, "Cellular", false};

    rButton menu_close = { {640, 120, 50, 40}, "<" };

    //
    std::string index_options   = "Default";
    rDropdownBox index          = { { 10, 180, 240, 40}, index_options.c_str(), 0, false}; // TODO: fix order
    rButton index_delete_button = { {260, 180, 50, 40}, "-", on_index_delete_button_click };
    rDropdownBox op             = { {380, 180, 240, 40}, "Add;Subtract;Multiply;Divide", 0, false}; // TODO: fix order
    rButton add                 = { {630, 180, 50, 40}, "+" };

    // noise boundary range and max width
    rSlider from             = { {10, 240, 450, 40}, "", "", 0.0f, -2.0f, 2.0f, on_noise_boundary_change_from_slider };
    rSlider to               = { {10, 300, 450, 40}, "", "", 0.1f, -2.0f, 2.0f, on_noise_boundary_change_to_slider };
    rSlider width            = { {10, 360, 450, 40}, "", "", 0.1f, 0.0f, 1.0f, on_noise_boundary_change_width_slider };
    rValueBoxFloat from_box  = { {540, 240, 140, 40}, "from", 0.0f, false, on_noise_boundary_change_from_box };
    rValueBoxFloat to_box    = { {540, 300, 140, 40}, "to", 0.1f, false, on_noise_boundary_change_to_box };
    rValueBoxFloat width_box = { {540, 360, 140, 40}, "width", 0.1f, false, on_noise_boundary_change_width_box };
    
    //
    rDropdownBox type         = { { 10, 180, 300, 40}, "Value;ValueFractal;Perlin;PerlinFractal;Simplex;SimplexFractal;Cellular;WhiteNoise;Cubic;CubicFractal", 6, false, on_type_change};
    rDropdownBox interp       = { { 10, 230, 300, 40}, "Linear;Hermite;Quintic", 0, false, on_interp_change};
    rSpinner seed             = { {480, 180, 200, 40}, "seed", 0, 0, 999999999, false, on_seed_change };
    rSpinner octaves          = { {480, 230, 200, 40}, "octaves", 3, 0, 100, false, on_octaves_change};
    
    rSlider gain                  = { { 10, 290, 450, 40}, "", "", 0.5f, 0.0f, 1.0f, on_gain_change_slider };
    rValueBoxFloat gain_box       = { {540, 290, 140, 40}, "gain", 0.5f, false, on_gain_change_box };
    rSlider frequency             = { {10, 350, 450, 40}, "", "", 0.02f, 0.0f, 0.1f, on_frequency_change_slider };
    rValueBoxFloat frequency_box  = { {540, 350, 140, 40}, "freq", 0.02f, false, on_frequency_change_box };
    rSlider lacunarity            = { {10, 410, 450, 40}, "", "", 2.0f, 0.0f, 10.0f, on_lacunarity_change_slider };
    rValueBoxFloat lacunarity_box = { {540, 410, 140, 40}, "lacu", 2.0f, false, on_lacunarity_change_box };

    rDropdownBox fractal_type  = { {10, 230, 300, 40}, "FBM;RigidMulti;Billow", 0, false, on_fractal_type_change};
    rDropdownBox return_type   = { {10, 280, 300, 40}, "Value;Distance;Distance2;Distance2Add;Distance2Sub;Distance2Mul;Distance2Div", 1, false, on_return_type_change};
    rDropdownBox distance      = { {10, 330, 300, 40}, "Euclidean;Manhattan;Natural", 0, false, on_distance_change};
    rSlider jitter              = { {10, 180, 440, 40}, "", "", 2.0f, 0.0f, 4.0f, on_jitter_change_slider };
    rValueBoxFloat jitter_box   = { {540, 180, 140, 40}, "jitter", 2.0f, false, on_jitter_change_box };
    rSpinner index0             = { {480, 280, 200, 40}, "0", 0, 0, 4, false, on_index0_change};
    rSpinner index1             = { {480, 330, 200, 40}, "1", 1, 0, 4, false, on_index1_change};
} ui_options;

// callbacks

void on_fractal_type_change()
{
    const FastNoise::FractalType types[] = { FastNoise::FractalType::FBM, FastNoise::FractalType::RigidMulti, FastNoise::FractalType::Billow };
    FastNoise::FractalType type = types[ui_options.fractal_type.active];
    if (type != grid.noise.at(0).noise.GetFractalType())
    {
        grid.noise.at(0).noise.SetFractalType(type);
        grid.update_new_noise();
    }
}
void on_return_type_change()
{
    const FastNoise::CellularReturnType types[] = { // not using LookupNoise
        FastNoise::CellularReturnType::CellValue,
        FastNoise::CellularReturnType::Distance,
        FastNoise::CellularReturnType::Distance2,
        FastNoise::CellularReturnType::Distance2Add,
        FastNoise::CellularReturnType::Distance2Sub,
        FastNoise::CellularReturnType::Distance2Mul,
        FastNoise::CellularReturnType::Distance2Div
    };
    FastNoise::CellularReturnType type = types[ui_options.return_type.active];
    if (type != grid.noise.at(0).noise.GetCellularReturnType())
    {
        grid.noise.at(0).noise.SetCellularReturnType(type);
        grid.update_new_noise();
    }
}
void on_distance_change() 
{
    const FastNoise::CellularDistanceFunction types[] = { FastNoise::CellularDistanceFunction::Euclidean, FastNoise::CellularDistanceFunction::Manhattan, FastNoise::CellularDistanceFunction::Natural };
    FastNoise::CellularDistanceFunction type = types[ui_options.distance.active];
    if (type != grid.noise.at(0).noise.GetCellularDistanceFunction())
    {
        grid.noise.at(0).noise.SetCellularDistanceFunction(type);
        grid.update_new_noise();
    }
}
void on_jitter_change_slider() 
{
    if (ui_options.jitter.value != grid.noise.at(0).noise.GetCellularJitter())
    {
        grid.noise.at(0).noise.SetCellularJitter(ui_options.jitter.value);
        grid.update_new_noise();
    }
}
void on_jitter_change_box() 
{
    if (ui_options.jitter_box.value != grid.noise.at(0).noise.GetCellularJitter())
    {
        grid.noise.at(0).noise.SetCellularJitter(ui_options.jitter_box.value);
        grid.update_new_noise();
    }
}
void on_index0_change() 
{
    int index0, index1;
    grid.noise.at(0).noise.GetCellularDistance2Indices(index0, index1);
    if (ui_options.index0.value != index0)
    {
        grid.noise.at(0).noise.SetCellularDistance2Indices(ui_options.index0.value, index1);
        grid.update_new_noise();
    }
}
void on_index1_change() 
{
    int index0, index1;
    grid.noise.at(0).noise.GetCellularDistance2Indices(index0, index1);
    if (ui_options.index1.value != index1)
    {
        grid.noise.at(0).noise.SetCellularDistance2Indices(index0, ui_options.index1.value);
        grid.update_new_noise();
    }
}

void on_gain_change_slider()
{
    if (ui_options.gain.value != grid.noise.at(0).noise.GetFractalGain())
    {
        grid.noise.at(0).noise.SetFractalGain(ui_options.gain.value);
        grid.update_new_noise();
    }
}
void on_gain_change_box()
{
    if (ui_options.gain_box.value != grid.noise.at(0).noise.GetFractalGain())
    {
        grid.noise.at(0).noise.SetFractalGain(ui_options.gain_box.value);
        grid.update_new_noise();
    }
}
void on_frequency_change_slider()
{
    if (ui_options.frequency.value != grid.noise.at(0).noise.GetFrequency())
    {
        grid.noise.at(0).noise.SetFrequency(ui_options.frequency.value);
        grid.update_new_noise();
    }
}
void on_frequency_change_box()
{
    if (ui_options.frequency_box.value != grid.noise.at(0).noise.GetFrequency())
    {
        grid.noise.at(0).noise.SetFrequency(ui_options.frequency_box.value);
        grid.update_new_noise();
    }
}
void on_lacunarity_change_slider()
{
    if (ui_options.lacunarity.value != grid.noise.at(0).noise.GetFractalLacunarity())
    {
        grid.noise.at(0).noise.SetFractalLacunarity(ui_options.lacunarity.value);
        grid.update_new_noise();
    }
}
void on_lacunarity_change_box()
{
    if (ui_options.lacunarity_box.value != grid.noise.at(0).noise.GetFractalLacunarity())
    {
        grid.noise.at(0).noise.SetFractalLacunarity(ui_options.lacunarity_box.value);
        grid.update_new_noise();
    }
}

void on_type_change()
{
    const FastNoise::NoiseType types[] = { 
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
    int index = ui_options.type.active;
    FastNoise::NoiseType type = types[index];
    grid.noise.at(0).noise.SetNoiseType(type);

    grid.update_new_noise();
}

void on_interp_change()
{
    const FastNoise::Interp interps[] = { FastNoise::Interp::Linear, FastNoise::Interp::Hermite, FastNoise::Interp::Quintic };
    int index = ui_options.interp.active;
    FastNoise::Interp interp = interps[index];
    grid.noise.at(0).noise.SetInterp(interp);
    
    grid.update_new_noise();
}

void on_seed_change()
{
    if (ui_options.seed.value != grid.noise.at(0).noise.GetSeed())
    {
        grid.noise.at(0).noise.SetSeed(ui_options.seed.value);
        grid.update_new_noise();
    }
}

void on_octaves_change()
{
    if (ui_options.octaves.value != grid.noise.at(0).noise.GetFractalOctaves())
    {
        grid.noise.at(0).noise.SetFractalOctaves(ui_options.octaves.value);
        grid.update_new_noise();
    }
}

void on_grid_size_change()
{
    // same callback used for all 3 widgets... Yay me...
    if (ui_grid.w.value != grid.grid_size.x || ui_grid.h.value != grid.grid_size.y || ui_grid.d.value != grid.grid_size.z)
    {
        grid.grid_size = { ui_grid.w.value, ui_grid.h.value, ui_grid.d.value };
        std::cout << "Grid size changed to " << grid.grid_size.x << "x" << grid.grid_size.y << "x" << grid.grid_size.z << std::endl;
    }
}

void on_chunk_size_change()
{ // "8;16;32;64;128"
    const int sizes[] = { 8, 16, 32, 64, 128 };
    int size = sizes[ui_grid.chunk_size.active];
    grid.chunk_size = { size, size, size };
    //std::cout << "Chunk size changed to " << grid.chunk_size.x << "x" << grid.chunk_size.y << "x" << grid.chunk_size.z << std::endl;
}

void on_index_delete_button_click()
{
    // remove the selected index from the dropdown
    // TODO: implement
}

void do_boundary_change()
{
    float from = ui_options.from.value;
    float to = ui_options.to.value;
    float width = ui_options.width.value;
    
    // TODO: update when newer noise is working
    grid.noise.at(0).range_min = from;
    grid.noise.at(0).range_max = to;
    grid.noise.at(0).range_width = width;

    grid.update_noise_range();
}

void on_noise_boundary_change_from_slider()
{
    //    
    float from = ui_options.from.value;
    float to = ui_options.to.value;
    float width = ui_options.width.value;

    if (from > to) std::swap(from, to);

    if (to - from > width) to = from + width;

    ui_options.from.value = from;
    ui_options.to.value = to;

    ui_options.from_box.write(from);
    ui_options.to_box.write(to);

    do_boundary_change();
}

void on_noise_boundary_change_to_slider() 
{
    //
    float from = ui_options.from.value;
    float to = ui_options.to.value;
    float width = ui_options.width.value;

    if (from > to) std::swap(from, to);

    if (to - from > width) from = to - width;

    ui_options.from.value = from;
    ui_options.to.value = to;

    ui_options.from_box.write(from);
    ui_options.to_box.write(to);

    do_boundary_change();
}

void on_noise_boundary_change_width_slider() 
{
    //
    float from = ui_options.from.value;
    float to = ui_options.to.value;
    float width = ui_options.width.value;
    
    if (from > to) std::swap(from, to);
    
    if (to - from > width) to = from + width;

    ui_options.from.value = from;
    ui_options.to.value = to;
    ui_options.width.value = width;

    ui_options.from_box.write(from);
    ui_options.to_box.write(to);
    ui_options.width_box.write(width);

    do_boundary_change();
}

void on_noise_boundary_change_from_box()
{
    float from = ui_options.from_box.value;
    on_noise_boundary_change_from_slider();
    ui_options.from.value = from;
    ui_options.from_box.write(from);
}

void on_noise_boundary_change_to_box()
{
    float to = ui_options.to_box.value;
    on_noise_boundary_change_to_slider();
    ui_options.to.value = to;
    ui_options.to_box.write(to);
}

void on_noise_boundary_change_width_box()
{
    float width = ui_options.width_box.value;
    on_noise_boundary_change_width_slider();
    ui_options.width.value = width;
    ui_options.width_box.write(width);
}

void draw_ui_options() {
    if (ui_options.menu_open.active) {
        if (ui_options.menu_close.draw()) {
            ui_options.menu_open.active = false;
        }

        //int before = ui_options.grid.active | (ui_options.noise.active << 1 | ui_options.cellular.active << 2);

        ui_options.grid.draw();
        if (ui_options.grid.active) {
            ui_options.noise.active = false;
            ui_options.cellular.active = false;
        }
        ui_options.noise.draw();
        if (ui_options.noise.active) {
            ui_options.grid.active = false;
            ui_options.cellular.active = false;
        }
        if (ui_options.type.active == 6) {
            ui_options.cellular.draw() ;
            if (ui_options.cellular.active) {
                ui_options.grid.active = false;
                ui_options.noise.active = false;
            }
        }

        int after = ui_options.grid.active | (ui_options.noise.active << 1 | ui_options.cellular.active << 2);
        
        if (after == 0) {
            ui_options.menu_open.active = false;
            ui_options.grid.active = true;
            return;
        }
       
        if (ui_options.grid.active) {
            ui_options.index_delete_button.draw();
            ui_options.add.draw();
            if (ui_options.from.draw()) ui_options.from_box.write(ui_options.from.value);
            if (ui_options.to.draw()) ui_options.to_box.write(ui_options.to.value);
            if (ui_options.width.draw()) ui_options.width_box.write(ui_options.width.value);
            ui_options.from_box.draw();
            ui_options.to_box.draw();
            ui_options.width_box.draw();
            ui_options.index.draw();
            ui_options.op.draw();
        }

        if (ui_options.noise.active) {
            ui_options.seed.draw();
            ui_options.octaves.draw();
            ui_options.gain_box.draw();
            ui_options.frequency_box.draw();
            ui_options.lacunarity_box.draw();
            if (ui_options.gain.draw()) ui_options.gain_box.write(ui_options.gain.value);
            if (ui_options.frequency.draw()) ui_options.frequency_box.write(ui_options.frequency.value);
            if (ui_options.lacunarity.draw()) ui_options.lacunarity_box.write(ui_options.lacunarity.value);
            
            ui_options.interp.draw();
            ui_options.type.draw();
        }

        if (ui_options.cellular.active) {
            if (ui_options.jitter.draw()) ui_options.jitter_box.write(ui_options.jitter.value);
            ui_options.jitter_box.draw();
            ui_options.index0.draw();
            ui_options.index1.draw();

            ui_options.distance.draw();
            ui_options.return_type.draw();
            ui_options.fractal_type.draw();
        }
    } else {
        ui_options.menu_open.draw();
    }
}

void draw_ui_grid() {
    if (ui_grid.menu_open.active) 
    {
        ui_grid.menu_line.draw();
        if (ui_grid.menu_close.draw()) {
            ui_grid.menu_open.active = false;
        }
        ui_grid.w.draw();
        ui_grid.h.draw();
        ui_grid.d.draw();
        ui_grid.chunk_size_label.draw();
        draw_ui_options();
        ui_grid.chunk_size.draw();
    } else {
        ui_grid.menu_open.draw();
    }

    
}

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
}

// ####### VARIABLES #######



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

void update_noise_grid() {
    if (uicam.dropdown.active == 0) {
        Vector3 pos = {0, 0, 0};
        grid.update(pos);
    } else {
        Vector3 pos = camera.position;
        grid.update(pos);
    }
    grid.render();
}

void draw_3d()
{
    /*switch (ui.camera_type.active) {
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
    }*/

    //UpdateCamera(&camera, CAMERA_ORBITAL);
    // removed and added to camera ui

    grid.light_point.position = camera.position;
    UpdateLightValues(grid.material.shader, grid.light_point);

    BeginMode3D(camera);
    DrawGrid(100, 8.0f);

    update_noise_grid();

    EndMode3D();
}

// ######################### MAIN #########################

int main() {
    // Initialization
    int screenWidth = 1280;
    int screenHeight = 720;
    
    InitWindow(screenWidth, screenHeight, "FastNoise x3 Playground (C)WLGfx Carl S Norwood 2026"); // corrected the year (Wed 27 Jan 26-12:47GMT)
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
            //draw_ui();
            draw_ui_grid();
            draw_camera_ui();
            
            DrawFPS(10, screenHeight - 30);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


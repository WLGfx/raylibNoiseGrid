#include "inc/myui.h"
#include "inc/r/raygui+/raygui+.h"
#include "raylib.h"
//#include <iostream>


void MYUI::update() {
    float screen_width = GetScreenWidth();
    float screen_height = GetScreenHeight();

    if (screen_width != window_size.width || screen_height != window_size.height) 
    {
        //std::cout << "UI widgets resizing..." << std::endl << std::flush;

        window_size.width = screen_width;
        window_size.height = screen_height;

        grid.update();
        noise_type.update();
        noise_sliders.update();
        noise_values.update();
        cellular.update();
        jitter.update();
        jitter_box.update();
        indexes.update();
        camera_type.update();
        camera_orbit_sliders.update();
        camera_orbit_speed_value.update();
        camera_orbit_zoom_speed.update();
        pan_mouse_sensitivity.update();
        pan_mouse_sensitivity_value.update();


        seed_box.value = seed.value;
        octaves_box.value = octaves.value;
        frequency_box.write(frequency.value);
        gain_box.write(gain.value);
        lacunarity_box.write(lacunarity.value);
        greater_than_box.write(greater_than.value);
        less_than_box.write(less_than.value);
        jitter_value.write(jitter_slider.value);

        tests.all.update();
        //tests.columns.update();
    }
}

void MYUI::draw() {
    menu_select.draw();

    if (menu_select.active == 0) grid.draw();

    if (menu_select.active == 1) {
        noise_sliders.draw();
        GuiDisable(); noise_values.draw(); GuiEnable();
        if (noise_type_dropdown.active == 6) { // cellular
            indexes.draw();
            cellular.draw();
            jitter.draw();
            GuiDisable(); jitter_box.draw(); GuiEnable();
        }
        noise_type.draw();
    }

    if (menu_select.active == 2) {
        camera_type.draw();

        if (camera_type_dropdown.active == rCameraMode::ORBITAL) {
            camera_orbit_sliders.draw();
            camera_orbit_zoom_speed.draw();
            GuiDisable(); 
                camera_orbit_speed_value.draw(); 
                camera_orbit_zoom_speed_value_box.draw(); 
            GuiEnable();            
        }
        if (camera_type_dropdown.active == rCameraMode::PAN) {
            pan_mouse_sensitivity.draw();
            GuiDisable(); pan_mouse_sensitivity_value.draw(); GuiEnable();
        }
        if (camera_type_dropdown.active == rCameraMode::FREE) {
            // TODO: Add free controls
        }
    }

    if (menu_select.active == 3) {
        tests.all.draw();
    }

    switch (camera_type_dropdown.active) {
        case rCameraMode::ORBITAL:
            {
                camera->orbit(camera_orbit_speed.value);
                float wheel_distance = -GetMouseWheelMove() * camera_orbit_zoom_speed.value + camera->get_distance();
                wheel_distance = std::max(0.1f, wheel_distance);
                camera->set_distance(wheel_distance);
            }
            break;
        case rCameraMode::PAN:
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    // get deltas for x and y movement
                    float deltaX = -GetMouseDelta().x * pan_mouse_sensitivity_slider.value;
                    float deltaY = GetMouseDelta().y * pan_mouse_sensitivity_slider.value;
                    // rotate camera around target using the deltas and frametime()
                    camera->yaw(deltaX * GetFrameTime(), true);
                    camera->pitch(deltaY * GetFrameTime(), true, true);
                }
                float wheel_distance = -GetMouseWheelMove() * camera_orbit_zoom_speed.value + camera->get_distance();
                wheel_distance = std::max(0.1f, wheel_distance);
                camera->set_distance(wheel_distance);
            }
            break;
        case rCameraMode::FREE:
            // TODO: Add free controls
            break;
    }
}

// ************************************************ GRID ************************************************

void MYUI::on_grid_chunk_size_changed() {
    //std::cout << "Grid chunk size changed" << std::endl;
    int size = chunk_sizes[grid_chunk_size.active];
    noise_grid->chunk.size = {size, size, size};
}

void MYUI::on_grid_size_changed() {
    //std::cout << "Grid size changed" << std::endl;
    vec3i size = {grid_width.value, grid_height.value, grid_depth.value};
    noise_grid->grid.size = size;
}

// ************************************************ NOISE ************************************************

void MYUI::on_noise_type_changed() {
    //std::cout << "Noise type changed" << std::endl;
    auto noise_type = noise_types[noise_type_dropdown.active];
    noise_grid->noise.SetNoiseType(noise_type);
    noise_grid->grid.old.x--;
}

void MYUI::on_noise_interp_changed() {
    //std::cout << "Noise interp changed" << std::endl;
    auto interp = interp_types[noise_interp_dropdown.active];
    noise_grid->noise.SetInterp(interp);
    noise_grid->grid.old.x--;
}

// sliders

void MYUI::on_seed_changed() {
    //std::cout << "Seed changed" << std::endl;
    seed_box.value = seed.value;
    noise_grid->noise.SetSeed(seed.value);
    noise_grid->grid.old.x--;
}

void MYUI::on_octaves_changed() {
    //std::cout << "Octaves changed" << std::endl;
    octaves_box.value = octaves.value;
    noise_grid->noise.SetFractalOctaves(octaves.value);
    noise_grid->grid.old.x--;
}

void MYUI::on_frequency_changed() {
    //std::cout << "Frequency changed" << std::endl;
    frequency_box.write(frequency.value);
    noise_grid->noise.SetFrequency(frequency.value);
    noise_grid->grid.old.x--;
}

void MYUI::on_gain_changed() {
    //std::cout << "Gain changed" << std::endl;
    gain_box.write(gain.value);
    noise_grid->noise.SetFractalGain(gain.value);
    noise_grid->grid.old.x--;
}

void MYUI::on_lacunarity_changed() {
    //std::cout << "Lacunarity changed" << std::endl;
    lacunarity_box.write(lacunarity.value);
    noise_grid->noise.SetFractalLacunarity(lacunarity.value);
    noise_grid->grid.old.x--;
}

void MYUI::on_greater_than_changed() {
    //std::cout << "Greater than changed" << std::endl;
    greater_than_box.write(greater_than.value);
    noise_grid->range.min = greater_than.value;
    noise_grid->grid.old.x--;
}

void MYUI::on_less_than_changed() {
    //std::cout << "Less than changed" << std::endl;
    less_than_box.write(less_than.value);
    noise_grid->range.max = less_than.value;
    noise_grid->grid.old.x--;
}

// cellular

void MYUI::on_fractal_changed() {
    //std::cout << "Fractal changed" << std::endl;
    noise_grid->noise.SetFractalType(fractal_types[fractal.active]);
    noise_grid->grid.old.x--;
}

void MYUI::on_distance_function_changed() {
    //std::cout << "Distance function changed" << std::endl;
    noise_grid->noise.SetCellularDistanceFunction(distance_functions[distance_function.active]);
    noise_grid->grid.old.x--;
}

void MYUI::on_return_type_changed() {
    //std::cout << "Return type changed" << std::endl;
    noise_grid->noise.SetCellularReturnType(return_types[return_type.active]);
    noise_grid->grid.old.x--;
}

void MYUI::on_jitter_changed() {
    //std::cout << "Jitter changed" << std::endl;
    jitter_value.write(jitter_slider.value);
    noise_grid->noise.SetCellularJitter(jitter_slider.value);
    noise_grid->grid.old.x--;
}

void MYUI::on_index_changed() {
    //std::cout << "Index changed" << std::endl;
    noise_grid->noise.SetCellularDistance2Indices(index0.value, index1.value);
    noise_grid->grid.old.x--;
}


void MYUI::on_camera_orbit_speed_changed() {
    //std::cout << "Camera orbit speed changed" << std::endl;
    camera_orbit_speed_value_box.write(camera_orbit_speed.value);
}

void MYUI::on_camera_orbit_distance_changed() {
    //std::cout << "Camera orbit distance changed" << std::endl;
    camera_orbit_distance_value_box.value = camera_orbit_distance.value;
    camera->set_distance(camera_orbit_distance.value);
}

void MYUI::on_pan_mouse_sensitivity_changed() {
    //std::cout << "Pan mouse sensitivity changed" << std::endl;
    pan_mouse_sensitivity_value_box.write(pan_mouse_sensitivity_slider.value);
}

void MYUI::on_camera_orbit_zoom_speed_changed() {
    //std::cout << "Camera orbit zoom speed changed" << std::endl;
    camera_orbit_zoom_speed_value_box.value = camera_orbit_zoom_speed.value;
}
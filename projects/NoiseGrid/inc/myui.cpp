#include "myui.h"
#include "inc/raygui+.h"
#include "raylib.h"
#include <iostream>


void MYUI::update() {
    float screen_width = GetScreenWidth();
    float screen_height = GetScreenHeight();

    if (screen_width != window_size.width || screen_height != window_size.height) 
    {
        std::cout << "UI widgets resizing..." << std::endl << std::flush;

        window_size.width = screen_width;
        window_size.height = screen_height;

        grid.update();
        noise_type.update();
        noise_sliders.update();
        noise_values.update();
        cellular.update();
        jitter.update();
        jitter_box.update();

        seed_box.value = seed.value;
        octaves_box.value = octaves.value;
        frequency_box.write(frequency.value);
        gain_box.write(gain.value);
        lacunarity_box.write(lacunarity.value);
        greater_than_box.write(greater_than.value);
        less_than_box.write(less_than.value);
        jitter_value.write(jitter_slider.value);
    }

    menu_select.draw();

    if (menu_select.active == 0) grid.draw();

    if (menu_select.active == 1) {
        noise_type.draw();
        noise_sliders.draw();
        GuiDisable(); noise_values.draw(); GuiEnable();
        if (noise_type_dropdown.active == 6) { // cellular
            cellular.draw();
            jitter.draw();
            GuiDisable(); jitter_box.draw(); GuiEnable();
        }
    }

}

// ************************************************ GRID ************************************************

void MYUI::on_grid_chunk_size_changed() {
    std::cout << "Grid chunk size changed" << std::endl;
    int size = chunk_sizes[grid_chunk_size.active];
    noise_grid->chunk.size = {size, size, size};
}

void MYUI::on_grid_size_changed() {
    std::cout << "Grid size changed" << std::endl;
    vec3i size = {grid_width.value, grid_height.value, grid_depth.value};
    noise_grid->grid.size = size;
}

// ************************************************ NOISE ************************************************

void MYUI::on_noise_type_changed() {
    std::cout << "Noise type changed" << std::endl;
    auto noise_type = noise_types[noise_type_dropdown.active];
    noise_grid->noise.SetNoiseType(noise_type);
    noise_grid->grid.old.x--;
}

void MYUI::on_noise_interp_changed() {
    std::cout << "Noise interp changed" << std::endl;
    auto interp = interp_types[noise_interp_dropdown.active];
    noise_grid->noise.SetInterp(interp);
    noise_grid->grid.old.x--;
}

// sliders

void MYUI::on_seed_changed() {
    std::cout << "Seed changed" << std::endl;
    seed_box.value = seed.value;
    noise_grid->noise.SetSeed(seed.value);
    noise_grid->grid.old.x--;
}

void MYUI::on_octaves_changed() {
    std::cout << "Octaves changed" << std::endl;
    octaves_box.value = octaves.value;
    noise_grid->noise.SetFractalOctaves(octaves.value);
    noise_grid->grid.old.x--;
}

void MYUI::on_frequency_changed() {
    std::cout << "Frequency changed" << std::endl;
    frequency_box.write(frequency.value);
    noise_grid->noise.SetFrequency(frequency.value);
    noise_grid->grid.old.x--;
}

void MYUI::on_gain_changed() {
    std::cout << "Gain changed" << std::endl;
    gain_box.write(gain.value);
    noise_grid->noise.SetFractalGain(gain.value);
    noise_grid->grid.old.x--;
}

void MYUI::on_lacunarity_changed() {
    std::cout << "Lacunarity changed" << std::endl;
    lacunarity_box.write(lacunarity.value);
    noise_grid->noise.SetFractalLacunarity(lacunarity.value);
    noise_grid->grid.old.x--;
}

void MYUI::on_greater_than_changed() {
    std::cout << "Greater than changed" << std::endl;
    greater_than_box.write(greater_than.value);
    noise_grid->range.min = greater_than.value;
    noise_grid->grid.old.x--;
}

void MYUI::on_less_than_changed() {
    std::cout << "Less than changed" << std::endl;
    less_than_box.write(less_than.value);
    noise_grid->range.max = less_than.value;
    noise_grid->grid.old.x--;
}

// cellular

void MYUI::on_fractal_changed() {
    std::cout << "Fractal changed" << std::endl;
    noise_grid->noise.SetFractalType(fractal_types[fractal.active]);
    noise_grid->grid.old.x--;
}

void MYUI::on_distance_function_changed() {
    std::cout << "Distance function changed" << std::endl;
    noise_grid->noise.SetCellularDistanceFunction(distance_functions[distance_function.active]);
    noise_grid->grid.old.x--;
}

void MYUI::on_return_type_changed() {
    std::cout << "Return type changed" << std::endl;
    noise_grid->noise.SetCellularReturnType(return_types[return_type.active]);
    noise_grid->grid.old.x--;
}

void MYUI::on_jitter_changed() {
    std::cout << "Jitter changed" << std::endl;
    jitter_value.write(jitter_slider.value);
    noise_grid->noise.SetCellularJitter(jitter_slider.value);
    noise_grid->grid.old.x--;
}

/*
void MYUI::draw() { 
    // update camera
    UpdateCamera(camera, CAMERA_ORBITAL);

    bool slow_fps = false;

    vbox.update(); 
    if (m_grid.draw() || m_grid.active) {
        slow_fps = true;
        m_noise.active = false;
        m_camera.active = false;
    }
    if (m_noise.draw() || m_noise.active) {
        slow_fps = true;
        m_grid.active = false;
        m_camera.active = false;
    }
    if (m_camera.draw() || m_camera.active) {
        slow_fps = true;
        m_grid.active = false;
        m_noise.active = false;
    }*/
    /*int fps = GetFPS();
    if (slow_fps && fps > 40) SetTargetFPS(30);
    else if (fps < 40) SetTargetFPS(60);*/
    
/*    GuiDrawIcon(ICON_GRID, m_grid.bounds.x + 10, m_grid.bounds.y + 6, 2, RAYWHITE);
    GuiDrawIcon(ICON_WAVE, m_noise.bounds.x + 10, m_noise.bounds.y + 6, 2, RAYWHITE);
    GuiDrawIcon(ICON_CAMERA, m_camera.bounds.x + 10, m_camera.bounds.y + 6, 2, RAYWHITE);

    m_effect_1.draw();

    if (m_grid.active) {
        grid_vbox.update();
        // drawing them separately so the dropdown is drawn on top
        chunk_size_line.draw();
        grid_wid.draw();
        grid_hgt.draw();
        grid_dep.draw();
        grid_label.draw();
        chunk_size.draw();
    }

    if (m_noise.active) {
        range_vbox.update();
        range_value_vbox.update();
        range_vbox.draw();
        range_value_vbox.draw();
        
        dropdowns_noise_hbox.update();
        dropdowns_noise_hbox.draw();

        if (noise_type.active == 6) { // Cellular
            fractal_params_hbox.update();
            fractal_params_hbox.draw();
            
            jitter_vbox.update();
            jitter_vbox.draw();
            
            jitter_slider_vbox.update();
            jitter_slider_vbox.draw();
        }
    }

    if (m_camera.active) {
        camera_vbox.update();
        camera_vbox.draw();
    }
}*/
/*
void MYUI::update_grid() {
    noise_grid->chunk.old.x -= 1;
}

void MYUI::set_noise_from_ui() {
    noise_grid->range.min = from_slider.value;
    noise_grid->range.max = to_slider.value;
    noise_grid->grid.size = {grid_wid.value, grid_hgt.value, grid_dep.value};
    noise_grid->chunk.size = {chunk_sizes[chunk_size.active], chunk_sizes[chunk_size.active], chunk_sizes[chunk_size.active]};
    noise_grid->noise.SetSeed(seed_slider.value);
    noise_grid->noise.SetFractalOctaves(octaves_slider.value);
    noise_grid->noise.SetFrequency(frequency_slider.value);
    noise_grid->noise.SetFractalGain(gain_slider.value);
    noise_grid->noise.SetFractalLacunarity(lacunarity_slider.value);
    noise_grid->noise.SetNoiseType(noise_types[noise_type.active]);
    noise_grid->noise.SetInterp(interp_types[interp.active]);
    noise_grid->noise.SetFractalType(fractal_types[fractal_type.active]);
    noise_grid->noise.SetCellularDistanceFunction(distance_functions[distance.active]);
    noise_grid->noise.SetCellularReturnType(return_types[return_type.active]);
    noise_grid->noise.SetCellularDistance2Indices(index0.value, index1.value);
    noise_grid->noise.SetCellularJitter(jitter.value);
}
*/
// custom callbacks
/*
void MYUI::on_effect_1_pressed() {
    //noise_grid->effect_drop();
}

void MYUI::on_grid_size_changed() {
    vec3i size = {grid_wid.value, grid_hgt.value, grid_dep.value};
    noise_grid->grid.size = size;
}

void MYUI::on_chunk_size_changed() {
    int size = chunk_sizes[chunk_size.active];
    noise_grid->chunk.size = {size, size, size};
}

void MYUI::on_seed_changed() {
    noise_grid->noise.SetSeed(seed_slider.value);
    seed_value.value = seed_slider.value;
    update_grid();
}

void MYUI::on_octaves_changed() {
    noise_grid->noise.SetFractalOctaves(octaves_slider.value);
    octaves_value.value = octaves_slider.value;
    update_grid();
}

void MYUI::on_frequency_changed() {
    noise_grid->noise.SetFrequency(frequency_slider.value);
    frequency_value.value = frequency_slider.value;
    frequency_value.write();
    update_grid();
}

void MYUI::on_gain_changed() {
    noise_grid->noise.SetFractalGain(gain_slider.value);
    gain_value.value = gain_slider.value;
    gain_value.write();
    update_grid();
}

void MYUI::on_lacunarity_changed() {
    noise_grid->noise.SetFractalLacunarity(lacunarity_slider.value);
    lacunarity_value.value = lacunarity_slider.value;
    lacunarity_value.write();
    update_grid();
}

void MYUI::on_from_changed() {
    noise_grid->range.min = from_slider.value;
    if (noise_grid->range.min + noise_grid->range.max > width_slider.value) {
        noise_grid->range.max = noise_grid->range.min + width_slider.value;
    }
    from_value.value = from_slider.value;
    from_value.write();
    update_grid();
}

void MYUI::on_to_changed() {
    noise_grid->range.max = to_slider.value;
    if (noise_grid->range.min + noise_grid->range.max > width_slider.value) {
        noise_grid->range.min = noise_grid->range.max - width_slider.value;
    }
    to_value.value = to_slider.value;
    to_value.write();
    update_grid();
}

void MYUI::on_width_changed() {
    if (from_slider.value + to_slider.value > width_slider.value) {
        from_slider.value = width_slider.value - to_slider.value;
    }
    width_value.value = width_slider.value;
    width_value.write();
    update_grid();
}

void MYUI::on_noise_type_changed() {
    FastNoise::NoiseType type = noise_types[noise_type.active];
    noise_grid->noise.SetNoiseType(type);
    update_grid();
}

void MYUI::on_interp_changed() {
    FastNoise::Interp type = interp_types[interp.active];
    noise_grid->noise.SetInterp(type);
    update_grid();
}

void MYUI::on_fractal_type_changed() {
    FastNoise::FractalType type = fractal_types[fractal_type.active];
    noise_grid->noise.SetFractalType(type);
    update_grid();
}

void MYUI::on_distance_changed() {
    FastNoise::CellularDistanceFunction function = distance_functions[distance.active];
    noise_grid->noise.SetCellularDistanceFunction(function);
    update_grid();
}

void MYUI::on_return_type_changed() {
    FastNoise::CellularReturnType type = return_types[return_type.active];
    noise_grid->noise.SetCellularReturnType(type);
    update_grid();
}

void MYUI::on_indexes_changed() { // nb. rSpinner
    if ((index0.editMode || index1.editMode)) {
        noise_grid->noise.SetCellularDistance2Indices(index0.value, index1.value);
        update_grid();
    }
}

void MYUI::on_jitter_value_changed() {
    noise_grid->noise.SetCellularJitter(jitter.value);
    jitter_slider.value = jitter.value;
    update_grid();
}

void MYUI::on_jitter_slider_changed() {
    noise_grid->noise.SetCellularJitter(jitter_slider.value);
    jitter.write(jitter_slider.value);
    update_grid();
}

void MYUI::on_orbital_speed_changed() {
    float speed = orbital_speed.value;
    std::cout << "Orbital speed: " << speed << std::endl;
}
*/


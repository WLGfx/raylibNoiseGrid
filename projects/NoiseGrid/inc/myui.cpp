#include "myui.h"
#include "inc/FastNoise.h"
#include "raylib.h"

void MYUI::draw() { 
    // update camera
    UpdateCamera(camera, CAMERA_ORBITAL);


    vbox.update(); 
    if (m_grid.draw() || m_grid.active) {
        m_noise.active = false;
        m_camera.active = false;
    }
    if (m_noise.draw() || m_noise.active) {
        m_grid.active = false;
        m_camera.active = false;
    }
    if (m_camera.draw() || m_camera.active) {
        m_grid.active = false;
        m_noise.active = false;
    }
    GuiDrawIcon(ICON_GRID, m_grid.bounds.x + 10, m_grid.bounds.y + 6, 2, RAYWHITE);
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
}

// custom callbacks

void MYUI::on_effect_1_pressed() {
    noise_grid->effect_drop();
}

void MYUI::on_chunk_size_changed() {
    const int sizes[] = {16, 24, 32, 48, 64, 96};
    int size = sizes[chunk_size.active];
    noise_grid->chunk_size = {size, size, size};
}

void MYUI::on_grid_size_changed() {
    noise_grid->grid_size = {grid_wid.value, grid_hgt.value, grid_dep.value};
}

void MYUI::on_seed_changed() {
    noise_grid->noise[0].noise.SetSeed(seed_slider.value);
    seed_value.value = seed_slider.value;
    noise_grid->update_new_noise();
}

void MYUI::on_octaves_changed() {
    noise_grid->noise[0].noise.SetFractalOctaves(octaves_slider.value);
    octaves_value.value = octaves_slider.value;
    noise_grid->update_new_noise();
}

void MYUI::on_frequency_changed() {
    noise_grid->noise[0].noise.SetFrequency(frequency_slider.value);
    frequency_value.value = frequency_slider.value;
    frequency_value.write();
    noise_grid->update_new_noise();
}

void MYUI::on_gain_changed() {
    noise_grid->noise[0].noise.SetFractalGain(gain_slider.value);
    gain_value.value = gain_slider.value;
    gain_value.write();
    noise_grid->update_new_noise();
}

void MYUI::on_lacunarity_changed() {
    noise_grid->noise[0].noise.SetFractalLacunarity(lacunarity_slider.value);
    lacunarity_value.value = lacunarity_slider.value;
    lacunarity_value.write();
    noise_grid->update_new_noise();
}

void MYUI::on_from_changed() {
    noise_grid->noise[0].range_min = from_slider.value;
    if (noise_grid->noise[0].range_min + noise_grid->noise[0].range_max > width_slider.value) {
        noise_grid->noise[0].range_max = noise_grid->noise[0].range_min + width_slider.value;
    }
    from_value.value = from_slider.value;
    from_value.write();
    noise_grid->update_new_noise();
}

void MYUI::on_to_changed() {
    noise_grid->noise[0].range_max = to_slider.value;
    if (noise_grid->noise[0].range_min + noise_grid->noise[0].range_max > width_slider.value) {
        noise_grid->noise[0].range_min = noise_grid->noise[0].range_max - width_slider.value;
    }
    to_value.value = to_slider.value;
    to_value.write();
    noise_grid->update_new_noise();
}

void MYUI::on_width_changed() {
    if (from_slider.value + to_slider.value > width_slider.value) {
        from_slider.value = width_slider.value - to_slider.value;
    }
    width_value.value = width_slider.value;
    width_value.write();
    noise_grid->update_new_noise();
}

void MYUI::on_noise_type_changed() {
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
    FastNoise::NoiseType type = types[noise_type.active];
    noise_grid->noise[0].noise.SetNoiseType(type);
    noise_grid->update_new_noise();
}

void MYUI::on_interp_changed() {
    const FastNoise::Interp types[] = { 
        FastNoise::Interp::Linear,
        FastNoise::Interp::Hermite,
        FastNoise::Interp::Quintic
    };
    FastNoise::Interp type = types[interp.active];
    noise_grid->noise[0].noise.SetInterp(type);
    noise_grid->update_new_noise();
}

void MYUI::on_fractal_type_changed() {
    const FastNoise::FractalType types[] = { 
        FastNoise::FractalType::FBM,
        FastNoise::FractalType::RigidMulti,
        FastNoise::FractalType::Billow
    };
    FastNoise::FractalType type = types[fractal_type.active];
    noise_grid->noise[0].noise.SetFractalType(type);
    noise_grid->update_new_noise();
}

void MYUI::on_distance_changed() {
    const FastNoise::CellularDistanceFunction functions[] = { 
        FastNoise::CellularDistanceFunction::Euclidean,
        FastNoise::CellularDistanceFunction::Manhattan,
        FastNoise::CellularDistanceFunction::Natural
    };
    FastNoise::CellularDistanceFunction function = functions[distance.active];
    noise_grid->noise[0].noise.SetCellularDistanceFunction(function);
    noise_grid->update_new_noise();
}

void MYUI::on_return_type_changed() {
    const FastNoise::CellularReturnType types[] = { 
        FastNoise::CellularReturnType::CellValue,
        FastNoise::CellularReturnType::Distance,
        FastNoise::CellularReturnType::Distance2,
        FastNoise::CellularReturnType::Distance2Add,
        FastNoise::CellularReturnType::Distance2Sub,
        FastNoise::CellularReturnType::Distance2Mul,
        FastNoise::CellularReturnType::Distance2Div
    };
    FastNoise::CellularReturnType type = types[return_type.active];
    noise_grid->noise[0].noise.SetCellularReturnType(type);
    noise_grid->update_new_noise();
}

void MYUI::on_indexes_changed() {
    noise_grid->noise[0].noise.SetCellularDistance2Indices(index0.value, index1.value);
    //noise_grid->update_new_noise();
}

void MYUI::on_jitter_value_changed() {
    noise_grid->noise[0].noise.SetCellularJitter(jitter.value);
    jitter_slider.value = jitter.value;
    noise_grid->update_new_noise();
}

void MYUI::on_jitter_slider_changed() {
    noise_grid->noise[0].noise.SetCellularJitter(jitter_slider.value);
    jitter.write(jitter_slider.value);
    noise_grid->update_new_noise();
}

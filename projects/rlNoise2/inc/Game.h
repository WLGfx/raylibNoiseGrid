#ifndef GAME_H
#define GAME_H

#include "gui_menu.h"
#include "gui_dimensions.h"
#include "gui_noise.h"
#include "gui_view.h"

#include "FastNoise/FastNoise.h"

#include <vector>

class Game {
public:
    Game();
    ~Game();

    struct UI {
        GuiMenu       menu;
        GuiDimensions dimensions;
        GuiNoise      noise;
        GuiView       view;
    } ui;
    
    void run();

    void _init_ui();
    void _debug();
    void _ui_handler();

    Camera camera = {
        {0, 40, 80},  // position
        {0, 0, 0},    // target
        {0, 1, 0},    // up
        45, CAMERA_PERSPECTIVE // fov, type
    };

    struct MAP {
        struct Vec3i { int x, y, z; };
        struct Vec2i { int x, y; };

        struct : public FastNoise { FastNoise  lookup; } noise;

        int     chunk_size          = 8;
        Vec3i   block_size          = { 1, 1, 1 };
        Vec3i   size                = { 10, 10, 10 };
        Vec3i   position            = { 0, 0, 0 };
        Vec3i   view_input_position = { 0, 0, 0 };

        struct Chunk {
            std::vector<float> values;
            std::vector<Vec3i> blocks;
        };

        std::vector<Chunk> chunks;

    } map;
};

#endif // GAME_H

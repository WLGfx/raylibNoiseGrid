#ifndef UI2_H
#define UI2_H

//#include "inc/FastNoise/FastNoise.h"
//#include "inc/NoiseGrid/NoiseGrid.h"
//#include "inc/r/rCamera/rCamera.h"

#include "inc/r/raygui+/rlgui.h"

#define RUI_HEIGHT 56

struct UI2 
{
    void update();
    void draw();

    struct { float width, height; } window_size;

    RHbox hbox = {{&vbox1}, 
                  10, rl::ANCHOR_BOTTOM_LEFT, rl::FIT_PARENT_WIDTH, 
                  {0,0,0,RUI_HEIGHT*5}, {}};
    
    RVbox vbox1 = {{&header1,&toggle1,&toggle2,&toggle3, &toggle4},
                   4,rl::ANCHOR_TOP_LEFT,rl::FIT_NONE,
                   {0,0,0,RUI_HEIGHT*2},{4,4,4,4},&hbox};
    
    RLine header1 = {150, RUI_HEIGHT, "Options"};
    RToggle toggle1 = {120, RUI_HEIGHT, "Grid",  false, std::bind(&UI2::on_toggle_grid,  this)};
    RToggle toggle2 = {120, RUI_HEIGHT, "Noise", false, std::bind(&UI2::on_toggle_noise, this)};
    RToggle toggle3 = {120, RUI_HEIGHT, "View",  false, std::bind(&UI2::on_toggle_view,  this)};
    RToggle toggle4 = {120, RUI_HEIGHT, ". . .", false, std::bind(&UI2::on_toggle_tests, this)};

    void on_toggle_grid() {}
    void on_toggle_noise() {}
    void on_toggle_view() {}
    void on_toggle_tests() {}

//
    RHbox dim_layout = {{&chunk, &grid},
                        10,rl::ANCHOR_BOTTOM_LEFT, rl::FIT_PARENT_WIDTH,
                        {0,0,640,RUI_HEIGHT*5},{}};

//
    RVbox chunk = {{&chunk_header,&chunk_size},
                   10,rl::ANCHOR_TOP_LEFT,rl::FIT_NONE,
                   {0,0,200,RUI_HEIGHT*6},{4,4,0,4},&dim_layout};

    RLine chunk_header = {200,RUI_HEIGHT,"Chunk"};
    RDropdownBox chunk_size = {200,RUI_HEIGHT,"8;16;24;32;48;56;64"};

//
    RVbox grid = {{&grid_header,&grid_width,&grid_height,&grid_depth},
                  10,rl::ANCHOR_TOP_LEFT,rl::FIT_NONE,
                  {0,0,200,RUI_HEIGHT*5},{4,4,0,4},&dim_layout};

    RLine grid_header = {200,RUI_HEIGHT,"Grid"};
    RSpinner grid_width  = {200,RUI_HEIGHT,5,3,100};
    RSpinner grid_height = {200,RUI_HEIGHT,5,3,100};
    RSpinner grid_depth  = {200,RUI_HEIGHT,5,3,100};


};

#endif // UI2_H
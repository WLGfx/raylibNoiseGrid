#include "ui2.h"
#include "inc/r/raygui+/rlgui.h"
#include "raylib.h"

void UI2::update() {
    float screen_width = GetScreenWidth();
    float screen_height = GetScreenHeight();

    if (screen_width != window_size.width || screen_height != window_size.height) 
    {
        window_size.width = screen_width;
        window_size.height = screen_height;

        hbox.update();
        
        chunk.update();
        grid.update();
        dim_layout.update();
    }
}

void UI2::draw() {
    hbox.draw();
    //GuiDrawIcon(ICON_RESIZE, 110, 100, 4, RAYWHITE);
    dim_layout.draw();
    chunk.draw();
    grid.draw();
}

#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#ifndef GUI_MENU_H
#define GUI_MENU_H

struct GuiMenu {
    int active = 2;

    void draw()
    {
        float h = GetScreenHeight() - 60;

        GuiToggleGroup({16, h, 112, 48}, "Map;Noise;View;. .", &active);
    }
};

#endif // GUI_LAYOUT_NAME_H

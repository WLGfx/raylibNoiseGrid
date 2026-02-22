#ifndef RUI_H
#define RUI_H

#include "raylib.h"
#include "raygui.h"

#include <vector>

enum rAnchor {
    ANCHOR_NONE,
    ANCHOR_CENTER,
    ANCHOR_TOP_LEFT,
    ANCHOR_TOP_RIGHT,
    ANCHOR_BOTTOM_LEFT,
    ANCHOR_BOTTOM_RIGHT,
    ANCHOR_TOP_CENTER,
    ANCHOR_BOTTOM_CENTER,
    ANCHOR_LEFT_CENTER,
    ANCHOR_RIGHT_CENTER
};

enum rVFit {
    VFIT_NONE,
    VFIT_STRETCH
};

enum rHFit {
    HFIT_NONE,
    HFIT_STRETCH
};

struct rControl {
    Rectangle rect;
};

struct rContainer {
    Rectangle rect;
    rAnchor anchor;
};

struct VBox : public rContainer {
    std::vector<rControl*> controls;
    rVFit vfit;
    rHFit hfit;

};

#endif // RUI_H
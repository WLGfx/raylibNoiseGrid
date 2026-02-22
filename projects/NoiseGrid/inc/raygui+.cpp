#include "raygui+.h"


void rContainer::anchor_to() {
    Rectangle outer;// = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};

    if (!parent) outer = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
    else outer = parent->bounds;

    switch (anchor) {
        case ANCHOR_CENTER: {
            bounds.x = outer.x + (outer.width - bounds.width) / 2 - padding.x - padding.width;
            bounds.y = outer.y + (outer.height - bounds.height) / 2 - padding.y - padding.height;
        } break;

        case ANCHOR_TOP_LEFT: {
            bounds.x = outer.x + padding.x;
            bounds.y = outer.y;
        } break;

        case ANCHOR_TOP_RIGHT: {
            bounds.x = outer.width - bounds.width - padding.x - padding.width;
            bounds.y = outer.y;
        } break;

        case ANCHOR_BOTTOM_LEFT: {
            bounds.x = outer.x + padding.x;
            bounds.y = outer.height - bounds.height - padding.y - padding.height;
        } break;

        case ANCHOR_BOTTOM_RIGHT: {
            bounds.x = outer.width - bounds.width - padding.x - padding.width;
            bounds.y = outer.height - bounds.height - padding.y - padding.height;
        } break;

        case ANCHOR_TOP_CENTER: {
            bounds.x = outer.x + (outer.width - bounds.width) / 2 - padding.x - padding.width;
            bounds.y = outer.y;
        } break;

        case ANCHOR_BOTTOM_CENTER: {
            bounds.x = outer.x + (outer.width - bounds.width) / 2 - padding.x - padding.width;
            bounds.y = outer.height - bounds.height;
        } break;

        case ANCHOR_LEFT_CENTER: {
            bounds.x = outer.x + padding.x;
            bounds.y = outer.y + (outer.height - bounds.height) / 2;
        } break;

        case ANCHOR_RIGHT_CENTER: {
            bounds.x = outer.width - bounds.width - padding.x - padding.width;
            bounds.y = outer.y + (outer.height - bounds.height) / 2;
        } break;

        case ANCHOR_NONE:
        break;
    }
}

int rContainer::draw() {
    for (auto &child : *children) {
        child->draw();
    }
    return 0;
}

void rHBox::update() {
    int children_count = children->size();
    if (children_count == 0) return;

    anchor_to();
    
    float spacing = bounds.width / children_count;
    
    for (int i = 0; i < children_count; i++) {
        (*children)[i]->bounds = {
            bounds.x + i * spacing,
            bounds.y,
            spacing - padding.x,
            bounds.height - TEXT_PADDING
        };
    }
}

void rVBox::update() {
    int children_count = children->size();
    if (children_count == 0) return;

    anchor_to();
    
    float spacing = bounds.height / children_count;
    
    for (int i = 0; i < children_count; i++) {
        (*children)[i]->bounds = {
            bounds.x,
            bounds.y + i * spacing,
            bounds.width,
            spacing - TEXT_PADDING
        };
    }
}


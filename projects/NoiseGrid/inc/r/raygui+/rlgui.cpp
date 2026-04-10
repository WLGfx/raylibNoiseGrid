#include "rlgui.h"
#include "raylib.h"
//#include <iostream>


void rl::ContainerBase::_set_anchor() {
    Rectangle outer = {0 + padding.x, 
                       0 + padding.y, 
                       (float)GetScreenWidth() - padding.width - padding.x, 
                       (float)GetScreenHeight() - padding.height};

    if (parent != nullptr) 
        outer = {parent->bounds.x + parent->padding.x,
                 parent->bounds.y + parent->padding.y,
                 parent->bounds.width - parent->padding.x - parent->padding.width,
                 parent->bounds.height - parent->padding.y - parent->padding.height};
    
    if (adjust == FIT_PARENT_WIDTH || adjust == FIT_BOTH) bounds.width = outer.width;
    if (adjust == FIT_PARENT_HEIGHT || adjust == FIT_BOTH) bounds.height = outer.height;

    switch (ranchor) {

        case ANCHOR_CENTER: {
            //bounds.x = outer.x + (outer.width) / 2;
            //bounds.y = outer.y + (outer.height) / 2;
        } break;

        case ANCHOR_TOP_LEFT: { // This one confirmed works
            bounds.x = outer.x;
            bounds.y = outer.y;
        } break;

        case ANCHOR_BOTTOM_LEFT: {
            bounds.x = outer.x;
            bounds.y = outer.height - bounds.height;
        } break;

        case ANCHOR_TOP_RIGHT: {
            //bounds.x = outer.width;
            //bounds.y = outer.y;
        } break;

        case ANCHOR_BOTTOM_RIGHT: {
            //bounds.x = outer.width;
            //bounds.y = outer.height - bounds.height;
        } break;

        case ANCHOR_TOP_CENTER: {
            //bounds.x = outer.x + (outer.width) / 2;
            //bounds.y = outer.y;
        } break;

        case ANCHOR_BOTTOM_CENTER: {
            //bounds.x = outer.x + (outer.width) / 2;
            //bounds.y = outer.height;
        } break;

        case ANCHOR_LEFT_CENTER: {
            //bounds.x = outer.x;
            //bounds.y = outer.y + (outer.height) / 2;
        } break;

        case ANCHOR_RIGHT_CENTER: {
            //bounds.x = outer.width;
            //bounds.y = outer.y + (outer.height) / 2;
        } break;

        case ANCHOR_NONE: {
            //offset.x = bounds.x;
            //offset.y = bounds.y;
        } break;
    }
}

void rl::ContainerBase::draw() {
    //DrawRectangleRec({bounds.x, bounds.y, bounds.width, bounds.height}, (Color){0x88, 0x88, 0x88, 0x88});

    if (!enabled) GuiDisable();
    for (int index = children.size() - 1; index >= 0; index--) { children[index]->draw(); }
    if (!enabled) GuiEnable();
}

void rl::HBox::update() {
    // bounds width and height need to be checked to fit children
    _set_anchor();

    if (children.empty()) return;

    // if true children fit in parents bounds width, else find max width
    bool fit_parent_width = adjust == FIT_PARENT_WIDTH  || adjust == FIT_BOTH;
    float childs_width    = (bounds.width - padding.x - padding.width - (children.size() - 1) * spacing) / children.size();
    float curr_x          = bounds.x + padding.x;

    float total_child_width = 0;
    
    // if true then children use parents bounds height, else find max height
    bool fit_parent_height = (adjust == FIT_PARENT_HEIGHT || adjust == FIT_BOTH);
    float childs_height    = bounds.height - padding.y - padding.height;
    float curr_y           = bounds.y + padding.y;

    float max_child_height = 0;

    // set childs x and y, calculates max width and height
    for (Bounds *child : children) 
    {
        child->x = curr_x;
        curr_x += fit_parent_width ? childs_width + spacing : child->width + spacing;
        total_child_width += child->w + spacing;
        if (fit_parent_width) child->w = childs_width;

        if (child->height > max_child_height) max_child_height = child->height;
        child->y = curr_y;
    }

    if (!fit_parent_width) width = total_child_width;
    if (!fit_parent_height) height = max_child_height;
    
    // set childrens height if not fitting to parent. update each child
    for (Bounds *child : children) {
        if (fit_parent_height) child->h = max_child_height;
        child->update();
    }
}

void rl::VBox::update() {
    _set_anchor();
    
    if (children.empty()) return;

    // if true then fit in bounds width, else find max width
    bool fit_parent_width = adjust == FIT_PARENT_WIDTH || adjust == FIT_BOTH;
    float childs_width = (bounds.width - padding.x - padding.width);
    float curr_x = bounds.x + padding.x;

    float max_child_width = 0;

    // if true then fit in bounds height, else find max height
    bool fit_parent_height = adjust == FIT_PARENT_HEIGHT || adjust == FIT_BOTH;
    float childs_height = (bounds.height - padding.y - padding.height - (children.size() - 1) * spacing / children.size());
    float curr_y = bounds.y + padding.y;

    float total_child_height = 0;

    // set childs x and y, gets max width and height
    for (Bounds *child : children)
    {
        child->x = curr_x;
        if (child->width > max_child_width) max_child_width = child->width;

        child->y = curr_y;
        curr_y += fit_parent_height ? childs_height + spacing : child->height + spacing;
        total_child_height += child->height + spacing;
        if (fit_parent_height) child->height = childs_height;
    }

    if (!fit_parent_width) width = max_child_width;
    if (!fit_parent_height) height = total_child_height;

    for (Bounds *child : children) {
        if (fit_parent_width) child->width = max_child_width;
        child->update();
    }
}

/*
The A box and the F box both in the center, cos that's where they start right?
You see, each have an A or an F in them right? Both in the middle.
Like the A box, be careful, and the F box is a tricky one. Cos they are important with what you do with them.
Okay, you'r asking what's the A box and F box?
The A box is the one with the A in it. Be careful how you use the A box.
The F box is the one with the F in it. As I said this one is just tricky. Oof the F box.
Oh, I see, what they actually are? The A box and the F box. Because, ...
The A box is the one that goes across.
The F box is the one that falls.

*/

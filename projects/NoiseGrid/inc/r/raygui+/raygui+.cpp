#include "raygui+.h"
#include "raylib.h"
//#include <iostream>


void rContainerBase::_set_anchor() {
    Rectangle outer = {0 + padding.x, 
                       0 + padding.y, 
                       (float)GetScreenWidth() - padding.width - padding.x, 
                       (float)GetScreenHeight() - padding.height};

    if (parent != nullptr) 
        outer = {parent->bounds.x + parent->padding.x,
                 parent->bounds.y + parent->padding.y,
                 parent->bounds.width - parent->padding.x - parent->padding.width,
                 parent->bounds.height - parent->padding.y - parent->padding.height};
    
    if (adjust == FIT_WIDTH || adjust == FIT_BOTH) bounds.width = outer.width;
    if (adjust == FIT_HEIGHT || adjust == FIT_BOTH) bounds.height = outer.height;

    switch (ranchor) {

        case ANCHOR_CENTER: {
            bounds.x = outer.x + (outer.width) / 2;
            bounds.y = outer.y + (outer.height) / 2;
        } break;

        case ANCHOR_TOP_LEFT: { // This one confirmed works
            bounds.x = outer.x;
            bounds.y = outer.y;
        } break;

        case ANCHOR_TOP_RIGHT: {
            bounds.x = outer.width;
            bounds.y = outer.y;
        } break;

        case ANCHOR_BOTTOM_LEFT: {
            bounds.x = outer.x;
            bounds.y = outer.height;
        } break;

        case ANCHOR_BOTTOM_RIGHT: {
            bounds.x = outer.width;
            bounds.y = outer.height;
        } break;

        case ANCHOR_TOP_CENTER: {
            bounds.x = outer.x + (outer.width) / 2;
            bounds.y = outer.y;
        } break;

        case ANCHOR_BOTTOM_CENTER: {
            bounds.x = outer.x + (outer.width) / 2;
            bounds.y = outer.height;
        } break;

        case ANCHOR_LEFT_CENTER: {
            bounds.x = outer.x;
            bounds.y = outer.y + (outer.height) / 2;
        } break;

        case ANCHOR_RIGHT_CENTER: {
            bounds.x = outer.width;
            bounds.y = outer.y + (outer.height) / 2;
        } break;

        case ANCHOR_NONE: {
            //offset.x = bounds.x;
            //offset.y = bounds.y;
        } break;
    }
}

void rContainerBase::draw() {
    //DrawRectangleRec({bounds.x, bounds.y, bounds.width, bounds.height}, (Color){0x88, 0x88, 0x88, 0x88});

    if (!enabled) GuiDisable();
    for (int index = children.size() - 1; index >= 0; index--) { children[index]->draw(); }
    if (!enabled) GuiEnable();
}

void rHBox::update() {
    _set_anchor();
    if (children.empty()) return;
    
    float total_width = 0;
    float total_height = 0;
    
    float curr_x = bounds.x + padding.x;
    float curr_y = bounds.y + padding.y;
    float child_width = (bounds.width - padding.x - padding.width - (children.size() - 1) * spacing) / children.size();
    float child_height = bounds.height - padding.y - padding.height;
    
    for (rBounds* child : children) {
        
        if (adjust == FIT_WIDTH  || adjust == FIT_BOTH) {
            child->width  = child_width;
            total_width += child_width + spacing;
        }
        else {
            child->x = curr_x;
            curr_x += child->width + spacing;
        }
        
        if (adjust == FIT_HEIGHT || adjust == FIT_BOTH) {
            child->height = child_height;
            if (total_height < child->height) total_height = child->height;    
        } else {
            child->y = curr_y;
        }
        
        child->update();
    }
    
    if ((adjust == FIT_WIDTH || adjust == FIT_BOTH)) { bounds.width = total_width + padding.width + padding.x; }
    if ((adjust == FIT_HEIGHT || adjust == FIT_BOTH)) { bounds.height = total_height + padding.height + padding.y; }
    
    //for (rBounds* child : children) { child->update(); }
}

void rVBox::update() {
    _set_anchor();
    
    if (children.empty()) return;
    
    float max_width = 0, max_height = 0;
    
    if (adjust == FIT_WIDTH || adjust == FIT_HEIGHT || adjust == FIT_BOTH) { for (rBounds* child : children) {
        
            if (adjust == FIT_HEIGHT || adjust == FIT_BOTH && child->height > max_height) max_height = child->height;
            if (adjust == FIT_WIDTH  || adjust == FIT_BOTH && child->width  > max_width)  max_width  = child->width;
        }
    
        if (adjust == FIT_WIDTH  || adjust == FIT_BOTH) bounds.width  = max_width;
        if (adjust == FIT_HEIGHT || adjust == FIT_BOTH) bounds.height = max_height;
    }

    float curr_x = bounds.x + padding.x;
    float curr_y = bounds.y + padding.y;
    
    float childs_width  =  bounds.width  - padding.x - padding.width;
    float childs_height = (bounds.height - padding.y - padding.height - (children.size() - 1) * spacing) / children.size();

    for (rBounds *child : children) {
        
        child->x = curr_x;
        child->y = curr_y;
        
        if (adjust == FIT_WIDTH  || adjust == FIT_BOTH) child->width  = childs_width;
        if (adjust == FIT_HEIGHT || adjust == FIT_BOTH) child->height = childs_height;
        
        child->update();

        curr_y += childs_height + spacing;
    }
}

/* The draw loop in reverse order

for (int index = children.size() - 1; index >= 0; index--) // reverse order
{
    children[index]->draw();
}
*/
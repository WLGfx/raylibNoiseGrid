#include "raygui+.h"
#include "raylib.h"
#include <iostream>


void rContainerBase::anchor_to() {
    Rectangle outer = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
    if (parent != nullptr) outer = parent->bounds;

    switch (anchor) {
        case ANCHOR_CENTER: {
            x_offset = outer.x + (outer.width - bounds.width) / 2 - padding.x - padding.width;
            y_offset = outer.y + (outer.height - bounds.height) / 2 - padding.y - padding.height;
        } break;
        case ANCHOR_TOP_LEFT: { // This one confirmed works
            x_offset = outer.x + bounds.x;
            y_offset = outer.y + bounds.y;
        } break;
        case ANCHOR_TOP_RIGHT: {
            x_offset = outer.width - bounds.width - padding.x - padding.width;
            y_offset = outer.y + padding.y;
        } break;
        case ANCHOR_BOTTOM_LEFT: {
            x_offset = outer.x + padding.x;
            y_offset = outer.height - bounds.height - padding.y - padding.height;
        } break;
        case ANCHOR_BOTTOM_RIGHT: {
            x_offset = outer.width - bounds.width - padding.x - padding.width;
            y_offset = outer.height - bounds.height - padding.y - padding.height;
        } break;
        case ANCHOR_TOP_CENTER: {
            x_offset = outer.x + (outer.width - bounds.width) / 2 - padding.x - padding.width;
            y_offset = outer.y;
        } break;
        case ANCHOR_BOTTOM_CENTER: {
            x_offset = outer.x + (outer.width - bounds.width) / 2 - padding.x - padding.width;
            y_offset = outer.height - bounds.height;
        } break;
        case ANCHOR_LEFT_CENTER: {
            x_offset = outer.x + padding.x;
            y_offset = outer.y + (outer.height - bounds.height) / 2;
        } break;
        case ANCHOR_RIGHT_CENTER: {
            x_offset = outer.width - bounds.width - padding.x - padding.width;
            y_offset = outer.y + (outer.height - bounds.height) / 2;
        } break;
        case ANCHOR_NONE:
        break;
    }

    std::cout << "Anchor to: " << left << ", " << top;
}

void rContainerBase::draw() {
    DrawRectangleRec({x_offset+1, y_offset+1, bounds.width-2, bounds.height-2}, (Color){0x88, 0x88, 0x88, 0x88});
    for (int index = children.size() - 1; index >= 0; index--) {
        children[index]->draw();
    }
}

void rHBox::update() {
    anchor_to();

    int children_count = children.size();
    
    float xpos = x_offset + padding.x;
    float ypos = y_offset + padding.y;
    float total_child_width = bounds.width - padding.x - padding.width;
    float child_width_each = (total_child_width - spacing * (children_count - 1)) / children_count;
    float max_child_height = 0;

    if (children_count > 0) {
        std::cout << " ... HBox: " << children_count << " children" << std::endl;
        
        for (rBounds* child : children) 
        {   
            child->left = xpos;
            child->top = ypos;

            if (adjust == FIT_WIDTH || adjust == FIT_BOTH) {
                child->width = child_width_each - spacing;
                xpos += child_width_each + spacing;
            }
            else xpos += child->width + spacing;

            if (adjust == FIT_HEIGHT || adjust == FIT_BOTH) {
                child->height = bounds.height - padding.height - padding.y;
            }

            if (child->height > max_child_height) {
                max_child_height = child->height;
            }
        }
    }
    
    if (!(adjust == FIT_WIDTH || adjust == FIT_BOTH))  { bounds.width = xpos - bounds.x; }
    if (!(adjust == FIT_HEIGHT || adjust == FIT_BOTH)) { bounds.height = max_child_height; }
}

void rVBox::update() {
    anchor_to();

    int children_count = children.size();

    float xpos = x_offset + padding.x;
    float ypos = y_offset + padding.y;
    float total_child_height = bounds.height - padding.y - padding.height;
    float child_height_each = (total_child_height - spacing * (children_count - 1)) / children_count;
    float max_child_width = 0;

    if (children_count > 0) { // currently uses width and height of children. TODO: fit to containers bounds 
        for (rBounds* child : children) 
        {
            child->left = xpos;
            child->top = ypos;

            if (adjust == FIT_HEIGHT || adjust == FIT_BOTH) {
                child->height = child_height_each - spacing;
                ypos += child_height_each + spacing;
            } 
            else ypos += child->height + spacing;

            if (adjust == FIT_WIDTH || adjust == FIT_BOTH) {
                child->width = bounds.width - padding.width - padding.x;
            }

            if (child->width > max_child_width) {
                max_child_width = child->width;
            }
        }
    }

    if (!(adjust == FIT_WIDTH || adjust == FIT_BOTH))  { bounds.width = max_child_width; }
    if (!(adjust == FIT_HEIGHT || adjust == FIT_BOTH)) { bounds.height = ypos - bounds.y; }
}


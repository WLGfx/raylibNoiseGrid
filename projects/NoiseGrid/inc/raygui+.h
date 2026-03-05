#ifndef RAYGUI_PLUS_H
#define RAYGUI_PLUS_H

#include <cstdio>
#include <functional>
#include <vector>
#include "raygui.h"

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

struct rBounds 
{
    Rectangle bounds;
    //rBounds *parent;
    //rAnchor anchor;
    
    rBounds() = default;
    
    rBounds(Rectangle bounds) 
    : bounds(bounds) {}
    
    virtual int draw() { return 0; }
    
    Vector2 get_position() { return {bounds.x, bounds.y}; }
    Vector2 get_size() { return {bounds.width, bounds.height}; }
    void set_position(Vector2 pos) { bounds.x = pos.x; bounds.y = pos.y; }
    void set_size(Vector2 size) { bounds.width = size.x; bounds.height = size.y; }
};

///////////////////////////////////////////////////////////////////////////

//                                                               CONTAINERS

///////////////////////////////////////////////////////////////////////////

struct rContainer : public rBounds 
{
    rContainer() = default;

    rContainer(Rectangle bounds, Rectangle& padding, rAnchor anchor = ANCHOR_NONE, std::vector<rBounds*> *children = nullptr, rContainer *parent = nullptr) 
    : rBounds(bounds), padding(padding), anchor(anchor), children(children), parent(parent) {}

    Rectangle   padding;
    rAnchor anchor;
    std::vector<rBounds*> *children;
    rContainer *parent;

    void add_child(rBounds *child) { children->push_back(child); }
    void anchor_to();
    
    virtual void update() { anchor_to(); }
    virtual int draw() override; 
};

struct rHBox : public rContainer 
{
    rHBox() = default;

    rHBox(Rectangle bounds, Rectangle& padding, rAnchor anchor = ANCHOR_NONE, std::vector<rBounds*> *children = nullptr, rContainer *parent = nullptr) 
    : rContainer(bounds, padding, anchor, children, parent) {}

    virtual void update() override;
};

///////////////////////////////////////////////////////////////////////////

//                                                                      NEW

///////////////////////////////////////////////////////////////////////////

struct rVBox : public rContainer 
{
    rVBox() = default;

    rVBox(Rectangle bounds, Rectangle& padding, rAnchor anchor = ANCHOR_NONE, std::vector<rBounds*> *children = nullptr, rContainer *parent = nullptr) 
    : rContainer(bounds, padding, anchor, children, parent) {}

    virtual void update() override;
};

struct rVLayout {

};




///////////////////////////////////////////////////////////////////////////

//                                                                  WIDGETS

///////////////////////////////////////////////////////////////////////////

struct rWindowBox : public rBounds {
    const char *title;

    rWindowBox() = default;
    
    rWindowBox(Rectangle bounds, const char *title) 
    : rBounds(bounds), title(title) {}

    int draw() { return GuiWindowBox(bounds, title); }
};

struct rGroupBox : public rBounds {
    rGroupBox() = default;
    
    rGroupBox(Rectangle bounds, const char *text) 
    : rBounds(bounds), text(text) {}

    const char *text;
    int draw() { return GuiGroupBox(bounds, text); }
};

struct rLine : public rBounds {
    rLine() = default;
    
    rLine(Rectangle bounds, const char *text) 
    : rBounds(bounds), text(text) {}

    const char *text;
    int draw() { return GuiLine(bounds, text); }
};

struct rPanel : public rBounds {
    rPanel() = default;
    
    rPanel(Rectangle bounds, const char *text) 
    : rBounds(bounds), text(text) {}

    const char *text;
    int draw() { return GuiPanel(bounds, text); }
};

struct rTabBar {
    rTabBar() = default;
    rTabBar(Rectangle bounds, const char **tabs, int count, int active) 
    : bounds(bounds), tabs(tabs), count(count), active(active) {}

    Rectangle bounds;
    const char **tabs;
    int count;
    int active;

    int draw() { return GuiTabBar(bounds, tabs, count, &active); }
};

struct rScrollPanel : public rBounds {
    rScrollPanel() = default;
    rScrollPanel(Rectangle bounds, const char *text, Rectangle content, Vector2 *scroll, Rectangle *view) 
    : rBounds(bounds), content(content), scroll(scroll), view(view) {}
    
    Rectangle content;
    Vector2 *scroll;
    Rectangle *view;
};


///////////////////////////////////////////////////////////////////////////

//                                                                 CONTROLS

///////////////////////////////////////////////////////////////////////////

struct rLabel : public rBounds {
    rLabel() = default;
    
    rLabel(Rectangle bounds, const char *text) 
    : rBounds(bounds), text(text) {}

    const char *text;
    int draw() { return GuiLabel(bounds, text); }
};

struct rButton : public rBounds {
    rButton() = default;
    rButton(Rectangle bounds, const char *text, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), text(text), on_click(on_click) {}

    const char *text;
    std::function<void()> on_click;

    int draw() { int result = GuiButton(bounds, text); if (result && on_click) on_click(); return result; }
};

struct RLabelButton : public rBounds {
    RLabelButton() = default;
    
    RLabelButton(Rectangle bounds, const char *text) 
    : rBounds(bounds), text(text) {}

    const char *text;
    int draw() { return GuiLabelButton(bounds, text); }
};

struct rToggle : public rBounds {
    rToggle() = default;
    rToggle(Rectangle bounds, const char *text, bool active = false, std::function<void()> on_click = nullptr) 
        : rBounds(bounds), text(text), active(active), on_click(on_click) {}
    
    const char *text;
    bool active;
    std::function<void()> on_click;

    int draw() 
    { 
        int result = GuiToggle(bounds, text, &active); 
        if (result && on_click) on_click(); 
        return result; 
    }
};

struct rToggleGroup : public rBounds {
    rToggleGroup() = default;
    
    rToggleGroup(Rectangle bounds, const char *text, int active) 
    : rBounds(bounds), text(text), active(active) {}
    
    const char *text;
    int active;

    int draw() { return GuiToggleGroup(bounds, text, &active); }
};

struct rToggleSlider : public rBounds {
    rToggleSlider() = default;
    
    rToggleSlider(Rectangle bounds, const char *text, int active) 
    : rBounds(bounds), text(text), active(active) {}
    
    const char *text;
    int active;
};

struct rCheckBox : public rBounds {
    rCheckBox() = default;
    
    rCheckBox(Rectangle bounds, const char *text, bool active, bool checked)
    : rBounds(bounds), text(text), active(active), checked(checked) {}
    
    const char *text;
    bool active;
    bool checked;

    int draw() { return GuiCheckBox(bounds, text, &checked); }
};

struct rComboBox : public rBounds {
    rComboBox() = default;
    
    rComboBox(Rectangle bounds, const char *text, int active) 
    : rBounds(bounds), text(text), active(active) {}
    
    const char *text;
    int active;
    
    int draw() { 
        int result = GuiComboBox(bounds, text, &active);
        if (result) {
            // do something when selected
        }
        return result;
    }
};

struct rDropdownBox : public rBounds {
    rDropdownBox() = default;
    
    rDropdownBox(Rectangle bounds, const char *text, int active, bool editMode = false, std::function<void()> on_click = nullptr)
    : rBounds(bounds), text(text), active(active), editMode(editMode), on_click(on_click) {}
    
    const char *text;
    int active;
    bool editMode;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiDropdownBox(bounds, text, &active, editMode);
        if (result) {
            editMode = !editMode;
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

struct rSpinner : public rBounds {
    rSpinner() = default;
    
    rSpinner(Rectangle bounds, const char *text, int value, int minValue, int maxValue, bool editMode = false, std::function<void()> on_click = nullptr)
    : rBounds(bounds), text(text), value(value), minValue(minValue), maxValue(maxValue), editMode(editMode), on_click(on_click) {}
    
    const char *text;
    int value;
    int minValue;
    int maxValue;
    bool editMode;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiSpinner(bounds, text, &value, minValue, maxValue, editMode);
        if (on_click) on_click();
        if (result) {
            editMode = !editMode;
        }
        return result;
    }
};

struct rValueBox : public rBounds {
    rValueBox() = default;
    
    rValueBox(Rectangle bounds, const char *text, int value, int minValue, int maxValue, bool editMode = false, std::function<void()> on_click = nullptr)
    : rBounds(bounds), text(text), value(value), minValue(minValue), maxValue(maxValue), editMode(editMode), on_click(on_click) {}
    
    const char *text;
    int value;
    int minValue;
    int maxValue;
    bool editMode;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiValueBox(bounds, text, &value, minValue, maxValue, editMode);
        if (result) {
            editMode = !editMode;
            if (on_click) {
                on_click();
            }
        }
        return result;
    }

    //void write(float value) {
    //    this->value = value;
    //    snprintf(textValue, sizeof(textValue), "%.2f", value);
    //}
};

struct rValueBoxFloat : public rBounds {
    rValueBoxFloat() = default;
    
    rValueBoxFloat(Rectangle bounds, const char *text, float value, bool editMode = false, std::function<void()> on_click = nullptr)
    : rBounds(bounds), text(text), value(value), editMode(editMode), on_click(on_click) { write(); }

    const char *text;
    float value;
    char textValue[16];
    bool editMode;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiValueBoxFloat(bounds, text, textValue, &value, editMode);
        if (result) {
            editMode = !editMode;
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
    void write() {
        snprintf(textValue, sizeof(textValue), "%.5f", value);
    }
    void write(float newValue) {
        value = newValue;
        write();
    }
};

struct rTextBox : public rBounds {
    rTextBox() = default;
    rTextBox(Rectangle bounds, char *text, int textSize, bool editMode, std::function<void()> on_click = nullptr)
    : rBounds(bounds), text(text), textSize(textSize), editMode(editMode), on_click(on_click) {}
    
    char *text;
    int textSize;
    bool editMode;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiTextBox(bounds, text, textSize, editMode);
        if (result) {
            editMode = !editMode;
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

struct rSlider : public rBounds {
    rSlider() = default;
    
    rSlider(Rectangle bounds, const char *text, const char *right, float value, float min, float max, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), text(text), right(right), value(value), min(min), max(max), on_click(on_click) {}
    
    const char *text;
    const char *right;
    float value;
    float min;
    float max;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiSlider(bounds, text, right, &value, min, max);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

struct rSliderBar : public rBounds {
    rSliderBar() = default;
    rSliderBar(Rectangle bounds, const char *text, const char *right, float value, float min, float max, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), text(text), right(right), value(value), min(min), max(max), on_click(on_click) {}
    
    const char *text;
    const char *right;
    float value;
    float min;
    float max;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiSliderBar(bounds, text, right, &value, min, max);
        if (result) {
            if (on_click) on_click();
        }
        return result;
    }
};

struct rProgressBar : public rBounds {
    rProgressBar() = default;
    rProgressBar(Rectangle bounds, const char *text, const char *right, float value, float min, float max, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), text(text), right(right), value(value), min(min), max(max), on_click(on_click) {}
    
    const char *text;
    const char *right;
    float value;
    float min;
    float max;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiProgressBar(bounds, text, right, &value, min, max);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

struct rStatusBar : public rBounds {
    rStatusBar() = default;
    
    rStatusBar(Rectangle bounds, const char *text) 
    : rBounds(bounds), text(text) {}
    
    const char *text;
};

struct rDummyRect : public rBounds {
    rDummyRect() = default;
    
    rDummyRect(Rectangle bounds, const char *text) 
    : rBounds(bounds), text(text) {}
    
    const char *text;

    int draw() { return GuiDummyRec(bounds, text); }
};

struct rGrid : public rBounds {
    rGrid() = default;
    
    rGrid(Rectangle bounds, const char *text, float spacing, int subdivs, Vector2 mouseCell, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), text(text), spacing(spacing), subdivs(subdivs), mouseCell(mouseCell), on_click(on_click) {}
    
    const char *text;
    float spacing;
    int subdivs;
    Vector2 mouseCell;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiGrid(bounds, text, spacing, subdivs, &mouseCell);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

struct rListView : public rBounds {
    rListView() = default;
    
    rListView(Rectangle bounds, const char *text, int scrollIndex, int active, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), text(text), scrollIndex(scrollIndex), active(active), on_click(on_click) {}
    
    const char *text;
    int scrollIndex;
    int active;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiListView(bounds, text, &scrollIndex, &active);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

struct rListViewEx {
    rListViewEx() = default;
    
    rListViewEx(Rectangle bounds, const char **text, int count, int scrollIndex, int active, int focus, std::function<void()> on_click = nullptr) 
    : bounds(bounds),  text(text), count(count), scrollIndex(scrollIndex), active(active), focus(focus), on_click(on_click) {}
    
    Rectangle bounds;
    const char **text;
    int count;
    int scrollIndex;
    int active;
    int focus;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiListViewEx(bounds, text, count, &scrollIndex, &active, &focus);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};


///////////////////////////////////////////////////////////////////////////

//                                                                   POPUPS

///////////////////////////////////////////////////////////////////////////

struct rMessageBox {
    rMessageBox() = default;
    
    rMessageBox(Rectangle bounds, const char *title, const char *message, const char *buttons, std::function<void()> on_click = nullptr) 
    : bounds(bounds), title(title), message(message), buttons(buttons), on_click(on_click) {}
    
    Rectangle bounds;
    const char *title;
    const char *message;
    const char *buttons;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiMessageBox(bounds, title, message, buttons);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

struct rTextInputBox {
    rTextInputBox() = default;
    rTextInputBox(Rectangle bounds, const char *title, const char *message, const char *buttons, char *text, int textMaxSize, bool secretViewActive, std::function<void()> on_click = nullptr) 
    : bounds(bounds), title(title), message(message), buttons(buttons), text(text), textMaxSize(textMaxSize), secretViewActive(secretViewActive), on_click(on_click) {}
    
    Rectangle bounds;
    const char *title;
    const char *message;
    const char *buttons;
    char *text;
    int textMaxSize;
    bool secretViewActive;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiTextInputBox(bounds, title, message, buttons, text, textMaxSize, &secretViewActive);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

struct rColorPicker {
    rColorPicker() = default;
    
    rColorPicker(Rectangle bounds, const char *text, Color color, std::function<void()> on_click = nullptr) 
    : bounds(bounds), text(text), color(color), on_click(on_click) {}
    
    Rectangle bounds;
    const char *text;
    Color color;
    std::function<void()> on_click;

    int draw() { 
        int result = GuiColorPicker(bounds, text, &color);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

struct rColorPanel {
    rColorPanel() = default;
    
    rColorPanel(Rectangle bounds, const char *text, Color color, std::function<void()> on_click = nullptr) 
    : bounds(bounds), text(text), color(color), on_click(on_click) {}
    
    Rectangle bounds;
    const char *text;
    Color color;
    std::function<void()> on_click;

    int draw() { 
        int result = GuiColorPicker(bounds, text, &color);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

struct rColorBarAlpha {
    rColorBarAlpha() = default;
    
    rColorBarAlpha(Rectangle bounds, const char *text, float alpha, std::function<void()> on_click = nullptr) 
    : bounds(bounds), text(text), alpha(alpha), on_click(on_click) {}
    
    Rectangle bounds;
    const char *text;
    float alpha;
    std::function<void()> on_click;

    int draw() { 
        int result = GuiColorBarAlpha(bounds, text, &alpha);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

struct rColorBarHue {
    rColorBarHue() = default;
    
    rColorBarHue(Rectangle bounds, const char *text, float value, std::function<void()> on_click = nullptr) 
    : bounds(bounds), text(text), value(value), on_click(on_click) {}
    
    Rectangle bounds;
    const char *text;
    float value;
    std::function<void()> on_click;

    int draw() { 
        int result = GuiColorBarHue(bounds, text, &value);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

struct rColorPickerHSV {
    rColorPickerHSV() = default;
    rColorPickerHSV(Rectangle bounds, const char *text, Vector3 colorHsv, std::function<void()> on_click = nullptr) 
    : bounds(bounds), text(text), colorHsv(colorHsv), on_click(on_click) {}
    
    Rectangle bounds;
    const char *text;
    Vector3 colorHsv;
    std::function<void()> on_click;

    int draw() { 
        int result = GuiColorPickerHSV(bounds, text, &colorHsv);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

struct rColorPanelHSV {
    rColorPanelHSV() = default;
    
    rColorPanelHSV(Rectangle bounds, const char *text, Vector3 colorHsv, std::function<void()> on_click = nullptr) 
    : bounds(bounds), text(text), colorHsv(colorHsv), on_click(on_click) {}
    
    Rectangle bounds;
    const char *text;
    Vector3 colorHsv;
    std::function<void()> on_click;

    int draw() { 
        int result = GuiColorPanelHSV(bounds, text, &colorHsv);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

#endif
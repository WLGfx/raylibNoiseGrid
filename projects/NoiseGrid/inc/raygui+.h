#ifndef RAYGUI_PLUS_H
#define RAYGUI_PLUS_H

// 0800 077 8360 HSBC Finincial support team mon-fri -6pm GMT
// 5 may 26 expires breathing space

#include <cstdio>
#include <functional>
#include <vector>
#include "raygui.h"
#include "raylib.h"

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
    rBounds() = default;
    rBounds(Rectangle bounds) : bounds(bounds) {}
    union {
        Rectangle      bounds;
        struct { float x = 0, y = 0, width = 0, height = 0; };
        struct { float left, top, right, bottom; };
    };
    int result = 0;
    virtual void draw() { result = 0; }
    virtual void update() {};
};

///////////////////////////////////////////////////////////////////////////

//                                                               CONTAINERS

///////////////////////////////////////////////////////////////////////////

enum rContainerAdjust {
    FIT_NONE,
    FIT_WIDTH,
    FIT_HEIGHT,
    FIT_BOTH
};

struct rContainerBase : public rBounds {
    rContainerBase() = default;
    rContainerBase(Rectangle              bounds, 
                   Rectangle              padding, 
                   std::vector<rBounds*>  children, 
                   rAnchor                anchor = ANCHOR_NONE,
                   rContainerAdjust       adjust = FIT_NONE) 
        : rBounds(bounds),  padding(padding), children(children), anchor(anchor), adjust(adjust) {}
    std::vector<rBounds*>  children;
    rAnchor                anchor;
    Rectangle              padding;
    rContainerBase        *parent = nullptr;
    rContainerAdjust       adjust;
    float x_offset = 0, y_offset = 0;
    virtual void  draw() override; 
    virtual void update() override { anchor_to(); };
            void anchor_to();
};

struct rHBox : public rContainerBase {
    rHBox() = default;
    rHBox(Rectangle              bounds, 
          Rectangle              padding, 
          std::vector<rBounds*>  children, 
          float                  spacing = 0, 
          rAnchor                anchor = ANCHOR_NONE, 
          rContainerAdjust       adjust = FIT_NONE) 
        : rContainerBase(bounds, padding, children, anchor, adjust), spacing(spacing) {}
    float spacing = 0;
    virtual void update() override;
};

struct rVBox : public rContainerBase {
    rVBox() = default;
    rVBox(Rectangle              bounds, 
          Rectangle              padding, 
          std::vector<rBounds*>  children, 
          float                  spacing = 0, 
          rAnchor                anchor = ANCHOR_NONE, 
          rContainerAdjust       adjust = FIT_NONE) 
        : rContainerBase(bounds, padding, children, anchor, adjust), spacing(spacing) {}
    float spacing = 0;
    virtual void update() override;
};

///////////////////////////////////////////////////////////////////////////

//                                                                      NEW

///////////////////////////////////////////////////////////////////////////

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

    void draw() override { result = GuiWindowBox(bounds, title); }
};

struct rGroupBox : public rBounds {
    rGroupBox() = default;
    
    rGroupBox(Rectangle bounds, const char *text) 
    : rBounds(bounds), text(text) {}

    const char *text;
    void draw() override { result = GuiGroupBox(bounds, text); }
};

struct rLine : public rBounds {
    rLine() = default;
    rLine(const char *text) : text(text) {}
    rLine(Rectangle bounds, const char *text) 
    : rBounds(bounds), text(text) {}

    const char *text;
    void draw() override { result = GuiLine(bounds, text); }
};

struct rPanel : public rBounds {
    rPanel() = default;
    
    rPanel(Rectangle bounds, const char *text) 
    : rBounds(bounds), text(text) {}

    const char *text;
    void draw() { result = GuiPanel(bounds, text); }
};

struct rTabBar : public rBounds {
    rTabBar() = default;
    rTabBar(Rectangle bounds, const char **tabs, int count, int active) : rBounds(bounds), tabs(tabs), count(count), active(active) {}

    const char **tabs;
    int count;
    int active;

    void draw() override { result = GuiTabBar(bounds, tabs, count, &active); }
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
    void draw() override { result = GuiLabel(bounds, text); }
};

struct rButton : public rBounds {
    rButton() = default;
    rButton(Rectangle bounds, const char *text, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), text(text), on_click(on_click) {}

    const char *text;
    std::function<void()> on_click;

    void draw() override { result = GuiButton(bounds, text); if (result && on_click) on_click(); }
};

struct RLabelButton : public rBounds {
    RLabelButton() = default;
    
    RLabelButton(Rectangle bounds, const char *text = nullptr) 
    : rBounds(bounds), text(text) {}

    const char *text = nullptr;
    void draw() override { result = GuiLabelButton(bounds, text); }
};

struct rToggle : public rBounds {
    rToggle() = default;
    rToggle(std::function<void()> on_click) : on_click(on_click) {}
    rToggle(Rectangle bounds, const char *text = "", bool active = false, std::function<void()> on_click = nullptr) : rBounds(bounds), text(text), active(active), on_click(on_click) {}
    
    const char *text = nullptr;
    bool active = false;
    std::function<void()> on_click = nullptr;

    void draw() override 
    { 
        result = GuiToggle(bounds, text, &active); // MOD made to raygui source for callbacks
        if (result && on_click) on_click(); 
    }
};

struct rToggleGroup : public rBounds {
    rToggleGroup() = default;
    
    rToggleGroup(Rectangle bounds, const char *text, int active) 
    : rBounds(bounds), text(text), active(active) {}
    
    const char *text;
    int active;

    void draw() override { result = GuiToggleGroup(bounds, text, &active); }
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

    void draw() override { result = GuiCheckBox(bounds, text, &checked); }
};

struct rComboBox : public rBounds {
    rComboBox() = default;
    
    rComboBox(Rectangle bounds, const char *text, int active) 
    : rBounds(bounds), text(text), active(active) {}
    
    const char *text;
    int active;
    
    void draw() override { 
        result = GuiComboBox(bounds, text, &active);
        if (result) {
            // do something when selected
        }
    }
};

struct rDropdownBox : public rBounds {
    rDropdownBox() = default;
    rDropdownBox(const char* text, int active = 0, std::function<void()> on_click = nullptr)
        : rBounds({0, 0, 200, 30}), text(text), active(active), editMode(false), on_click(on_click) {}
    rDropdownBox(Rectangle bounds, const char *text, int active, bool editMode = false, std::function<void()> on_click = nullptr)
        : rBounds(bounds), text(text), active(active), editMode(editMode), on_click(on_click) {}
    
    const char *text;
    int active;
    bool editMode;
    std::function<void()> on_click;
    
    void draw() override { 
        result = GuiDropdownBox(bounds, text, &active, editMode);
        if (result) {
            editMode = !editMode;
            if (on_click) {
                on_click();
            }
        }
    }
};

struct rSpinner : public rBounds {
    rSpinner() = default;
    rSpinner(int value, int min, int max, std::function<void()> on_click = nullptr)
        : rBounds({0, 0, 100, 30}), value(value), minValue(min), maxValue(max), on_click(on_click) {}
    rSpinner(Rectangle bounds, const char *text, int value, int minValue, int maxValue, bool editMode = false, std::function<void()> on_click = nullptr)
        : rBounds(bounds), text(text), value(value), minValue(minValue), maxValue(maxValue), editMode(editMode), on_click(on_click) {}
    
    const char *text = nullptr;
    int value;
    int minValue;
    int maxValue;
    bool editMode;
    std::function<void()> on_click;
    
    void draw() override { 
        result = GuiSpinner(bounds, text, &value, minValue, maxValue, editMode);
        if (result) {
            if (on_click) on_click();
            editMode = !editMode;
        }
    }
};

struct rValueBox : public rBounds {
    rValueBox() = default;
    rValueBox(int value, int min, int max, const char * text = nullptr, std::function<void()> on_click = nullptr) 
        : value(value), minValue(min), maxValue(max), text(text), on_click(on_click) {}
    rValueBox(Rectangle bounds, const char *text, int value, int minValue, int maxValue, bool editMode = false, std::function<void()> on_click = nullptr)
        : rBounds(bounds), text(text), value(value), minValue(minValue), maxValue(maxValue), editMode(editMode), on_click(on_click) {}
    
    const char *text;
    int value;
    int minValue;
    int maxValue;
    bool editMode = false;
    std::function<void()> on_click;
    
    void draw() override { 
        result = GuiValueBox(bounds, text, &value, minValue, maxValue, editMode);
        if (result) {
            if (on_click) on_click();
            editMode = !editMode;
        }
    }
};

struct rValueBoxFloat : public rBounds {
    rValueBoxFloat() = default;
    rValueBoxFloat(float value, const char * text = nullptr, std::function<void()> on_click = nullptr) 
        : value(value), text(text), on_click(on_click) { write(); }
    rValueBoxFloat(Rectangle bounds, const char *text, float value, bool editMode = false, std::function<void()> on_click = nullptr)
    : rBounds(bounds), text(text), value(value), editMode(editMode), on_click(on_click) { write(); }

    const char *text;
    float value;
    char textValue[16];
    bool editMode = false;
    std::function<void()> on_click;
    
    void draw() override { 
        result = GuiValueBoxFloat(bounds, text, textValue, &value, editMode);
        if (result) {
            if (on_click) on_click();
            editMode = !editMode;
        }
    }
    void write() {
        snprintf(textValue, sizeof(textValue), "%.6f", value);
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
    
    void draw() override { 
        result = GuiTextBox(bounds, text, textSize, editMode);
        if (result) {
            editMode = !editMode;
            if (on_click) {
                on_click();
            }
        }
    }
};

struct rSlider : public rBounds {
    rSlider() = default;
    rSlider(float value, float min, float max, std::function<void()> on_click = nullptr, const char *text = nullptr)
        : value(value), min(min), max(max), on_click(on_click), text(text) {}
    rSlider(Rectangle bounds, const char *text, const char *right, float value, float min, float max, std::function<void()> on_click = nullptr) 
        : rBounds(bounds), text(text), right(right), value(value), min(min), max(max), on_click(on_click) {}
    
    const char *text = nullptr;
    const char *right = nullptr;
    float value;
    float min;
    float max;
    std::function<void()> on_click = nullptr;
    
    void draw() override { 
        result = GuiSlider(bounds, text, right, &value, min, max);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
    }
};

struct rSliderBar : public rBounds {
    rSliderBar() = default;
    rSliderBar(float value, float min, float max, std::function<void()> on_click = nullptr, const char *text = nullptr)
        : value(value), min(min), max(max), on_click(on_click), text(text) {}
    rSliderBar(Rectangle bounds, const char *text, const char *right, float value, float min, float max, std::function<void()> on_click = nullptr) 
        : rBounds(bounds), text(text), right(right), value(value), min(min), max(max), on_click(on_click) {}
    
    const char *text = nullptr;
    const char *right = nullptr;
    float value;
    float min;
    float max;
    std::function<void()> on_click;
    
    void draw() override { 
        result = GuiSliderBar(bounds, text, right, &value, min, max);
        if (result && on_click) on_click();
    }
};

struct rProgressBar : public rBounds {
    rProgressBar() = default;
    rProgressBar(Rectangle bounds, const char *text, const char *right, float value, float min, float max, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), text(text), right(right), value(value), min(min), max(max), on_click(on_click) {}
    
    const char *text = nullptr;
    const char *right = nullptr;
    float value;
    float min;
    float max;
    std::function<void()> on_click;
    
    void draw() override { 
        result = GuiProgressBar(bounds, text, right, &value, min, max);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
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

    void draw() override { GuiDummyRec(bounds, text); }
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
    
    void draw() override { 
        result = GuiGrid(bounds, text, spacing, subdivs, &mouseCell);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
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
    
    void draw() override { 
        result = GuiListView(bounds, text, &scrollIndex, &active);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
    }
};

struct rListViewEx : public rBounds {
    rListViewEx() = default;
    
    rListViewEx(Rectangle bounds, const char **text, int count, int scrollIndex, int active, int focus, std::function<void()> on_click = nullptr) 
    : rBounds(bounds),  text(text), count(count), scrollIndex(scrollIndex), active(active), focus(focus), on_click(on_click) {}
    
    const char **text;
    int count;
    int scrollIndex;
    int active;
    int focus;
    std::function<void()> on_click;
    
    void draw() override { 
        result = GuiListViewEx(bounds, text, count, &scrollIndex, &active, &focus);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
    }
};


///////////////////////////////////////////////////////////////////////////

//                                                                   POPUPS

///////////////////////////////////////////////////////////////////////////

struct rMessageBox : public rBounds {
    rMessageBox() = default;
    
    rMessageBox(Rectangle bounds, const char *title, const char *message, const char *buttons, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), title(title), message(message), buttons(buttons), on_click(on_click) {}
    
    //Rectangle bounds;
    const char *title;
    const char *message;
    const char *buttons;
    std::function<void()> on_click;
    
    void draw() { 
        result = GuiMessageBox(bounds, title, message, buttons);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
    }
};

struct rTextInputBox : public rBounds   {
    rTextInputBox() = default;
    rTextInputBox(Rectangle bounds, const char *title, const char *message, const char *buttons, char *text, int textMaxSize, bool secretViewActive, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), title(title), message(message), buttons(buttons), text(text), textMaxSize(textMaxSize), secretViewActive(secretViewActive), on_click(on_click) {}
    
    //Rectangle bounds;
    const char *title;
    const char *message;
    const char *buttons;
    char *text;
    int textMaxSize;
    bool secretViewActive;
    std::function<void()> on_click;
    
    void draw() { 
        result = GuiTextInputBox(bounds, title, message, buttons, text, textMaxSize, &secretViewActive);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
    }
};

struct rColorPicker : public rBounds {
    rColorPicker() = default;
    
    rColorPicker(Rectangle bounds, const char *text, Color color, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), text(text), color(color), on_click(on_click) {}
    
    Rectangle bounds;
    const char *text;
    Color color;
    std::function<void()> on_click;

    void draw() { 
        result = GuiColorPicker(bounds, text, &color);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
    }
};

struct rColorPanel : public rBounds {
    rColorPanel() = default;
    
    rColorPanel(Rectangle bounds, const char *text, Color color, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), text(text), color(color), on_click(on_click) {}
    
    Rectangle bounds;
    const char *text;
    Color color;
    std::function<void()> on_click;

    void draw() { 
        result = GuiColorPicker(bounds, text, &color);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
    }
};

struct rColorBarAlpha : public rBounds {
    rColorBarAlpha() = default;
    
    rColorBarAlpha(Rectangle bounds, const char *text, float alpha, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), text(text), alpha(alpha), on_click(on_click) {}
    
    Rectangle bounds;
    const char *text;
    float alpha;
    std::function<void()> on_click;

    void draw() { 
        result = GuiColorBarAlpha(bounds, text, &alpha);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
    }
};

struct rColorBarHue : public rBounds {
    rColorBarHue() = default;
    
    rColorBarHue(Rectangle bounds, const char *text, float value, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), text(text), value(value), on_click(on_click) {}
    
    Rectangle bounds;
    const char *text;
    float value;
    std::function<void()> on_click;

    void draw() { 
        result = GuiColorBarHue(bounds, text, &value);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
    }
};

struct rColorPickerHSV : public rBounds {
    rColorPickerHSV() = default;
    rColorPickerHSV(Rectangle bounds, const char *text, Vector3 colorHsv, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), text(text), colorHsv(colorHsv), on_click(on_click) {}
    
    Rectangle bounds;
    const char *text;
    Vector3 colorHsv;
    std::function<void()> on_click;

    void draw() { 
        result = GuiColorPickerHSV(bounds, text, &colorHsv);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
    }
};

struct rColorPanelHSV : public rBounds {
    rColorPanelHSV() = default;
    rColorPanelHSV(Rectangle bounds, const char *text, Vector3 colorHsv, std::function<void()> on_click = nullptr) 
    : rBounds(bounds), text(text), colorHsv(colorHsv), on_click(on_click) {}
    
    Rectangle bounds;
    const char *text;
    Vector3 colorHsv;
    std::function<void()> on_click;

    void draw() { 
        result = GuiColorPanelHSV(bounds, text, &colorHsv);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
    }
};

#endif
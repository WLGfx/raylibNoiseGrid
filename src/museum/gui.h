#ifndef GUI_H
#define GUI_H

#include "raylib.h"
#include "raygui.h"

#include <cstdio>
#include <functional>

struct rBounds {
    enum Anchor {
        ANCHOR_NONE = 0,    ANCHOR_CENTER, // currently unused
        ANCHOR_TOP_LEFT,    ANCHOR_TOP_RIGHT,
        ANCHOR_BOTTOM_LEFT, ANCHOR_BOTTOM_RIGHT,
    };

    enum HAlign {
        HALIGN_LEFT, HALIGN_RIGHT, HALIGN_CENTER,
    };

    enum VAlign {
        VALIGN_TOP, VALIGN_BOTTOM, VALIGN_CENTER,
    };

    rBounds() = default;
    rBounds(Rectangle bounds, const char *text, rBounds *parent = nullptr, rBounds::Anchor anchor = rBounds::ANCHOR_NONE) 
    : bounds(bounds), text(text), parent(parent), anchor(anchor) {}

    void add_child(rBounds *child);
    void set_anchor(rBounds *parent, rBounds::Anchor anchor);
    void update_to_parent();
    void move(float x, float y);
    void resize(float w, float h);
    
    Rectangle bounds;
    const char *text;
    
    Anchor anchor;
    
    rBounds *parent;
    std::vector<rBounds*> children;
};

struct rHBox_Child {
    rBounds *child;
    Rectangle *padding;
    rBounds::HAlign halign;
};

struct rHBox : public rBounds {
    rHBox() = default;
    rHBox(Rectangle bounds, const char *text, rHBox_Child *children = nullptr, int count = 0) 
    : rBounds(bounds, text), children(children, children + count) {}

    std::vector<rHBox_Child> children;

    void add_child(rBounds *child, Rectangle *padding = nullptr, rBounds::HAlign halign = rBounds::HALIGN_LEFT) { 
        rHBox_Child ch = {child, padding, halign};
        children.push_back(ch); 
        update_to_parent();
    }

    void update_children() {
        // divide bounds height by children count
        // place each child in order going downwards
        float h = bounds.height / children.size();
        for (int i = 0; i < children.size(); i++) {
            children[i].child->bounds = { 
                bounds.x, 
                bounds.y + i * h, 
                bounds.width, 
                h 
            };
        }
    }
};

struct rWindowBox : public rBounds {
    rWindowBox() = default;
    rWindowBox(Rectangle bounds, const char *title) : rBounds(bounds, title) {}

    int draw() { return GuiWindowBox(bounds, text); }
};

struct rGroupBox : public rBounds {
    rGroupBox() = default;
    rGroupBox(Rectangle bounds, const char *text) : rBounds(bounds, text) {}

    int draw() { return GuiGroupBox(bounds, text); }
};

struct rLine : public rBounds {
    rLine() = default;
    rLine(Rectangle bounds, const char *text) : rBounds(bounds, text) {}

    int draw() { return GuiLine(bounds, text); }
};

struct rPanel : public rBounds {
    rPanel() = default;
    rPanel(Rectangle bounds, const char *text) : rBounds(bounds, text) {}

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
    : rBounds(bounds, text), content(content), scroll(scroll), view(view) {}
    
    Rectangle content;
    Vector2 *scroll;
    Rectangle *view;
};

struct rLabel : public rBounds {
    rLabel() = default;
    rLabel(Rectangle bounds, const char *text) : rBounds(bounds, text) {}

    int draw() { return GuiLabel(bounds, text); }
};

struct rButton : public rBounds {
    rButton() = default;
    rButton(Rectangle bounds, const char *text, std::function<void()> on_click = nullptr) : rBounds(bounds, text), on_click(on_click) {}

    std::function<void()> on_click;

    int draw() { int result = GuiButton(bounds, text); if (result && on_click) on_click(); return result; }
};

struct RLabelButton : public rBounds {
    RLabelButton() = default;
    RLabelButton(Rectangle bounds, const char *text) : rBounds(bounds, text) {}

    int draw() { return GuiLabelButton(bounds, text); }
};

struct rToggle : public rBounds {
    rToggle() = default;
    rToggle(Rectangle bounds, const char *text, bool active, std::function<void()> on_click = nullptr) 
        : rBounds(bounds, text), active(active), on_click(on_click) {}
    
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
    rToggleGroup(Rectangle bounds, const char *text, int active) : rBounds(bounds, text), active(active) {}
    
    int active;

    int draw() { return GuiToggleGroup(bounds, text, &active); }
};

struct rToggleSlider : public rBounds {
    rToggleSlider() = default;
    rToggleSlider(Rectangle bounds, const char *text, int active) : rBounds(bounds, text), active(active) {}
    
    int active;
};

struct rCheckBox : public rBounds {
    rCheckBox() = default;
    rCheckBox(Rectangle bounds, const char *text, bool active, bool checked)
    : rBounds(bounds, text), active(active), checked(checked) {}
    
    bool active;
    bool checked;

    int draw() { return GuiCheckBox(bounds, text, &checked); }
};

struct rComboBox : public rBounds {
    rComboBox() = default;
    rComboBox(Rectangle bounds, const char *text, int active) : rBounds(bounds, text), active(active) {}
    
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
    rDropdownBox(Rectangle bounds, const char *text, int active, bool editMode, std::function<void()> on_click = nullptr)
    : rBounds(bounds, text), active(active), editMode(editMode), on_click(on_click) {}
    
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
    rSpinner(Rectangle bounds, const char *text, int value, int minValue, int maxValue, bool editMode, std::function<void()> on_click = nullptr)
    : rBounds(bounds, text), value(value), minValue(minValue), maxValue(maxValue), editMode(editMode), on_click(on_click) {}
    
    int value;
    int minValue;
    int maxValue;
    bool editMode;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiSpinner(bounds, text, &value, minValue, maxValue, editMode);
        if (on_click) {
            on_click();
        }
        if (result) {
            editMode = !editMode;
        }
        return result;
    }
};

struct rValueBox : public rBounds {
    rValueBox() = default;
    rValueBox(Rectangle bounds, const char *text, int value, int minValue, int maxValue, bool editMode, std::function<void()> on_click = nullptr)
    : rBounds(bounds, text), value(value), minValue(minValue), maxValue(maxValue), editMode(editMode), on_click(on_click) {}
    
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
};

struct rValueBoxFloat : public rBounds {
    rValueBoxFloat() = default;
    rValueBoxFloat(Rectangle bounds, const char *text, float value, bool editMode, std::function<void()> on_click = nullptr)
    : rBounds(bounds, text), value(value), editMode(editMode), on_click(on_click) { write(); }

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
    : rBounds(bounds, ""), text(text), textSize(textSize), editMode(editMode), on_click(on_click) {}
    
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
    : rBounds(bounds, text), right(right), value(value), min(min), max(max), on_click(on_click) {}
    
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
    : rBounds(bounds, text), right(right), value(value), min(min), max(max), on_click(on_click) {}
    
    const char *right;
    float value;
    float min;
    float max;
    std::function<void()> on_click;
    
    int draw() { 
        int result = GuiSliderBar(bounds, text, right, &value, min, max);
        if (result) {
            if (on_click) {
                on_click();
            }
        }
        return result;
    }
};

struct rProgressBar : public rBounds {
    rProgressBar() = default;
    rProgressBar(Rectangle bounds, const char *text, const char *right, float value, float min, float max, std::function<void()> on_click = nullptr) 
    : rBounds(bounds, text), right(right), value(value), min(min), max(max), on_click(on_click) {}
    
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
    : rBounds(bounds, text) {}
};

struct rDummyRect : public rBounds {
    rDummyRect() = default;
    rDummyRect(Rectangle bounds, const char *text) 
    : rBounds(bounds, text) {}
};

struct rGrid : public rBounds {
    rGrid() = default;
    rGrid(Rectangle bounds, const char *text, float spacing, int subdivs, Vector2 mouseCell, std::function<void()> on_click = nullptr) 
    : rBounds(bounds, text), spacing(spacing), subdivs(subdivs), mouseCell(mouseCell), on_click(on_click) {}
    
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
    : rBounds(bounds, text), scrollIndex(scrollIndex), active(active), on_click(on_click) {}
    
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
    : bounds(bounds), text(text), count(count), scrollIndex(scrollIndex), active(active), focus(focus), on_click(on_click) {}
    
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

/*#define rWindowBox(b) GuiWindowBox(b.bounds, b.text)
#define rGroupBox(b) GuiGroupBox(b.bounds, b.text)
#define rLine(b) GuiLine(b.bounds, b.text)
#define rPanel(b) GuiPanel(b.bounds, b.text)
#define rTabBar(b) GuiTabBar(b.bounds, b.text)
#define rScrollPanel(b) GuiScrollPanel(b.bounds, b.text, b.content, b.scroll, b.view)

#define rLabel(b) GuiLabel(b.bounds, b.text)
#define rButton(b) GuiButton(b.bounds, b.text)
#define rLabelButton(b) GuiLabelButton(b.bounds, b.text)
#define rToggle(b) GuiToggle(b.bounds, b.text, &b.active)
#define rToggleGroup(b) GuiToggleGroup(b.bounds, b.text, &b.active)
#define rToggleSlider(b) GuiToggleSlider(b.bounds, b.text, &b.active)
#define rCheckBox(b) GuiCheckBox(b.bounds, b.text, b.active, b.editMode)
#define rComboBox(b) GuiComboBox(b.bounds, b.text, b.active, b.editMode)

#define rDropdownBox(b) GuiDropdownBox(b.bounds, b.text, &b.active, b.editMode)
#define rSpinner(b) GuiSpinner(b.bounds, b.text, &b.value, b.minValue, b.maxValue, b.editMode)
#define rValueBox(b) GuiValueBox(b.bounds, b.text, &b.value, b.minValue, b.maxValue, b.editMode)
#define rValueBoxFloat(b) GuiValueBoxFloat(b.bounds, b.text, b.textValue, &b.value, b.editMode)
#define rTextBox(b) GuiTextBox(b.bounds, b.text, b.editMode)

#define rSlider(b) GuiSlider(b.bounds, b.text, b.right, &b.value, b.min, b.max)
#define rSliderBar(b) GuiSliderBar(b.bounds, b.text, b.right, &b.value, b.min, b.max)
#define rProgressBar(b) GuiProgressBar(b.bounds, b.text, b.right, &b.value, b.min, b.max)
#define rStatusBar(b) GuiStatusBar(b.bounds, b.text)
#define rDummyRect(b) GuiDummyRec(b.bounds, b.text)
#define rGrid(b) GuiGrid(b.bounds, b.text, b.spacing, b.subdivs, b.mouseCell)

#define rListView(b) GuiListView(b.bounds, b.text, b.scrollIndex, b.active)
#define rListViewEx(b) GuiListViewEx(b.bounds, b.text, b.count, &b.scrollIndex, &b.active, &b.focus)
#define rMessageBox(b) GuiMessageBox(b.bounds, b.title, b.message, b.buttons)
#define rTextInputBox(b) GuiTextInputBox(b.bounds, b.title, b.message, b.buttons, b.text, b.textMaxSize, b.secretViewActive)
#define rColorPicker(b) GuiColorPicker(b.bounds, b.text, b.color)
#define rColorPanelHSV(b) GuiColorPanelHSV(b.bounds, b.text, b.colorHsv)
#define rColorBarAlpha(b) GuiColorBarAlpha(b.bounds, b.text, &b.value)
#define rColorBarHue(b) GuiColorBarHue(b.bounds, b.text, &b.value)
#define rColorPickerHSV(b) GuiColorPickerHSV(b.bounds, b.text, b.colorHsv)
#define rColorPanelHSV(b) GuiColorPanelHSV(b.bounds, b.text, b.colorHsv)

yay got rid of those macros... yuk.
*/

/**********************
I was going to work on an anchorage layout system but gave up

enum class LayoutAnchor {
    TOP_LEFT,     // Absolute positioning
    TOP_RIGHT,    // Relative to parent's top-right
    BOTTOM_LEFT,  // Relative to parent's bottom-left  
    BOTTOM_RIGHT, // Relative to parent's bottom-right
    CENTER,       // Center within parent
    STRETCH,      // Stretch to fill parent (with padding)
    VERTICAL_CENTER, // Vertical center, fixed width
    HORIZONTAL_CENTER // Horizontal center, fixed height
};

struct UIBounds {
    Rectangle rect;                // Current bounds
    LayoutAnchor anchor;           // How to position relative to parent
    Vector2 size;                  // Fixed size (if anchor != STRETCH)
    Vector2 minSize;               // Minimum size constraints
    Vector2 maxSize;               // Maximum size constraints
    Vector2 offset;                // Offset from anchored position
    float padding[4];              // top, right, bottom, left (for STRETCH anchor)
    
    UIBounds() : rect{0,0,0,0}, anchor(LayoutAnchor::TOP_LEFT), 
                 size{0,0}, minSize{0,0}, maxSize{9999,9999}, offset{0,0},
                 padding{0,0,0,0} {}
    
    UIBounds(Rectangle r, LayoutAnchor a = LayoutAnchor::TOP_LEFT) 
        : rect(r), anchor(a), size{r.width, r.height}, minSize{0,0}, 
          maxSize{9999,9999}, offset{0,0}, padding{0,0,0,0} {}
    
    // Helper to create common bounds
    static UIBounds Fixed(float x, float y, float width, float height) {
        return UIBounds{Rectangle{x, y, width, height}, LayoutAnchor::TOP_LEFT};
    }
    
    static UIBounds Stretch(float padding = 0) {
        UIBounds bounds;
        bounds.anchor = LayoutAnchor::STRETCH;
        bounds.padding[0] = bounds.padding[1] = 
        bounds.padding[2] = bounds.padding[3] = padding;
        return bounds;
    }
    
    static UIBounds Stretch(float top, float right, float bottom, float left) {
        UIBounds bounds;
        bounds.anchor = LayoutAnchor::STRETCH;
        bounds.padding[0] = top;
        bounds.padding[1] = right;
        bounds.padding[2] = bottom;
        bounds.padding[3] = left;
        return bounds;
    }
    
    static UIBounds Center(float width, float height) {
        UIBounds bounds;
        bounds.anchor = LayoutAnchor::CENTER;
        bounds.size = {width, height};
        return bounds;
    }
};*/

#endif // GUI_H
#ifndef GUI_H
#define GUI_H

#include "raylib.h"
#include "raygui.h"

#define write_float_to_char_buffer(buffer, value) snprintf(buffer, sizeof(buffer), "%.3f", value)

struct rBounds {
    rBounds() = default;
    rBounds(Rectangle bounds, const char *text) : bounds(bounds), text(text) {}

    Rectangle bounds;
    const char *text;
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
    rButton(Rectangle bounds, const char *text) : rBounds(bounds, text) {}

    int draw() { return GuiButton(bounds, text); }
};

struct RLabelButton : public rBounds {
    RLabelButton() = default;
    RLabelButton(Rectangle bounds, const char *text) : rBounds(bounds, text) {}

    int draw() { return GuiLabelButton(bounds, text); }
};

struct rToggle : public rBounds {
    rToggle() = default;
    rToggle(Rectangle bounds, const char *text, bool active) : rBounds(bounds, text), active(active) {}
    
    bool active;

    int draw() { return GuiToggle(bounds, text, &active); }
};

struct rToggleGroup : public rBounds {
    rToggleGroup() = default;
    rToggleGroup(Rectangle bounds, const char *text, int *active) : rBounds(bounds, text), active(active) {}
    
    int *active;

    int draw() { return GuiToggleGroup(bounds, text, active); }
};

struct rToggleSlider : public rBounds {
    rToggleSlider() = default;
    rToggleSlider(Rectangle bounds, const char *text, int *active) : rBounds(bounds, text), active(active) {}
    
    int *active;
};

struct rCheckBox : public rBounds {
    rCheckBox() = default;
    rCheckBox(Rectangle bounds, const char *text, bool *active, bool checked)
    : rBounds(bounds, text), active(active), checked(checked) {}
    
    bool *active;
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
    rDropdownBox(Rectangle bounds, const char *text, int active, bool editMode)
    : rBounds(bounds, text), active(active), editMode(editMode) {}
    
    int active;
    bool editMode;
    
    int draw() { 
        int result = GuiDropdownBox(bounds, text, &active, editMode);
        if (result) {
            editMode = !editMode;
        }
        return result;
    }
};

struct rSpinner : public rBounds {
    rSpinner() = default;
    rSpinner(Rectangle bounds, const char *text, int value, int minValue, int maxValue, bool editMode)
    : rBounds(bounds, text), value(value), minValue(minValue), maxValue(maxValue), editMode(editMode) {}
    
    int value;
    int minValue;
    int maxValue;
    bool editMode;
    
    int draw() { 
        int result = GuiSpinner(bounds, text, &value, minValue, maxValue, editMode);
        if (result) {
            editMode = !editMode;
        }
        return result;
    }
};

struct rValueBox : public rBounds {
    rValueBox() = default;
    rValueBox(Rectangle bounds, const char *text, int value, int minValue, int maxValue, bool editMode)
    : rBounds(bounds, text), value(value), minValue(minValue), maxValue(maxValue), editMode(editMode) {}
    
    int value;
    int minValue;
    int maxValue;
    bool editMode;
    
    int draw() { 
        int result = GuiValueBox(bounds, text, &value, minValue, maxValue, editMode);
        if (result) {
            editMode = !editMode;
        }
        return result;
    }
};

struct rValueBoxFloat : public rBounds {
    rValueBoxFloat() = default;
    rValueBoxFloat(Rectangle bounds, const char *text, float value, bool editMode)
    : rBounds(bounds, text), value(value), editMode(editMode) {}

    float value;
    char textValue[16];
    bool editMode;
    
    int draw() { 
        int result = GuiValueBoxFloat(bounds, text, textValue, &value, editMode);
        if (result) {
            editMode = !editMode;
        }
        return result;
    }
};

struct rTextBox : public rBounds {
    rTextBox() = default;
    rTextBox(Rectangle bounds, char *text, int textSize, bool editMode)
    : rBounds(bounds, ""), text(text), textSize(textSize), editMode(editMode) {}
    
    char *text;
    int textSize;
    bool editMode;
    
    int draw() { 
        int result = GuiTextBox(bounds, text, textSize, editMode);
        if (result) {
            editMode = !editMode;
        }
        return result;
    }
};

struct rSlider : public rBounds {
    rSlider() = default;
    rSlider(Rectangle bounds, const char *text, const char *right, float value, float min, float max) 
    : rBounds(bounds, text), right(right), value(value), min(min), max(max) {}
    
    const char *right;
    float value;
    float min;
    float max;
    
    int draw() { return GuiSlider(bounds, text, right, &value, min, max); }
};

struct rSliderBar : public rBounds {
    rSliderBar() = default;
    rSliderBar(Rectangle bounds, const char *text, const char *right, float value, float min, float max) 
    : rBounds(bounds, text), right(right), value(value), min(min), max(max) {}
    
    const char *right;
    float value;
    float min;
    float max;
    
    int draw() { return GuiSliderBar(bounds, text, right, &value, min, max); }
};

struct rProgressBar : public rBounds {
    rProgressBar() = default;
    rProgressBar(Rectangle bounds, const char *text, const char *right, float value, float min, float max) 
    : rBounds(bounds, text), right(right), value(value), min(min), max(max) {}
    
    const char *right;
    float value;
    float min;
    float max;
    
    int draw() { return GuiProgressBar(bounds, text, right, &value, min, max); }
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
    rGrid(Rectangle bounds, const char *text, float spacing, int subdivs, Vector2 mouseCell) 
    : rBounds(bounds, text), spacing(spacing), subdivs(subdivs), mouseCell(mouseCell) {}
    
    float spacing;
    int subdivs;
    Vector2 mouseCell;
    
    int draw() { return GuiGrid(bounds, text, spacing, subdivs, &mouseCell); }
};

struct rListView : public rBounds {
    rListView() = default;
    rListView(Rectangle bounds, const char *text, int scrollIndex, int active) 
    : rBounds(bounds, text), scrollIndex(scrollIndex), active(active) {}
    
    int scrollIndex;
    int active;
    
    int draw() { return GuiListView(bounds, text, &scrollIndex, &active); }
};

struct rListViewEx {
    rListViewEx() = default;
    rListViewEx(Rectangle bounds, const char **text, int count, int scrollIndex, int active, int focus) 
    : bounds(bounds), text(text), count(count), scrollIndex(scrollIndex), active(active), focus(focus) {}
    
    Rectangle bounds;
    const char **text;
    int count;
    int scrollIndex;
    int active;
    int focus;
    
    int draw() { return GuiListViewEx(bounds, text, count, &scrollIndex, &active, &focus); }
};

struct rMessageBox {
    rMessageBox() = default;
    rMessageBox(Rectangle bounds, const char *title, const char *message, const char *buttons) 
    : bounds(bounds), title(title), message(message), buttons(buttons) {}
    
    Rectangle bounds;
    const char *title;
    const char *message;
    const char *buttons;
    
    int draw() { return GuiMessageBox(bounds, title, message, buttons); }
};

struct rTextInputBox {
    rTextInputBox() = default;
    rTextInputBox(Rectangle bounds, const char *title, const char *message, const char *buttons, char *text, int textMaxSize, bool secretViewActive) 
    : bounds(bounds), title(title), message(message), buttons(buttons), text(text), textMaxSize(textMaxSize), secretViewActive(secretViewActive) {}
    
    Rectangle bounds;
    const char *title;
    const char *message;
    const char *buttons;
    char *text;
    int textMaxSize;
    bool secretViewActive;
    
    int draw() { return GuiTextInputBox(bounds, title, message, buttons, text, textMaxSize, &secretViewActive); }
};

struct rColorPicker {
    rColorPicker() = default;
    rColorPicker(Rectangle bounds, const char *text, Color color) 
    : bounds(bounds), text(text), color(color) {}
    
    Rectangle bounds;
    const char *text;
    Color color;

    int draw() { return GuiColorPicker(bounds, text, &color); }
};

struct rColorPanel {
    rColorPanel() = default;
    rColorPanel(Rectangle bounds, const char *text, Color color) 
    : bounds(bounds), text(text), color(color) {}
    
    Rectangle bounds;
    const char *text;
    Color color;

    int draw() { return GuiColorPicker(bounds, text, &color); }
};

struct rColorBarAlpha {
    rColorBarAlpha() = default;
    rColorBarAlpha(Rectangle bounds, const char *text, float alpha) 
    : bounds(bounds), text(text), alpha(alpha) {}
    
    Rectangle bounds;
    const char *text;
    float alpha;

    int draw() { return GuiColorBarAlpha(bounds, text, &alpha); }
};

struct rColorBarHue {
    rColorBarHue() = default;
    rColorBarHue(Rectangle bounds, const char *text, float value) 
    : bounds(bounds), text(text), value(value) {}
    
    Rectangle bounds;
    const char *text;
    float value;

    int draw() { return GuiColorBarHue(bounds, text, &value); }
};

struct rColorPickerHSV {
    rColorPickerHSV() = default;
    rColorPickerHSV(Rectangle bounds, const char *text, Vector3 colorHsv) 
    : bounds(bounds), text(text), colorHsv(colorHsv) {}
    
    Rectangle bounds;
    const char *text;
    Vector3 colorHsv;

    int draw() { return GuiColorPickerHSV(bounds, text, &colorHsv); }
};

struct rColorPanelHSV {
    rColorPanelHSV() = default;
    rColorPanelHSV(Rectangle bounds, const char *text, Vector3 colorHsv) 
    : bounds(bounds), text(text), colorHsv(colorHsv) {}
    
    Rectangle bounds;
    const char *text;
    Vector3 colorHsv;

    int draw() { return GuiColorPanelHSV(bounds, text, &colorHsv); }
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
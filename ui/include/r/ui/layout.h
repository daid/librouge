#pragma once

#include "r/rect.h"
#include <memory>
#include <vector>

namespace r::ui {

class Element;
class Layout {
public:
    virtual void update(std::vector<std::unique_ptr<Element>>& children, Recti r) = 0;
};

class LayoutSettings {
public:
    enum class Alignment {
        Min, Mid, Max
    };
    struct Sides {
        int left = 0;
        int right = 0;
        int top = 0;
        int bottom = 0;
    };

    Alignment align_horizontal = Alignment::Min;
    Alignment align_vertical = Alignment::Min;
    ivec2 min_size{1, 1};
    ivec2 max_size{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
    Sides margin;
    Sides padding;

    LayoutSettings operator|(const LayoutSettings& other) const;
};

LayoutSettings size(int w, int h);
LayoutSettings min_size(int w, int h);
LayoutSettings max_size(int w, int h);
LayoutSettings topleft();
LayoutSettings top();
LayoutSettings topright();
LayoutSettings left();
LayoutSettings center();
LayoutSettings right();
LayoutSettings bottomleft();
LayoutSettings bottom();
LayoutSettings bottomright();
LayoutSettings margin(int amount);
LayoutSettings margin(int h, int v);
LayoutSettings margin(int left, int right, int top, int bottom);
LayoutSettings padding(int amount);
LayoutSettings padding(int h, int v);
LayoutSettings padding(int left, int right, int top, int bottom);

}

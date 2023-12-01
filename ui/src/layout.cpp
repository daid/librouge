#include "r/ui/layout.h"

namespace r::ui {

LayoutSettings LayoutSettings::operator|(const LayoutSettings& other) const {
    LayoutSettings res = *this;
    res.min_size.x = std::max(res.min_size.x, other.min_size.x);
    res.min_size.y = std::max(res.min_size.y, other.min_size.y);
    res.max_size.x = std::min(res.max_size.x, other.max_size.x);
    res.max_size.y = std::min(res.max_size.y, other.max_size.y);
    if (other.align_horizontal != LayoutSettings::Alignment::Min)
        res.align_horizontal = other.align_horizontal;
    if (other.align_vertical != LayoutSettings::Alignment::Min)
        res.align_vertical = other.align_vertical;
    res.margin.left = std::max(res.margin.left, other.margin.left);
    res.margin.right = std::max(res.margin.right, other.margin.right);
    res.margin.top = std::max(res.margin.top, other.margin.top);
    res.margin.bottom = std::max(res.margin.bottom, other.margin.bottom);
    res.padding.left = std::max(res.padding.left, other.padding.left);
    res.padding.right = std::max(res.padding.right, other.padding.right);
    res.padding.top = std::max(res.padding.top, other.padding.top);
    res.padding.bottom = std::max(res.padding.bottom, other.padding.bottom);
    return res;
}

LayoutSettings size(int w, int h) {
    LayoutSettings res;
    res.min_size = {w, h};
    res.max_size = {w, h};
    return res;
}

LayoutSettings min_size(int w, int h) {
    LayoutSettings res;
    res.min_size = {w, h};
    return res;
}

LayoutSettings max_size(int w, int h) {
    LayoutSettings res;
    res.max_size = {w, h};
    return res;
}

LayoutSettings topleft() {
    LayoutSettings res;
    res.align_horizontal = LayoutSettings::Alignment::Min;
    res.align_vertical = LayoutSettings::Alignment::Min;
    return res;
}

LayoutSettings top() {
    LayoutSettings res;
    res.align_horizontal = LayoutSettings::Alignment::Mid;
    res.align_vertical = LayoutSettings::Alignment::Min;
    return res;
}

LayoutSettings topright() {
    LayoutSettings res;
    res.align_horizontal = LayoutSettings::Alignment::Max;
    res.align_vertical = LayoutSettings::Alignment::Min;
    return res;
}

LayoutSettings left() {
    LayoutSettings res;
    res.align_horizontal = LayoutSettings::Alignment::Min;
    res.align_vertical = LayoutSettings::Alignment::Mid;
    return res;
}

LayoutSettings center() {
    LayoutSettings res;
    res.align_horizontal = LayoutSettings::Alignment::Mid;
    res.align_vertical = LayoutSettings::Alignment::Mid;
    return res;
}

LayoutSettings right() {
    LayoutSettings res;
    res.align_horizontal = LayoutSettings::Alignment::Max;
    res.align_vertical = LayoutSettings::Alignment::Mid;
    return res;
}

LayoutSettings bottomleft() {
    LayoutSettings res;
    res.align_horizontal = LayoutSettings::Alignment::Min;
    res.align_vertical = LayoutSettings::Alignment::Max;
    return res;
}

LayoutSettings bottom() {
    LayoutSettings res;
    res.align_horizontal = LayoutSettings::Alignment::Mid;
    res.align_vertical = LayoutSettings::Alignment::Max;
    return res;
}

LayoutSettings bottomright() {
    LayoutSettings res;
    res.align_horizontal = LayoutSettings::Alignment::Max;
    res.align_vertical = LayoutSettings::Alignment::Max;
    return res;
}

LayoutSettings margin(int amount) {
    LayoutSettings res;
    res.margin.left = res.margin.right = res.margin.top = res.margin.bottom = amount;
    return res;
}
LayoutSettings margin(int h, int v) {
    LayoutSettings res;
    res.margin.left = res.margin.right = h;
    res.margin.top = res.margin.bottom = v;
    return res;
}
LayoutSettings margin(int left, int right, int top, int bottom) {
    LayoutSettings res;
    res.margin.left = left;
    res.margin.right = right;
    res.margin.top = top;
    res.margin.bottom = bottom;
    return res;
}
LayoutSettings padding(int amount) {
    LayoutSettings res;
    res.padding.left = res.padding.right = res.padding.top = res.padding.bottom = amount;
    return res;
}
LayoutSettings padding(int h, int v) {
    LayoutSettings res;
    res.padding.left = res.padding.right = h;
    res.padding.top = res.padding.bottom = v;
    return res;
}
LayoutSettings padding(int left, int right, int top, int bottom) {
    LayoutSettings res;
    res.padding.left = left;
    res.padding.right = right;
    res.padding.top = top;
    res.padding.bottom = bottom;
    return res;
}

}

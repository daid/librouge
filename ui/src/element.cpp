#include "r/ui/element.h"
#include <algorithm>


namespace r::ui {

Element::~Element() {
}

void Element::updateRenderingRect(Recti r)
{
    rendering_rect.size.x = std::clamp(r.size.x - layout.margin.left - layout.margin.right, layout.min_size.x, layout.max_size.x);
    rendering_rect.size.y = std::clamp(r.size.y - layout.margin.top - layout.margin.bottom, layout.min_size.y, layout.max_size.y);
    switch(layout.align_horizontal) {
    case LayoutSettings::Alignment::Min:
        rendering_rect.position.x = r.position.x + layout.margin.left;
        break;
    case LayoutSettings::Alignment::Mid:
        rendering_rect.position.x = r.position.x + (r.size.x - rendering_rect.size.x) / 2;
        break;
    case LayoutSettings::Alignment::Max:
        rendering_rect.position.x = r.position.x + r.size.x - rendering_rect.size.x - layout.margin.right;
        break;
    }
    switch(layout.align_vertical) {
    case LayoutSettings::Alignment::Min:
        rendering_rect.position.y = r.position.y + layout.margin.top;
        break;
    case LayoutSettings::Alignment::Mid:
        rendering_rect.position.y = r.position.y + (r.size.y - rendering_rect.size.y) / 2;
        break;
    case LayoutSettings::Alignment::Max:
        rendering_rect.position.y = r.position.y + r.size.y - rendering_rect.size.y - layout.margin.bottom;
        break;
    }
}

void Element::draw(r::frontend::Renderer& r)
{
    (void)r;
}

}

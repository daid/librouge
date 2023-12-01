#include "r/ui/panel.h"
#include "r/frontend/renderer.h"


namespace r::ui {

Panel::Panel(const LayoutSettings& ls, Color color)
: Element(ls), color(color) {
}

void Panel::draw(r::frontend::Renderer& r) {
    r.draw(rect(), ' ', color, color);
}

}


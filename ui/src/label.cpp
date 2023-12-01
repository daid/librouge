#include "r/ui/label.h"
#include "r/frontend/renderer.h"


namespace r::ui {

Label::Label(const LayoutSettings& ls, const std::string& text)
: Element(ls), text(text) {
}

void Label::draw(r::frontend::Renderer& r) {
    r.print(rect().position, text.c_str());
}

}

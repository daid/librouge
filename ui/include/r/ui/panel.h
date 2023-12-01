#pragma once

#include "element.h"
#include "r/color.h"
#include <string>


namespace r::ui {

class Panel : public Element {
public:
    Panel(const LayoutSettings& ls, Color color);

private:
    void draw(r::frontend::Renderer& r) override;

    Color color;
};

}

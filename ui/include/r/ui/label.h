#pragma once

#include "element.h"
#include <string>


namespace r::ui {

class Label : public Element {
public:
    Label(const LayoutSettings& ls, const std::string& text);

private:
    void draw(r::frontend::Renderer& r) override;

    std::string text;
};

}

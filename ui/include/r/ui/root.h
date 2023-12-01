#pragma once

#include "element.h"

namespace r::ui {

class Root : public Element {
public:
    Root();

    void render(r::frontend::Renderer& r);

private:
    static void layoutRecursive(Element* e);
    static void renderRecursive(r::frontend::Renderer& r, Element* e);
};

}

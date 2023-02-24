#pragma once

#include "r/vec.h"
#include "r/color.h"
#include "r/rect.h"

namespace r::frontend {

class Renderer {
public:
    virtual ivec2 size() = 0;
    virtual void draw(ivec2 position, char c, Color forground_color) = 0;
    virtual void draw(ivec2 position, char c, Color forground_color, Color background_color) = 0;

    void drawBox(Rect2i rect, Color forground_color, Color background_color);
};

}

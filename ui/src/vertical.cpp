#include "r/ui/vertical.h"
#include "r/ui/element.h"


namespace r::ui {

void Vertical::update(std::vector<std::unique_ptr<Element>>& children, Recti r)
{
    int min_height = 0;
    int extra_height = 0;
    int inf_count = 0;
    for(auto& c : children) {
        min_height += c->layout.min_size.y + c->layout.margin.top + c->layout.margin.bottom;
        if (c->layout.max_size.y == std::numeric_limits<int>::max()) {
            inf_count += 1;
        } else {
            extra_height += c->layout.max_size.y - c->layout.min_size.y;
        }
    }

    int inf_height = r.size.y - min_height;
    int extra_remainder = 0;
    if (inf_height >= extra_height) {
        inf_height -= extra_height;
        extra_remainder = extra_height;
    } else {
        extra_remainder = inf_height;
        inf_height = 0;
    }
    int y = r.position.y;
    for(auto& c : children) {
        int h = c->layout.min_size.y + c->layout.margin.top + c->layout.margin.bottom;
        if (c->layout.max_size.y == std::numeric_limits<int>::max()) {
            h += inf_height / inf_count;
        } else {
            int e = c->layout.max_size.y - c->layout.min_size.y;
            h += e * extra_remainder / extra_height;
        }
        c->updateRenderingRect({{r.position.x, y}, {r.size.x, h}});
        y += h;
    }
}

}

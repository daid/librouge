#include "r/ui/horizontal.h"
#include "r/ui/element.h"


namespace r::ui {

void Horizontal::update(std::vector<std::unique_ptr<Element>>& children, Recti r)
{
    int min_width = 0;
    int extra_width = 0;
    int inf_count = 0;
    for(auto& c : children) {
        min_width += c->layout.min_size.x + c->layout.margin.left + c->layout.margin.right;
        if (c->layout.max_size.x == std::numeric_limits<int>::max()) {
            inf_count += 1;
        } else {
            extra_width += c->layout.max_size.x - c->layout.min_size.x;
        }
    }

    int inf_width = r.size.x - min_width;
    int extra_remainder = 0;
    if (inf_width >= extra_width) {
        inf_width -= extra_width;
        extra_remainder = extra_width;
    } else {
        extra_remainder = inf_width;
        inf_width = 0;
    }
    int x = r.position.x;
    for(auto& c : children) {
        int w = c->layout.min_size.x + c->layout.margin.left + c->layout.margin.right;
        if (c->layout.max_size.x == std::numeric_limits<int>::max()) {
            w += inf_width / inf_count;
        } else {
            int e = c->layout.max_size.x - c->layout.min_size.x;
            w += e * extra_remainder / extra_width;
        }
        c->updateRenderingRect({{x, r.position.y}, {w, r.size.y}});
        x += w;
    }
}

}

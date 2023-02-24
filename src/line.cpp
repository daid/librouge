#include "r/line.h"

namespace r {

void traceLine(ivec2 start, ivec2 end, std::function<bool(ivec2)> callback)
{
    vec2 s{start.x+0.5f, start.y+0.5f};
    vec2 e{end.x+0.5f, end.y+0.5f};
    vec2 d{e - s};
    auto n = std::max(std::abs(start.x - end.x), std::abs(start.y - end.y));
    for(int step = 0; step < n; step++) {
        if (!callback(floor(s + d * float(step) / float(n))))
            return;
    }
    callback(end);
}

}
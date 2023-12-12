#include "r/line.h"
#include <algorithm>


namespace r {

TraceLine::TraceLine(ivec2 start, ivec2 end)
: start_point(start), end_point(end) {
}

TraceLine::Iterator::Iterator(ivec2 start, ivec2 end, int index)
: start_point(start), end_point(end), index(index) {
}

bool TraceLine::Iterator::operator!=(const Iterator& other) const { return index != other.index; }
void TraceLine::Iterator::operator++() { index++; }
ivec2 TraceLine::Iterator::operator*() const {
    auto n = std::max(std::abs(start_point.x - end_point.x), std::abs(start_point.y - end_point.y));
    if (n == 0) return start_point;
    auto s = vec2{start_point.x + 0.5f, start_point.y + 0.5f};
    auto d = vec2{end_point.x - start_point.x, end_point.y - start_point.y};
    return floor(s + d * float(index) / float(n));
}

TraceLine::Iterator TraceLine::begin() {
    return Iterator(start_point, end_point, 0);
}
TraceLine::Iterator TraceLine::end() {
    return Iterator(start_point, end_point, std::max(std::abs(start_point.x - end_point.x), std::abs(start_point.y - end_point.y)) + 1);
}

}
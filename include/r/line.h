#pragma once

#include "r/vec.h"
#include <functional>

namespace r {

class TraceLine {
public:
    TraceLine(ivec2 start, ivec2 end);

    class Iterator {
    public:
        Iterator(ivec2 start, ivec2 end, int index);
        bool operator!=(const Iterator& other) const;
        void operator++();
        ivec2 operator*() const;
    private:
        ivec2 start_point, end_point;
        int index;
    };

    Iterator begin();
    Iterator end();

private:
    ivec2 start_point, end_point;
};

}

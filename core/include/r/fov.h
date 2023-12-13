#pragma once

#include "r/vec.h"
#include "r/direction.h"
#include <functional>


namespace r {

class VisitFOV
{
public:
    VisitFOV(ivec2 _center, int _radius, const std::function<bool(ivec2)>& _callback)
    : center(_center), radius(_radius), callback(_callback)
    {
        if (!callback(center))
            return;
        visitFOVStep<Direction::Right>();
        visitFOVStep<Direction::Down>();
        visitFOVStep<Direction::Left>();
        visitFOVStep<Direction::Up>();
    }

private:
    template<Direction DIR> void visitFOVStep()
    {
        visitFOVStep<DIR, DIR - 1>(1, 0.0, 1.0f);
        visitFOVStep<DIR, DIR + 1>(1, 0.0, 1.0f);
    }
    template<Direction DIR, Direction SIDE> void visitFOVStep(int row, float fmin, float fmax)
    {
        if (row > radius)
            return;
        if (fmax - fmin < 0.001)
            return;
        auto dir = offset(DIR);
        auto side = offset(SIDE);
        for(int n=0; n<=row; n++) {
            auto start = (float(n) - 0.5f) / (float(row) + 0.5f);
            auto end = (float(n) + 0.5f) / (float(row) + 0.5f);

            if (end < fmin)
                continue;
            if (start > fmax)
                continue;

            auto offset = dir * row + side * n;
            auto p = center + offset;

            if (offset.x * offset.x + offset.y * offset.y < radius * radius) {
                if (!callback(p)) {
                    visitFOVStep<DIR, SIDE>(row + 1, fmin, start);
                    fmin = end;
                }
            } else {
                break;
            }
        }

        visitFOVStep<DIR, SIDE>(row + 1, fmin, fmax);
    }

    ivec2 center;
    int radius;
    const std::function<bool(ivec2)>& callback;
};

}

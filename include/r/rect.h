#pragma once

#include "r/vec.h"

namespace r {

template<typename T> class Rect
{
public:
    Rect() {}
    Rect(tvec2<T> _position, tvec2<T> _size) : position(_position), size(_size) {}
    Rect(T x, T y, T w, T h) : position(x, y), size(w, h) {}

    tvec2<T> center() const
    {
        return position + size / T(2);
    }

    bool contains(tvec2<T> p) const
    {
        return p.x >= position.x && p.x <= position.x + size.x && p.y >= position.y && p.y <= position.y + size.y;
    }

    bool overlaps(const Rect<T>& other) const
    {
        return position.x + size.x >= other.position.x && other.position.x + other.size.x >= position.x &&
            position.y + size.y >= other.position.y && other.position.y + other.size.y >= position.y;
    }

    void growToInclude(tvec2<T> p)
    {
        if (p.x < position.x)
        {
            size.x = size.x - p.x + position.x;
            position.x = p.x;
        }
        if (p.y < position.y)
        {
            size.y = size.y - p.y + position.y;
            position.y = p.y;
        }
        if (p.x > position.x + size.x)
            size.x = p.x - position.x;
        if (p.y > position.y + size.y)
            size.y = p.y - position.y;
    }

    void shrinkToFitWithin(const Rect<T>& other)
    {
        if (position.x < other.position.x)
        {
            size.x -= other.position.x - position.x;
            position.x = other.position.x;
        }
        if (position.y < other.position.y)
        {
            size.y -= other.position.y - position.y;
            position.y = other.position.y;
        }
        if (position.x + size.x > other.position.x + other.size.x)
            size.x -= (position.x + size.x) - (other.position.x + other.size.x);
        if (position.y + size.y > other.position.y + other.size.y)
            size.y -= (position.y + size.y) - (other.position.y + other.size.y);
        if (size.x < 0)
        {
            position.x += size.x;
            size.x = 0;
        }
        if (size.y < 0)
        {
            position.y += size.y;
            size.y = 0;
        }
    }

    class Iterator {
    public:
        Iterator(tvec2<T> _p, T _end_x)
        : p(_p), start_x(_p.x), end_x(_end_x)
        {}

        void operator++()
        {
            p.x += 1;
            if (p.x >= end_x) {
                p.x = start_x;
                p.y += 1;
            }
        }

        tvec2<T> operator*() const {
            return p;
        }

        bool operator!= (const Iterator& other) const {
            return p != other.p;
        }
    private:
        tvec2<T> p;
        T start_x;
        T end_x;
    };
    
    Iterator begin() const
    {
        return {position, position.x + size.x};
    }
    Iterator end() const
    {
        return {{position.x, position.y + size.y}, position.x + size.x};
    }

    tvec2<T> position;
    tvec2<T> size;
};

typedef Rect<int> Rect2i;
typedef Rect<float> Rect2f;
typedef Rect<double> Rect2d;

}

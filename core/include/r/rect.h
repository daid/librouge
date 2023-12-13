#pragma once

#include "r/vec.h"

namespace r {

template<typename T> class Rect
{
public:
    Rect() {}
    Rect(tvec2<T> _position, tvec2<T> _size) : position{_position}, size{_size} {}
    Rect(T x, T y, T w, T h) : position{x, y}, size{w, h} {}

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

    Rect<T> growToInclude(tvec2<T> p) const
    {
        auto result = *this;
        if (p.x < result.position.x)
        {
            result.size.x = result.size.x - p.x + result.position.x;
            result.position.x = p.x;
        }
        if (p.y < result.position.y)
        {
            result.size.y = result.size.y - p.y + result.position.y;
            result.position.y = p.y;
        }
        if (p.x > result.position.x + size.x)
            size.x = p.x - result.position.x;
        if (p.y > result.position.y + size.y)
            result.size.y = p.y - result.position.y;
        return result;
    }

    Rect<T> shrink(tvec2<T> p) const
    {
        auto result = Rect<T>(position + p, size - p * 2);
        if (result.size.x < 0) result.size.x = 0;
        if (result.size.y < 0) result.size.y = 0;
        return result;
    }

    Rect<T> shrinkToFitWithin(const Rect<T>& other) const
    {
        auto result = *this;
        if (result.position.x < other.position.x)
        {
            result.size.x -= other.position.x - result.position.x;
            result.position.x = other.position.x;
        }
        if (result.position.y < other.position.y)
        {
            result.size.y -= other.position.y - result.position.y;
            result.position.y = other.position.y;
        }
        if (result.position.x + result.size.x > other.position.x + other.size.x)
            result.size.x -= (position.x + size.x) - (other.position.x + other.size.x);
        if (result.position.y + result.size.y > other.position.y + other.size.y)
            result.size.y -= (result.position.y + result.size.y) - (other.position.y + other.size.y);
        if (result.size.x < 0)
        {
            result.position.x += result.size.x;
            result.size.x = 0;
        }
        if (result.size.y < 0)
        {
            result.position.y += result.size.y;
            result.size.y = 0;
        }
        return result;
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

typedef Rect<int> Recti;
typedef Rect<float> Rectf;
typedef Rect<double> Rectd;

}

#pragma once

#include <vector>
#include <functional>
#include "r/vec.h"
#include "rect.h"

namespace r {

// Two dimensional array that can be resized, unlike a normal std::vector, the r::Vector2 is cleared on resize.
template<typename T> class Vector2
{
public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    ivec2 size() const
    {
        return _size;
    }

    void resize(ivec2 new_size)
    {
        _size = new_size;
        data.clear();
        data.resize(_size.x * _size.y, {});
    }

    void resize(ivec2 new_size, const T& fill)
    {
        _size = new_size;
        data.clear();
        data.resize(_size.x * _size.y, fill);
    }

    T& operator[](ivec2 position)
    {
        return data[position.x + position.y * _size.x];
    }

    const T& operator[](ivec2 position) const
    {
        return data[position.x + position.y * _size.x];
    }

    T& operator()(ivec2 position)
    {
        return data[position.x + position.y * _size.x];
    }

    T& operator()(int x, int y)
    {
        return data[x + y * _size.x];
    }

    const T& operator()(ivec2 position) const
    {
        return data[position.x + position.y * _size.x];
    }

    const T& operator()(int x, int y) const
    {
        return data[x + y * _size.x];
    }

    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.end(); }

private:
    ivec2 _size{0, 0};
    std::vector<T> data;
};

}
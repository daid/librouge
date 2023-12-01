#pragma once

#include "r/vec.h"
#include "random.h"
#include <cmath>


namespace r {

enum class Direction {
    Up, Right, Down, Left,
};

static inline constexpr Direction operator+(Direction d, int n) {
    return static_cast<Direction>((static_cast<int>(d) + n) & 3);
}
static inline constexpr Direction operator-(Direction d, int n) {
    return static_cast<Direction>((static_cast<int>(d) - n) & 3);
}

static inline glm::ivec2 offset(Direction dir) {
    switch(dir) {
    case Direction::Up: return {0, -1};
    case Direction::Down: return {0, 1};
    case Direction::Left: return {-1, 0};
    case Direction::Right: return {1, 0};
    }
    return {0, 0};
}

static inline Direction toDirection(glm::ivec2 offset) {
    if (std::abs(offset.x) >= std::abs(offset.y)) {
        if (offset.x > 0) return Direction::Right;
        return Direction::Left;
    }
    if (offset.y > 0) return Direction::Down;
    return Direction::Up;
}

static inline Direction randomDirection() {
    return static_cast<Direction>(irandom(0, 3));
}

class DirectionIterator
{
public:
    class Iterator
    {
    public:
        Direction operator*() { return dir; }
        void operator++() { dir = static_cast<Direction>(static_cast<int>(dir) + 1); }
        bool operator!=(const Iterator& other) { return dir != other.dir; }

        Direction dir;
    };
    Iterator begin() { return Iterator{static_cast<Direction>(0)}; }
    Iterator end() { return Iterator{static_cast<Direction>(4)}; }
};


enum class Direction8 {
    Up, UpRight, Right, DownRight, Down, DownLeft, Left, UpLeft,
};

static inline constexpr Direction8 operator+(Direction8 d, int n) {
    return static_cast<Direction8>((static_cast<int>(d) + n) & 7);
}
static inline constexpr Direction8 operator-(Direction8 d, int n) {
    return static_cast<Direction8>((static_cast<int>(d) - n) & 7);
}

static inline glm::ivec2 offset(Direction8 dir) {
    switch(dir) {
    case Direction8::Up: return {0, -1};
    case Direction8::UpRight: return {1, -1};
    case Direction8::Right: return {1, 0};
    case Direction8::DownRight: return {1, 1};
    case Direction8::Down: return {0, 1};
    case Direction8::DownLeft: return {-1, 1};
    case Direction8::Left: return {-1, 0};
    case Direction8::UpLeft: return {-1, -1};
    }
    return {0, 0};
}

static inline Direction8 randomDirection8() {
    return static_cast<Direction8>(irandom(0, 7));
}

class Direction8Iterator
{
public:
    class Iterator
    {
    public:
        Direction8 operator*() { return dir; }
        void operator++() { dir = static_cast<Direction8>(static_cast<int>(dir) + 1); }
        bool operator!=(const Iterator& other) { return dir != other.dir; }

        Direction8 dir;
    };
    Iterator begin() { return Iterator{static_cast<Direction8>(0)}; }
    Iterator end() { return Iterator{static_cast<Direction8>(8)}; }
};

}

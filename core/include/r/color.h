#pragma once

#include <cmath>
#include <algorithm>

namespace r {

class Color;

class HsvColor
{
public:
    HsvColor()
    : hue(0), saturation(0), value(0)
    {
    }

    HsvColor(float h, float s, float v)
    : hue(std::clamp(h, 0.0f, 360.0f)), saturation(std::clamp(s, 0.0f, 100.0f)), value(std::clamp(v, 0.0f, 100.0f))
    {
    }
    
    // hue: 0-360
    // saturation: 0-100
    // value: 0-100
    float hue;
    float saturation;
    float value;
};

//RGB color in range 0.0-1.0
class Color
{
public:
    float r, g, b;

    Color()
    : r(0), g(0), b(0)
    {
    }

    Color(float red, float green, float blue)
    : r(std::clamp(red, 0.0f, 1.0f)), g(std::clamp(green, 0.0f, 1.0f)), b(std::clamp(blue, 0.0f, 1.0f))
    {
    }

    Color(const HsvColor& hsv)
    {
        float c = hsv.value / 100.0f * hsv.saturation / 100.0f;
        float x = c * (1.0f - std::abs(std::fmod((hsv.hue / 60.0f), 2.0f) - 1.0f));
        float m = hsv.value / 100.0f - c;

        if (hsv.hue < 60) {
            r = (c + m);
            g = (x + m);
            b = m;
        } else if (hsv.hue < 120) {
            r = (x + m);
            g = (c + m);
            b = m;
        } else if (hsv.hue < 180) {
            r = m;
            g = (c + m);
            b = (x + m);
        } else if (hsv.hue < 240) {
            r = m;
            g = (x + m);
            b = (c + m);
        } else if (hsv.hue < 300) {
            r = (x + m);
            g = m;
            b = (c + m);
        } else {
            r = (c + m);
            g = m;
            b = (x + m);
        }
    }

    Color operator+(const Color& other) const { return Color(r + other.r, g + other.g, b + other.b); }
    Color operator*(const Color& other) const { return Color(r * other.r, g * other.g, b * other.b); }
    Color operator*(float f) const { return Color(r * f, g * f, b * f); }

    Color& operator+=(const Color& other) { r = std::clamp(r + other.r, 0.0f, 1.0f); g = std::clamp(g + other.g, 0.0f, 1.0f); b = std::clamp(b + other.b, 0.0f, 1.0f); return *this; }
    Color& operator*=(const Color& other) { r = std::clamp(r * other.r, 0.0f, 1.0f); g = std::clamp(g * other.g, 0.0f, 1.0f); b = std::clamp(b * other.b, 0.0f, 1.0f); return *this; }
    Color& operator*=(float f) { r = std::clamp(r * f, 0.0f, 1.0f); g = std::clamp(g * f, 0.0f, 1.0f); b = std::clamp(b * f, 0.0f, 1.0f); return *this; }

    Color max(const Color& other) const { return Color(std::max(r, other.r), std::max(g, other.g), std::max(b, other.b)); }

    bool operator==(const Color& other) { return r == other.r && b == other.b && g == other.g; }
    bool operator!=(const Color& other) { return r != other.r || b != other.b || g != other.g; }
};

}

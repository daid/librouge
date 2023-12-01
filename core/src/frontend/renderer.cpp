#include "r/frontend/renderer.h"

namespace r::frontend {

void Renderer::draw(Recti rect, char c, Color forground_color, Color background_color)
{
    for(auto p : rect) {
        draw(p, c, forground_color, background_color);
    }
}

void Renderer::drawBox(Recti rect, Color forground_color, Color background_color) {
    for(auto p : rect) {
        draw(p, ' ', forground_color, background_color);
    }
    for(int x=rect.position.x; x<rect.position.x+rect.size.x; x++) {
        draw({x, rect.position.y}, '-', forground_color, background_color);
        draw({x, rect.position.y + rect.size.y - 1}, '-', forground_color, background_color);
    }
    for(int y=rect.position.y; y<rect.position.y+rect.size.y; y++) {
        draw({rect.position.x, y}, '|', forground_color, background_color);
        draw({rect.position.x + rect.size.x - 1, y}, '|', forground_color, background_color);
    }
    draw({rect.position.x, rect.position.y}, '+', forground_color, background_color);
    draw({rect.position.x + rect.size.x - 1, rect.position.y}, '+', forground_color, background_color);
    draw({rect.position.x, rect.position.y + rect.size.y - 1}, '+', forground_color, background_color);
    draw({rect.position.x + rect.size.x - 1, rect.position.y + rect.size.y - 1}, '+', forground_color, background_color);
}

void Renderer::print(ivec2 position, const char* fmt, ...)
{
    for(char c = *fmt; *fmt; c = *++fmt) {
        draw(position, c, {1, 1, 1});
        position.x += 1;
    }
}

}
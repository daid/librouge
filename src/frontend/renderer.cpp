#include "r/frontend/renderer.h"

namespace r::frontend {
    
void Renderer::drawBox(Rect2i rect, Color forground_color, Color background_color) {
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

}
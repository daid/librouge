#include "r/engine.h"
#include "r/keys.h"
#include "r/actionqueue.h"
#include "r/vector2.h"
#include "r/random.h"
#include "r/line.h"
#include "r/fov.h"
#include <stdio.h>

r::ActionQueue<int> queue;
r::Engine engine({.minimal_screen_size{40, 30}});

class Tile {
public:
    enum Type {
        Floor,
        Wall,
        Door,
    } type = Type::Wall;
    int glyph() { return ".#+"[int(type)]; }
};
r::Vector2<Tile> map;

class MainState : public r::GameState
{
public:
    void onUpdate(float delta) override { (void)delta; }

    void onRender(r::frontend::Renderer& renderer) override {
        auto camera_offset = renderer.size() / 2 - position;
        renderer.draw(r::Recti{{}, renderer.size()}, ' ', {1, 1, 1}, {0.1,0.1,0.1});
        for(auto p : r::Recti{{}, map.size()}) {
            renderer.draw(p + camera_offset, map[p].glyph(), {0.3, 0.3, 0.3}, {0.1,0.1,0.1});
        }
        r::VisitFOV(position, 10, [&](auto p) {
            if (p.x < 0 || p.y < 0 || p.x >= map.size().x || p.y >= map.size().y) return false;
            renderer.draw(p + camera_offset, map[p].glyph(), {1, 1, 1}, {0.1,0.1,0.1});
            return map[p].type != Tile::Type::Wall;
        });
        renderer.draw(position + camera_offset, '@', {1,1,1});
    }
    
    void onKey(int key) override {
        switch(key) {
        case r::KEY_ESCAPE: engine.popState(); break;
        case r::KEY_LEFT: position.x -= 1; break;
        case r::KEY_RIGHT: position.x += 1; break;
        case r::KEY_UP: position.y -= 1; break;
        case r::KEY_DOWN: position.y += 1; break;
        }
    }

    r::ivec2 position{4,4};
};

int main(int argc, char** argv) {
    (void)argc; (void)argv;

    map.resize({40, 30});
    r::Vector2<r::vec2> centers;
    centers.resize(map.size() / 10);
    for(int x=0;x<map.size().x; x+=10) {
        for(int y=0;y<map.size().y; y+=10) {
            int w = r::irandom(3, 5);
            int h = r::irandom(3, 5);
            int rx = r::irandom(0, 9 - w);
            int ry = r::irandom(0, 9 - h);
            for(auto p : r::Recti{x+rx,y+ry, w, h})
                map[p].type = Tile::Type::Floor;
            centers[{x/10,y/10}] = {x + rx + w / 2, y + ry + h / 2};
        }
    }
    for(int x=0;x<centers.size().x-1;x++) {
        for(int y=0;y<centers.size().y-1;y++) {
            for(auto p : r::TraceLine(centers[{x, y}], centers[{x + 1, y}])) {
                map[p].type = Tile::Type::Floor;
            }
            for(auto p : r::TraceLine(centers[{x, y}], centers[{x, y + 1}])) {
                map[p].type = Tile::Type::Floor;
            }
        }
    }

    engine.pushState<MainState>();
    engine.run();
    return 0;
}

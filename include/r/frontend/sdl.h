#pragma once

#include "r/engine.h"
#include "r/frontend/base.h"
#include "r/vector2.h"

namespace r::frontend {

class SDL : public Frontend {
public:
    SDL(const Engine::Config& config);

    void processEvents(GameState& gamestate) override;
    void present() override;
    bool isOpen() override;

    ivec2 size() override;
    void draw(ivec2 position, char c, Color forground_color) override;
    void draw(ivec2 position, char c, Color forground_color, Color background_color) override;

private:
    ivec2 minimal_size{1, 1};
    struct Tile {
        char c{0};
        Color forground_color;
        Color background_color;
    };
    Vector2<Tile> tiles;
};

}
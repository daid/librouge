#pragma once

#include "r/engine.h"
#include "r/frontend/base.h"
#include "r/vector2.h"

namespace r::frontend {

class Terminal : public Frontend {
public:
    Terminal(const Engine::Config& config);

    void processEvents(GameState& gamestate) override;
    void present() override;
    bool isOpen() override;

    ivec2 size() override;
    void draw(ivec2 position, char c, Color forground_color) override;
    void draw(ivec2 position, char c, Color forground_color, Color background_color) override;

private:
    struct Tile {
        char c{0};
        Color forground_color;
        Color background_color;

        char previous_c{0};
        Color previous_forground_color;
        Color previous_background_color;
    };
    Vector2<Tile> tiles;
    ivec2 cursor_pos{-1, -1};
    bool quit = false;
};

}
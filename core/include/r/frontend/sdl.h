#pragma once

#include "r/engine.h"
#include "r/frontend/base.h"
#include "r/vector2.h"

class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

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
    void draw(ivec2 position, Color background_color) override;

private:
    ivec2 mousePositionToTilePosition(int x, int y);

    ivec2 minimal_size{1, 1};
    struct Tile {
        char c{0};
        Color forground_color;
        Color background_color;
    };
    Vector2<Tile> tiles;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *font_texture;
    ivec2 glyph_size{8, 8};
    bool quit = false;
};

}
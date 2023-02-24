#pragma once

#include "r/frontend/renderer.h"

namespace r {

class GameState {
public:
    virtual void render(frontend::Renderer& renderer) = 0;
    virtual void onKey(int key) = 0;
};

}
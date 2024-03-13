#pragma once

#include "r/frontend/renderer.h"
#include "r/gamestate.h"


namespace r::frontend {

class Frontend : public Renderer {
public:
    virtual void processEvents(GameState& gamestate, bool blocking) = 0;
    virtual void present() = 0;
    virtual bool isOpen() = 0;
};

}

#pragma once

#include "r/frontend/renderer.h"

namespace r {

/*
    A GameState is the primary interface from the engine to the game.
    It is pushed on the running engine as a stack, the topmost GameState is active,
    this allows for easy handling of menus, and other potential modal interactions.

    All gamestates are always rendered, but only the topmost gets update and keypress calls.
*/
class GameState {
public:
    virtual ~GameState() {}

    // onUpdate is called every frame with the frame delta time, if this State is the topmost state
    virtual void onUpdate(float delta) { (void)delta; }
    // onFixedUpdate is called 60 times per second, if this State is the topmost state
    virtual void onFixedUpdate() {}
    // onRender is called every frame to render this state to the screen.
    virtual void onRender(frontend::Renderer& renderer) { (void)renderer; }
    // onKey is called every time the player pressed a key on the keyboard, if this State is the topmost state
    // For ascii based keys, the value is the ascii value. 
    virtual void onKey(int key) { (void)key; }

    virtual void onMouseDown(ivec2 pos, int button) { (void)pos; (void)button; }
    virtual void onMouseMove(ivec2 pos, int button) { (void)pos; (void)button; }
    virtual void onMouseUp(ivec2 pos, int button) { (void)pos; (void)button; }
    virtual void onMouseWheel(ivec2 pos, int amount) { (void)pos; (void)amount; }
};

}

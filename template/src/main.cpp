#include "r/engine.h"
#include "r/keys.h"
#include <stdio.h>


r::Engine engine({});

class MainState : public r::GameState
{
public:
    void onUpdate(float delta) { }

    void onRender(r::frontend::Renderer& renderer) override {
        renderer.draw(r::Recti{{}, renderer.size()}, '.', {0.2, 0.2, 0.2}, {0.1,0.1,0.1});
        renderer.draw(position, '@', {1,1,1});
    }
    
    void onKey(int key) {
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
    engine.pushState<MainState>();
    engine.run();
    return 0;
}

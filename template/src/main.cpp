#include "r/engine.h"
#include <stdio.h>


r::Engine engine({});

class MainState : public r::GameState
{
public:
    void render(r::frontend::Renderer& renderer) override {
        renderer.drawBox({{0, 0}, renderer.size()}, {1, 1, 1}, {0, 0, 0});
    }
    void onKey(int key) {
        printf("%02x\n", key);
        if (key == 0x1B) engine.popState();
    }
};

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    engine.pushState<MainState>();
    engine.run();
    return 0;
}

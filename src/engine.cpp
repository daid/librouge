#include "r/engine.h"
#include "r/frontend/sdl.h"

namespace r {

Engine::Engine(const Config& config_) : config(config_) {
}

void Engine::run()
{
    frontend = std::make_unique<frontend::SDL>(config);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(Engine::update, this, 0, 1);
#else
    while(frontend->isOpen() && !state_stack.empty()) {
        update(this);
    }
#endif
}

void Engine::update(Engine* engine)
{
    for(auto& state : engine->state_stack) {
        state->render(*engine->frontend);
    }
    if (!engine->state_stack.empty())
        engine->frontend->processEvents(*engine->state_stack.back());
    engine->frontend->present();
    engine->dead_states.clear();
}

void Engine::popState()
{
    //Cannot just delete the state here, as it is likely we are being called from the state, so move it to the dead states for later deletion.
    dead_states.push_back(std::move(state_stack.back()));
    state_stack.pop_back();
}

}

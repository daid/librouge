#include "r/engine.h"
#include "r/frontend/sdl.h"
#include "r/frontend/terminal.h"
#include <string.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


namespace r {

Engine::Engine(const Config& config_) : config(config_) {
}

void Engine::run()
{
    if (auto t = getenv("ROUGE_FRONTEND")) {
        if (strcmp(t, "SDL") == 0) frontend = std::make_unique<frontend::SDL>(config);
        if (strcmp(t, "term") == 0) frontend = std::make_unique<frontend::Terminal>(config);
    }
    if (!frontend)
        frontend = std::make_unique<frontend::SDL>(config);
    if (!frontend->isOpen())
        frontend = std::make_unique<frontend::Terminal>(config);

#ifdef __EMSCRIPTEN__
    config.continuous_loop = true;
    emscripten_set_main_loop_arg(Engine::staticUpdate, this, 0, 1);
#else
    while(frontend->isOpen() && !state_stack.empty()) {
        update();
    }
#endif
}

void Engine::staticUpdate(void* engine)
{
    static_cast<Engine*>(engine)->update();
}

void Engine::update()
{
    auto now = std::chrono::steady_clock::now();
    auto time_delta = std::chrono::duration<float>(now - last_update).count();
    last_update = now;
    time_delta = std::min(time_delta, 0.5f);
    
    //Run updates on top most state.
    if (config.continuous_loop) {
        fixed_update_accumulator += time_delta;
        static constexpr auto fixed_update_delta = 1.0f / 60.0f;
        while(fixed_update_accumulator > fixed_update_delta)
        {
            fixed_update_accumulator -= fixed_update_delta;
            if (!state_stack.empty())
            state_stack.back()->onFixedUpdate();
        }
    }
    if (!state_stack.empty())
        state_stack.back()->onUpdate(time_delta);

    // Process frontend events (keypress/mouse/etc...)
    if (!state_stack.empty())
        frontend->processEvents(*state_stack.back(), !config.continuous_loop);

    // Render all states from bottom to top, so topmost state is rendered last.
    for(auto& state : state_stack) {
        state->onRender(*frontend);
    }
    frontend->present();

    // Actually delete states that where poped.
    dead_states.clear();
}

void Engine::popState()
{
    //Cannot just delete the state here, as it is highly likely we are being called from the state,
    // so move it to the dead states for later deletion.
    dead_states.push_back(std::move(state_stack.back()));
    state_stack.pop_back();
}

}

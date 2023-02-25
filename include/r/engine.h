#pragma once

#include "r/vec.h"
#include "r/gamestate.h"
#include "r/frontend/base.h"
#include <memory>
#include <vector>
#include <chrono>


namespace r {

class Engine {
public:
    struct Config {
        ivec2 minimal_screen_size{80,25};
    };

    Engine(const Config& config);
    void run();
    template<typename T, typename ... Args> void pushState(Args&& ... args) {
        state_stack.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }
    void popState();
private:
    static void staticUpdate(void* engine);
    void update();

    Config config;
    std::unique_ptr<frontend::Frontend> frontend;
    std::vector<std::unique_ptr<GameState>> state_stack;
    std::vector<std::unique_ptr<GameState>> dead_states;

    std::chrono::steady_clock::time_point last_update = std::chrono::steady_clock::now();
    float fixed_update_accumulator = 0.0f;
};

}
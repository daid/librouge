#pragma once

#include "r/vec.h"
#include "r/gamestate.h"
#include "r/frontend/base.h"
#include <memory>
#include <vector>


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
    static void update(Engine* engine);

    Config config;
    std::unique_ptr<frontend::Frontend> frontend;
    std::vector<std::unique_ptr<GameState>> state_stack;
    std::vector<std::unique_ptr<GameState>> dead_states;
};

}
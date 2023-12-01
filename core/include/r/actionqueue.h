#pragma once

#include <vector>
#include <algorithm>


namespace r {

template<typename T> class ActionQueue {
public:
    void push(T&& value, unsigned int delay) {
        auto it = std::upper_bound(queue.begin(), queue.end(), time_offset + delay, [](const auto& a, const auto& b) {
            return a < b.first;
        });
        queue.insert(it, {time_offset + delay, std::forward<T>(value)});
    }

    T pop() {
        auto& front = queue.front();
        T result = std::move(front.second);
        time_offset = front.first;
        queue.erase(queue.begin());
        return result;
    }

    bool empty() {
        return queue.empty();
    }

private:
    unsigned int time_offset = 0;
    std::vector<std::pair<unsigned int, T>> queue;
};

}
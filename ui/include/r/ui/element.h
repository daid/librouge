#pragma once

#include "r/rect.h"
#include "layout.h"
#include <vector>
#include <memory>


namespace r::frontend { class Renderer; }

namespace r::ui {

class Root;
class Element {
public:
    LayoutSettings layout;

    Element() {}
    Element(const LayoutSettings& ls) : layout(ls) {}
    virtual ~Element();

    template<typename T, typename... ARGS> T* add(ARGS&&... args) {
        auto ptr = std::make_unique<T>(std::forward<ARGS>(args)...);
        auto ret = ptr.get();
        children.emplace_back(std::move(ptr));
        return ret;
    }

    template<typename T, typename... ARGS> void layoutManager(ARGS&&... args) {
        layout_manager = std::make_unique<T>(std::forward<ARGS>(args)...);
    }

    // Should be called from layout manager
    void updateRenderingRect(Recti r);
protected:
    const Recti& rect() { return rendering_rect; }

private:
    Element(const Element&) = delete;
    Element& operator=(const Element&) = delete;

    Recti rendering_rect; // Rendering area, set by layouting.

    virtual void draw(r::frontend::Renderer& r);

    std::unique_ptr<Layout> layout_manager;
    std::vector<std::unique_ptr<Element>> children;

    friend class Root;
};

}

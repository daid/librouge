#include "r/ui/root.h"
#include "r/frontend/renderer.h"

namespace r::ui {

Root::Root() {
}

void Root::render(r::frontend::Renderer& r) {
    updateRenderingRect({{0, 0}, r.size()});
    layoutRecursive(this);
    renderRecursive(r, this);
}

void Root::layoutRecursive(Element* e)
{
    auto r = e->rendering_rect;
    r.position += ivec2{e->layout.padding.left, e->layout.padding.top};
    r.size -= ivec2{e->layout.padding.left + e->layout.padding.right, e->layout.padding.top + e->layout.padding.bottom};
    if (e->layout_manager) {
        e->layout_manager->update(e->children, r);
    } else {
        for(auto& c : e->children)
            c->updateRenderingRect(r);
    }
    for(auto& c : e->children)
        layoutRecursive(c.get());
}

void Root::renderRecursive(r::frontend::Renderer& r, Element* e)
{
    e->draw(r);
    for(auto& c : e->children)
        renderRecursive(r, c.get());
}

}

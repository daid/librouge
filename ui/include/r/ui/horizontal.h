#pragma once

#include "layout.h"

namespace r::ui {

class Horizontal : public Layout {
public:
    void update(std::vector<std::unique_ptr<Element>>&, Recti r) override;
};

}

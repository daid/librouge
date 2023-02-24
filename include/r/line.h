#pragma once

#include "r/vec.h"
#include <functional>

namespace r {

void traceLine(ivec2 start, ivec2 end, std::function<bool(ivec2)> callback);

}

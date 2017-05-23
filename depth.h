#pragma once

#include <vector>
#include "image.h"

namespace mycv {

Image getDisparityMap(const Image& _first, const Image& _second, int _window_size_px, int _min_disparity, int _max_disparity);

}

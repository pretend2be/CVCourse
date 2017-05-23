#include "depth.h"

namespace mycv {

Image getDisparityMap(const Image& _first, const Image& _second, int _window_size_px, int _min_disparity, int _max_disparity){
    auto H = _first.getHeight(), W = _first.getWidth();
    auto half = _window_size_px / 2;
    int max_disparity = _max_disparity - _min_disparity;
    Image result(H,W);

    for(int i = 0; i < H; i++)
        for(int j = 0; j < W; j++){
            auto s = std::numeric_limits<double>::max();
            int disparity;
            for(int k = j > max_disparity ? j - max_disparity : 0; k < W; k++){
                auto sum = 0.0;
                for(int u = -half; u <= half; u++)
                    for(int v = -half; v <= half; v++){
                        auto p = _first.get(i + u, j + v);
                        auto l = _second.get(i + u, k + v);
                        double q = p - l;
                        sum += fabs(q);
                        if(sum > s) goto k1;
                    }
                k1:
                if( sum < s ) {
                    s = sum;
                    disparity = j - k;
                    if(disparity < _min_disparity){
                        disparity = _min_disparity;
                        goto t1;
                    }
                }
            }
            if(disparity > max_disparity) disparity = _min_disparity;
            t1:
            result.set(i, j, disparity);
        }
    return result;
}

}

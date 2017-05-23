#include "qimagemaker.h"

namespace mycv {

QImage toQImage(const Image& image){
    auto H = image.getHeight();
    auto W = image.getWidth();
    QImage pic(W, H, QImage::Format_RGB32);

    for(int y = 0; y < H; y++)
        for(int x = 0; x < W; x++){
            auto color = int(image.get(y, x) * 255);
            pic.setPixel(x, y, qRgb(color, color, color));
        }

    return pic;
}

}

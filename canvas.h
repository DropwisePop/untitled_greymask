#ifndef CANVAS_H
#define CANVAS_H


#include <QList>
#include "layer.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;



class Canvas
{
public:
    Canvas();

    enum CanvasColorMode { GREY, BGRA };

    const Layer& getLayer(int index);
    //I AM CONFUSED WHY THIS (const ref return) WORKS
    //if it is a const reference, why can I apply lumaToAlpha()
    Mat getLayerComposite(Layer base, Layer over, Layer::PreOrPostCode code);
    void startCanvas(Mat base, CanvasColorMode mode);

    static Mat makeCheckMat(int rows, int cols, CanvasColorMode mode);

private:
    CanvasColorMode mCanvasColorMode;
    QList<Layer> mLayerSet;
    Mat mLastLayerComposite;
};



#endif // CANVAS_H

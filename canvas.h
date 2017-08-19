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

    Layer getLayer(int index);
    Mat getLayerComposite(Layer base, Layer over, Layer::PreOrPostCode code);
    void startCanvas(Mat base, CanvasColorMode mode);


private:
    CanvasColorMode mCanvasColorMode;
    QList<Layer> mLayerSet;
    Mat mLastLayerComposite;
    Mat makeCheckMat(int rows, int cols, CanvasColorMode mode);
};



#endif // CANVAS_H

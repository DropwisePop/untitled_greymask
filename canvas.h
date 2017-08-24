#ifndef CANVAS_H
#define CANVAS_H

#include <QList>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
class Layer;

class Canvas
{
public:
    Canvas();

    static Mat makeCheckMat(int rows, int cols);

    void startCanvas(Mat base);
    Mat getMatComposite(Mat base, Mat over);
    Layer& getLayer(int index);

    int getCeiling();
    void setCeiling(int);
    int getFloor();
    void setFloor(int);

private:
    QList<Layer> mLayerSet;
    Mat mLastLayerComposite;

    int mCeiling;
    int mFloor;
};

#endif // CANVAS_H

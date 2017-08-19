#ifndef LAYER_H
#define LAYER_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

class Layer
{
public:
    Layer(Mat matPrePost);

    enum PreOrPostCode {PRE, POST};

    Mat getPreEffectMat();
    Mat getPostEffectMat();
    void setPreEffectMat(Mat mat);
    void setPostEffectMat(Mat mat);

    //EFFECTS...
    void lumaToAlpha();

private:
    Mat mPreEffectMat;
    Mat mPostEffectMat;
};

#endif // LAYER_H

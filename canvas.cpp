#include "canvas.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <QDebug>

Canvas::Canvas()
{
    mCanvasColorMode = Canvas::BGRA;
}

const Layer& Canvas::getLayer(int index)
{
    if(0 <= index && index < mLayerSet.size()){
        return mLayerSet.at(index);
    }
    throw std::invalid_argument("layer index not in range");
}

Mat Canvas::getLayerComposite(Layer base, Layer over, Layer::PreOrPostCode code)
{
    //THE HARD PART
}

void Canvas::startCanvas(Mat base, CanvasColorMode mode)
{
    mLayerSet.clear();

    if (base.channels() == 1 && mode == Canvas::BGRA){
        cvtColor(base, base, CV_GRAY2BGRA);
    } else if (base.channels() > 1 && mode == Canvas::GREY){
        cvtColor(base, base, CV_BGRA2GRAY);
    }

    Layer layer00(makeCheckMat(base.rows, base.cols, mode));
    Layer layer01(base);

    mLayerSet.append(layer00);
    mLayerSet.append(layer01);
}

Mat Canvas::makeCheckMat(int rows, int cols, CanvasColorMode mode)
{
    int checkSize = 8;
    Mat checkMat;
    if (mode == Canvas::BGRA){
        checkMat = Mat(rows, cols, CV_8UC4);
    } else {
        checkMat = Mat(rows, cols, CV_8UC1);
    }

    uchar color = 0;
    uchar colorAtStartOfRow = color;
    for(int i = 0 ; i < rows ; i += checkSize ){

        if (colorAtStartOfRow == 255){
            color = 205;
        } else {
            color = 255;
        }
        colorAtStartOfRow = color;

        for(int j = 0 ; j < cols ; j = j + checkSize ){

            Point topLeft(j, i);
            int bottomRightX = j + checkSize;
            int bottomRightY = i + checkSize;
            if ( bottomRightX > cols ){
                bottomRightX = cols;
            }
            if ( bottomRightY > rows ){
                bottomRightY = rows;
            }
            Point bottomRight(bottomRightX, bottomRightY);

            Mat ROI = checkMat( Rect(topLeft, bottomRight) );
            if(checkMat.channels() == 4){
                ROI.setTo(Scalar(color, color, color, 255));
            } else {
                ROI.setTo(Scalar::all(color));
            }

            if (color == 255){
                color = 205;
            } else {
                color = 255;
            }
        }
    }

    return checkMat;
}



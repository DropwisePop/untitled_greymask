#include <opencv2/imgproc/imgproc.hpp>
#include "canvas.h"
#include "layer.h"

#include <QDebug>

Canvas::Canvas(): mCeiling(255), mFloor(0)
{
    //empty
}

void Canvas::startCanvas(Mat base)
{
    //TODO: this NEEDS to delete the layers it contains
    mLayerSet.clear();

    Layer layer00(makeCheckMat(base.rows, base.cols), *this);
    Layer layer01(base, *this);

    mLayerSet.append(layer00);
    mLayerSet.append(layer01);
}

Mat Canvas::getMatComposite(Mat base, Mat over)
{
    //THE HARD PART
}

Layer& Canvas::getLayer(int index)
{
    if(0 <= index && index < mLayerSet.size()){
        return mLayerSet[index];
    }
    throw std::invalid_argument("layer index not in range");
}

int Canvas::getCeiling()
{
    return mCeiling;
}

void Canvas::setCeiling(int ceiling)
{
    mCeiling = ceiling;
}

int Canvas::getFloor()
{
    return mFloor;
}

void Canvas::setFloor(int floor)
{
    mFloor = floor;
}

Mat Canvas::makeCheckMat(int rows, int cols)
{
    int checkSize = 8;
    Mat checkMat(rows, cols, CV_8UC4);

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
            ROI.setTo(Scalar(color, color, color, 255));

            if (color == 255){
                color = 205;
            } else {
                color = 255;
            }
        }
    }
    return checkMat;
}





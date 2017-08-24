#include <opencv2/imgproc/imgproc.hpp>
#include "layer.h"
#include <QDebug>



Layer::Layer(Mat matPrePost, Canvas& parentCanvas):
    mParentCanvas(parentCanvas)
{
    if (matPrePost.channels() == 1){
        cvtColor(matPrePost, matPrePost, CV_GRAY2RGBA);
    }
    mPreEffectMat = matPrePost;
    mPostEffectMat = matPrePost;    //TODO: attempt clone
}

Mat Layer::getPreEffectMat()
{
    return mPreEffectMat;
}

Mat Layer::getPostEffectMat()
{
    return mPostEffectMat;
}

void Layer::lumaToAlpha()
{
    qDebug() << "lumaToAlpha() called";
    //TODO: this is NOT IDEAL but replacing messes up the contrast
    Mat post(mPreEffectMat.rows, mPreEffectMat.cols, CV_8UC4, Scalar::all(0) );

    //put luma of pre into alpha of post
    const int step = post.channels();
    const int alphaChannelIndex = 3;
    for (int i = 0; i < post.rows; i++){
        uchar *prow_mat   = mPreEffectMat.ptr(i);
        uchar *prow_matMask = post.ptr(i);
        for (int j = 0, k = 0; j!= post.cols * step; j += step, k++){
            prow_matMask[j + alphaChannelIndex] = ~prow_mat[j];
            //TODO: map values
    }
    mPostEffectMat = post;
}


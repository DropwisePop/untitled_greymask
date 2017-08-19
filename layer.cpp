#include "layer.h"

#include <opencv2/imgproc/imgproc.hpp>

Layer::Layer(Mat matPrePost)
{
    mPreEffectMat = matPrePost;
    mPostEffectMat = matPrePost;
}

Mat Layer::getPreEffectMat()
{
    return mPreEffectMat;
}

Mat Layer::getPostEffectMat()
{
    return mPostEffectMat;

}

void Layer::setPreEffectMat(Mat mat)
{
    mPreEffectMat = mat;

}

void Layer::setPostEffectMat(Mat mat)
{
    mPostEffectMat = mat;
}

void Layer::lumaToAlpha()
{
    Mat pre = getPreEffectMat();
    if(pre.channels() > 1){
        cvtColor(pre, pre, CV_BGRA2GRAY);
    }
    Mat post= Mat(pre.rows, pre.cols, CV_8UC4, Scalar::all(0) );

    //put luma of pre into alpha of post
    const int step = post.channels();
    const int alphaChannelIndex = 3;
    for (int i = 0; i < post.rows; i++){
        uchar *prow_mat   = pre.ptr(i);
        uchar *prow_matMask = post.ptr(i);
        for (int j = 0, k = 0; j!= post.cols * step; j += step, k++){
            prow_matMask[j + alphaChannelIndex] = ~prow_mat[k];
        }
    }

    setPostEffectMat(post);
}


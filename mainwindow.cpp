#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QShortcut>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    new QShortcut(QKeySequence(Qt::Key_O), this, SLOT(showFileDialog()) );
    new QShortcut(QKeySequence(Qt::Key_Right), this, SLOT(showNextImage()) );
    new QShortcut(QKeySequence(Qt::Key_Left), this, SLOT(showPrevImage()) );
    new QShortcut(QKeySequence(Qt::Key_S), this, SLOT(saveMat()) );


    mImageFileListIndex = 0;
    mImageFileList.append("C:/pics/boats.jpg");

    //loadImageFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showFileDialog()
{
    QStringList fileList = QFileDialog::getOpenFileNames(
                        this,
                        "images to process...",
                        "C:/users/ben/Dropbox/z_gallery/commercial/",
                        "Images (*.png *.bmp *.jpg)");

    if (fileList.size() != 0)
        mImageFileList = fileList;
    mImageFileListIndex = 0;
    loadImageFile();
}

void MainWindow::showNextImage()
{
    mImageFileListIndex = (mImageFileListIndex + 1)
                           % mImageFileList.size();
    loadImageFile();
}

void MainWindow::showPrevImage()
{
    int i = (mImageFileListIndex - 1);
    int size = mImageFileList.size();
    mImageFileListIndex = (i % size + size) % size;

    loadImageFile();
}

//current implementation saves top layer
void MainWindow::saveMat()
{
    Mat resultMat = mCanvas.getLayer(1).getPostEffectMat();
    imwrite("C:/users/ben/desktop/bashing/out/"
            + std::to_string(std::time(0)) + ".png", resultMat);
}

void MainWindow::loadImageFile()
{
    QString str = mImageFileList.at(mImageFileListIndex);
    Mat mat = imread(str.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);

    mCanvas.startCanvas(mat, Canvas::BGRA);
    mCanvas.getLayer(1).lumaToAlpha();

    //Mat canvasMat = mCanvas.getLayerComposite(mCanvas.getLayer(0),
    //                                          mCanvas.getLayer(1),
    //                                          Layer::POST);

    Mat canvasMat = mCanvas.getLayer(1).getPostEffectMat();

    updateCanvasLabel(canvasMat);
}

void MainWindow::updateCanvasLabel(Mat mat)
{
    //assuming incoming mat always greyscale
    QPixmap pixmap;
    // Set the color table (used to translate colour indexes to qRgb values)
    QVector<QRgb> colorTable;
    for (int i=0; i<256; i++)
        colorTable.push_back(qRgb(i,i,i));
    // Copy input Mat
    const uchar *qImageBuffer = (const uchar*)mat.data;
    // Create QImage with same dimensions as input Mat
    QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
    img.setColorTable(colorTable);
    pixmap = QPixmap::fromImage(img);

    ui->imageLabel->setPixmap(pixmap);
    ui->imageLabel->setScaledContents(true);
}



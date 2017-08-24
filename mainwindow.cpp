#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "canvas.h"
#include "layer.h"
#include "utils.h"
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

    //shortcuts controlling value range for images in post-effect
    new QShortcut(QKeySequence(Qt::Key_BracketLeft),
                  this, SLOT(decrementCeiling()) );
    new QShortcut(QKeySequence(Qt::Key_BracketRight),
                  this, SLOT(incrementCeiling()) );
    new QShortcut(QKeySequence(Qt::Key_Semicolon),
                  this, SLOT(decrementFloor()) );
    new QShortcut(QKeySequence(Qt::Key_Apostrophe),
                  this, SLOT(incrementFloor()) );
    //increments by ten...
    new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_BracketLeft),
                  this, SLOT(decrementCeilingByTen()) );
    new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_BracketRight),
                  this, SLOT(incrementCeilingByTen()) );
    new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Semicolon),
                  this, SLOT(decrementFloorByTen()) );
    new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Apostrophe),
                  this, SLOT(incrementFloorByTen()) );

    //TODO: new default image
    mImageFileListIndex = 0;
    mImageFileList.append("C:/pics/boats.jpg");
 
    loadImageFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImageFile()
{
    QString str = mImageFileList.at(mImageFileListIndex);
    Mat mat = imread(str.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);

    mCanvas.startCanvas(mat);
    applyMatEffects();
    updateCanvasLabel();
}

void MainWindow::applyMatEffects(){
    mCanvas.getLayer(1).lumaToAlpha();
}

void MainWindow::showFileDialog()
{
    //TODO: save last opened folder
    //TODO: create default folder if it does not exist
    //TDOO: default directories should be GENERIC
    QStringList fileList = QFileDialog::getOpenFileNames(
                        this,
                        "images to process...",
                        "C:/users/ben/Dropbox/z_gallery/commercial/",
                        "Images (*.png *.bmp *.jpg)");

    //TODO: if fileList.size() is zero, show error dialog
    if (fileList.size() != 0)
        mImageFileList = fileList;
    mImageFileListIndex = 0;

    loadImageFile();
}

void MainWindow::showNextImage()
{
    //TODO: LOCK THESE until load is finished
    mImageFileListIndex = (mImageFileListIndex + 1)
                           % mImageFileList.size();
    loadImageFile();
}

void MainWindow::showPrevImage()
{
    //TODO: LOCK THESE until load is finished
    int i = (mImageFileListIndex - 1);
    int size = mImageFileList.size();
    mImageFileListIndex = (i % size + size) % size;

    loadImageFile();
}

void MainWindow::saveMat()
{
    //TODO: allow redefinition of outFolder
    //TODO: allow save only if image or image sliders have changed
    //TODO: if resultMat already exists and state has changed, append -02
    Mat resultMat = mCanvas.getLayer(1).getPostEffectMat();
    imwrite("C:/users/ben/desktop/bashing/out/"
            + std::to_string(std::time(0)) + ".png", resultMat);
}

void MainWindow::updateCanvasLabel()
{
    //TODO: proper composites depending on (enum?) previewmode
    //Mat mat = mCanvas.getLayerComposite(mCanvas.getLayer(0),
    //                                          mCanvas.getLayer(1),
    //                                          Layer::POST);

    Mat mat = mCanvas.getLayer(1).getPostEffectMat();

    QPixmap pixmap;
    if(mat.type()==CV_8UC1)
    {
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
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        pixmap = QPixmap::fromImage(img.rgbSwapped());
    }
    // 8-bits unsigned, NO. OF CHANNELS=4
    if(mat.type()==CV_8UC4)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGBA8888);    //RGBA or BGRA? potential problem
        pixmap = QPixmap::fromImage(img.rgbSwapped());
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage or QPixmap.";
        return;
    }

    //TODO: imageLabel should always draw image scaled correctly
    //TODO: imageLabel should always be square (if not in fullscreen)
    ui->imageLabel->setPixmap(pixmap);
    ui->imageLabel->setScaledContents(true);
}



//FLOOR, CEILING FUNCTIONS...
void MainWindow::decrementCeiling()
{
    int floor = mCanvas.getFloor();
    int ceiling = mCanvas.getCeiling();
    if (ceiling > floor){
        mCanvas.setCeiling(--ceiling);
        ui->ceilingLabel->setText(QString::number(ceiling));
    }
    applyMatEffects();
    updateCanvasLabel();
}

void MainWindow::incrementCeiling()
{
    int ceiling = mCanvas.getCeiling();
    if (ceiling < 255){
        mCanvas.setCeiling(++ceiling);
        ui->ceilingLabel->setText(QString::number(ceiling));
    }
    applyMatEffects();
    updateCanvasLabel();
}

void MainWindow::decrementFloor()
{
    int floor = mCanvas.getFloor();
    if (floor > 0){
        mCanvas.setFloor(--floor);
        ui->floorLabel->setText(QString::number(floor));
    }
    applyMatEffects();
    updateCanvasLabel();
}

void MainWindow::incrementFloor()
{
    int floor = mCanvas.getFloor();
    int ceiling = mCanvas.getCeiling();
    if (floor < ceiling){
        mCanvas.setFloor(++floor);
        ui->floorLabel->setText(QString::number(floor));
    }
    applyMatEffects();
    updateCanvasLabel();
}

void MainWindow::decrementCeilingByTen()
{
    int floor = mCanvas.getFloor();
    int ceiling = mCanvas.getCeiling();
    if (ceiling - 10 <= floor){
        ceiling = floor;
        mCanvas.setCeiling(ceiling);
    } else {
        mCanvas.setCeiling(ceiling -= 10);
    }
    ui->ceilingLabel->setText(QString::number(ceiling));

    applyMatEffects();
    updateCanvasLabel();
}

void MainWindow::incrementCeilingByTen()
{
    int ceiling = mCanvas.getCeiling();
    if (ceiling + 10 >= 255){
        ceiling = 255;
        mCanvas.setCeiling(ceiling);
    } else {
        mCanvas.setCeiling(ceiling += 10);
    }
    ui->ceilingLabel->setText(QString::number(ceiling));

    applyMatEffects();
    updateCanvasLabel();
}

void MainWindow::decrementFloorByTen()
{
    int floor = mCanvas.getFloor();
    if (floor - 10 <= 0){
        floor = 0;
        mCanvas.setFloor(floor);
    } else {
        mCanvas.setFloor(floor -= 10);
    }
    ui->floorLabel->setText(QString::number(floor));

    applyMatEffects();
    updateCanvasLabel();
}

void MainWindow::incrementFloorByTen()
{
    int floor = mCanvas.getFloor();
    int ceiling = mCanvas.getCeiling();
    if (floor + 10 >= ceiling){
        floor = ceiling;
        mCanvas.setFloor(floor);
    } else {
        mCanvas.setFloor(floor += 10);
    }
    ui->floorLabel->setText(QString::number(floor));

    applyMatEffects();
    updateCanvasLabel();
}
//END FLOOR, CEILING FUNCTIONS.





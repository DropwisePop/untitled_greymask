#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStringList>
#include <QMainWindow>
#include "canvas.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void showFileDialog();
    void showNextImage();
    void showPrevImage();
    void saveMat();


private:

    Ui::MainWindow *ui;

    void loadImageFile();
    void updateCanvasLabel(Mat mat);

    QStringList mImageFileList;
    int mImageFileListIndex;
    Canvas mCanvas;


};

#endif // MAINWINDOW_H

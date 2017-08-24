#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStringList>
#include <QLabel>
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

    void decrementCeiling();
    void incrementCeiling();
    void decrementFloor();
    void incrementFloor();
    void decrementCeilingByTen();
    void incrementCeilingByTen();
    void decrementFloorByTen();
    void incrementFloorByTen();

private:
    Ui::MainWindow *ui;

    void loadImageFile();
    void applyMatEffects();
    void updateCanvasLabel();

    QStringList mImageFileList;
    int mImageFileListIndex;
    Canvas mCanvas;
};

#endif // MAINWINDOW_H

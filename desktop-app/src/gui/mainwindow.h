#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>
#include <QShortcut>

#include <SDL.h>

#include <algorithm>
#include <mutex>
#include <memory>
#include "opencv2/opencv.hpp"

#include "file_storage.h"

#include "new_conference_wizard.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showCam();

protected:
    void closeEvent(QCloseEvent *event);
    void loadEffects();

private slots:
    void cameraSelector_activated();
    void showAboutBox();
    void refreshCams();
    void newConference();
    
private:
    Ui::MainWindow *ui;

    // Current image
    // When user click "Capture", we take photo here then have it
    // to [Photos] folder
    cv::Mat current_img;
    std::mutex current_img_mutex;

    // File Storage
    ml_cam::FileStorage fs;


    QGraphicsPixmapItem pixmap;
    cv::VideoCapture video;


    // Camera to use
    int MAX_CAMS = 5; // Max number of camera supported. This number used to scan cameras
    int current_camera_index = 0;
    int selected_camera_index = 0;

public:
    void setCurrentImage(const cv::Mat & img);
    cv::Mat getCurrentImage();
    void playShutter();

};

#endif // MAINWINDOW_H

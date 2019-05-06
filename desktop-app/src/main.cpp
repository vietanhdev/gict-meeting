#include <QApplication>
#include <stdlib.h>
#include <thread>
#include <iostream>
#include <string>
#include "filesystem_include.h"
#include "framelesswindow.h"
#include "DarkStyle.h"
#include "mainwindow.h"
#include "file_storage.h"
#include "video_stream_service.h"


int main(int argc, char *argv[]) {

    // Set camera paths
    if (argc == 2) {
        VideoStreamService & v = VideoStreamService::instance();
        v.setCameraPath(std::string(argv[1]));
    }


    QApplication a(argc, argv);

    // Style our application with custom dark style
    a.setStyle(new DarkStyle);

    // Create frameless window (and set windowState or title)
    FramelessWindow framelessWindow;
    framelessWindow.setWindowTitle("iCT Video Conference");
    framelessWindow.setWindowIcon(a.style()->standardIcon(QStyle::SP_DesktopIcon));
    
    // Create our mainwindow instance
    MainWindow &mainWindow = MainWindow::instance();

    // Add the mainwindow to our custom frameless window
    framelessWindow.setContent(&mainWindow);
    framelessWindow.show();

    return a.exec();
}

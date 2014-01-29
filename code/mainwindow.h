#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QPushButton;
class QTimer;

#include "head_tracking/facetrack.h"

class mainwindow : public QMainWindow
{
    Q_OBJECT

private:
    Facetrack tracker_;
    QLabel* webcamView_;
    QPixmap imgWebcam_;
    QTimer* timer_;
    QPushButton* startBtn_;

public:
    mainwindow(QWidget *parent = 0);
    void init(void);
    void mainLoop(void);

signals:
    void signalNewFrame(QPixmap pNewFrame);

public slots:
    void slotStart();

private slots:
   void slotGetNewFrame();
   void slotUpdateFrame(QPixmap pNewFrame);

};

#endif // MAINWINDOW_H

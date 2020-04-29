#ifndef SMARTROOMS_H
#define SMARTROOMS_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStatusBar>
#include <QLabel>
#include <QListView>
#include <QCheckBox>
#include <QPushButton>
#include <QGraphicsPixmapItem>
#include <QMutex>
#include <QStandardItemModel>


#include <QCamera>
#include "opencv2/opencv.hpp"
#include "capture_thread.h"
QT_BEGIN_NAMESPACE
namespace Ui { class SmartRooms; }
QT_END_NAMESPACE

class SmartRooms : public QMainWindow
{
    Q_OBJECT

public:
    SmartRooms(QWidget *parent = nullptr);
    ~SmartRooms();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_homeButton_clicked();

    void on_homeButton_2_clicked();

    void on_homeButton_3_clicked();
    void openCamera();

private:
    Ui::SmartRooms *ui;
    CaptureThread *capturer;
    QCamera *camera;

};
#endif // SMARTROOMS_H

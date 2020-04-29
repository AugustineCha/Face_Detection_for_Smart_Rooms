#include "smartrooms.h"
#include "ui_smartrooms.h"
#include <qcommonstyle.h>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QKeyEvent>
#include <QDebug>
#include <QCameraInfo>
#include <QGridLayout>
#include <QIcon>
#include <QStandardItem>
#include <QSize>

#include "opencv2/videoio.hpp"

SmartRooms::SmartRooms(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SmartRooms)
    , capturer(nullptr)
{
    ui->setupUi(this);
    QPixmap bkgnd("/home/rahimai/Documents/University/Spring 2020/CPP/SmartRooms/Face_Detection_for_Smart_Rooms/home.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    this->setFixedSize(800,600);

    ui->pushButton->setFlat(true);
    ui->pushButton_2->setFlat(true);
    ui->pushButton_3->setFlat(true);
    ui->pushButton_6->setFlat(true);
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    // I have two cemaras and use the second one here
    camera = new QCamera(cameras[1]);
    connect(ui->pushButton_6, SIGNAL(triggered(bool)), this, SLOT(openCamera()));

    QCommonStyle style;
    ui->homeButton->setIcon(style.standardIcon(QStyle::SP_ArrowBack));
    ui->homeButton_2->setIcon(style.standardIcon(QStyle::SP_ArrowBack));
    ui->homeButton_3->setIcon(style.standardIcon(QStyle::SP_ArrowBack));


}

SmartRooms::~SmartRooms()
{
    delete ui;
}

void SmartRooms::on_pushButton_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(1);
}

void SmartRooms::on_pushButton_2_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(2);
}

void SmartRooms::on_pushButton_3_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(3);
}

void SmartRooms::on_pushButton_6_clicked()
{
    ui -> stackedWidget -> setCurrentIndex(4);
}


void SmartRooms::on_homeButton_clicked()
{
    ui -> stackedWidget ->setCurrentIndex(0);
}

void SmartRooms::on_homeButton_2_clicked()
{
    ui -> stackedWidget ->setCurrentIndex(0);
}


void SmartRooms::on_homeButton_3_clicked()
{
    ui -> stackedWidget ->setCurrentIndex(0);
}

void SmartRooms::openCamera()
{
    camera->setCaptureMode(QCamera::CaptureVideo);
    camera->start();
}


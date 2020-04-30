#ifndef SMARTROOMS_H
#define SMARTROOMS_H

#include <QMainWindow>


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

    void on_homeButton_clicked();

    void on_homeButton_2_clicked();

    void on_homeButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::SmartRooms *ui;
};
#endif // SMARTROOMS_H



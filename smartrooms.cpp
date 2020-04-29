#include "smartrooms.h"
#include "ui_smartrooms.h"
#include <qcommonstyle.h>
#include <QMessageBox>
#include <string>
#include <vector>
#include <global.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>


using namespace std;

SmartRooms::SmartRooms(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SmartRooms)
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

    QCommonStyle style;
    ui->homeButton->setIcon(style.standardIcon(QStyle::SP_ArrowBack));
    ui->homeButton_2->setIcon(style.standardIcon(QStyle::SP_ArrowBack));
    ui->homeButton_3->setIcon(style.standardIcon(QStyle::SP_ArrowBack));

    for (const auto& kv: area_map){
        cout << "Hello"<< endl;
        cout << kv.first << "this" << endl;
        QString name = QString::fromStdString(kv.first);
        ui -> comboBox -> addItem(name);
    }
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

//Button for adding Persons
void SmartRooms::on_pushButton_5_clicked()
{
    //Reading in user input
    QString firstname_q = ui -> lineEditRoom_3 -> text();
    QString lastname_q = ui -> lineEditRoom_4 -> text();
    QString id_q = ui -> lineEditRoom_5 -> text();

    //Converting user input from QString to std::string or int
    string firstname = firstname_q.toStdString();
    string lastname = lastname_q.toStdString();
    int id = stoi(id_q.toStdString());

    //Check if key already exists
    if (person_ids.find(id) != person_ids.end()){

        QMessageBox messageBox;
        messageBox.critical(0,"Error 401","ID already exists!");
        messageBox.setFixedSize(500,200);

    }
    else{

        //Save input both temporarily and permanently
        vector<string> names = {firstname, lastname};

        fstream data_ostream;
        data_ostream.open("person_ids.txt", std::ios::app);

        if (!data_ostream.good()){

            std::cout << "Error: Loading data failed.\n";
            data_ostream.close();

        }
        else{

            person_ids[id] = names;
            data_ostream << firstname << "," << lastname << "," << id << endl;

            }
        data_ostream.close();

    }

    //Clear fields
    ui -> lineEditRoom_3 -> clear();
    ui -> lineEditRoom_4 -> clear();
    ui -> lineEditRoom_5 -> clear();

}

//This function splits a string by a given delimiter
void splitbydelimiter(const string& str, vector<int>& cont,
              char delim = ',')
{
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos) {
        cont.push_back(stoi(str.substr(previous, current - previous)));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(stoi(str.substr(previous, current - previous)));
}


//Button for adding areas
void SmartRooms::on_pushButton_4_clicked()
{
    //Reading in user input
    QString areaname_q = ui -> lineEditRoom -> text();
    QString idlist_q = ui -> lineEditRoom_2 -> text();

    //Converting user input from QString to std::string
    string areaname = areaname_q.toStdString();
    string idlist = idlist_q.toStdString();

    //Split user input ids
    vector<int> authorized_ids;
    splitbydelimiter(idlist, authorized_ids);

    //Save input both temporarily and permanently
    fstream dataar_ostream;
    dataar_ostream.open("areas.txt", std::ios::app);

    if (!dataar_ostream.good()){

        std::cout << "Error: Loading data failed.\n";
        dataar_ostream.close();

    }
    else{

        area_map[areaname] = authorized_ids;

        dataar_ostream << areaname << ",";
        for(auto x: authorized_ids){
            dataar_ostream << x << ",";
        }
        dataar_ostream << "\n";

    }

    //Clear fields
    ui -> lineEditRoom -> clear();
    ui -> lineEditRoom_2 -> clear();



}

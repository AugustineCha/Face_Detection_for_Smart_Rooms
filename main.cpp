#include "smartrooms.h"
#include <QApplication>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <global.h>

using namespace std;

unordered_map<int, vector<string>>person_ids;
unordered_map<string, vector<int>> area_map;


bool startup(unordered_map<int, vector<string>>& person_ids, unordered_map<string, vector<int>>& area_map){

    //Read in both saved person information as well as defined areas
    fstream dataid_istream, dataar_istream;
    dataid_istream.open("person_ids.txt", std::ios::in | std::ios::app);
    dataar_istream.open("areas.txt", std::ios::in | std::ios::app);

    if (!dataid_istream.good() && !dataar_istream.good()){

        std::cout << "Error: Loading data failed.\n";
        dataid_istream.close();
        dataar_istream.close();
        return false;

    }
    else{

        std::string line;
        vector<string> names;
        int counter = 0;
        while(getline(dataid_istream, line, ',')){
            counter +=1;
            names.push_back(line);
            if (counter == 3){
                person_ids[stoi(line)] = names;
                names.clear();
            }
        }


        string area_name;
        vector<int> authorized;
        int counter2 = 0;

        while(getline(dataar_istream, line, ',')){

            if(line=="\n"){
                area_map[area_name] = authorized;
                authorized.clear();
                continue;
            }

            if(counter2 > 0){
                authorized.push_back(stoi(line));
            }
            else{
                counter2 += 1;
                area_name = line;
            }

        }


    }
    dataid_istream.close();
    dataar_istream.close();
    return true;

}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SmartRooms w;
    w.show();
    person_ids[3] = {"empty", "empty"};
    area_map["Garage"] = {1, 2, 3};

    if(startup(person_ids, area_map)){
        return a.exec();
    }
    else{
        a.quit();
        return -1;
    }
}

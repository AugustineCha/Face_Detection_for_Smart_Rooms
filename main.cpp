#include "smartrooms.h"
#include <QApplication>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <global.h>
#include <typeinfo>

using namespace std;

unordered_map<int, vector<string>>person_ids;
unordered_map<string, vector<int>> area_map;
unordered_map<int, string> img_map;


bool startup(unordered_map<int, vector<string>>& person_ids, unordered_map<string, vector<int>>& area_map, unordered_map<int, string>& img_map){

    //Read in both saved person information as well as defined areas
    fstream dataid_istream, dataar_istream;
    dataid_istream.open("/Users/augustinecha/Face_Detection_for_Smart_Rooms/person_ids.txt", std::ios::in | std::ios::app);
    dataar_istream.open("/Users/augustinecha/Face_Detection_for_Smart_Rooms/areas.txt", std::ios::in | std::ios::app);

    if (!dataid_istream.good() && !dataar_istream.good()){

        std::cout << "Error: Loading data failed.\n";
        dataid_istream.close();
        dataar_istream.close();
        return false;

    }
    else{

        std::string line;
        vector<string> names;
        int id_read;
        int counter = 0;
        while(getline(dataid_istream, line, ',')){
            counter +=1;
            names.push_back(line);
            if (counter == 3){
                id_read = stoi(line);
                person_ids[id_read] = names;
                names.clear();
            }
            if (counter == 4){
                img_map[id_read] = line;
            }
        }


        string area_name;
        vector<int> authorized;
        int counter2 = 0;

        while(getline(dataar_istream, line, ',')){

            if(line=="#"){
                area_map[area_name] = authorized;
                authorized.clear();
                counter2 = 0;
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

    if(startup(person_ids, area_map, img_map)){
        return a.exec();
    }
    else{
        a.quit();
        return -1;
    }
}

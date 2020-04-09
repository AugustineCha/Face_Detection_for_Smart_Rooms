#include <string>
#include <vector>
#include <iostream>

using namespace std;


class Area{

    public:
        explicit Area(string, int);
    
    private:
        string area_name;
        vector<int> authorized_ids;
        
};

Area::Area(string name, vector<int> ids){
    
    area_name = name;
    authorized_ids = ids;

}



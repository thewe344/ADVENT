#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_set>

using namespace std;

struct disk_fragmenter
{
    string data{};
};


void load_input(disk_fragmenter* data, stringstream & stream){
    char c{};
    int64_t index = 0;
    while(stream.get(c)){
        for(int i{}; i<= int(c);i++){
            if (index%2 == 0 || index == 0){
                data->data.push_back(index);
            }else{    
                data->data.push_back('.');
            }
        index++;
    }
    cout << data->data << "\n";
    }
}

void read_file(disk_fragmenter* data, string const& filename = "input_test1.txt") {
    ifstream myFile(filename);
    stringstream stream;
    if (myFile.is_open()){
        cout << "Opened file \n";
        stream << myFile.rdbuf();
        load_input(data,stream);
        myFile.close();
    }else{
        cout << "error";
    }
}

void mani(){
    disk_fragmenter data{};
    disk_fragmenter* pntr = &data;
    read_file(pntr);
}
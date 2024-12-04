#include <fstream>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

struct XMAS{
    vector<string> str{};
    uint64_t totalXmas{};
    vector<char> lookUpTableChar {'M','S','M','S'};
    vector<vector<pair<int,int>>> lookUpTablePair{{ {-1,-1},{1,1},{1,-1},{-1,1} },
                                                    { {-1,1},{1,-1},{1,1},{-1,-1} },
                                                    { {-1,-1},{1,1},{-1,1},{1,-1} },
                                                    { {1,-1},{-1,1},{1,1},{-1,-1} }
    };
};

void look_up_table(XMAS & dataStruct, int const& i, int const& j){
    for(int k{};k < static_cast<int>(dataStruct.lookUpTablePair.size());k++){
        string tempStr{};
        for(int n{};n < static_cast<int>(dataStruct.lookUpTablePair[k].size());n++){
            if( (j + dataStruct.lookUpTablePair[k][n].second >=0) && (j + dataStruct.lookUpTablePair[k][n].second < static_cast<int>(dataStruct.str[i].size())) ){
                if( (i + dataStruct.lookUpTablePair[k][n].first >=0) && (i + dataStruct.lookUpTablePair[k][n].first < static_cast<int>(dataStruct.str.size())) ){
                    int x = j + dataStruct.lookUpTablePair[k][n].second;
                    int y = i + dataStruct.lookUpTablePair[k][n].first;
                    if(dataStruct.lookUpTableChar[n] != dataStruct.str[y][x]){                                               
                        break;
                    }else{
                        tempStr += dataStruct.str[y][x];
                    }
                }else{
                    break;
                }
            }
        }
        if (tempStr == "MSMS"){
            dataStruct.totalXmas += 1;
            return;
        }
    }
    
}

void find_xmas(XMAS & dataStruct){
    for(int i{}; i < static_cast<int>(dataStruct.str.size());i++){
        for(int j{}; j < static_cast<int>(dataStruct.str[i].size()); j++){
            if((dataStruct.str[i][j] == 'A' ) && (j !=0 ) && (j != (static_cast<int>(dataStruct.str.size())-1)) ){
                look_up_table(dataStruct,i,j);
            }
        }
    }
}

void read_file(XMAS & dataStruct){
    ifstream myFile{};
    string line{};
    myFile.open("input.txt");
    if (myFile.is_open()){
        cout << "Opened file \n";
        while (getline(myFile, line)) {
            dataStruct.str.push_back(line);
        }
        myFile.close();
    }else{
        cout << "error";
    }
}

int main(){
    XMAS dataStruct;
    read_file(dataStruct);
    find_xmas(dataStruct);
    cout << "Total XMAS found: ";
    cout <<  dataStruct.totalXmas;
}
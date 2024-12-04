#include <fstream>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

struct XMAS{
    vector<string> str{};
    uint64_t totalXmas{};
    vector<char> lookUpTableChar {'X','M','A','S'};
    vector<vector<pair<int,int>>> lookUpTablePair{{ {0,0},{0,1},{0,2},{0,3} },          // To the right (Infront)
                                                    { {0,0},{0,-1},{0,-2},{0,-3} },     // To the left (inback)
                                                    { {0,0},{1,0},{2,0},{3,0} },        // Down (Forward)
                                                    { {0,0},{-1,0},{-2,0},{-3,0} },     // Up (Backward)
                                                    { {0,0},{1,1},{2,2},{3,3} },        // Right-down (sideways down)
                                                    { {0,0},{-1,1},{-2,2},{-3,3} },     // Right-Up (sideways up)
                                                    { {0,0},{1,-1},{2,-2},{3,-3} },     // Left-down (sideways down)
                                                    { {0,0},{-1,-1},{-2,-2},{-3,-3} }   // Left-Up (sideways up)
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
                    if(dataStruct.lookUpTableChar[n] != dataStruct.str[y][x]){ // y and x flipped becasuse when i was thinking on, 
                        break;                                                  // i imagined the whole text as a coordinate system,
                    }else{                                                      // So if i move left/right = x and up/down = y
                        tempStr += dataStruct.str[y][x];                        // So the outermost vector is y and inner one is x
                    }
                }else{
                    break;
                }
            }
        }
        if (tempStr == "XMAS"){
            dataStruct.totalXmas += 1;
        }
    }
    
}


void find_xmas(XMAS & dataStruct){
    for(int i{}; i < static_cast<int>(dataStruct.str.size());i++){
        for(int j{}; j < static_cast<int>(dataStruct.str[i].size()); j++){
            if(dataStruct.str[i][j] == 'X'){
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
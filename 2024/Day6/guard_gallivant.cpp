#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <utility>

using namespace std;

struct MAP{
    char currentOrientation{};
    uint64_t totalVisitedPositions{};

    pair<int64_t,int64_t> currentPosition{};

    vector<vector<char>> mapPositions{};
    
    unordered_map<char,pair<int8_t,int8_t>> lookUpDirection{{'^',{-1,0}},
                                                            {'>',{0,1}},
                                                            {'v',{1,0}},
                                                            {'<',{0,-1}}};
    unordered_map<char,char> newDirection{  {'^','>'},
                                            {'>','v'},
                                            {'v','<'},
                                            {'<','^'}};
};

void load_input(MAP & map, stringstream & stream){
    char c;
    vector<char> tempVec{};
    while(stream.get(c)){
        if (c == '\n'){
            map.mapPositions.push_back(tempVec);
            tempVec.clear();
        }else if(c == '^' || c == '>' || c == 'v' || c == '<'){
            tempVec.push_back(c);
            map.currentPosition.first = static_cast<int64_t>(map.mapPositions.size());
            map.currentPosition.second = static_cast<int64_t>(tempVec.size()) - 1;
            map.currentOrientation = c;
        }else{
            tempVec.push_back(c);
        }
    }
    map.mapPositions.push_back(tempVec);
}

void read_file(MAP& map, string const& filename = "input.txt") {
    ifstream myFile(filename);
    stringstream stream;
    if (myFile.is_open()){
        cout << "Opened file \n";
        stream << myFile.rdbuf();
        load_input(map,stream);
        myFile.close();
    }else{
        cout << "error";
    }
}

void check_guard_position(MAP& map){
    int i{};
    while(true){
        int64_t currentPositionI = map.currentPosition.first;
        int64_t currentPositionJ = map.currentPosition.second;
        int64_t toAddI = map.lookUpDirection[map.currentOrientation].first;
        int64_t toAddJ = map.lookUpDirection[map.currentOrientation].second;

        if(currentPositionI+toAddI < 0 || currentPositionJ+toAddJ < 0 || currentPositionI+toAddI > static_cast<int64_t>(map.mapPositions.size()) || currentPositionJ+toAddJ > static_cast<int64_t>(map.mapPositions[currentPositionI+toAddI].size())){
            map.mapPositions[currentPositionI][currentPositionJ] = 'X';
            i++;
            break;
        }

        if (map.mapPositions[currentPositionI+toAddI][currentPositionJ+toAddJ] == '#'){
            map.currentOrientation = map.newDirection[map.currentOrientation];
        }else{
            if (map.mapPositions[currentPositionI][currentPositionJ] != 'X'){
                map.mapPositions[currentPositionI][currentPositionJ] = 'X';
                i++;
            }
            currentPositionI += toAddI;
            currentPositionJ += toAddJ;
            map.currentPosition.first = currentPositionI;
            map.currentPosition.second = currentPositionJ;
        } 
    }
}



int main(){
    MAP map;
    read_file(map);
    //check_guard_position(map);
    cout << "Before move: ";
    cout << "\n";
    for(auto i : map.mapPositions){
        for(auto j : i){
            cout << j;
        }
        cout << "\n";
    }
    cout << "Starting position is: (";
    cout << map.currentPosition.first;
    cout << ",";
    cout << map.currentPosition.second;
    cout << ")";
    cout << "\n";
    check_guard_position(map);
    cout << "After move: ";
    cout << "\n";
    for(auto i : map.mapPositions){
        for(auto j : i){
            cout << j;
        }
        cout << "\n";
    }
}
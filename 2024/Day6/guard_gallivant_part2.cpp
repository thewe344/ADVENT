#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <utility>

using namespace std;

class MAP{
public:

    struct Node {
        int64_t row;
        int64_t col;
        char orientation;

        bool operator==(const Node& other) const {
            return row == other.row && col == other.col && orientation == other.orientation;
        }
    };

    struct NodeHash {
        std::size_t operator()(const Node& n) const {
            std::size_t h1 = std::hash<int64_t>()(n.row);
            std::size_t h2 = std::hash<int64_t>()(n.col);
            std::size_t h3 = std::hash<char>()(n.orientation);
            return ((h1 << 1) ^ h2) ^ (h3 << 2);
        }
    };

    int totals{};

    pair<int8_t,int8_t> startPosition{};
    char startOrientation{};

    char currentOrientation{};
    pair<int64_t,int64_t> currentPosition{};

    vector<vector<char>> mapPositions{};

    unordered_map<Node, bool, NodeHash> visitedNodes;

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
            map.startPosition.first = static_cast<int64_t>(map.mapPositions.size());
            map.startPosition.second = static_cast<int64_t>(tempVec.size()) - 1;
            map.startOrientation = c;
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

bool check_guard_position(MAP& map){
    int64_t currentPositionI{};
    int64_t currentPositionJ{};
    int64_t toAddI{};
    int64_t toAddJ{};
    map.visitedNodes.clear();
    while(true){
        currentPositionI = map.currentPosition.first;
        currentPositionJ = map.currentPosition.second;
        toAddI = map.lookUpDirection[map.currentOrientation].first;
        toAddJ = map.lookUpDirection[map.currentOrientation].second;

        if (currentPositionI + toAddI < 0 || 
            currentPositionJ + toAddJ < 0 ||
            currentPositionI + toAddI >= static_cast<int64_t>(map.mapPositions.size()) ||
            currentPositionJ + toAddJ >= static_cast<int64_t>(map.mapPositions[currentPositionI].size())) {

            return false;
        }

        if (map.mapPositions[currentPositionI+toAddI][currentPositionJ+toAddJ] == '#' ||map.mapPositions[currentPositionI+toAddI][currentPositionJ+toAddJ] == '0'){
            auto result = map.visitedNodes.insert({{currentPositionI,currentPositionJ,map.currentOrientation}, true});
            if(!result.second){
                return true;
            }else{
                map.currentOrientation = map.newDirection[map.currentOrientation];
            }

        }else{
            map.currentPosition.first += toAddI;
            map.currentPosition.second += toAddJ;
        } 
    }
}



int main(){
    MAP map;
    read_file(map);
    char tempStorage{};
    for(int64_t i{};i < static_cast<int64_t>(map.mapPositions.size());i++){

        for(int64_t j{};j < static_cast<int64_t>(map.mapPositions[i].size());j++){
            map.currentOrientation = map.startOrientation;
            map.currentPosition = map.startPosition;
            if(map.mapPositions[i][j] == '#' || map.mapPositions[i][j] == map.currentOrientation){
                continue;
            }
            tempStorage = map.mapPositions[i][j];
            map.mapPositions[i][j] = '0';

            if( check_guard_position(map) ){
                map.totals ++;
            } 
            map.mapPositions[i][j] = tempStorage;
        }
    }
    cout << "Total loops are: ";
    cout << map.totals;
}
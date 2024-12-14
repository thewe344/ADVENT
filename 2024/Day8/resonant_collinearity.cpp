#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_set>

using namespace std;
    

class AntennaMap{
public:
    int64_t totalAntiNodes{};
    vector<vector<char>> data{};
    vector<pair<char,pair<int64_t,int64_t>>> antennaPositionsAndFrequency{};

    struct Node{
        int64_t row{};
        int64_t col{};

        bool operator==(const Node& other) const {
            return row == other.row && col == other.col;
        }
    };

    struct NodeHash {
        size_t operator()(const Node& n) const {
            size_t h1 = hash<int64_t>()(n.row);
            size_t h2 = hash<int64_t>()(n.col);
            return ((h1 << 1) ^ h2);
        }
    };
    unordered_set<Node,NodeHash> visitedNodes{};


    AntennaMap() {};
    ~AntennaMap() = default;

private:

};

void load_input(AntennaMap & map, stringstream & stream){
    char c{};
    int64_t row{};
    int64_t col{-1};
    vector<char> tempValues{};
    while(stream.get(c)){
        if (c == '\n'){
            map.data.push_back(tempValues);
            tempValues.clear();
            row++;
            col = -1;
        }else if (c != '.'){
            // Load character (Antenna and freq) and position.
            map.antennaPositionsAndFrequency.push_back({c,{row,col}});
        }
        tempValues.push_back(c);
        col++;
    }
    map.data.push_back(tempValues);
}

void read_file(AntennaMap & map, string const& filename = "input_test.txt") {
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
 
void evaluate_node(AntennaMap & map, pair<int64_t,int64_t> const& node){
    if(node.first >= 0 &&
        node.second >= 0 &&
        node.first <= static_cast<int64_t>(map.data.size()) && 
        node.second <= static_cast<int64_t>(map.data[0].size())){
            auto result = map.visitedNodes.insert({node.first,node.second}).second;
            if(result){
                map.totalAntiNodes++;
                if(map.data[node.first][node.second] == '.'){
                    map.data[node.first][node.second] = '#';
                }
            }
            
    }
}

void find_possible(AntennaMap & map, pair<char,pair<int64_t,int64_t>> const& currentlyAntenna, int64_t const& index){
    if(index >= static_cast<int64_t>(map.antennaPositionsAndFrequency.size())){
        return;
    }
    pair<int64_t,int64_t> antiNode1{};
    pair<int64_t,int64_t> antiNode2{};

    int64_t currentRow = currentlyAntenna.second.first;
    int64_t currentCol = currentlyAntenna.second.second;

    int64_t objectiveRow = map.antennaPositionsAndFrequency[index].second.first;
    int64_t objectiveCol = map.antennaPositionsAndFrequency[index].second.second;

    int64_t rowDistance = (currentRow - objectiveRow);
    int64_t colDistance = (currentCol - (objectiveCol));

    antiNode1.first = currentRow + rowDistance;
    antiNode1.second = currentCol + colDistance;

    if(currentlyAntenna.first == map.antennaPositionsAndFrequency[index].first){
        antiNode2.first = objectiveRow +(-1*rowDistance);
        antiNode2.second = objectiveCol +(-1*colDistance);
    }else if(currentlyAntenna.first != map.antennaPositionsAndFrequency[index].first){
        antiNode2.first = objectiveRow;
        antiNode2.second = objectiveCol+1;
    }

    evaluate_node(map,antiNode1);
    evaluate_node(map,antiNode2);

    if(index < static_cast<int64_t>(map.antennaPositionsAndFrequency.size())){
        find_possible(map, currentlyAntenna, index+1);
    }
    return;
}

void search_map(AntennaMap & map){
    for(int64_t i{};i < static_cast<int64_t>(map.antennaPositionsAndFrequency.size()); i++){
        find_possible(map,map.antennaPositionsAndFrequency[i],i+1);
    }
}

int main(){
    AntennaMap map{};
    read_file(map);
    cout << "Read data map with size:"
        << map.data.size()
        << endl;

    for (auto i : map.data){
        for(auto j : i){
            cout << j;
        }
    }
    cout << "\nWith positions at: \n";
    for(auto i: map.antennaPositionsAndFrequency){
        cout << "Freq: " 
            << i.first
            << " position: ("
            << i.second.first
            << ","
            << i.second.second
            << ")"
            << endl;
    }
    search_map(map);
    cout << "After search we get map:" << endl;
    for (auto i : map.data){
        for(auto j : i){
            cout << j;
        }
    }
    cout << "With total antinodes = " << map.totalAntiNodes << endl;
}
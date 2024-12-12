#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_set>


using namespace std;

class Garden{

public:
    vector<vector<char>> individualPlants{};
    vector<pair<int,int>> directionsToCheck{{0,1}, // Right
                                            {1,0}, // Down
                                            {0,-1}, // Left
                                            {-1,0}, // Up
                                            };

    // Used same principle as in i think day7 to save already looked at positions
    // But only looking at the position
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
    unordered_set<Node,NodeHash> visitedNodes;

    // Temporary struct storage for variables
    struct AreaAndPerimeter{
        char plant{};     
        uint64_t totalSubArea{};
        uint64_t totalSubPerimiter{};
        uint64_t individualPrice{};
    };
    
    int64_t totalPrice{};

    Garden() {}

    ~Garden() = default;

private:

};

void load_input(Garden & garden, stringstream & stream){
    char c;
    vector<char> tempVec{};
    while(stream.get(c)){
        if (c == '\n'){
            garden.individualPlants.push_back(tempVec);
            tempVec.clear();
        }else{
            tempVec.push_back(c);
        }
    }
    garden.individualPlants.push_back(tempVec);
}


void read_file(Garden & garden, string const& filename = "input.txt") {
    ifstream myFile(filename);
    stringstream stream;
    if (myFile.is_open()){
        cout << "Opened file \n";
        stream << myFile.rdbuf();
        load_input(garden,stream);
        myFile.close();
    }else{
        cout << "error";
    }
}

//This function traverse recursive along the lookup directions until it do not find anything
void traverse_one_area(Garden & garden, int64_t & row, int64_t & col, Garden::AreaAndPerimeter & tempStorer){
    int64_t new_row{}; 
    int64_t new_col{}; 
    for(auto n : garden.directionsToCheck){
        if( row + n.first < 0 ||
            col + n.second < 0 ||
            row + n.first >= static_cast<int64_t>(garden.individualPlants.size()) ||
            col + n.second >= static_cast<int64_t>(garden.individualPlants[row].size()) || 
            tempStorer.plant != garden.individualPlants[row + n.first][col + n.second]){
            //If at boundry, add one to perimeter 
            tempStorer.totalSubPerimiter +=1;
            continue;
           }

        auto haveNotAlreadyVisited = garden.visitedNodes.insert({row + n.first, col + n.second});
        if(haveNotAlreadyVisited.second){
            new_row = row + n.first;
            new_col = col + n.second;
            tempStorer.totalSubArea += 1;
            traverse_one_area(garden, new_row, new_col, tempStorer);
        }else{
            continue;
        }
    }
}

void find_regions(Garden & garden){
    Garden::AreaAndPerimeter tempStorer{};
    for(int64_t row{}; row < static_cast<int64_t>(garden.individualPlants.size()); row++){
        for(int64_t col{}; col < static_cast<int64_t>(garden.individualPlants[row].size()); col++){
            if( garden.visitedNodes.empty()){
                garden.visitedNodes.insert({row, col});
            }else if (! garden.visitedNodes.insert({row, col}).second ){
                //Already checked that position
                continue;
            }
            // If not in checked positions start to search the plantation grid
            tempStorer = {{garden.individualPlants[row][col]},{1},{}}; // Added one here, lazy fix because otherwise all areas was area-1
            traverse_one_area(garden,row,col,tempStorer);
            tempStorer.individualPrice = tempStorer.totalSubArea * tempStorer.totalSubPerimiter; 
            garden.totalPrice += tempStorer.individualPrice;
        }
    }
}

int main(){
    Garden garden{};
    read_file(garden);
    find_regions(garden);
    cout << "Total price = ";
    cout << garden.totalPrice;
}
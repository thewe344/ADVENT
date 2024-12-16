#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

// +x = to the right, -x = moving to the left
// +y = moving down, -y = moving up.

class Bathroom{

public:

    int64_t leftUp{};
    int64_t leftDown{};
    int64_t rightUp{};
    int64_t rightDown{};
    int64_t safetyFactor{};

    struct Robot{
        pair<int64_t,int64_t> position; //p=x,y
        pair<int64_t,int64_t> velocity; //v=x,y
    };
    
    // y     x        nr of robots
    vector<vector<vector<Robot>>> map;
    vector<vector<vector<Robot>>> tempMap;

    void reset_map(vector<vector<vector<Robot>>> & targetMap){
        int y{103}; 
        int x{101};
        targetMap = vector<vector<vector<Robot>>>(y, vector<vector<Robot>>(x));
    };

    // Constructor, initillize map-grid with zero sized robot vector
    Bathroom() {
        reset_map(map); 
        reset_map(tempMap);
    };
    ~Bathroom() = default;


private:
};

void add_robot(vector<vector<vector<Bathroom::Robot>>> &  map, Bathroom::Robot tempRobot){
    if (map[tempRobot.position.second][tempRobot.position.first].empty() ){
            map[tempRobot.position.second][tempRobot.position.first].push_back(tempRobot);
        }else{
            vector<Bathroom::Robot> tempRobotVector{};
            tempRobotVector = map[tempRobot.position.second][tempRobot.position.first];
            tempRobotVector.push_back(tempRobot);
            map[tempRobot.position.second][tempRobot.position.first] = tempRobotVector;
        }
}

void add_input(Bathroom & data, string & line){
    regex regPattern(R"(p=(\d+),(\d+)\s+v=(-?\d+),(-?\d+))");
    smatch matchedNumbers{};
    if(regex_search(line, matchedNumbers, regPattern)){
        Bathroom::Robot tempRobot{};
        tempRobot.position.first = stoll(matchedNumbers[1]);
        tempRobot.position.second = stoll(matchedNumbers[2]);
        tempRobot.velocity.first = stoll(matchedNumbers[3]);
        tempRobot.velocity.second = stoll(matchedNumbers[4]);
        // If those coordinates dont contain any robots
        add_robot(data.map,tempRobot);
    }
}

void read_file(Bathroom & data, string const& filename = "input.txt"){
    ifstream myFile{filename};
    string line{};
    if (myFile.is_open()){
        cout << "Opened file \n";
        while (getline(myFile, line)) {
            add_input(data,line);
        }
        myFile.close();
    }else{
        cout << "error";
    }
}

int64_t evaluate_new_position(int64_t const& current, int64_t const& velocity, size_t const& size){
    if(current + velocity < 0){
        return static_cast<int64_t>(size) + (current + velocity);
    }else if(current + velocity >= static_cast<int64_t>(size)){
        return (current + velocity) - static_cast<int64_t>(size);
    }else{
        return current + velocity;
    }
}

void move_robot(Bathroom & data, vector<Bathroom::Robot> const& vectorOfRobots){
    for(auto i : vectorOfRobots){
        Bathroom::Robot tempRobot{};
        int64_t currentX = i.position.first;
        int64_t currentY = i.position.second;
        int64_t velocityX = i.velocity.first;
        int64_t velocityY = i.velocity.second;
        int64_t newX{};
        int64_t newY{};

        newX = evaluate_new_position(currentX, velocityX, data.map[0].size());
        newY = evaluate_new_position(currentY, velocityY, data.map.size());

        tempRobot.position = {newX,newY};
        tempRobot.velocity = i.velocity;
        add_robot(data.tempMap, tempRobot);
    }
}


void append_to_file(Bathroom const& data, int64_t const& time,const string& filename="output_text.txt") {
    std::ofstream file(filename, std::ios::app);
    if (!file) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }
    file << "At time: " << time << endl;
    for (auto i : data.map){
        for (auto j : i){
            if(j.empty()){
                file << '.';
            }else{
                file << j.size();
            }
        }
        file << endl;
    }
    file << std::endl;
    file.close();
}

void move_in_time(Bathroom & data){
    for(int64_t i{}; i<10000; i++){
        for(auto j : data.map){
            for(auto k : j){
                if(k.empty()){
                    continue;
                }else{
                    move_robot(data, k);
                }
            }
        }
        data.map = data.tempMap;
        data.reset_map(data.tempMap);
         if ((i - 78) % 101 == 0 && (i - 78) >= 0){
            append_to_file(data,i);
        }else if ((i - 51) % 103 == 0 && (i - 51) >= 0){
            append_to_file(data,i);
        }
    }
}

void count_in_quadrant(Bathroom& data) {
    int64_t midRow = static_cast<int64_t>(data.map.size()) / 2;
    int64_t midCol = static_cast<int64_t>(data.map[0].size()) / 2;

    for (int64_t i = 0; i < static_cast<int64_t>(data.map.size()); i++) {
        for (int64_t j = 0; j < static_cast<int64_t>(data.map[i].size()); j++) {
            if (data.map[i][j].empty()) {
                continue;
            }

            // Determine the quadrant and update the corresponding count
            if (i < midRow && j < midCol) {
                data.leftUp += data.map[i][j].size();
            } else if (i < midRow && j >= midCol) {
                data.rightUp += data.map[i][j].size();
            } else if (i >= midRow && j < midCol) {
                data.leftDown += data.map[i][j].size();
            } else {
                data.rightDown += data.map[i][j].size();
            }
        }
    }

    data.safetyFactor = data.leftUp * data.rightUp * data.rightDown * data.leftDown;
}


int main(){
    Bathroom data;
    read_file(data);
    for (auto i : data.map){
        for (auto j : i){
            if(j.empty()){
                cout << '.';
            }else{
                cout << j.size();
            }
        }
        cout << endl;
    }
    move_in_time(data);
    cout << "After 100 seconds" << endl;
    for (auto i : data.map){
        for (auto j : i){
            if(j.empty()){
                cout << '.';
            }else{
                cout << j.size();
            }
        }
        cout << endl;
    }
    count_in_quadrant(data);
    cout << "With a safety factor " << data.safetyFactor;

}
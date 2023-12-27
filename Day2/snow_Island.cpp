#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

struct setStruct{
    std::string setInput{};
    bool works{true};
    int red{};
    int green{};
    int blue{};
};

struct setVectorStruct {
    std::vector<setStruct> setVector;
};

struct gameStruct{
    int gameID{};
    bool allTrue{true};
    int red{};
    int green{};
    int blue{};
    int product{};
};

int find_game_id(std::string const& input){
    int gameID{};
    if (isdigit(input[6]) && isdigit(input[7])) {
        gameID = stoi(input.substr(5, 3));
    } else if (isdigit(input[6])) {
        gameID = stoi(input.substr(5, 2));
    } else {
        gameID = input[5] - '0';
    }
    return gameID;
}

size_t find_start_pos(int const& gameID){
    size_t startPos{};
    if (gameID < 10) {
        startPos = 8;
    } else if (gameID < 100) {
        startPos = 9;
    } else {
        startPos = 10;
    }
    return startPos;
}


void find_Set(setVectorStruct & mysetVectorStruct, size_t const& startPos, std::string const& input){
    size_t tracker{startPos};
    size_t nextPos{};
    while (true){
        setStruct newSetStruct;
        nextPos = input.find(';',tracker);

        if (nextPos == std::string::npos) {
            newSetStruct.setInput = input.substr(tracker);
            mysetVectorStruct.setVector.push_back(newSetStruct);
            break;
        };
        
        newSetStruct.setInput = input.substr(tracker,nextPos-tracker);
        mysetVectorStruct.setVector.push_back(newSetStruct);
        tracker = nextPos+2;
    };
};


void search_set(setVectorStruct & mysetVectorStruct, gameStruct & myGameStruct){
    int counterLast{};
    int counterFirst{};
    for (int i{}; i<static_cast<int>(mysetVectorStruct.setVector.size()); i++){
        for(int n{}; n < static_cast<int>(mysetVectorStruct.setVector[i].setInput.length());n++){

            while(isdigit(mysetVectorStruct.setVector[i].setInput[n+counterLast])){
                if(counterLast == 0){
                    counterFirst = n;
                };
                counterLast++;
            };

            if ((mysetVectorStruct.setVector[i].setInput[n] == 'r') and (mysetVectorStruct.setVector[i].setInput[n-1] != 'g')){
                mysetVectorStruct.setVector[i].red = stoi(mysetVectorStruct.setVector[i].setInput.substr(counterFirst,counterLast));
                if(mysetVectorStruct.setVector[i].red > 12){
                    mysetVectorStruct.setVector[i].works = false;
                };
                counterLast = 0;
            }else if ((mysetVectorStruct.setVector[i].setInput[n]) == 'g'){
                mysetVectorStruct.setVector[i].green = stoi(mysetVectorStruct.setVector[i].setInput.substr(counterFirst,counterLast));
                if( mysetVectorStruct.setVector[i].green > 13){
                    mysetVectorStruct.setVector[i].works = false;
                };
                counterLast = 0;
            }else if ((mysetVectorStruct.setVector[i].setInput[n]) == 'b'){
                mysetVectorStruct.setVector[i].blue = stoi(mysetVectorStruct.setVector[i].setInput.substr(counterFirst,counterLast));
                if(mysetVectorStruct.setVector[i].blue > 14){
                    mysetVectorStruct.setVector[i].works = false;
                };
                counterLast = 0;
            };
        };
        myGameStruct.allTrue = myGameStruct.allTrue && mysetVectorStruct.setVector[i].works;

        if(mysetVectorStruct.setVector[i].red > myGameStruct.red){
            myGameStruct.red = mysetVectorStruct.setVector[i].red;
        }

        if(mysetVectorStruct.setVector[i].green > myGameStruct.green){
            myGameStruct.green = mysetVectorStruct.setVector[i].green;
        }

        if(mysetVectorStruct.setVector[i].blue > myGameStruct.blue){
            myGameStruct.blue = mysetVectorStruct.setVector[i].blue;
        }
        
        
    }
    myGameStruct.product = myGameStruct.red * myGameStruct.green * myGameStruct.blue; 
}


gameStruct find_Cubes(std::string const& input){
    size_t startPos{};
    gameStruct myGameStruct{};
    setVectorStruct mysetVectorStruct;
    
    myGameStruct.gameID = find_game_id(input);
    startPos = find_start_pos(myGameStruct.gameID);
    find_Set(mysetVectorStruct, startPos, input);
    search_set(mysetVectorStruct, myGameStruct);

    return myGameStruct;
};



int main() {
    std::ios::sync_with_stdio(false);
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream fileStream("input.txt"); // Open file for reading
    if (!fileStream) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }
    
    int counter{};
    int sumOfAllProducts{};
    gameStruct newGameStruct;
    std::string fileString;
    while (getline(fileStream, fileString)) {
        newGameStruct = find_Cubes(fileString);
        sumOfAllProducts += newGameStruct.product;
        if (newGameStruct.allTrue)
        {
            counter += newGameStruct.gameID;
        }
    }

    fileStream.close(); // Close file

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    std::cout << "Part 1 ";
    std::cout << counter;
    std::cout << "\n\n";
    std::cout << "Part 2 ";
    std::cout << sumOfAllProducts;
    return 0;
}

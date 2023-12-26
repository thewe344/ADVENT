#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

struct colourStruct{
    std::string setInput{};
    bool works{true};
};

struct setStruct {
    std::vector<colourStruct> colourSets;
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


void find_Set(setStruct & mySetStruct, size_t const& startPos, std::string const& input){
    size_t tracker{startPos};
    size_t nextPos{};
    while (true){
        colourStruct newColourSet;
        nextPos = input.find(';',tracker);

        if (nextPos == std::string::npos) {
            newColourSet.setInput = input.substr(tracker);
            mySetStruct.colourSets.push_back(newColourSet);
            break;
        }
        
        newColourSet.setInput = input.substr(tracker,nextPos-tracker);
        mySetStruct.colourSets.push_back(newColourSet);
        tracker = nextPos+2;
    }
}

int find_Cubes(std::string const& input){
    int gameID{}; 
    size_t startPos{};
    bool allTrue{false};
    setStruct mySetStruct;
    

    gameID = find_game_id(input);
    startPos = find_start_pos(gameID);
    find_Set(mySetStruct, startPos, input);

    std::cout << "From Game ID ";
    std::cout << gameID ;
    std::cout << "\n";
    for (int i{}; i<static_cast<int>(mySetStruct.colourSets.size()); i++){
        int counterLast{};
        int counterFirst{};
        for(int n{}; n<static_cast<int>(mySetStruct.colourSets[i].setInput.length());n++){

            while(isdigit(mySetStruct.colourSets[i].setInput[n+counterLast])){
                if(counterLast == 0){
                    counterFirst = n;
                }
                counterLast++;
            }

            if ((mySetStruct.colourSets[i].setInput[n] == 'r') and (mySetStruct.colourSets[i].setInput[n-1] != 'g')){
                if(stoi(mySetStruct.colourSets[i].setInput.substr(counterFirst,counterLast))>=12){
                    mySetStruct.colourSets[i].works = false;
                };
                counterLast = 0;
            }else if ((mySetStruct.colourSets[i].setInput[n]) == 'g'){
                if(stoi(mySetStruct.colourSets[i].setInput.substr(counterFirst,counterLast))>=13){
                    mySetStruct.colourSets[i].works = false;
                };
                counterLast = 0;
            }else if ((mySetStruct.colourSets[i].setInput[n]) == 'b'){
                if(stoi(mySetStruct.colourSets[i].setInput.substr(counterFirst,counterLast))>=14){
                    mySetStruct.colourSets[i].works = false;
                };
                counterLast = 0;
            }
        }
        
        allTrue = allTrue && mySetStruct.colourSets[i].works;


    }
    if (allTrue){
        return gameID;
    }
    
    return 0;
};



int main() {
    std::ios::sync_with_stdio(false);
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream fileStream("input.txt"); // Open file for reading
    if (!fileStream) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }
    
    int number{};

    std::string fileString;
    while (getline(fileStream, fileString)) {
        number += find_Cubes(fileString);
    }

    fileStream.close(); // Close file

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    std::cout << number;
    return 0;
}

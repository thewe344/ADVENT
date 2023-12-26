#include <string>
#include <vector>
#include <iomanip>
#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <chrono>

using namespace std;

std::string convertStringToLower(std::string & checker){
    for (auto& x : checker) { 
        x = tolower(x); 
    }
    return checker;
}

//This function reads data from file
int find_Number(std::string string){
    std::vector<std::string> search= {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
    std::string number="00";
    bool firstFound = false;
    bool lastFound = false;
    for (int i{}; i < int(string.length()) ; i++){

        if ((isdigit(string[i])) && (isdigit(string[i]))){
            if ((not firstFound)){
                number[0] = string[i];
                firstFound = true;
            }else{
                number[1] = string[i];
                lastFound = true;
            }
        }else{
            for (int n{}; n < int(search.size()); n++){
                if (tolower(string[i]) == search[n][0]){
                    std::string substring = string.substr(i, search[n].length());
                    if(convertStringToLower(substring) == search[n]){
                        if ((not firstFound)){
                            number[0] =  '0' + (n+1);
                            firstFound = true;
                        }else{
                            number[1] = '0' + (n+1);
                            lastFound = true;
                        };
                        i += (search[n].length())-2;
                    }
                }
            }
        }
    }
    if (!lastFound){
        number[1] = number[0];
        number[0] = number[0];
    }
    try{
        return std::stoi(number);
    } catch(std::invalid_argument const& ex){
        return 0;
    };
}

int main (){
    auto start = std::chrono::high_resolution_clock::now();
    int sumOfNumbers= 0;
    std::ifstream file_stream; //file stream to read from file
    std::string file_string{}; //string to place each line of file in
    file_stream.open("input.txt", ios::in); // opens file for reading
    while(getline(file_stream, file_string)) //as long as there is someting to read from file, read
    {
        sumOfNumbers = sumOfNumbers + find_Number(file_string);
    }
    file_stream.close(); // close file from reading
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    std::cout << "Total sum is: "; 
    std::cout << sumOfNumbers;
    return 0;
}
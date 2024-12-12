#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>

using namespace std;

class DATA {
public:
    std::vector<int64_t> sumAndOrProducts;
    std::vector<std::vector<int64_t>> values;

    // Constructor
    DATA() {}

    // Destructor
    ~DATA() = default;



private:
};

void load_input(DATA & data, stringstream & stream){
    char c{};
    char previous{};
    string str{};
    vector<int64_t> tempValues{};
    while(stream.get(c)){
        if (c == '\n'){
            tempValues.push_back(static_cast<int64_t>(stoi(str)));
            data.values.push_back(tempValues);
            tempValues.clear();
            str.clear();
        }else if(c == ':'){
            cout << "String is now: ";
            cout << str;
            cout << "\n";
            data.sumAndOrProducts.push_back(static_cast<int64_t>(stoi(str)));
            str.clear();
        }else if(c == ' ' && previous != ':'){
            cout << "String is now: ";
            cout << str;
            cout << "\n";
            tempValues.push_back(static_cast<int64_t>(stoi(str)));
            str.clear();
        }else{
            str += c;
        }
        previous = c;
    }
    tempValues.push_back(static_cast<int64_t>(stoi(str)));
    data.values.push_back(tempValues);
}

void read_file(DATA& data, string const& filename = "input_test.txt") {
    ifstream myFile(filename);
    stringstream stream;
    if (myFile.is_open()){
        cout << "Opened file \n";
        stream << myFile.rdbuf();
        load_input(data,stream);
        myFile.close();
    }else{
        cout << "error";
    }
}

int64_t bitwise_add(int64_t x, int64_t y) {
    while (y != 0) {
        int64_t carry = x & y;  
        x = x ^ y;
        y = carry << 1;
    }
    return x;
}

int64_t bitwise_multiply(int64_t a, int64_t b) {
    int64_t result = 0;
    while (b != 0) {
        if (b & 1) {
            result = bitwise_add(result, a);
        }
        a <<= 1;
        b >>= 1;
    }
    return result;
}

void check_operators(std::vector<int64_t> const& values, int64_t const& result){
/*     bool done = true;
    size_t sizeOfValuesToCheck = values.size(); */
    int64_t tempResult{};
    //while(done){
        tempResult = values[0];
        for (int64_t j{}; j < static_cast<int64_t>(values.size())-1; j++ ){
            cout << values[j];
            cout << " ";
            tempResult = bitwise_multiply(tempResult,values[j+1]) <= result ? bitwise_multiply(tempResult,values[j+1]) : (bitwise_add(tempResult,values[j+1]) <= result ? bitwise_add(tempResult,values[j+1]) : bitwise_multiply(tempResult,values[j+1])); 
        }
        if (tempResult == result){
            cout << " This one worked, with result: ";
        }else{
            cout << " Not ok, with result: ";
        }
        cout << tempResult;
        cout << " \n";
    //}
}

void evaluate_functions(DATA & data){
    for(int64_t i{}; i < static_cast<int64_t>(data.sumAndOrProducts.size()); i++){
        cout << "Now checking : ";
        cout << data.sumAndOrProducts[i];
        cout << ": ";
        check_operators(data.values[i],data.sumAndOrProducts[i]);
        cout << "\n";
    }
}



int main(){
    DATA data;
    read_file(data);
    cout << "Size of data.sumAndOrProducts: ";
    cout << data.sumAndOrProducts.size();
    cout << "\nAnd size of data.values: ";
    cout << data.values.size();
    cout << "\n";
    evaluate_functions(data);
}
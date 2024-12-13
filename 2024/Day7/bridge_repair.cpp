#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <chrono>


using namespace std;
using namespace chrono;

class DATA {
public:
    std::vector<int64_t> sumAndOrProducts;
    std::vector<std::vector<int64_t>> values;
    int64_t totalSums{};
    DATA() {}
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
            tempValues.push_back(static_cast<int64_t>(stoull(str)));
            data.values.push_back(tempValues);
            tempValues.clear();
            str.clear();
        }else if(c == ':'){
            data.sumAndOrProducts.push_back(static_cast<int64_t>(stoull(str)));
            str.clear();
        }else if(c == ' ' && previous != ':'){
            tempValues.push_back(static_cast<int64_t>(stoull(str)));
            str.clear();
        }else{
            str += c;
        }
        previous = c;
    }
    tempValues.push_back(static_cast<int64_t>(stoull(str)));
    data.values.push_back(tempValues);
}

void read_file(DATA& data, string const& filename = "input.txt") {
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


void create_permutations(size_t const& size, string str, vector<string> & differentVariantions, vector<char> operators= {'x','+','|'}){
    if(size == str.size()){
        differentVariantions.push_back(str);
        return;
    }

    for(char c: operators){
        create_permutations(size, str+c, differentVariantions);
    }

}  

int64_t check_operators(std::vector<int64_t> const& values, int64_t const& result){
    int64_t tempResult{};
    vector<string> differentVariantions{};
    create_permutations( (values.size()-1), "", differentVariantions);
    tempResult = static_cast<int64_t>(values[0]);
    for(auto i : differentVariantions){
        for( int64_t j{}; j < static_cast<int64_t>(i.size())+1; j++){
            if(i[j] == 'x'){
                tempResult = tempResult*values[j+1];
            }else if(i[j] == '+'){
                tempResult = tempResult + values[j+1];
            }else if(i[j] == '|'){
                tempResult = stoull(to_string(tempResult) + to_string(values[j+1]));
            }
            if(tempResult > result){
                break;
            }
        }
        if(tempResult == result){
            return tempResult;
        }else{
            tempResult = values[0];
        }
    }
    return 0;
        
}

void evaluate_functions(DATA & data){
    for(int64_t i{}; i < static_cast<int64_t>(data.sumAndOrProducts.size()); i++){
        data.totalSums += check_operators(data.values[i],data.sumAndOrProducts[i]);
    }
}



int main(){
    DATA data;
    read_file(data);
    auto start = high_resolution_clock::now();
    evaluate_functions(data);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
    cout << "Total sums are = " << data.totalSums;

}
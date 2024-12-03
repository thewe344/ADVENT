#include <fstream>
#include <iostream>
#include <regex>

using namespace std;

struct MULTIPLICATORS{
    //Badly declared regex, but it works
    const regex regexFind{"(don[']t[(][)])|(do[(][)])|((mul)[(][0-9]+[,][0-9]+[)])"}; 
    const regex regexMul{"\\((\\d+),(\\d+)\\)"};
    //Store total
    uint64_t total{};  
    //Check and se if valid
    bool doOrDonts = true;
};

void find_mul(MULTIPLICATORS & mults, string & line){
    auto findBegin = std::sregex_iterator(line.begin(), line.end(), mults.regexFind);
    auto findEnd = std::sregex_iterator();
    for (sregex_iterator i = findBegin; i != findEnd; ++i)
    {
        smatch matchMul = *i;
        string matchMulStr = matchMul.str();

        if (matchMulStr == "don't()"){
            mults.doOrDonts = false;
        }else if (matchMulStr == "do()"){
            mults.doOrDonts = true;
        }

        smatch numbers;
        if(regex_search(matchMulStr, numbers, mults.regexMul) && mults.doOrDonts){
            uint64_t tempTotal = stoull(numbers[1])*stoull(numbers[2]);
            mults.total += tempTotal;
        }
        
    }
    
}

void read_file(MULTIPLICATORS & mults){
    ifstream myFile{};
    string line{};
    myFile.open("input.txt");
    if (myFile.is_open()){
        cout << "Opened file \n";
        while (getline(myFile, line)) {
            find_mul(mults,line);
        }
        myFile.close();
    }else{
        cout << "error";
    }
}

int main(){
    MULTIPLICATORS mults;
    read_file(mults);
    cout << "Found totals: ";
    cout << mults.total;
}
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <regex>

using namespace std;

struct SLEIGH_MANUAL{
    uint64_t totalOfMiddleNr{};
    map<uint16_t,vector<uint16_t>> pageOrderingRules{};
    vector<vector<uint16_t>> pageNumbers{};
    const regex regexRules{"(\\d+)\\|(\\d+)"};
    const regex regexPageNr{"(\\d+)"};
};

void load_input(SLEIGH_MANUAL & manualInfo, string & line){
    smatch matchedRules;
    if(regex_search(line, matchedRules, manualInfo.regexRules) ){
        manualInfo.pageOrderingRules[stoi(matchedRules[1])].push_back(stoi(matchedRules[2]));
        cout << "Rules found: ";
        cout << matchedRules[1];
        cout << " | ";
        cout << matchedRules[2];
        cout << "\n";
    }else if(line == "\n"){
        return;
    }else{
        auto findBegin = std::sregex_iterator(line.begin(), line.end(), manualInfo.regexPageNr);
        auto findEnd = std::sregex_iterator();
        string matchNumbers{};
        uint16_t tempNr{};
        vector<uint16_t> tempVec{};
        for (sregex_iterator i = findBegin; i != findEnd; ++i){
            smatch match = *i;
            tempNr = stoul(match.str());
            tempVec.push_back(tempNr);
            cout << tempNr;
            cout << " ";
        }
        manualInfo.pageNumbers.push_back(tempVec);
        cout << "\n";
    }
}


void read_file(SLEIGH_MANUAL & manualInfo){
    ifstream myFile{};
    string line{};
    myFile.open("input.txt");
    if (myFile.is_open()){
        cout << "Opened file \n";
        while (getline(myFile, line)) {
            load_input(manualInfo,line);
        }
        myFile.close();
    }else{
        cout << "error";
    }
}

bool check_if_allowed(SLEIGH_MANUAL & manualInfo, vector<uint16_t> const& i,uint16_t const& j){
    for(auto k : manualInfo.pageOrderingRules[i[j]]){
        auto vectorIter = find(i.begin(),i.end(),k);
        // If value in map ("come-after rule") exists somewhere in the number vector
        if( vectorIter != i.end() ){
            // If that value has index bigger then the nr we are looking at. ex "75 47 61 53 29"  looking at 75, and mapped value 29, 75 => i = 0 and 29 => i = 4
            if ( (vectorIter - i.begin()) < j ){
                cout << "Not okey";
                cout << "\n";
                return false;
            }
        }
        cout << k;
        cout << " ";
    }
    return true;
}

void find_correct(SLEIGH_MANUAL & manualInfo){
    //Looking at page nr's, ex 75 47 61 53 29 
    for (auto i : manualInfo.pageNumbers){
        bool allowed = false;
        // Looking at specific page nr ex 75
        cout << "Looking at line: ";
        for(auto j : i){
            cout << j;
            cout << " ";
        }
        cout << "\n";
        for(uint16_t j {}; j < static_cast<uint16_t>(i.size()); j++ ){
            // If specific page nr is in the map (Left hand side rules)
            if( manualInfo.pageOrderingRules.count(i[j]) ){
                cout << "in if to check mapped values: ";
                // Look at what nrs that map contains (ex for 75 has 29, 53, 47, 61, 13) (Right side of rules, one nr can have multiple "come-after rules")
                allowed = check_if_allowed(manualInfo,i,j);
                cout << "\n\n";
            }
            if(!allowed){
                break;
            }
        }
        if ( allowed ){
            cout << "checkingchecking";
            uint16_t indexOfMiddle = (static_cast<uint16_t>(i.size()))/2;
            cout << "Middle nr index is: ";
            cout << indexOfMiddle;
            cout << "\nAnd nr is: ";
            cout << i[indexOfMiddle];
            cout << "\n\n";
            manualInfo.totalOfMiddleNr+=i[indexOfMiddle];
        } 
    }
}



int main(){
    SLEIGH_MANUAL manualInfo{};
    read_file(manualInfo);
    find_correct(manualInfo);
    cout << "Total of middler numbers are : ";
    cout << manualInfo.totalOfMiddleNr;

}
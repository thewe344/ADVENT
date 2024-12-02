#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
//#include <map>

using namespace std;

struct REPORTS{
    vector<vector<uint16_t>> data;
    uint64_t totalSafeData;

};

void add_input(REPORTS & reportData, string & line){
    vector<uint16_t> tempVec{};
    for (ulong i{}; i<line.size();i++){
        string tempStr{};
        while ((line[i] <= 57) && (line[i] >= 48 )){
            tempStr += line[i];
            i++;
        }
        tempVec.push_back(stoi(tempStr));
    }
    reportData.data.push_back(tempVec);
}

void read_file(REPORTS & reportData){
    ifstream myFile{};
    string line{};
    myFile.open("input.txt",ios::out);
    if (myFile.is_open()){
        cout << "Opened file \n";
        while (getline(myFile, line)) {
            add_input(reportData,line);
        }
        myFile.close();
    }else{
        cout << "error";
    }
}



uint64_t is_it_really_unsafe(vector<uint16_t> & data, vector<uint16_t> const& unsafeIndexes){
    
    for(ulong i{};i<data.size();i++){
        bool safe = true;
        vector<uint16_t> tempVec = data;
        cout << "in loop to double check: ";
        for(ulong k{};k< tempVec.size(); k++){
            cout << tempVec[k];
            cout << " ";
        }
        cout << "\n";

        tempVec.erase(tempVec.begin() + i);

        cout << "After erase: ";
        for(ulong k{};k< tempVec.size(); k++){
            cout << tempVec[k];
            cout << " ";
        }
        cout << "\n";
        
        bool increase = tempVec[1] > tempVec[0];

        for(ulong j{}; j < tempVec.size();j++){
            if (( increase != (tempVec[j] > tempVec[j+1] ? false : true) ) && (j+1 < tempVec.size())){
                safe = false;
                break;
            }else if((increase ? ( ( (tempVec[j+1]-tempVec[j]) > 3) || ( (tempVec[j+1]-tempVec[j]) <= 0) ) : ( ( (tempVec[j]-tempVec[j+1]) > 3) || ( (tempVec[j]-tempVec[j+1]) <= 0) ) )&& (j+1 < tempVec.size())){
                safe = false;
                break;
            }
        }
        if (safe){
            return 1;
        }
    }
    return 0;
}

uint64_t find_safe(vector<uint16_t> & data){
    bool increase = data[1] > data[0]; // Gets first to se if its increasing or decreasing
    vector<uint16_t> unsafeIndexes{};
    for (ulong j{}; j < data.size();j++){
        if (( increase != (data[j] > data[j+1] ? false : true) ) && (j+1 < data.size())){
            cout << data[j];
            cout << " is bigger or smaller then ";
            cout << data[j+1];
            cout << "\n";
            unsafeIndexes.push_back(j);
            //return 0;
        }else if((increase ? ( ( (data[j+1]-data[j]) > 3) || ( (data[j+1]-data[j]) <= 0) ) : ( ( (data[j]-data[j+1]) > 3) || ( (data[j]-data[j+1]) <= 0) ) )&& (j+1 < data.size())){
            //Yeah this is dumb, started small, got bigger
            cout << data[j];
            cout << " has a/no difference to ";
            cout << data[j+1];
            cout << "\n";
            unsafeIndexes.push_back(j);
            //return 0;
        }
    }
    if (!unsafeIndexes.empty()){
        uint64_t tempShow = is_it_really_unsafe(data,unsafeIndexes);
        cout << "After second try ";
        cout << tempShow;
        cout << "\n\n";
        return tempShow;
    }
    return 1;
}


void itterate_list(REPORTS & reportData){
    for(ulong i{};i < reportData.data.size(); i++){

        cout << "At vector index: ";
        cout << i;
        cout << "\n";
        cout << "Lokking at data: ";
        for(ulong j{};j< reportData.data[i].size(); j++){
            cout << reportData.data[i][j];
            cout << " ";
        }
        cout << "\n";

        reportData.totalSafeData += find_safe(reportData.data[i]);
    }
}

int main(){
    REPORTS reportData{{},{}};

    read_file(reportData);
    itterate_list(reportData);
    cout << "Size of data array: ";
    cout << reportData.data.size();
    cout << "\n\n";
/*     cout << "Out of \n";
    for(ulong i{};i< reportData.data.size(); i++){
        for(ulong j{};j< reportData.data[i].size(); j++){
            cout << reportData.data[i][j];
            cout << " ";
        }
        cout << "\n";
    } */
    cout << "There are ";
    cout << reportData.totalSafeData;
    cout << " safe sets\n";


}
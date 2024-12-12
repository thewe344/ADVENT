#include <fstream>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct LOCATION_ID
{
    uint32_t totalDistance;
    uint32_t similarityScore;
    vector<uint32_t> left;
    vector<uint32_t> right;
    map<uint32_t, uint32_t> rightCounts; // add same numbers from right to one spot, 
};
                                            
string find_number(string & line){ 
    string results = "";
    if( (line[0] <= 57) && (line[0] >= 48 ) ){ 
        results += line[0];
        line = line.substr(1);
        results += find_number(line);
    }else if (line[0] == 32){ //32 = whitespace
        line = line.substr(3); 
    }
    return results;
}

void sort_input(vector<uint32_t> & vec, uint32_t const& value, uint32_t index = 0){
    if ( vec.empty() ){
        vec.push_back(value);
    }else if( (vec.size()==1) && (vec[0] <= value) ){
        vec.push_back(value);
    }else if( vec[0] >= value ){
        vec.insert(vec.begin(), value);
    }else if (index > 0 && value >= vec[index - 1] && value <= vec[index]) {
        vec.insert(vec.begin()+index, value);
    }else if ( (index + 1 < static_cast<uint32_t>(vec.size() ) ) && (value > vec[index]) ){
        sort_input(vec,value, index+1);
    }else {
        vec.push_back(value);
    }
    return;
}

void add_input(LOCATION_ID *ids, string & line){
    uint32_t tempLeft = stoi(find_number(line));
    uint32_t tempRight = stoi(line);

    sort_input(ids->left, tempLeft);
    sort_input(ids->right, tempRight);

}

void read_file(LOCATION_ID *ids){
    ifstream myFile{};
    string line{};
    myFile.open("input.txt",ios::out);
    if (myFile.is_open()){
        while (getline(myFile, line)) {
            add_input(ids,line);
        }
        myFile.close();
    }else{
        cout << "error";
    }
}

uint32_t find_similarity(LOCATION_ID *ids, uint32_t const& currentNumber, uint32_t index = 0){
    if (index >= ids->right.size()) {
        return ids->rightCounts[currentNumber];
    }

    if (ids->right[index] < currentNumber) {
        return find_similarity(ids, currentNumber, index + 1);
    } else if (ids->right[index] == currentNumber) {
        ids->rightCounts[currentNumber]++;
        return find_similarity(ids, currentNumber, index + 1);
    } else {
        return ids->rightCounts[currentNumber];
    }
}

void find_distance(LOCATION_ID *ids){
    for (ulong i{}; i < ids->left.size();i++ ){
        ids->totalDistance += (ids->left[i] > ids->right[i]) ? (ids->left[i] - ids->right[i]) : (ids->right[i] - ids->left[i]);

        ids->similarityScore += ids->rightCounts.count(ids->left[i]) ? ids->rightCounts[ids->left[i]]*ids->left[i] : find_similarity(ids, ids->left[i])*ids->left[i];
    }
}


int main(){

    LOCATION_ID ids = {{},{},{},{},{}};
    LOCATION_ID* ptr = &ids;

    read_file(ptr);

    find_distance(ptr);
    
    cout << "Total distance is: ";
    cout << ids.totalDistance;
    cout << " and total similarity score is: ";
    cout << ids.similarityScore;
    cout << "\n\n";
}
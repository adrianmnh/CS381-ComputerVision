/* 

Created by Adrian Noa

Objective: To take an image asset and smoothen each of its pixels using binary and nonbinary threshold operations
Usage:

g++ noa_adrian_main.cpp -o main.exe && ./main.exe data.txt out1.txt out2.txt

OR

g++ noa_adrian_main.cpp -o main.exe

./main.exe data.txt out1.txt out2.txt

*/


#include <iostream>
#include <fstream>
#include <string>
#include "graph.cpp"
using namespace std;

int main(int argc, const char* argv[]) {
    cout << endl;

    if (argc != 2){
        printf("Not enough arguments\n");
        return 1;
    }

    ifstream inFile(argv[1]);
    // ofstream outFile(argv[2]); // binary threshold operation

    if (!inFile.is_open()) {
        cout << "Unable to open file" << endl;
        exit(2);
    }

    int numRows, numCols, minVal, maxVal, pixel;

    inFile >> numRows >> numCols >> minVal >> maxVal;

    int hist[maxVal+1];
    int max_y = 0;

    for (int i = 0; i<=maxVal; i++){
            inFile >> pixel >> hist[i];
            max_y = (hist[i] > max_y) ? max_y = hist[i] : max_y; 
    }
    // cout << max_y;

    inFile.close();

    // for(int k:hist)cout<< k;

    // findMax(hist, maxVal);
    graphLine(maxVal,max_y,hist);

    return 0;
}
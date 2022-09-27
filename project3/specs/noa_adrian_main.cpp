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

class BiMean{

    int numRows, numCols, minVal, maxVal;
    int maxHeight = 0; // largest histAry[i]
    int maxGval; // maximum calculated distribution value;
    int offSet; // one-tenth of the maxVal-minVal
/* 
        in bimodal histogram, the first modal occupies at least one-tenth of
        the histogram population from minVal to maxVal of the histogram
*/ 
    int dividePt; // Initialy set of offset, increases by 1 each iteration.
/*
        Selected treshold value is at the point at divedePt where the "distance"
        between the two bi-Gaussian curves and the histogram is the minimum
*/
    int* histAry = new int[maxVal+1];   // 1D[maxVal+1] to store Histogram Array
                                        // Needs to be dynamically allocated at run-time





};

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
    // int* hist = new int[maxVal+1];
    int maxHeight = 0; // largest histAry[i]

    for (int i = 0; i<=maxVal; i++){
            inFile >> pixel >> hist[i];
            maxHeight = (hist[i] > maxHeight) ? maxHeight = hist[i] : maxHeight; 
    }

    inFile.close();

    int maxGval; // maximum calculated distribution value;
    int offSet; // one-tenth of the maxVal-minVal
    /* 
        in bimodal histogram, the first modal occupies at least one-tenth of
        the histogram population from minVal to maxVal of the histogram
    */ 
   int dividePt; // Initialy set of offset, increases by 1 each iteration.
   /*
        Selected treshold value is at the point at divedePt where the "distance"
        between the two bi-Gaussian curves and the histogram is the minimum
    */
    // int* histAry = new int[maxVal+1]; 
    // 1D[maxVal+1] "modified" Gaussian function
    // histAry = {0};    
    int* histAry[maxVal+1]={0};

    for(int i=0; i<maxVal+1; i++)
    cout << *(histAry+i) << endl;
    // for(int& k:histAry)cout<< k;

    // findMax(hist, maxVal);
    // graphLine(maxVal,maxHeight,hist);
    cout << "deleteing dynamic arrays";
    // delete[] hist;
    // delete[] histAry;

    return 0;
}
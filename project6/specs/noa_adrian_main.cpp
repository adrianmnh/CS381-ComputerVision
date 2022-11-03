/*
Computer Vision
Project 6
Created by Adrian Noa

usage:

g++ noa_adrian_main.cpp -o main && ./main img1

*/



#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <algorithm>
using namespace std;

class SkeletonCompression{

public:

    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int newMinVal;
    int newMaxVal;
    int** ZFAry; //a 2D array, need to dynamically allocatevof size numRows + 2 by numCols + 2.
    int** skeletonAry; //a 2D array, need to dynamically allocate of size numRows + 2 by numCols + 2.

    SkeletonCompression(ifstream& inFile){
        inFile >> numRows >> numCols >> minVal >> maxVal;

        ZFAry = new int*[numRows+2];
        skeletonAry = new int*[numRows+2];

        for (int i = 0; i < numRows+2; i++){
            ZFAry[i] = new int[numCols+2](); // initialize and set zero
            skeletonAry[i] = new int[numCols+2](); 
        }

        loadImage(inFile);
        
    }
// - methods:

    void zero2D(int** Ary){ // algorithm is given in class.
        for (int i = 1; i <= numRows+1; i++) 
            for (int j = 1; j <= numCols+1; j++) 
                Ary[i][j]=0;
    }
    void loadImage(ifstream& inFile){
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                inFile >> ZFAry[i][j];
            }
        }
    }
    void compute8Distance(ofstream& outFile){ // See algorithm below.
        fistPass8Distance();
        reformatPrettyPrint(ZFAry, outFile, "Distance Transform First Pass");
        secondPass8Distance();
        reformatPrettyPrint(ZFAry, outFile, "Distance Transform Second Pass"); 
    }
    void fistPass8Distance(){ // algorithm is given in class.
        int pixel;   
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                pixel = ZFAry[i][j];
                if(pixel){
                    ZFAry[i][j] = checkNeighbors(ZFAry, "min", 1, i, j)+1;
                }
            }
        }
    }
    int checkNeighbors(int** Ary, string min_max="max", int set=1, int i=0 , int j=0 ){

        // return "min" or return "max"
        // neighbor array = {a,b,c, d} set=1 or {f,g,h, e} set=-1
        int arr[] = {Ary[i -1*set][j-1], Ary[i -1*set][j],Ary[i -1*set][j+1],Ary[i][j -1*set]};

        if(min_max == "min"){
            return *min_element(begin(arr), end(arr));
        } 
        return *max_element(begin(arr), end(arr));

    }
    void secondPass8Distance(){ // algorithm is given in class.
        int pixel;   // keep track of newMinVal and newMaxVal.
        newMinVal = 9999;
        newMaxVal = -1;
        for (int i = numRows; i >= 1; i--) {
            for (int j = numCols; j >= 1; j--) {
                pixel = ZFAry[i][j];
                if(pixel){
                    ZFAry[i][j] = min(checkNeighbors(ZFAry, "min", -1, i, j)+1, pixel);
                    if(ZFAry[i][j] < newMinVal) newMinVal = ZFAry[i][j];
                    if(ZFAry[i][j] > newMaxVal) newMaxVal = ZFAry[i][j];
                }
            }
        }
    }
    void imageCompression(ofstream& outFile, ofstream& skeleton){ // See algorithm below
        computeLocalMaxima();
        reformatPrettyPrint(skeletonAry, outFile, "Skeleton Image from Local Maxima");

        extractSkeleton(skeleton);
    }
    bool isLocalMaxima(int i, int j){ // A pixel is local maxima if >= to all its 8 neighbors. On your own
        int pixel = ZFAry[i][j];
        if (pixel >= checkNeighbors(ZFAry, "max", 1, i,j) && pixel >= checkNeighbors(ZFAry, "max", -1, i, j)) 
            return true;
        return false;
    }
    void computeLocalMaxima(){ // Check all pixels, ZFAry[i,j] in ZFAry
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                if(isLocalMaxima(i,j)){
                    skeletonAry[i][j] = ZFAry[i][j];
                }
            }
        }
    }
    // Please note, in real life, i and j need to subtract by 1 since skeletonAry has been framed;
    // however, for easy programming and since we are reusing ZFAry,
    // i and j do not need to subtract by 1.
    void extractSkeleton (ofstream& skeletonFile){ // if skeletonAry[i,j] > 0 write the triplet (i.e., i, j, skeletonAry[i,j]) to
        skeletonFile << numRows << " " << numCols << " " << newMinVal << " " << newMaxVal << endl;
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                if(skeletonAry[i][j] > 0)
                    skeletonFile << i << " " << j << " " << skeletonAry[i][j] << endl;
                
            }
        }
    }
    void loadSkeleton(ifstream& skeletonFile){ // Load the skeleton file onto inside frame of ZFAry
        zero2D(ZFAry);
        int i, j, val;
        skeletonFile >> numRows >> numCols >> newMinVal >> newMaxVal;
        // while(!skeletonFile.eof()){
        while(skeletonFile >> i >> j >> val){
            ZFAry[i][j] = val;
        }
    }
    void imageDeCompression(ofstream& outFile){  // See algorithm below
        firstPassExpansion();
        print(ZFAry);
        reformatPrettyPrint(ZFAry, outFile, "Expansion First Pass");
        secondPassExpansion();
        reformatPrettyPrint(ZFAry, outFile, "Expansion Second Pass");

    }
    void firstPassExpansion(){ // algorithm is given in class.
        int pixel;
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                pixel = ZFAry[i][j];
                if(pixel==0){
                    ZFAry[i][j] = max(max(checkNeighbors(ZFAry, "max", 1, i, j)-1, //top
                                        checkNeighbors(ZFAry, "max", -1, i, j)-1), //bottom
                                    pixel);
                }
            }
        }
    }
    void secondPassExpansion(){ // algorithm is given in class.
        int pixel, m;   // keep track of newMinVal and newMaxVal.
        newMinVal = 9999;
        newMaxVal = -1;
        for (int i = numRows; i >= 1; i--) {
            for (int j = numCols; j >= 1; j--) {
                pixel = ZFAry[i][j];
                m = max(checkNeighbors(ZFAry, "max", 1, i, j)-1, checkNeighbors(ZFAry, "max", -1, i, j)-1);
                if(pixel < m){
                    ZFAry[i][j] = m;
                    // if(ZFAry[i][j] < newMinVal) newMinVal = ZFAry[i][j];
                    // if(ZFAry[i][j] > newMaxVal) newMaxVal = ZFAry[i][j];
                }
            }
        }
    }
    void threshold(int threshold, ofstream& decompress){  // do a threshold on pixels inside of ZFAry with the threshold value at 1;
        newMinVal = 0;
        newMaxVal = threshold;
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                int pixel = ZFAry[i][j];
                if(pixel >= 1){
                    ZFAry[i][j] = 1;
                }
            }
        }
        extractImage(decompress);
    }
// i.e., if ZFAry (i, j) >= 1
// output 1 and a blank space to decompressed file.
// else
// output 0 and a blank space to decompressed file.
    void reformatPrettyPrint(int** Ary, ofstream& outFile, string s){ // reuse codes from your previous project
        drawTitle(outFile, s);
        for(int i = 0; i <= numRows+1;i++) {
            for (int j = 0; j <= numCols+1; j++) {
                if(Ary[i][j]>=10) outFile << Ary[i][j] <<" ";
                else if(Ary[i][j]>0) outFile << " " << Ary[i][j] <<" ";
                else outFile << " . ";
            }
            outFile << endl;        
        }
    }
    void drawTitle(ofstream &outFile, string str){
        int maxL = numCols * 3;
        int l = ((maxL+1)/2);
        int sl = l - str.length()/2;
        for(int r=-1; r<3; r++){
            for (int i = 0; i <= maxL + (3+1)*1; i++){
                if(r==0 || r==2) outFile << "-";
                else if(r==1){
                    if(i==sl-1) outFile << "  " << str << "  ";
                    else if(i<=sl-1) outFile << " ";
                    else if (i>sl+str.length()+2) outFile << " ";
                } else outFile << " ";
            } outFile << endl;
        }
    }
    void print(int** Ary){
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                cout << Ary[i][j] << " ";
            }
            cout << "\n";
            
        }
    }
    void extractImage(ofstream& outFile){
        outFile << numRows << " " << numCols << " " << newMinVal << " " << newMaxVal << endl;
        // outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                outFile << ZFAry[i][j] << " ";
            }
            outFile << "\n";
            
        }
    }
    ~SkeletonCompression(){
        for (int i = 0; i < numRows+2; i++){
            delete[] ZFAry[i];
            delete[] skeletonAry[i];
        }
        delete[] ZFAry;
        delete[] skeletonAry;
    }
};

int main(int argc, const char* argv[]){
    ifstream inFile(argv[1]);
    ofstream outFile(string(argv[1])+"_outFile");
    ofstream skeletonFile(string(argv[1])+"_skeleton");
    ofstream deCompressFile(string(argv[1])+"_deCompressed");

    SkeletonCompression sc = SkeletonCompression(inFile);
    
    sc.compute8Distance(outFile);
    // sc.print();

    sc.imageCompression(outFile, skeletonFile);
    skeletonFile.close();

    ifstream skeletonFile2(string(argv[1])+"_skeleton");
    sc.loadSkeleton(skeletonFile2);
    skeletonFile2.close();

    sc.imageDeCompression(outFile);
    sc.threshold(1, deCompressFile);

    deCompressFile.close();
    outFile.close();
    inFile.close();
    return 0;
}

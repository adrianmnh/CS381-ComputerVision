/* 

Created by Adrian Noa

Objective: 

To take an image's histogram from a text and find the best threshold using the bi-Gaussian automatic threshold selection method.

This process yields a Gaussian function which will be graphed along with the histogram, and the best threshold.

Usage:

g++ noa_adrian_main.cpp -o main.exe && ./main.exe BiGuass_data2.txt outFile1.txt outFile2.txt && rm ./main.exe

*/


#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <string>
using namespace std;


class Morphology{

public:

    int numImgCols;
    int numImgRows;
    int imgMin;
    int imgMax;
    int numStructRows;
    int numStructCols;
    int structMin;
    int structMax;
    int rowOrigin;
    int colOrigin;

    int rowFrameSize;    // set to (numStructRows / 2), integer division, i.e., 3/2 is 1; 4/2 is 2; 5/2 is 2.
    int colFrameSize;    // set to (numStructCols / 2).
    int extraRows;      // set to (rowFrameSize * 2)
    int extraCols;      // set to (colFrameSize * 2)
    int rowSize;        // set to (numImgRows + extraRows)
    int colSize;        // set to (numImgCols + extraCols)

    int** zeroFramedAry; // a dynamically allocate 2D array, size of rowSize by colSize, for the input image.
    int** morphAry; // Same size as zeroFramedAry.
    int** tempAry; // Same size as zeroFramedAry.// tempAry is to be used as the intermediate result in opening and closing operations.
    int ** structAry; //a dynamically allocate 2D array of size numStructRows by numStructCols, for structuring element.

    Morphology(ifstream &inFile1, ifstream &inFile2){

        cout << "Morphology" << endl;

        
        // read(inFile1, inFile2);
        // allocate();

        // loadImg(inFile1);
        // loadstruct(inFile2);
        // printImg();
        // printStruct();
    }

    void read(ifstream &inFile1, ifstream &inFile2){
        inFile1 >> numImgRows >> numImgCols >> imgMin >> imgMax;
        inFile2 >> numStructRows >> numStructCols >> structMin >> structMax >> rowOrigin >> colOrigin;

        rowFrameSize = numStructRows / 2;
        colFrameSize = numStructCols / 2;
        extraRows = rowFrameSize + rowOrigin;    
        extraCols = colFrameSize + colOrigin;    
        rowSize = numImgRows + extraRows;
        colSize = numImgCols + extraCols;

        cout << endl 
        << numImgCols << "-numImgCols " << endl
        << numImgRows << "-numImgRows " << endl
        << imgMin << "-imgMin " << endl
        << imgMax << "-imgMax " << endl
        << numStructRows << "-numStructRows " << endl
        << numStructCols << "-numStructCols " << endl
        << structMin << "-structMin " << endl
        << structMax << "-structMax " << endl
        << rowOrigin << "-rowOrigin " << endl
        << colOrigin << "-colOrigin " << endl
        << rowFrameSize << "-rowFrameSize " << endl
        << colFrameSize << "-colFrameSize " << endl
        << extraRows << "-extraRows " << endl
        << extraCols << "-extraCols " << endl
        << rowSize << "-rowSize " << endl  
        << colSize << "-colSize" << endl << endl;
    } 

    void allocate(){

        zeroFramedAry = new int*[rowSize];
        cout << "Allocated Histogram 2D char array" << endl;

        morphAry = new int*[rowSize];
        cout << "Allocated Gaussian graph 2D char array" << endl;

        tempAry = new int*[rowSize];
        cout << "Allocated Histogram 2D char array" << endl;

        structAry = new int*[rowSize];
        cout << "Allocated Struct 2D array" << endl;

        for(int i=0; i<rowSize;i++) {
            if(i<numStructRows+1) structAry[i] = new int[numStructCols+1]();
            zeroFramedAry[i] = new int[colSize]();
            morphAry[i] = new int[colSize]();
            tempAry[i] = new int[colSize]();
            // for (int j = 0; j < numImgCols+1; j++) {
            //     // zeroFramedAry[i][j]=' ';
            //     // morphAry[i][j]=' ';
            //     // tempAry[i][j]=' ';
            //     // if(j<=numStructCols)structAry[i][j]=' ';
            // }            
        }
        cout << "FINISHED ALLOCATING" << endl;
    } 
    void zero2DAry(int** Ary, int nRows, int nCols){
        for (int i = 0; i < nRows; i++){
            for (int j = 0; j < nCols; j++){
                Ary[i][j] = 0;
            }
        }
    } // Set the entire Ary (nRows by nCols) to zero.

    void loadImg(ifstream &inFile1){ // load imgFile to zeroFramedAry inside of frame, begins at (rowOrigin, colOrigin)
        for(int i = 0; i<numImgRows;i++) {
            for (int j = 0; j < numImgCols; j++) {
                inFile1 >> zeroFramedAry[i+rowOrigin][j+colOrigin];
            }            
        }

    }

    void loadstruct(ifstream &inFile2){// load structFile to structAry.
        for(int i=0; i<numStructRows;i++) {
            for (int j = 0; j < numStructCols; j++) {
                inFile2 >> structAry[i][j];
            }            
        }
    }

    void ComputeDilation(int** inAry, int** outAry){ // process every pixel in inAry, put result to outAry // see algorithm below.
        for (int i = rowOrigin; i < numImgRows+rowOrigin; i++){
            for (int j = colOrigin; j < numImgCols+colOrigin; j++){
                if(inAry[i][j] > 0){
                    onePixelDilation(i, j, inAry, outAry);
                }
            }   
        }        
    } 
    void ComputeErosion(int** inAry, int** outAry){ // process every pixel in inAry, put result to outAry // see algorithm below.
        for (int i = rowOrigin; i < numImgRows+rowOrigin; i++){
            for (int j = colOrigin; j < numImgCols+colOrigin; j++){
                if(inAry[i][j] > 0){
                    onePixelErosion(i, j, inAry, outAry);
                }
            }   
        }
    }
    void ComputeOpening(){ // see algorithm below.
        zero2DAry(tempAry, rowSize, colSize);
        ComputeErosion(zeroFramedAry, tempAry);
        ComputeDilation(tempAry, morphAry);
    }

    void ComputeClosing(){ // see algorithm below.
        zero2DAry(tempAry, rowSize, colSize);
        ComputeDilation(zeroFramedAry, tempAry);
        ComputeErosion(tempAry, morphAry);
    }

    void onePixelDilation(int i, int j, int** inAry, int** outAry){ // Perform dilation on pixel (i, j) with structAry. // See algorithm below.
        int iOffset = i-rowOrigin; 
        int jOffset = j-colOrigin;
        for (int i = 0  ; i < numStructRows; i++){
            for (int j = 0 ; j < numStructCols; j++){
                if(structAry[i][j] > 0){
                    outAry[iOffset+i][jOffset+j] = 1;
                }
            }   
        }
    }
    void onePixelErosion(int i, int j, int** inAry, int** outAry){ // Perform erosion on pixel (i, j) with structAry. // See algorithm below.
        int iOffset = i-rowOrigin; 
        int jOffset = j-colOrigin;
        bool matchFlag = true;
        for (int i = 0  ; i < numStructRows && matchFlag==true; i++){
            for (int j = 0 ; j < numStructCols && matchFlag==true; j++){
                if(structAry[i][j] > 0 && inAry[iOffset+i][jOffset+j]<=0){
                    matchFlag = false;
                }
            }   
        }
        outAry[i][j] = 0;
        if(matchFlag) outAry[i][j] = 1;
    }

    void AryToFile(int** Ary, ofstream &outFile, string s) {// output the image header (from input image header) //then output the rows and cols of Ary to outFile *excluding* the framed borders of Ary.
        prettyPrintImg(morphAry, outFile, s);
    }
    // void prettyPrint(Ary, outFile) // Remark: use “Courier new” font and small font size to fit in the page.// if Ary [i, j] == 0 output “. ” // a period follows by a blank// else output “1 “ // 1 follows by a blank



    ~Morphology(){

        for(int i=0; i<rowSize; i++){
            if(i<numStructRows+1) delete[] structAry[i];
            delete[] zeroFramedAry[i];
            delete[] morphAry[i];
            delete[] tempAry[i];
        }

        cout << "delete all subarrays in histgraph and gausGraph" << endl;
        delete[] zeroFramedAry;
        cout << "deleting histGraph memory allocation" << endl;
        delete[] morphAry;
        cout << "deleting gausGraph memory allocation";
        cout << "delete all subarrays in histgraph and gausGraph" << endl;
        delete[] tempAry;
        cout << "deleting histGraph memory allocation" << endl;
        delete[] structAry;
        cout << "deleting gausGraph memory allocation";

    }

    void printImg(){
        for (int i = 0; i < rowSize; i++){
            for (int j = 0; j < colSize; j++){
                if(zeroFramedAry[i][j]==1)cout << zeroFramedAry[i][j];
                else cout << ".";
                cout << "  ";
            }
            cout << endl;
        }
    }

    void printStruct(){
        for (int i = 0; i < numStructRows; i++){
            for (int j = 0; j < numStructCols; j++){
                if(structAry[i][j]==1) cout << structAry[i][j];
                else cout << ".";
                cout << " ";
            }
            cout << endl;
        }
    }

    void prettyPrintImg(int** Ary, ofstream &outFile, string s){
        string s2 = toString(numImgRows,numImgCols,imgMin,imgMax);
        drawTitle(outFile, s, s2);
        for(int i = 0; i<numImgRows;i++) {
            for (int j = 0; j < numImgCols; j++) {
                if(Ary[i+rowOrigin][j+colOrigin]>0) outFile << "1  ";
                else outFile << ".  ";
            }
            outFile << endl;        
        }
    }

    void prettyPrintFrame(int** Ary, ofstream &outFile, string s){
        string s2 = toString(rowSize,colSize,imgMin,imgMax);
        drawTitle(outFile, s, s2);

        for(int i = 0; i<rowSize;i++) {
            for (int j = 0; j < colSize; j++) {
                if(Ary[i][j]>0) outFile << "1  ";
                else outFile << ".  ";
            }
            outFile << endl;        
        }
    }

    void prettyPrintStruct(ofstream &outFile, string s){
        string s2 = toString(numStructRows,numStructCols,structMin,structMax);
        drawTitle(outFile, s, s2);
        for(int i = 0; i<numStructRows; i++) {
            for (int j = 0; j < numStructCols; j++) {
                if(structAry[i][j]>0) outFile << "1  ";
                else outFile << ".  ";
            }
            outFile << endl;        
        }
    }

    string toString(int a, int b, int c, int d){
        return to_string(a)+" "+to_string(b)+" "+to_string(c)+" "+to_string(d);
    }

    void drawTitle(ofstream &outFile, string str, string str2){
        int size = max(str.length(), str2.length());

        int l = ((size*2+1)/2)-1;
        int sl = l - str.length()/2;

        int l2 = ((size*2+1)/2)-1;
        int sl2 = l - str2.length()/2;

        for(int r=-1; r<4; r++){
            for (int i = 0; i < (size+1)*2; i++){
                if(r==-1 || r==1 || r ==3) outFile << "-";
                else if(r==0){
                    if(i==sl-1) outFile << "  " << str << "  ";
                    else if(i<=sl-1) outFile << " ";
                    else if (i>sl+str.length()+2) outFile << " ";
                } else {
                    if(i==sl2-1) outFile << "  " << str2 << "  ";
                    else if(i<=sl2-1) outFile << " ";
                    else if (i>sl2+str2.length()+2) outFile << " ";
                } 
            } outFile << endl;
        }
    }

};

int main(int argc, const char* argv[]) {
    cout << endl;

    //Step 0   

    if (argc != 8){
        printf("Not enough arguments\n");
        return 1;
    }

    ifstream inFile1(argv[1]);
    ifstream inFile2(argv[2]);     
    ofstream dilateOutFile(argv[3]); 
    ofstream erodeOutFile(argv[4]); 
    ofstream openingOutFile(argv[5]); 
    ofstream closingOutFile(argv[6]); 
    ofstream prettyPrintFile (argv[7]);

    Morphology m = Morphology(inFile1, inFile2);

    //Step 1
    m.read(inFile1, inFile2);

    //Step 2
    m.allocate();

    // Step 3
    m.zero2DAry(m.zeroFramedAry, m.rowSize, m.colSize);

    // Step 4
    m.loadImg(inFile1);
    m.prettyPrintImg(m.zeroFramedAry, prettyPrintFile, "Input Image");

    //Step 5
    m.loadstruct(inFile2);
    m.prettyPrintStruct(prettyPrintFile, "Struct");

    // Step 9
    m.zero2DAry(m.morphAry, m.rowSize, m.colSize);
    m.ComputeClosing();
    m.AryToFile(m.morphAry, closingOutFile, "Closing Operation");
    m.prettyPrintImg(m.morphAry, prettyPrintFile, "Closing Operation");

    //Step 7
    m.zero2DAry(m.morphAry, m.rowSize, m.colSize);
    m.ComputeErosion(m.zeroFramedAry, m.morphAry);
    m.AryToFile(m.morphAry, erodeOutFile, "Erosion Operation");
    m.prettyPrintImg(m.morphAry, prettyPrintFile, "Erosion Operation");

    //Step 6
    m.zero2DAry(m.morphAry, m.rowSize, m.colSize);
    m.ComputeDilation(m.zeroFramedAry, m.morphAry);
    // arrayToFile
    m.prettyPrintFrame(m.morphAry, prettyPrintFile, "Dilation Operation");

    // Step 8
    m.zero2DAry(m.morphAry, m.rowSize, m.colSize);
    m.ComputeOpening();
    // arrayToFile
    m.prettyPrintImg(m.morphAry, prettyPrintFile, "Opening Operation");



    inFile1.close();
    inFile2.close();
    dilateOutFile.close(); 
    erodeOutFile.close(); 
    openingOutFile.close(); 
    closingOutFile.close(); 
    prettyPrintFile.close();


    return 0;
}
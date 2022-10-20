/* 

Created by Adrian Noa

Objective: 

To perform morphology operations on an image using a structuring element. The core operations are Erosion and Dilation.

- Erosion operation transforms an image by turning pixels into 1 if all pixels surrounding the current point match the pixels in the structuting element. 0 Otherwise.

- Dilation operation transforms all pixels surrounding the current point in the image into the structuring element if the origin is 1. 0 Otherwise

- Opening operation performs a erosion followed by a dilation

- Closing operation performs dilation followed by erosion

This program yields a series of morphed image files created by applying the operations to the input image

Usage:

This will execute the program and place all outputs in a separate file.

g++ noa_adrian_main.cpp -o main.exe && ./main.exe data1.txt Elem1.txt ./out/_1close ./out/_2erode ./out/_3dilate ./out/_4open ./out/_pretty

*/


#include <iostream>
#include <sys/stat.h>
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
        cout << "Morphology Class" << endl;
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
    } 

    void allocate(){

        zeroFramedAry = new int*[rowSize];
        cout << "Allocated Framed 2D array" << endl;

        morphAry = new int*[rowSize];
        cout << "Allocated image morphing 2D array" << endl;

        tempAry = new int*[rowSize];
        cout << "Allocated temp 2D array" << endl;

        structAry = new int*[rowSize];
        cout << "Allocated Struct 2D array" << endl;

        for(int i=0; i<rowSize;i++) {
            if(i<numStructRows+1) structAry[i] = new int[numStructCols+1]();
            zeroFramedAry[i] = new int[colSize]();
            morphAry[i] = new int[colSize]();
            tempAry[i] = new int[colSize]();
        }
    } 

    void zero2DAry(int** Ary, int nRows, int nCols){ // Set the entire Ary (nRows by nCols) to zero. 
        for (int i = 0; i < nRows; i++){
            for (int j = 0; j < nCols; j++){
                Ary[i][j] = 0;
            }
        }
    } 

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
        for (int i = rowOrigin; i < numImgRows+rowOrigin; i++)
            for (int j = colOrigin; j < numImgCols+colOrigin; j++)
                if(inAry[i][j] > 0)
                    onePixelErosion(i, j, inAry, outAry);
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
        for (int i = 0  ; i < numStructRows; i++)
            for (int j = 0 ; j < numStructCols; j++)
                if(structAry[i][j] > 0)
                    outAry[iOffset+i][jOffset+j] = 1;
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

    void AryToFile(int** Ary, ofstream &outFile) {// output the image header (from input image header) //then output the rows and cols of Ary to outFile *excluding* the framed borders of Ary.
        string s = toString(numImgRows,numImgCols,imgMin,imgMax);
        outFile << s << endl;
        for(int i = 0; i<numImgRows;i++) {
            for (int j = 0; j < numImgCols; j++) {
                if(Ary[i+rowOrigin][j+colOrigin]>0) outFile << "1 ";
                else outFile << "0 ";
            }
            outFile << endl;        
        }    
    }
    // void prettyPrint(Ary, outFile) // Remark: use “Courier new” font and small font size to fit in the page.// if Ary [i, j] == 0 output “. ” // a period follows by a blank// else output “1 “ // 1 follows by a blank



    ~Morphology(){

        for(int i=0; i<rowSize; i++){
            if(i<numStructRows+1) delete[] structAry[i];
            delete[] zeroFramedAry[i];
            delete[] morphAry[i];
            delete[] tempAry[i];
        }
        cout << "delete all subarrays in zeroFramed, morphAry, tempAry, structAry" << endl;

        delete[] zeroFramedAry;
        cout << "deleting zeroFramed memory allocation" << endl;
        delete[] morphAry;
        cout << "deleting morphAry memory allocation" << endl;
        delete[] tempAry;
        cout << "deleting tempAry memory allocation" << endl;
        delete[] structAry;
        cout << "deleting structAry memory allocation";

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

    string stringpath = "./out/";
    int status = mkdir(stringpath.c_str(),0777);

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
    ofstream prettyPrintFile(argv[7]);

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
    m.AryToFile(m.morphAry, closingOutFile);
    m.prettyPrintImg(m.morphAry, prettyPrintFile, "Closing Operation");

    //Step 7
    m.zero2DAry(m.morphAry, m.rowSize, m.colSize);
    m.ComputeErosion(m.zeroFramedAry, m.morphAry);
    m.AryToFile(m.morphAry, erodeOutFile);
    m.prettyPrintImg(m.morphAry, prettyPrintFile, "Erosion Operation");

    //Step 6
    m.zero2DAry(m.morphAry, m.rowSize, m.colSize);
    m.ComputeDilation(m.zeroFramedAry, m.morphAry);
    m.AryToFile(m.morphAry, dilateOutFile);
    m.prettyPrintFrame(m.morphAry, prettyPrintFile, "Dilation Operation");

    // Step 8
    m.zero2DAry(m.morphAry, m.rowSize, m.colSize);
    m.ComputeOpening();
    m.AryToFile(m.morphAry, openingOutFile);
    m.prettyPrintImg(m.morphAry, prettyPrintFile, "Opening Operation");

    inFile1.close();
    inFile2.close();
    dilateOutFile.close(); 
    erodeOutFile.close(); 
    openingOutFile.close(); 
    closingOutFile.close(); 
    prettyPrintFile.close();

    cout << endl << "Files written to folder: ./out/"<< endl << endl;

    return 0;
}
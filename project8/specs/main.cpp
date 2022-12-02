#include <iostream>
#include <string>
#include <cmath>
#include <fstream>

using namespace std;

class ChainCode {

public:

    struct point {
        int row;
        int col;
    };

    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int label; // object's label > 0


    int** imgAry; // a 2D array to store the input image,
// needs to dynamically allocate at run time (numRows+2 by numCols+2) and initialized to zero.
    int** reconstructAry; // a 2D array needs to dynamically allocate at run time (numRows+2 by numCols+2)
// to be use for the reconstructed object: the boundary of object and for filled object,

    point coordOffset[8]; // coordOffset [ ] are: [(0, +1), (-1, +1), (-1, 0), (-1, -1), (0, -1), (+1, -1), (+1, 0), (+1, +1)]
    // for calculating the row and col positions of a given pixel’s 3x3 neighbors. For example, given P(r, c),
    //neighbors at 0 direction would be (r+coordOffset[0].row, c+ coordOffset[0].col)
    //which is imgAry (r+0, c+1); at 1 direction would be (r+coordOffset[1].row, c+ coordOffset[1].col),
    //which is imgAry (r-1, c+1), etc. You may hard code this offSet table.


    int zeroTable[8] = { 6, 0, 0, 2, 2, 4, 4, 6 }; // The look-up table to update the lastZero direction from currentP to
    // nextP. You may *hard code* this table as given in the lecture.

    point startP; // the row and col position of the first none zero pixel.
    point currentP; // the row and col position of the current none zero border pixel
    point nextP; // the row and col position of the next none-zero border pixel
    int lastZero; // Range from 0 to 7; it is the direction of the last zero scanned before currentP
    int chainDir; // The chain code direction from currentP to nextP

// methods:

    ChainCode(ifstream& inFile){
        cout << "chaincode object"<<endl;
        allocate(inFile);
        loadImage(inFile);
    }

    void allocate(ifstream& inFile){
        cout << "Allocate\n";
        inFile >> numRows >> numCols >> minVal >> maxVal;
        // cout << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;

        label = 1;
        lastZero = 0;

        imgAry = new int*[numRows+2];
        reconstructAry = new int*[numRows+2];
        for (int i = 0; i < numRows+2; i++){
            imgAry[i] = new int[numCols+2]();
            reconstructAry[i] = new int[numCols+2]();
        }
        coordOffset[0].row = 0;
        coordOffset[0].col = 1;

        coordOffset[1].row = -1;
        coordOffset[1].col = 1;

        coordOffset[2].row = -1;
        coordOffset[2].col = 0;

        coordOffset[3].row = -1;
        coordOffset[3].col = -1;

        coordOffset[4].row = 0;
        coordOffset[4].col = -1;

        coordOffset[5].row = 1;
        coordOffset[5].col = -1;

        coordOffset[6].row = 1;
        coordOffset[6].col = 0;

        coordOffset[7].row = 1;
        coordOffset[7].col = 1;

    }
    
    void loadImage(ifstream& inFile){ // Read from the inFile store onto imgAry, begin at (1,1)
        for (int i = 1; i <= numRows; i++)
            for (int j = 1; j <= numCols; j++)
                inFile >> imgAry[i][j];
    }

    void getChainCode(ofstream& ccFile){ // see the algorithm below.
        ccFile << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;

        int lastQ = findFirstP(ccFile); // steps 0 -> 2
        cout << "First point found\n";

        do{

            // print();
            lastQ = (lastQ+1) % 8;
            chainDir = findNextP(currentP, lastQ);
            ccFile << chainDir << " ";
            nextP.row = currentP.row + coordOffset[chainDir].row;
            nextP.col = currentP.col + coordOffset[chainDir].col;
            currentP = nextP;

            // if(chainDir == 0) lastQ = zeroTable[7];
            // else lastQ = zeroTable[(chainDir-1)%8];
            lastQ = zeroTable[ (chainDir-1 + 8) % 8]; // c++ mod on negatives doesnt work well
            // cout <<"(" << currentP.row << "," << currentP.col << ")\n";
        }
        while(currentP.row != startP.row || currentP.col != startP.col);

        cout << "Chain Code algo finished\n";

    }

    int findFirstP(ofstream& ccFile){

        for (int i = 1; i <= numRows; i++)
            for (int j = 1; j <= numCols; j++)
                if(imgAry[i][j]>0){
                    startP.row = i;
                    startP.col = j;
                    ccFile << imgAry[i][j] << " " << i << " " << j << "\n";
                    currentP.row = i;
                    currentP.col = j;
                    int lastQ = 4;
                    return lastQ;         
                }
        return -1;
    }

    int findNextP(point currentP, int lastQ){ // see algorithm below.
        int index = lastQ;
        bool found = false;
        int iRow, jCol;
        while (!found){
            iRow = currentP.row + coordOffset[index].row;
            jCol = currentP.col + coordOffset[index].col;
            // cout <<"(" << iRow << "," << jCol << ")\n";

            if(imgAry[iRow][jCol] > 0 ){
                chainDir = index;
                // cout << chainDir << endl;
                found = true;
                imgAry[iRow][jCol] = label;
            } else {
                index = (index+1) % 8;
            }
        }
        return chainDir;
    }

    void print(){
        for (int i = 0; i < numRows+2; i++) {
            for (int j = 0; j < numCols+2; j++) {
                if(imgAry[i][j] == 0)cout << "0 ";
                else cout << imgAry[i][j] << " ";        
            }
            cout << "\n";
        }

    }

    void decompression(ifstream& in_ccFile, ofstream& debugFile ){ // see algorithm below.

        Ary2File(reconstructAry, debugFile, "Reconstruct Array");

        int temp, label, startRow, startCol, mark, nextChainCode, row, col;
        bool flag = true;
        in_ccFile >> temp >> temp >> temp >> temp;

        in_ccFile >> label >> startRow >> startCol;

        reconstructAry[startRow][startCol] = label;
        mark = -1*label;
        point P;
        P.row = startRow;
        P.col = startCol;
        lastZero = 4;

        while(flag){
            //step 1
            in_ccFile >> nextChainCode;

            while(lastZero != nextChainCode){
            // while(lastZero < nextChainCode){

                //step 2
                row = P.row + coordOffset[lastZero].row;
                col = P.col + coordOffset[lastZero].col;

                // if ( reconstructAry[row][col] == 0 ){
                if ( reconstructAry[row][col] != label ){
                    reconstructAry[row][col] = mark;
                } else {
                    debugFile << "Somethings wrong\n";
                    debugFile << reconstructAry[row][col] << endl;
                    debugFile << row << ","<<col<<endl;
                }
                //step 3
                lastZero = ++lastZero % 8;
                // lastZero++;
                // lastZero = (lastZero+1)%8;

                //if 0 == 7
            }

            //step 5
            P.row = P.row + coordOffset[nextChainCode].row;
            P.col = P.col + coordOffset[nextChainCode].col;

            reconstructAry[P.row][P.col] = label;

            lastZero = zeroTable[(nextChainCode-1 +8) %8 ];
            // lastZero = (nextChainCode - 1 + 8)%8;
            // lastZero = zeroTable[(nextChainCode - 1) %8 ];

            // if(nextChainCode == 0) lastZero = zeroTable[7];
            // else lastZero = zeroTable[(nextChainCode-1)%8];

            //step 6
            if(P.row == startRow && P.col == startCol) flag = false;
        }

        Ary2File(reconstructAry, debugFile, "Decompressed Chain Code");
        prettyPrintImg(reconstructAry, debugFile, "Decompressed Chain Code pretty print");
        
        cout << "decompression end \n";

        fillInterior(reconstructAry, label);
        prettyPrintImg(reconstructAry, debugFile, "Interior Filled pretty print");

        threshold(0);
        prettyPrintImg(reconstructAry, debugFile, "Threshold Image pretty print");




    }
    void fillInterior (int** Ary, int label ){ // fill interior object pixels. See algorithm below
        for (int i = 1; i <= numRows; i++){
            cout << "row: " << i<< endl;
            bool odd = false;
            for (int j = 1; j <= numCols; j++) {
                bool flag = true;
                if(Ary[i][j] == -1){
                    odd = false;
                    cout << "first: " << j<< endl;

                    while( Ary[i][++j] == -1 && j<=numCols){
                        cout << j << " continue -1\n";
                        continue;                        
                    }

                    if(Ary[i][j] == label){
                        loop:
                        while( !odd && j<=numCols){
                            Ary[i][j++] = label;
                            cout << j << " next\n";
                            if(Ary[i][j] == -1 && Ary[i][j+1] == label) {
                                cout << "continue looping odd peaks \n";
                                goto loop;
                                odd = false;
                            }
                            else if(Ary[i][j] == -1) {
                                cout << "Loop break\n";
                                odd = true;
                            }
                        }
                    }

                }
            }
        }    
    }

    void threshold(int thr){ // turn negative pixels to zero. See algorithm below.
        for (int i = 0; i <= numRows + 1; i++){
            for (int j = 0; j <= numCols + 1; j++) {
                if( reconstructAry[i][j] > thr )
                    reconstructAry[i][j] = label;
                else reconstructAry[i][j] = minVal;
            }
            
        }
    }

    void Ary2File (int** ary, ofstream& outFile, string s){ // output inside frame of ary to file, use reformatPrettyPrint.
        drawTitle(outFile, s);

        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                if(ary[i][j] == 0)outFile << "  " << ary[i][j];
                else if(ary[i][j]<0)outFile << " " << ary[i][j];  
                else outFile << "  " << ary[i][j];        
            }
            outFile << "\n";
        }

    }

    void prettyPrintImg(int** ary, ofstream& outFile, string s){
        drawTitle(outFile, s);
        for (int i = 0; i < numRows+2; i++) {
            for (int j = 0; j < numCols+2; j++) {
                if(ary[i][j] == 0)outFile << "  .";
                else if(ary[i][j]<0)outFile << "  -";  
                else outFile << "  " << ary[i][j];        
            }
            outFile << "\n";
        }
    }

    void drawTitle(ofstream& outFile, string str){
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

    ~ChainCode(){
        cout << "destroy";
        for (int i = 0; i < numRows+2; i++){
            delete[] imgAry[i];
            delete[] reconstructAry[i];
        }
        delete[] imgAry;
        delete[] reconstructAry;
    }

};

int main(int argc, char const *argv[]){

    cout << "Project 8"<<endl;

    ifstream inFile(argv[1]);
    ofstream outFile(argv[2]);
    ofstream ccFile(argv[3]);
    ofstream debugFile(argv[4]);
    /* code */
    ChainCode cc = ChainCode(inFile);


    cc.prettyPrintImg(cc.imgAry, outFile, "Image");

    cc.getChainCode(ccFile);
    ccFile.close();

    ifstream in_ccFile(argv[3]);
    cc.decompression(in_ccFile, debugFile);

    inFile.close();
    in_ccFile.close();
    debugFile.close();
    return 0;
}

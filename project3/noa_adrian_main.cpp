/* 

Created by Adrian Noa

Objective: To take an image asset and smoothen each of its pixels using binary and nonbinary threshold operations
Usage:

g++ noa_adrian_main.cpp -o main.exe && ./main.exe data.txt out1.txt out2.txt

OR

g++ noa_adrian_main.cpp -o main.exe && ./main.exe BiGuass_data2.txt outFile1.txt outFile2.txt && rm ./main.exe

*/


#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <string>
using namespace std;


class BiMean{

public:

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

    // All arrays need to be dynamically allocated at run-time
    int* histAry;       // 1D[maxVal+1] to store Histogram Array
                        
    int* GaussAry;      // 1D[maxVal+1] to store "modified" Gaussian function

    char** histGraph;   // 2D[maxVal+1 x maxHeight+1] initialize to "blank"
/*        for displaying the histogram curve.       */

    char** GaussGraph;  // 2D[maxVal+1 x maxHeight+1] initialize to "blank" 
/*        for displaying Gaussian curves in 2D.       */
//    int* arr = new int[m * n];

    BiMean(ifstream &inFile){
    // BiMean(int numRows, int numCols,int minVal,int maxVal){

    inFile >> numRows >> numCols >> minVal >> maxVal;
        histAry = new int[maxVal+1]();
        cout << "Allocated 1D Histogram Array" << endl;
        
        // for(int i=0; i<3; i++)for(int j=0; j<3; j++)histGraph[i][j] = 'g';   

        // int **test = new int*[3]{3*new int[]()};
        // for(int i=0; i<3; i++)for(int j=0; j<3; j++)test[i][j] = 1;   
        // for(int i=0; i<3; i++)for(int j=0; j<3; j++) cout << test[i][j] << " ";
    }

    int loadHist(ifstream &inFile){ // add histogram to histAry from inFile and returns the max among hist[i]
        int numberOfPixels;
        int max = -999;
        for (int i = 0; i<maxVal+1; i++){
            inFile >> numberOfPixels >> numberOfPixels;
            histAry[i] = numberOfPixels;
            max = (numberOfPixels > max) ? max = numberOfPixels : max; 
            // if(pixel > max) max=pixel;
            // cout << numberOfPixels << endl;
        }
        // cout<< max;
        return max;
    }

    void allocate(){

        // this->GaussAry = new int[maxVal+1]{0};

        // this->histGraph = new char*[maxVal+1];

        // this->GaussGraph = new char*[maxVal+1];

        // for(int i=0; i<maxVal+1;i++) {
        //     histGraph[i] = new char[maxHeight+1]();
        //     GaussGraph[i] = new char[maxHeight+1]();
        //     for (int j = 0; j < maxHeight+1; j++) {
        //         histGraph[i][j]=' ';
        //         GaussGraph[i][j]=' ';
        //     }            
        // }

        this->GaussAry = new int[maxVal+1]{0};
        cout << "Allocated 1D Gauss array" << endl;

        this->histGraph = new char*[maxHeight+1];
        cout << "Allocated Histogram 2D char array" << endl;

        this->GaussGraph = new char*[maxHeight+1];
        cout << "Allocated Gaussian graph 2D char array" << endl;

        for(int i=0; i<maxHeight+1;i++) {
            histGraph[i] = new char[maxVal+1]();
            GaussGraph[i] = new char[maxVal+1]();
            for (int j = 0; j < maxVal+1; j++) {
                histGraph[i][j]=' ';
                GaussGraph[i][j]=' ';
            }            
        }
        cout << "Allocated histogram and gaussian subarrays" << endl;

        

        // for (int i = 0; i < maxHeight+1; i++) {
        //     cout << histGraph[i]<<endl;
        // }
        

        // for(int i=0; i<maxVal+1; i++){
        //     for(int j=0; j<maxHeight+1; j++) 
        //         cout << (int)histGraph[i][j];
        //     cout << endl;
        // }
        // cout << endl;
        // // for(int i=0; i<maxVal+1; i++)for(int j=0; j<maxHeight+1; j++) cout << (int)GaussGraph[i][j] << "c";
        // cout << endl;
    } 
    

    

    void plotGraph(int* ary, char** graph, char symbol){
        int j;
        for (int i = 0; i < maxVal+1; i++){
            graph[maxHeight][i] = '-';
            if(ary[i] >= 0){
                // for (j = maxHeight+1-ary[i]; j < maxHeight+1; j++) {
                // graph[maxHeight-ary[i]][i] = symbol; //  display as curve
            for (j = maxHeight-ary[i]; j < maxHeight; j++) {     // display
                graph[j][i] = symbol;                            // as
            }                                                    // line graph
            }
        }

        // for (int i = 0; i < maxHeight+1; i++){
        //     if(ary[i] > 0){
        //         graph[ary[i][ary[i]] = symbol;
        //     }
        //     for (int j = 0; j < ary[i] ; j++)
        //     {
        //         graph[i][j] = 'c';
        //     }   
        // }
        // addVertical(graph, 6);

        // for (int i = 0; i < maxHeight+1; i++) {
        //     cout << histGraph[i]<<endl;
        // }
    }

    void addVertical(char** graph, int thr){
        for (int j = 0; j < maxHeight; j++) {
            graph[j][thr] = '|';
        }  
    }

    double computeMean(int leftIndex, int rightIndex, int maxHeight){
        int sum = 0;
        int numPixels = 0;
        maxHeight = 0;

        for (int i = leftIndex; i<rightIndex; i++){
            sum += histAry[i]*i;
            numPixels += histAry[i];
            if(histAry[i]>maxHeight){
                maxHeight = histAry[i];
            }
        }
        // cout << double(sum)/(double)numPixels << endl;
        return double(sum)/(double)numPixels;
    }

    double computeVar(int leftIndex, int rightIndex, double mean){
        double sum = 0.0;
        int numPixels = 0;
        for (int i=leftIndex ; i < rightIndex ; i++){
            sum += (double)histAry[i] * pow((double)i-mean,2);
            numPixels += histAry[i];
        }
        // cout << (double)sum/(double)numPixels << endl;
        return (double)sum/(double)numPixels;
    }

    double modifiedGauss(int x, double mean, double var, int maxHeight){
        //G(X) = maxHeight * exp( - ( (x-mean)^2 / (2* c2) )
        double G = (double)maxHeight * exp(-1 * pow(x-mean, 2) / (2*var));
        return G;
    }

    void setZero(int* ary){
        for (int i = 0; i < maxVal+1; i++) {
            ary[i]=0;
        }
    }

    int biMeanGauss(int dividePt, ofstream &outFile){
        outFile << "DividePt" << "\tLeftSum" << "\t\tRightSum" << "\tTotalSum" << "\tDifference" << "\tBestThreshold" << endl;
        int bestThr = dividePt;
        double sum1;
        double sum2;
        double total;
        double minSumDiff = 99999.9;
        while( dividePt < (maxVal - offSet)){
            setZero(GaussAry);
            sum1 = fitGauss(0, dividePt, GaussAry);
            sum2 = fitGauss(dividePt, maxVal, GaussAry);
            total = sum1 + sum2;
            if(total<minSumDiff) {
                minSumDiff = total;
                bestThr = dividePt;
            }
            toFile(outFile, dividePt, sum1, sum2, total, minSumDiff, bestThr);
            dividePt++;
        }
        
        return bestThr;
    }

    void toFile(ofstream &outFile, int dividePt, double sum1, double sum2, double total, double minSumDiff, int bestThr){
        outFile << "\t" << dividePt 
                << "\t\t" << sum1 
                << "\t\t" << sum2 
                << "\t\t" << total 
                << "\t\t" << minSumDiff 
                << "\t\t\t" << bestThr << endl;
    }

    double fitGauss(int leftIndex, int rightIndex, int* GaussAry){
        double mean, var, Gval, maxGval, sum = 0.0;
        mean = computeMean(leftIndex, rightIndex, maxHeight);
        var = computeVar(leftIndex, rightIndex, mean);
        for (int i = leftIndex; i <= rightIndex; i++){
            Gval = modifiedGauss(i, mean, var, maxHeight);
            sum += abs(Gval - (double)histAry[i]);
            GaussAry[i] = (int)Gval;
        }
        return sum;
    }

    void bestFitGauss(int bestThrVal){
        double sum1, sum2;
        setZero(GaussAry);
        sum1 = fitGauss(0, bestThrVal, GaussAry);
        sum2 = fitGauss(bestThrVal, maxVal, GaussAry);
    }


    void drawGraph(ofstream &outFile, char** graphAry, string str, int* ary){
        
        drawTitle(outFile, str);
        for (int i = 0; i < maxHeight+1; i++){
            for (int j = 0; j < maxVal+1; j++){
                outFile << graphAry[i][j] << " ";
            }
            outFile << endl;
        }
        outFile << endl;
    }

    void drawTitle(ofstream &outFile, string str){
        int l = ((maxVal*2+1)/2)-1;
        int sl = l - str.length()/2;
        for(int r=0; r<3; r++){
            for (int i = 0; i < (maxVal+1)*2; i++){
                if(r==0) outFile << "*";
                else if(r==1){
                    if(i==sl-1) outFile << "  " << str << "  ";
                    else if(i<=sl-1) outFile << "*";
                    else if (i>sl+str.length()+2) outFile << "*";
                }
                else outFile << "*";
            }
            outFile << endl;
        }
    }


    void plotOverlay(ofstream &outFile, int bestThr){

        char** overlay = allocateOverlay();

        for (int i = 0; i < maxVal+1; i++) {            // draw histogram
                overlay[maxHeight][i] = '-';
            for (int j = maxHeight-histAry[i]; j < maxHeight; j++) {
                if(j < maxHeight-GaussAry[i]) overlay[j][i] = '+'; 
                else overlay[j][i] = '/';
            } 
            for (int j = maxHeight-GaussAry[i]; j < maxHeight; j++){        // draw gaussian curve
                if(j < maxHeight-histAry[i] && (int)overlay[j][i] > 0) overlay[j][i] = '+';
                else overlay[j][i] = 'o';
            }         
        }
        addVertical(overlay, bestThr);
        drawToFile(outFile, overlay);

        deleteOverlay(overlay);
    }

    char** allocateOverlay(){
        char **overlay = new char*[maxHeight+1];
        for (int i = 0; i < maxHeight+1; i++){
            overlay[i] = new char[maxVal+1];
            for(int j=0; j<maxVal+1; j++) {
                overlay[i][j] = ' ';
            }
        }
        cout << "allocated 2d for overlay" << endl;
        return overlay;
    }

    void drawToFile(ofstream &outFile, char** graph){
        for (int i = 0; i < maxHeight+1; i++){
            for (int j = 0; j < maxVal+1; j++){
                outFile << graph[i][j] << " ";
            }
            outFile << endl;
            // outFile << graph[i] << endl;
        }
    }
    void deleteOverlay(char** graph){
        for (int i = 0; i < maxHeight+1; i++){
            delete[] graph[i];
        }
        delete[] graph;
    }

    void plotAll(ofstream &outFile, int bestThr){
        drawTitle(outFile, "hello");
        plotOverlay(outFile, bestThr);
    }

    ~BiMean(){
        delete[] histAry;
        delete[] GaussAry;
        cout << "deleted histogram and gaussian dynamic arrays" << endl;
        for(int i=0; i<maxHeight+1; i++){
            delete[] histGraph[i];
            delete[] GaussGraph[i];
        }
        cout << "delete all subarrays in histgraph and gausGraph" << endl;
        delete[] histGraph;
        cout << "deleting histGraph memory allocation" << endl;
        delete[] GaussGraph;
        cout << "deleting gausGraph memory allocation";
    }

};

int main(int argc, const char* argv[]) {
    cout << endl;

    if (argc != 4){
        printf("Not enough arguments\n");
        return 1;
    }

    ifstream inFile(argv[1]);
    ofstream outFile1(argv[2]); 
    ofstream outFile2(argv[3]); 

    if (!inFile.is_open()) {
        cout << "Unable to open file" << endl;
        exit(2);
    }

    BiMean biMean = BiMean(inFile);

    // int numRows, numCols, minVal, maxVal;
    biMean.maxHeight = biMean.loadHist(inFile);

    biMean.allocate();

    // imageProcessing.shout();
    biMean.plotGraph(biMean.histAry, biMean.histGraph, '*');
    biMean.drawGraph(outFile1, biMean.histGraph, "Histogram Graph", biMean.histAry);

    biMean.offSet = (biMean.maxVal - biMean.minVal) / 10;

    biMean.dividePt = biMean.offSet;
    int bestThrVal = biMean.biMeanGauss(biMean.dividePt, outFile2);

    biMean.bestFitGauss(bestThrVal);
    biMean.plotGraph(biMean.GaussAry, biMean.GaussGraph, '+');
    biMean.drawGraph(outFile1, biMean.GaussGraph, "Gaussian Curve Graph", biMean.GaussAry);

    outFile1 << "Best Threshold value: " << bestThrVal << endl;

    biMean.addVertical(biMean.histGraph, bestThrVal);

    biMean.drawGraph(outFile1, biMean.histGraph, "Best Threshold Histogram", biMean.histAry);

    biMean.plotAll(outFile1, bestThrVal);


    inFile.close();
    outFile1.close();
    outFile2.close();

    return 0;
}
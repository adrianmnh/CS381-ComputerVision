/*
Computer Vision
Project 2
Created by Adrian Noa

usage:

java adrian_noa_main.java input mask 38 inputImg avgOut AvgThreshold MedianOut MedianTreshold GaussOut GaussThreshold

Change the value of third parameter(38) to change the Treshold Value
*/

import java.io.*;
import java.util.Scanner;
import java.util.Arrays;

public class CVProject2 {

    public static void main(String[] args) throws IOException{
        if (args.length != 10){
            System.out.println("Not enough of arguments");
            return;
        }
        int threshold = Integer.parseInt(args[2]);
        try(
            Scanner inFile = readFile(args[0]);
            Scanner maskFile = readFile(args[1]);
            BufferedWriter inputImg = createFile(args[3]);
            BufferedWriter AvgOut = createFile(args[4]);
            BufferedWriter AvgThreshold = createFile(args[5]);
            BufferedWriter MedianOut = createFile(args[6]);
            BufferedWriter MedianThreshold = createFile(args[7]);
            BufferedWriter GaussOut = createFile(args[8]);
            BufferedWriter GaussThreshold = createFile(args[9]);
        ){
            ImageProcessing imgProcessing = new ImageProcessing(inFile, maskFile, threshold);
            load(imgProcessing);

            imgProcessing.imgReformat(imgProcessing.mirrorFramedAry, imgProcessing.minVal, imgProcessing.maxVal, inputImg);

            imgProcessing.computeAvg();
            imgProcessing.imgReformat(imgProcessing.avgAry, imgProcessing.newMin, imgProcessing.newMax, AvgOut);
            imgProcessing.threshold(imgProcessing.avgAry, imgProcessing.thrAry);
            imgProcessing.imgReformat(imgProcessing.thrAry, imgProcessing.newMin, imgProcessing.newMax, AvgThreshold);

            imgProcessing.computeMedian();
            imgProcessing.imgReformat(imgProcessing.medianAry, imgProcessing.newMin, imgProcessing.newMax, MedianOut);
            imgProcessing.threshold(imgProcessing.medianAry, imgProcessing.thrAry);
            imgProcessing.imgReformat(imgProcessing.thrAry, imgProcessing.newMin, imgProcessing.newMax, MedianThreshold);

            imgProcessing.computeGauss();
            imgProcessing.imgReformat(imgProcessing.gaussAry, imgProcessing.newMin, imgProcessing.newMax, GaussOut);
            imgProcessing.threshold(imgProcessing.gaussAry, imgProcessing.thrAry);
            imgProcessing.imgReformat(imgProcessing.thrAry, imgProcessing.newMin, imgProcessing.newMax, GaussThreshold);
        }
    }

    public static void load(ImageProcessing imgProcessing){
        imgProcessing.allocate();
        imgProcessing.loadMask();
        imgProcessing.loadMask1DAry();
        imgProcessing.loadImage();
        imgProcessing.mirrorFraming();
    }

    public static Scanner readFile(String s) throws IOException{
        return new Scanner(new BufferedReader(new FileReader(s)));
    }

    public static BufferedWriter createFile(String s) throws IOException{
        return new BufferedWriter(new FileWriter(s));
    }
}

public class ImageProcessing {
        Scanner inFile, maskFile;
        int numRows, numCols, minVal, maxVal; // image header
        int maskRows, maskCols, maskMin, maskMax; //mask header
        int newMin, newMax, maskWeight;
        int thrVal;  // threshold value
        boolean includeZero;
        int[][] mirrorFramedAry, avgAry, medianAry, gaussAry, thrAry, mask2DAry;
        int[] neighbor1DAry, mask1DAry;

        ImageProcessing(Scanner inFile, Scanner maskFile, int threshold){
            this.inFile = inFile;
            this.maskFile = maskFile;
            this.numRows = inFile.nextInt();
            this.numCols = inFile.nextInt();
            this.minVal = inFile.nextInt();
            this.maxVal = inFile.nextInt();
            this.maskRows = maskFile.nextInt();
            this.maskCols = maskFile.nextInt();
            this.maskMin = maskFile.nextInt();
            this.maskMax = maskFile.nextInt();
            this.thrVal = threshold;
            this.includeZero = true; // change to false to Visualize treshold output files (no zeros only 1s)
        }

        public void threshold(int[][] in, int[][]out){
            newMin = 0;
            newMax = 1;
            for(int i=1; i<numRows+2; i++){
                for(int j=1; j<numCols+2; j++){
                    if (in[i][j] >= thrVal)
                        out[i][j] = 1;
                    else
                        out[i][j] = 0;
                }
            }
        }

        public void imgReformat(int[][] inAry, int newMin, int newMax, BufferedWriter outImg) throws IOException{

            outImg.write(Integer.toString(numRows)+" ");
            outImg.write(Integer.toString(numCols)+" ");
            outImg.write(Integer.toString(newMin)+" ");
            outImg.write(Integer.toString(newMax)+"\n");

            int maxWidth = Integer.toString(newMax).length();

            String s;
            for (int i = 1; i<numRows+1; i++){
                for (int j = 1; j<numCols+1; j++){
                    s = Integer.toString(inAry[i][j]);
                    if(includeZero==true)
                        outImg.write(s);
                    else{
                        if(inAry[i][j]!=0)
                            outImg.write(s);
                        else outImg.write(" ");

                    }
                    int pixelWidth = s.length();
                    while(pixelWidth <= maxWidth){
                        outImg.write(" ");
                        pixelWidth++;
                    }
                }
                outImg.write("\n");
            }
        }

        public void mirrorFraming(){
            mirrorFramedAry[0][0] = mirrorFramedAry[2][2];
            mirrorFramedAry[0][numCols+1] = mirrorFramedAry[2][numCols-1];
            mirrorFramedAry[numRows+1][0] = mirrorFramedAry[numRows-1][2];
            mirrorFramedAry[numRows+1][numCols+1] = mirrorFramedAry[numRows-1][numCols-1];
            for(int i=1; i<numCols+1; i++){
                mirrorFramedAry[0][i] = mirrorFramedAry[2][i];
                mirrorFramedAry[numRows+1][i] = mirrorFramedAry[numRows-1][i];
            }
            for(int i=1; i<numRows+1; i++){
                mirrorFramedAry[i][0] = mirrorFramedAry[i][2];
                mirrorFramedAry[i][numCols+1] = mirrorFramedAry[i][numCols-1];
            }
        }

        public void loadImage(){
            for(int i=1; i<numRows+1; i++){
                for(int j=1; j<numCols+1; j++){
                    mirrorFramedAry[i][j] = inFile.nextInt();
                }
            }
        }

        public void loadMask(){
            for(int i=0; i<maskRows; i++){
                for(int j=0; j<maskCols; j++){
                    mask2DAry[i][j] = maskFile.nextInt();
                }
            }
        }

        public void loadMask1DAry(){
            this.maskWeight = 0;
            for(int i=0; i<maskRows; i++){
                for(int j=0; j<maskCols; j++){
                    mask1DAry[maskCols*i+j] = mask2DAry[i][j];
                    this.maskWeight += mask2DAry[i][j];
                }
            }
        }

        public void loadNeighbor1DAry(int r, int c){
            for(int i=0; i<3; i++){
                for(int j=0; j<3; j++){
                    neighbor1DAry[ (i*3) + j ] = mirrorFramedAry[r-1+i][c-1+j];
                }
            }
        }

        public void sort(int[] a){
            Arrays.sort(a);
        }

        public void computeAvg(){
            newMin = 9999;
            newMax = 0;
            for(int i=1; i<numRows+1; i++){
                for(int j=1; j<numCols+1; j++){
                    int avg = 0;
                    loadNeighbor1DAry(i,j);
                    for(int pixel:neighbor1DAry)avg+=pixel;
                    avgAry[i][j] = avg/9;
                    if (newMin > avgAry[i][j])
                        newMin = avgAry[i][j];
                    if (newMax < avgAry[i][j])
                        newMax = avgAry[i][j];
                }
            }
        }

        public void computeMedian(){
            newMin = 9999;
            newMax = 0;

            for(int i=1; i<numRows+1; i++){
                for(int j=1; j<numCols+1; j++){
                    loadNeighbor1DAry(i,j);
                    sort(neighbor1DAry);
                    medianAry[i][j] = neighbor1DAry[4];
                    if (newMin > medianAry[i][j])
                        newMin = medianAry[i][j];
                    if (newMax < medianAry[i][j])
                        newMax = medianAry[i][j];
                }
            }
        }

        public void computeGauss(){
            newMin = 9999;
            newMax = 0;
            for(int i=1; i<numRows+1; i++){
                for(int j=1; j<numCols+1; j++){
                    loadNeighbor1DAry(i,j);
                    gaussAry[i][j] = convolution(neighbor1DAry, mask1DAry);
                    if (newMin > gaussAry[i][j])
                        newMin = gaussAry[i][j];
                    if (newMax < gaussAry[i][j])
                        newMax = gaussAry[i][j];
                }
            }
        }

        public int convolution(int[] a, int[] mask){
            int result = 0;
            for(int i=0; i<9; i++){
                result += neighbor1DAry[i] * mask1DAry[i];
            }
            return result/maskWeight;
        }

        public void allocate(){
            mirrorFramedAry = new int[this.numRows+2][this.numCols+2];
            avgAry = new int[this.numRows+2][this.numCols+2];
            medianAry = new int[this.numRows+2][this.numCols+2];
            gaussAry = new int[this.numRows+2][this.numCols+2];
            thrAry = new int[this.numRows+2][this.numCols+2];
            mask2DAry = new int[this.maskRows][this.maskCols];
            mask1DAry = new int[this.maskRows*this.maskCols];
            neighbor1DAry = new int[9];
        }

    }

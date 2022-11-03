/*
Computer Vision
Project 5
Created by Adrian Noa

usage:

java adrian_noa_main.java 8 data1 pretty label property

*/

import java.util.*;
import java.io.*;

// import 

public class CVProject5 {
    public static void main(String[] args) throws IOException {
        if (args.length != 5) {
            System.out.println("Not enough of arguments");
            return;
        }

        try (
            // Scanner inFile = new Scanner(new File(args[1]));
            Scanner inFile = readFile(args[1]);
            BufferedWriter prettyPrintFile = createFile(args[2]);
            BufferedWriter labelFile = createFile(args[3]);
            BufferedWriter propertyFile = createFile(args[4]);
        ) {

            ConnCompLabel connCompLabel = new ConnCompLabel(inFile);

            // step 1
            connCompLabel.zero2D(connCompLabel.zeroFramedAry);

            // step 2
            connCompLabel.loadImage(inFile);

            // step 3
            int whichConnectness = Integer.parseInt(args[0]);

            // step 4 & 5
            
            connCompLabel.connectPass1(whichConnectness); // pass 1
            prettyPrintFile.write("\n");
            connCompLabel.makeBorder(prettyPrintFile);
            prettyPrintFile.write("Result of Pass 1: \n");
            connCompLabel.makeBorder(prettyPrintFile);
            connCompLabel.imgReformat(prettyPrintFile);
            connCompLabel.makeBorder(prettyPrintFile);
            prettyPrintFile.write("Equivalence Array after Pass 1: \n");
            connCompLabel.makeBorder(prettyPrintFile);
            connCompLabel.printEQAry(prettyPrintFile);
            connCompLabel.makeBorder(prettyPrintFile);
            prettyPrintFile.write("\n");
            
            connCompLabel.connectPass2(whichConnectness); // pass 2
            prettyPrintFile.write("\n");
            connCompLabel.makeBorder(prettyPrintFile);
            prettyPrintFile.write("Result of Pass 2: \n");
            connCompLabel.makeBorder(prettyPrintFile);
            connCompLabel.imgReformat(prettyPrintFile);
            connCompLabel.makeBorder(prettyPrintFile);
            prettyPrintFile.write("Equivalence Array after Pass 2: \n");
            connCompLabel.makeBorder(prettyPrintFile);
            connCompLabel.printEQAry(prettyPrintFile);
            connCompLabel.makeBorder(prettyPrintFile);
            prettyPrintFile.write("\n");
            

            // step 6
            connCompLabel.trueNumCC = connCompLabel.manageEQAry();
            connCompLabel.makeBorder(prettyPrintFile);
            prettyPrintFile.write("Equivalence Array after management: \n");
            connCompLabel.makeBorder(prettyPrintFile);
            connCompLabel.printEQAry(prettyPrintFile);
            connCompLabel.makeBorder(prettyPrintFile);
            connCompLabel.newMin = 0;
            connCompLabel.newMax = connCompLabel.trueNumCC;
            connCompLabel.allocateCCproperty(connCompLabel.trueNumCC);

            // step 7
            connCompLabel.connectPass3(connCompLabel.zeroFramedAry,
            connCompLabel.CCproperty);

            // step 8
            prettyPrintFile.write("\n");
            connCompLabel.makeBorder(prettyPrintFile);
            prettyPrintFile.write("Algorithm Pass 3 \n"); // pass 3
            connCompLabel.makeBorder(prettyPrintFile);
            connCompLabel.imgReformat(prettyPrintFile);
            
            // step 9
            connCompLabel.makeBorder(prettyPrintFile);
            prettyPrintFile.write("Equivalence Array after Pass 3 \n");
            connCompLabel.makeBorder(prettyPrintFile);            
            connCompLabel.printEQAry(prettyPrintFile);
            connCompLabel.makeBorder(prettyPrintFile);
            
            // step 10
            labelFile.write(Integer.toString(connCompLabel.numRows)+" ");
            labelFile.write(Integer.toString(connCompLabel.numCols)+" ");
            labelFile.write(Integer.toString(connCompLabel.newMin)+" ");
            labelFile.write(Integer.toString(connCompLabel.newMax)+"\n");

            // step 11
            connCompLabel.printImg(labelFile);
            
            // step 12
            connCompLabel.printCCproperty(propertyFile);
            
            // step 13
            connCompLabel.drawBoxes(connCompLabel.CCproperty);
            
            // step 14
            prettyPrintFile.write("\n");
            connCompLabel.makeBorder(prettyPrintFile);
            prettyPrintFile.write("Bounding Boxes result:\n");
            connCompLabel.makeBorder(prettyPrintFile);
            connCompLabel.imgReformat(prettyPrintFile);
            connCompLabel.makeBorder(prettyPrintFile);
            
            // step 15
            prettyPrintFile.write("Number of Connected Components: " +
            Integer.toString(connCompLabel.trueNumCC)+"\n");
            connCompLabel.makeBorder(prettyPrintFile);
            
            System.out.println();

        }
    }
    public static Scanner readFile(String str) throws IOException{
        return new Scanner(new BufferedReader(new FileReader(str)));
    }

    public static BufferedWriter createFile(String str) throws IOException{
        return new BufferedWriter(new FileWriter(str));
    }
}

class Property {
    int label = 0;
    int numPixels = 0;
    int minR = 0;
    int minC = 0;
    int maxR = 0;
    int maxC = 0;
}

class ConnCompLabel extends Property {
    int numRows, numCols, minVal, maxVal, newLabel, newMin, newMax, trueNumCC;
    int zeroFramedAry[][];
    int nonZeroNeighborAry[];
    int EquivalenceArray[];
    Property[] CCproperty;

    ConnCompLabel(Scanner inFile) {
        this.numRows = inFile.nextInt();
        this.numCols = inFile.nextInt();
        this.minVal = inFile.nextInt();
        this.maxVal = inFile.nextInt();
        this.newLabel = 0;
        this.zeroFramedAry = new int[numRows + 2][numCols + 2];
        this.nonZeroNeighborAry = new int[5];
        this.EquivalenceArray = new int[(numRows * numCols) / 4];
    }

    public int[][] zero2D(int[][] arr) {
        /*
         * Initialize a 2-D array to zero
         */
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                arr[i][j] = 0;
            }
        }
        return arr;
    }

    public void loadImage(Scanner b) throws IOException {
        /*
         * Read from input file and write to zeroFramedAry
         * begin at (1,1)
         */
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                this.zeroFramedAry[i][j] = b.nextInt();
            }
        }
    }

    public void imgReformat(BufferedWriter file) throws IOException {
        for (int i = 1; i <= this.numRows; i++) {
            for (int j = 1; j <= this.numCols; j++) {
                if (this.zeroFramedAry[i][j] == 0)
                    file.write(".   ");
                else if(zeroFramedAry[i][j]>=10)
                    file.write(Integer.toString(this.zeroFramedAry[i][j])+"  ");
                else file.write(Integer.toString(this.zeroFramedAry[i][j])+"   ");
            }
            file.write("\n");
        }
    }

    
    public ArrayList<Integer> tempArr(int set, int connectness, int i, int j) {
        
        // a b c
        //
        // d P(i,j) e
        //
        // f g h
        ArrayList<Integer> temp = new ArrayList<Integer>();
        temp.add(this.zeroFramedAry[i + (-1)*set][j]); // b or g
        temp.add(this.zeroFramedAry[i][j + (-1)*set]); // d or e

        ArrayList<Integer> zero = new ArrayList<Integer>();
        zero.add(0);
        if (connectness == 8) {
            temp.add(this.zeroFramedAry[i + (-1)*set][j - 1]); // a or f
            temp.add(this.zeroFramedAry[i + (-1)*set][j + 1]); // c or h
        }
        temp.removeAll(zero);
        return temp;
    }

    public void changeLabels(int connectness, int val, int i, int j){
        if(this.zeroFramedAry[i + 1][j] != 0 )
            this.zeroFramedAry[i + 1][j] = val; // g

        if(this.zeroFramedAry[i][j + 1] != 0 )
            this.zeroFramedAry[i][j + 1] = val; // e

        if (connectness == 8) {
            if(this.zeroFramedAry[i + 1][j - 1] != 0 )
                this.zeroFramedAry[i + 1][j - 1] = val; // f
            if(this.zeroFramedAry[i + 1][j + 1] != 0 )
                this.zeroFramedAry[i + 1][j + 1] = val; // h
        }
    }
    
    public boolean case1(ArrayList<Integer> arr) {
        
        // if(Arrays.stream(arr).sum() == 0)
        if (arr.size() == 0) {
            System.out.print(" - case1 - all are zero\n");
            return true;
        }
        return false;
    }

    public boolean case2(ArrayList<Integer> arr) {

        // if( Arrays.stream(arr).min() == Arrays.stream(arr).max() ){
            if (arr.stream().mapToInt(Integer::intValue).min().getAsInt() == arr.stream().mapToInt(Integer::intValue).max()
                .getAsInt()) {
                    System.out.print(" - case2\n");
                    return true;
        }
        return false;
    }
    
    public boolean case3(ArrayList<Integer> arr) {

        // if( Arrays.stream(arr).min() == Arrays.stream(arr).max() ){
            if (arr.stream().mapToInt(Integer::intValue).min().getAsInt() != arr.stream().mapToInt(Integer::intValue).max()
                .getAsInt()) {
                    System.out.print(" - case3\n");
                    return true;
        }
        return false;
    }
    
    public void printMap() {
        for (int i = 1; i <= this.numRows; i++) {
            for (int j = 1; j <= this.numCols; j++) {
                if (this.zeroFramedAry[i][j] == 0)
                    System.out.print(".  ");
                    else if(this.zeroFramedAry[i][j]>=10)
                    System.out.print(this.zeroFramedAry[i][j]+" ");
                    else System.out.print(this.zeroFramedAry[i][j]+" ");
                }
                System.out.print("\n");
            }
            
    }

        
    public void connectPass1(int connectness) {
        for (int i = 1; i <= this.numRows; i++)
            for (int j = 1; j <= this.numCols; j++) {
                int pixel = this.zeroFramedAry[i][j];
                if (pixel > 0) {
                    ArrayList<Integer> temp = tempArr(1, connectness, i, j);
                        if (case1(temp)) {
                            this.zeroFramedAry[i][j] = ++this.newLabel;
                            updateEquivalencyTable(this.newLabel, this.newLabel);
                        } else {                      
                            int min = temp.stream().mapToInt(Integer::intValue).min().getAsInt();
                            
                            this.zeroFramedAry[i][j] = min;

                            if(case3(temp)){
                                for(int t:temp)
                                    updateEquivalencyTable(t,min);
                            }
                    }
                }
            }
    }

    public void connectPass2(int connectness) {

        boolean flag = true;
        for (int i = this.numRows-1; i >= 1; i--) {
            for (int j = this.numCols-1; j >= 1; j--) {
                int pixel = zeroFramedAry[i][j];
                if (pixel > 0) {
                    ArrayList<Integer> temp = tempArr(-1, connectness, i, j);
                    for(int t:temp) if(t!=pixel) flag=false;
                    // case 3
                    if(!flag) {
                        int min = temp.stream().mapToInt(Integer::intValue).min().getAsInt();
                        changeLabels(connectness, min, i, j);
                        zeroFramedAry[i][j] = min;
                        flag = true;
                        updateEquivalencyTable(pixel, min);
                    }
                }
            }
        }
    }

    public void connectPass3(int[][] inArr, Property[] CCproperty) {
        int k = 0;
        int nummPixs;
        int pixel;
        for (int i = 1; i <= this.trueNumCC; i++) {
            this.CCproperty[i].label = i;
            this.CCproperty[i].numPixels = 0;
            this.CCproperty[i].minR = this.numRows;
            this.CCproperty[i].maxR = 0;
            this.CCproperty[i].minC = this.numCols;
            this.CCproperty[i].maxC = 0;
        }
        for (int r = 1; r <= this.numRows; r++) {
            for (int c = 1; c <= this.numCols; c++) {
                pixel = inArr[r][c];
                if (pixel > 0) {
                    inArr[r][c] = this.EquivalenceArray[pixel];
                    k = inArr[r][c];
                    nummPixs = this.CCproperty[k].numPixels;
                    CCproperty[k].numPixels = ++nummPixs;
                    if (r < this.CCproperty[k].minR) {
                        this.CCproperty[k].minR = r - 1;
                    }
                    if (r > this.CCproperty[k].maxR) {
                        this.CCproperty[k].maxR = r - 1;
                    }
                    if (c < this.CCproperty[k].minC) {
                        this.CCproperty[k].minC = c - 1;
                    }
                    if (c > this.CCproperty[k].maxC) {
                        this.CCproperty[k].maxC = c - 1;
                    }
                }
            }
        }
    }

    public void allocateCCproperty(int truenumcc) {
        this.CCproperty = new Property[truenumcc + 1];
        for (int i = 0; i <= truenumcc; i++) {
            this.CCproperty[i] = new Property();
        }
    }

    public void drawBoxes(Property[] CCproperty) {
        int minRow, minCol, maxRow, maxCol, label;
        for (int index = 1; index <= this.trueNumCC; index++) {
            minRow = this.CCproperty[index].minR + 1;
            minCol = this.CCproperty[index].minC + 1;
            maxRow = this.CCproperty[index].maxR + 1;
            maxCol = this.CCproperty[index].maxC + 1;
            label = this.CCproperty[index].label;

            for (int i = minCol; i <= maxCol; i++) {
                this.zeroFramedAry[minRow][i] = label;
                this.zeroFramedAry[maxRow][i] = label;
            }
            for (int i = minRow; i <= maxRow; i++) {
                this.zeroFramedAry[i][minCol] = label;
                this.zeroFramedAry[i][maxCol] = label;
            }
        }
    }

    public void updateEquivalencyTable(int index, int label) {
        this.EquivalenceArray[index] = label;
    }

    public int manageEQAry() {
        int counter = 0;
        for (int i = 1; i < this.EquivalenceArray.length; i++) {
            if (i == this.EquivalenceArray[i]) {
                this.EquivalenceArray[i] = ++counter;
            } else {
                this.EquivalenceArray[i] = this.EquivalenceArray[this.EquivalenceArray[i]];
            }
        }
        return counter;
    }

    public void printCCproperty(BufferedWriter out) throws IOException {
        out.write(Integer.toString(this.numRows) + " ");
        out.write(Integer.toString(this.numCols) + " ");
        out.write(Integer.toString(this.minVal) + " ");
        out.write(Integer.toString(this.trueNumCC) + " \n");
        out.write(Integer.toString(this.trueNumCC) + " \n");
        for (int i = 1; i <= this.trueNumCC; i++) {
            out.write(Integer.toString(this.CCproperty[i].label) + "\n");
            out.write(Integer.toString(this.CCproperty[i].numPixels) + "\n");
            out.write(Integer.toString(this.CCproperty[i].minR) + " ");
            out.write(Integer.toString(this.CCproperty[i].minC) + "\n");
            out.write(Integer.toString(this.CCproperty[i].maxR) + " ");
            out.write(Integer.toString(this.CCproperty[i].maxC) + "\n");
        }
    }

    public void printEQAry(BufferedWriter file) throws IOException {
        for (int i = 1; i <= this.newLabel; i++)
            file.write(Integer.toString(this.EquivalenceArray[i]) + " ");
        file.write("\n");
    }

    public void printImg(BufferedWriter out) throws IOException {
        String str = Integer.toString(this.maxVal);
        int width = str.length();

        for (int r = 1; r <= this.numRows; r++) {
            for (int c = 1; c <= this.numCols; c++) {
                if (this.zeroFramedAry[r][c] > 0) {
                    out.write(Integer.toString(this.zeroFramedAry[r][c]));
                } else {
                    out.write(".");
                }
                String str2 = Integer.toString(this.zeroFramedAry[r][c]);
                int width2 = str2.length();
                out.write(" ");
                while (width2 < width) {
                    out.write(" ");
                    width2++;
                }
            }
            out.write("\n");
        }
    }

    public void makeBorder(BufferedWriter out) throws IOException {
        for (int i = 0; i <= this.numRows + 1; i++) {
            out.write("---");
        }
        out.write("\n");
    }

}

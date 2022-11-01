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
            // connCompLabel.connectPass1(whichConnectness);

            connCompLabel.connect4Pass1(connCompLabel.zeroFramedAry);

            prettyPrintFile.write("Result of Pass 1: \n");
            connCompLabel.imgReformat(prettyPrintFile);
            prettyPrintFile.write("\n");
            connCompLabel.makeBorder(prettyPrintFile);
            connCompLabel.printEQAry(prettyPrintFile);
            // if (whichConnectness == 4) {
            // prettyPrintFile.write("EQ Ary after Pass 1: \n");
            // connCompLabel.makeBorder(prettyPrintFile);
            // // connCompLabel.zeroFramedAry =
            // connCompLabel.connect4Pass2(connCompLabel.zeroFramedAry);
            // prettyPrintFile.write("Result of Pass 2: \n");
            // connCompLabel.imgReformat(prettyPrintFile);
            // connCompLabel.makeBorder(prettyPrintFile);
            // prettyPrintFile.write("EQ Ary after Pass 2: \n");
            // connCompLabel.printEQAry(prettyPrintFile);
            // connCompLabel.makeBorder(prettyPrintFile);
            // }

            // step 5
            if (whichConnectness == 8) {
                // connCompLabel.zeroFramedAry =
                // connCompLabel.connect8Pass1(connCompLabel.zeroFramedAry);
                // RFPRettyPrintFile.write("Algorithm Pass 1: \n");
                // connCompLabel.imgReformat(connCompLabel.zeroFramedAry, RFPRettyPrintFile);
                // connCompLabel.makeBorder(RFPRettyPrintFile);
                // RFPRettyPrintFile.write("EQ Ary after Pass 1: \n");
                // connCompLabel.printEQAry(RFPRettyPrintFile);
                // connCompLabel.makeBorder(RFPRettyPrintFile);
                // connCompLabel.zeroFramedAry =
                // connCompLabel.connect8Pass2(connCompLabel.zeroFramedAry);
                // RFPRettyPrintFile.write("Algorithm Pass 2: \n");
                // connCompLabel.imgReformat(connCompLabel.zeroFramedAry, RFPRettyPrintFile);
                // connCompLabel.makeBorder(RFPRettyPrintFile);
                // RFPRettyPrintFile.write("EQ Ary after Pass 2:\n");
                // connCompLabel.printEQAry(RFPRettyPrintFile);
                // connCompLabel.makeBorder(RFPRettyPrintFile);
            }

            // // step 6
            // connCompLabel.trueNumCC = connCompLabel.manageEQAry();
            // RFPRettyPrintFile.write("EQ Ary after management: \n");
            // connCompLabel.printEQAry(RFPRettyPrintFile);
            // connCompLabel.makeBorder(RFPRettyPrintFile);
            // connCompLabel.newMin = 0;
            // connCompLabel.newMax = connCompLabel.trueNumCC;
            // connCompLabel.allocateCCproperty(connCompLabel.trueNumCC);

            // // step 7
            // connCompLabel.connectPass3(connCompLabel.zeroFramedAry,
            // connCompLabel.CCproperty);

            // // step 8
            // RFPRettyPrintFile.write("Algorithm Pass 3 \n");
            // connCompLabel.imgReformat(connCompLabel.zeroFramedAry, RFPRettyPrintFile);
            // connCompLabel.makeBorder(RFPRettyPrintFile);

            // // step 9
            // RFPRettyPrintFile.write("EQ Ary after Pass 3 \n");
            // connCompLabel.printEQAry(RFPRettyPrintFile);
            // connCompLabel.makeBorder(RFPRettyPrintFile);

            // // step 10
            // labelFile.write(Integer.toString(connCompLabel.numRows)+" ");
            // labelFile.write(" ");
            // labelFile.write(Integer.toString(connCompLabel.numCols)+" ");
            // labelFile.write(" ");
            // labelFile.write(Integer.toString(connCompLabel.newMin)+" ");
            // labelFile.write(" ");
            // labelFile.write(Integer.toString(connCompLabel.newMax)+"\n");

            // // step 11
            // connCompLabel.printImg(labelFile);

            // // step 12
            // connCompLabel.printCCproperty(propertyFile);

            // // step 13
            // connCompLabel.drawBoxes(connCompLabel.zeroFramedAry,
            // connCompLabel.CCproperty);

            // // step 14
            // RFPRettyPrintFile.write("\nBounding Boxes result:\n");
            // connCompLabel.imgReformat(connCompLabel.zeroFramedAry, RFPRettyPrintFile);
            // connCompLabel.makeBorder(RFPRettyPrintFile);

            // // step 15
            // RFPRettyPrintFile.write("Number of Connected Components: " +
            // Integer.toString(connCompLabel.trueNumCC));

            // connCompLabel.connectPass2(connCompLabel.zeroFramedAry, 8);
            System.out.println();
            connCompLabel.printMap();

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

    public int[] minus1D(int[] arr) {
        // initialize a 1-D array to -1
        for (int i = 0; i < arr.length; i++) {
            arr[i] = -1;
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
                    file.write(".");
                else
                    file.write(Integer.toString(this.zeroFramedAry[i][j]));
                file.write("  ");
            }
            file.write("\n");
        }
    }

    
    public int[][] connect8Pass2(int[][] inArr) {
        // update Equivalence theorem
        int pixel, label;
        int e, f, g, h;
        int helper = 0;
        boolean flag = true;
        for (int i = this.numRows - 1; i >= 1; i--) {
            for (int j = this.numCols - 1; j >= 1; j--) {
                pixel = inArr[i][j];
                if (pixel > 0) {
                    helper = 0;
                    this.nonZeroNeighborAry[helper++] = pixel;
                    e = inArr[i][j + 1];
                    f = inArr[i + 1][j - 1];
                    g = inArr[i + 1][j];
                    h = inArr[i + 1][j + 1];
                    if (e != 0) {
                        this.nonZeroNeighborAry[helper++] = e;
                        if (pixel != e) {
                            flag = false;
                        }
                    }
                    if (f != 0) {
                        this.nonZeroNeighborAry[helper++] = f;
                        if (pixel != f) {
                            flag = false;
                        }
                    }
                    if (g != 0) {
                        this.nonZeroNeighborAry[helper++] = g;
                        if (pixel != g) {
                            flag = false;
                        }
                    }
                    if (h != 0) {
                        this.nonZeroNeighborAry[helper++] = h;
                        if (pixel != h) {
                            flag = false;
                        }
                    }
                    // case 3
                    if (!flag) {
                        label = findMin();
                        inArr[i][j] = label;
                        if (inArr[i][j + 1] != 0) {
                            inArr[i][j + 1] = label;
                        }
                        if (inArr[i + 1][j - 1] != 0) {
                            inArr[i + 1][j - 1] = label;
                        }
                        if (inArr[i + 1][j] != 0) {
                            inArr[i + 1][j] = label;
                        }
                        if (inArr[i + 1][j + 1] != 0) {
                            inArr[i + 1][j + 1] = label;
                        }
                        updateEquivalencyTable(pixel, label);
                        flag = true;
                    }
                }
            }
        }
        return inArr;
    }
    
    public ArrayList<Integer> tempArr(int[][] inArr, int connectness, int i, int j) {
        
        // a b c
        //
        // d P(i,j) e
        //
        // f g h
        ArrayList<Integer> temp = new ArrayList<Integer>();
        temp.add(inArr[i - 1][j]); // b
        temp.add(inArr[i][j - 1]); // d
        ArrayList<Integer> zero = new ArrayList<Integer>();
        zero.add(0);
        if (connectness == 8) {
            temp.add(inArr[i - 1][j - 1]); // a
            temp.add(inArr[i - 1][j + 1]); // c
        }
        temp.removeAll(zero);
        return temp;
        
    }
    
    public boolean case1(ArrayList<Integer> arr) {
        
        // if(Arrays.stream(arr).sum() == 0)
        if (arr.size() == 0) {
            System.out.print(" - case1\n");
            return true;
        }
        return false;
    }
    
    public boolean case2or3(ArrayList<Integer> arr) {

        // if( Arrays.stream(arr).min() == Arrays.stream(arr).max() ){
            if (arr.stream().mapToInt(Integer::intValue).min().getAsInt() == arr.stream().mapToInt(Integer::intValue).max()
                .getAsInt()) {
                    System.out.print(" - case2\n");
                    return true;
        }
        return false;
    }
    
    public void printMap() {
        for (int i = 1; i <= this.numRows; i++) {
            for (int j = 1; j <= this.numCols; j++) {
                if (this.zeroFramedAry[i][j] == 0)
                    System.out.print(".");
                    else
                    System.out.print(this.zeroFramedAry[i][j]);
                    System.out.print("  ");
                }
                System.out.print("\n");
            }
            
    }

        public int[][] connect8Pass1(int[][] inArr) {
            int pixel, minLabel;
            int helper;
            int a, b, c, d;
            boolean flag = true;
            for (int i = 1; i <= this.numRows; i++) {
                for (int j = 1; j <= this.numCols; j++) {
                    pixel = inArr[i][j];
                    if (pixel > 0) {
                        helper = 0;
                        a = inArr[i - 1][j - 1];
                        b = inArr[i - 1][j];
                        c = inArr[i - 1][j + 1];
                        d = inArr[i][j - 1];
                        // case 1
                        if (a == 0 && b == 0 && c == 0 && d == 0) {
                            this.newLabel++;
                            updateEquivalencyTable(this.newLabel, this.newLabel);
                            inArr[i][j] = this.newLabel;
                        } else {
                            if (a != 0) {
                                this.nonZeroNeighborAry[helper++] = a;
                            }
                            if (b != 0) {
                                this.nonZeroNeighborAry[helper++] = b;
                            }
                            if (c != 0) {
                                this.nonZeroNeighborAry[helper++] = c;
                            }
                            if (d != 0) {
                                this.nonZeroNeighborAry[helper++] = d;
                            }
    
                            for (int e = 1; e < helper; e++) {
                                if (this.nonZeroNeighborAry[e] != this.nonZeroNeighborAry[e - 1]) {
                                    flag = false;
                                }
                            }
    
                            // case 2
                            if (flag) {
                                inArr[i][j] = this.nonZeroNeighborAry[0];
                                this.nonZeroNeighborAry = minus1D(this.nonZeroNeighborAry);
                            }
                            // case 3
                            else {
                                
                                minLabel = findMin();
                                if (inArr[i - 1][j - 1] != 0) {
                                    updateEquivalencyTable(inArr[i - 1][j - 1], minLabel);
                                }
                                if (inArr[i - 1][j] != 0) {
                                    updateEquivalencyTable(inArr[i - 1][j], minLabel);
                                }
                                if (inArr[i - 1][j + 1] != 0) {
                                    updateEquivalencyTable(inArr[i - 1][j + 1], minLabel);
                                }
                                if (inArr[i][j - 1] != 0) {
                                    updateEquivalencyTable(inArr[i][j - 1], minLabel);
                                }
                                inArr[i][j] = minLabel;
                                updateEquivalencyTable(pixel, minLabel);
                            }
                        }
                    }
                }
            }
            return inArr;
        }
        
        public void connectPass1(int connectness) {
        System.out.println();
        printMap();
        for (int i = 1; i <= this.numRows; i++) {
            for (int j = 1; j <= this.numCols; j++) {
                int pixel = this.zeroFramedAry[i][j];
                if (pixel > 0) {
                    System.out.print("i-"+i);
                    System.out.println("   j-"+j);
    
                    ArrayList<Integer> temp = tempArr(this.zeroFramedAry, connectness, i, j);
    
                    for (int t : temp){
                        System.out.print("\narray: ");
                        System.out.print(t + " ");
                        System.out.println();
                    }
                    
                        if (case1(temp)) {
                            System.out.print("2nd ..i-"+i);
                            System.out.println("   j-"+j);
                            this.zeroFramedAry[i][j] = ++this.newLabel;
                            updateEquivalencyTable(this.newLabel, this.newLabel);
                        } else {
                            // case2or3(temp);
                            
                            //     int min = temp.stream().mapToInt(Integer::intValue).min().getAsInt();
                                
    
                            //     System.out.println();
                                
                            //     updateEquivalencyTable(this.zeroFramedAry[i][j], min);
                            //     for(int t:temp){
                            //         if(t>0){
                            //             updateEquivalencyTable(t,min);
                            //         }
                            //     }
                                
        
                            //     this.zeroFramedAry[i][j] = min;
                            //     updateEquivalencyTable(pixel,min);
                            // }
                            int helper = 0;
                            boolean flag = true;
                            int a = zeroFramedAry[i - 1][j - 1];
                            int b = zeroFramedAry[i - 1][j];
                            int c = zeroFramedAry[i - 1][j + 1];
                            int d = zeroFramedAry[i][j - 1];

                            // if (a != 0) {
                            //     this.nonZeroNeighborAry[helper++] = a;
                            // }
                            if (b != 0) {
                                this.nonZeroNeighborAry[helper++] = b;
                            }
                            // if (c != 0) {
                            //     this.nonZeroNeighborAry[helper++] = c;
                            // }
                            if (d != 0) {
                                this.nonZeroNeighborAry[helper++] = d;
                            }
    
                            for (int e = 1; e < helper; e++) {
                                if (this.nonZeroNeighborAry[e] != this.nonZeroNeighborAry[e - 1]) {
                                    flag = false;
                                }
                            }
    
                            // case 2
                            if (flag) {
                                zeroFramedAry[i][j] = this.nonZeroNeighborAry[0];
                                this.nonZeroNeighborAry = minus1D(this.nonZeroNeighborAry);
                            }
                            // case 3
                            else {
                                
                                int minLabel = findMin();
                                // if (zeroFramedAry[i - 1][j - 1] != 0) {
                                //     updateEquivalencyTable(zeroFramedAry[i - 1][j - 1], minLabel);
                                // }
                                if (zeroFramedAry[i - 1][j] != 0) {
                                    updateEquivalencyTable(zeroFramedAry[i - 1][j], minLabel);
                                }
                                // if (zeroFramedAry[i - 1][j + 1] != 0) {
                                //     updateEquivalencyTable(zeroFramedAry[i - 1][j + 1], minLabel);
                                // }
                                if (zeroFramedAry[i][j - 1] != 0) {
                                    updateEquivalencyTable(zeroFramedAry[i][j - 1], minLabel);
                                }
                                zeroFramedAry[i][j] = minLabel;
                                updateEquivalencyTable(pixel, minLabel);
                            }

    
                    }
    
                }
            }
        }
            
        

        printMap();
    }

    public int[][] connect4Pass1(int[][] inArr) {
        int pixel, label;
        int a, b;
        int helper = 0;
        boolean flag = true;
        for (int i = 1; i <= this.numRows; i++) {
            for (int j = 1; j <= this.numCols; j++) {
                pixel = inArr[i][j];
                if (pixel > 0) {
                    helper = 0;
                    a = inArr[i - 1][j];
                    b = inArr[i][j - 1];
                    // case 1
                    ArrayList<Integer> temp = tempArr(this.zeroFramedAry, 4, i, j);
                    if (case1(temp)){
                        this.newLabel++;
                        updateEquivalencyTable(this.newLabel, this.newLabel);
                        inArr[i][j] = this.newLabel;

                    } else {
                        /********************** */
                        
                        if (a != 0) {
                            this.nonZeroNeighborAry[helper++] = a;
                        }
                        if (b != 0) {
                            this.nonZeroNeighborAry[helper++] = b;
                            if (a != b && a != 0) {
                                flag = false;
                            }
                        }
                        /********************** */
                        
                        
                        

                        System.out.print("his ");
                        for (int x:nonZeroNeighborAry) if(x>0)System.out.print(x+" ");
                        System.out.print("\nmine  ");
                        for(int x:temp) System.out.print(x+" ");
                        System.out.println("\n");

                        // case 2
                        if (flag) {

                            int min = temp.stream().mapToInt(Integer::intValue).min().getAsInt();
                            inArr[i][j] = min;

                            // inArr[i][j] = this.nonZeroNeighborAry[0];
                            // this.nonZeroNeighborAry = minus1D(this.nonZeroNeighborAry);
                            for(int k : nonZeroNeighborAry) k = k*77;
                        }
                        // case 3
                        else {
                            
                            // label = findMin();
                            int min = temp.stream().mapToInt(Integer::intValue).min().getAsInt();

                            if (inArr[i - 1][j] != 0) {
                                updateEquivalencyTable(inArr[i - 1][j], min);
                            }
                            if (inArr[i][j - 1] != 0) {
                                updateEquivalencyTable(inArr[i][j - 1], min);
                            }
                            inArr[i][j] = min;
                            flag = true;
                        }
                    }
                }
            }
        }
        return inArr;
    }

    public int[][] connectPass2(int[][] inArr, int connect) {

        for (int i = 0; i <= this.numRows + 1; i++) {
            for (int j = 0; j <= this.numCols + 1; j++) {
                System.out.print(inArr[i][j] + "  ");
            }
            System.out.print("\n");

        }

        // int e = inArr[i][j + 1];
        // int g = inArr[i + 1][j];
        // ArrayList<Integer> temp = {e,g};
        // if(connect == 8){
        // int f = inArr[i+1][j-1];
        // int h = inArr[i+1][j+1];
        // // Arrays.stream(temp).push
        // }

        int pixel, label;
        // if(connecint[] a; e,f,g,h;
        // int

        // if(e==g){

        // }

        // if(connect == 8 ){
        // if(e==f && g==h){

        // }

        // }

        return null;

    }

    public int[][] connect4Pass2(int[][] inArr) {
        // update Equivalence theorem
        int pixel, label;
        int e, g;
        int helper = 0;
        boolean flag = true;
        for (int i = this.numRows - 1; i >= 1; i--) {
            for (int j = this.numCols - 1; j >= 1; j--) {
                pixel = inArr[i][j];
                if (pixel > 0) {
                    helper = 0;
                    this.nonZeroNeighborAry[helper++] = pixel;
                    e = inArr[i][j + 1];
                    g = inArr[i + 1][j];
                    if (e != 0) {
                        this.nonZeroNeighborAry[helper++] = e;
                        if (pixel != e)
                            flag = false;
                    }
                    if (g != 0) {
                        this.nonZeroNeighborAry[helper++] = g;
                        if (pixel != g)
                            flag = false;
                    }
                    // case 3
                    if (!flag) {
                        label = findMin();
                        inArr[i][j] = label;
                        if (inArr[i + 1][j] != 0)
                            inArr[i + 1][j] = label;
                        if (inArr[i][j + 1] != 0)
                            inArr[i][j + 1] = label;
                        updateEquivalencyTable(pixel, label);
                        flag = true;
                    }
                }
            }
        }
        return inArr;
    }

    public void connectPass3(int[][] inArr, Property[] CCproperty) {
        int k = 0;
        int nummPixs;
        int pixel;
        // algo in specs
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

    public void drawBoxes(int[][] inArr, Property[] CCproperty) {
        // algo in specs
        int minRow, minCol, maxRow, maxCol, label;
        for (int index = 1; index <= this.trueNumCC; index++) {
            minRow = this.CCproperty[index].minR + 1;
            minCol = this.CCproperty[index].minC + 1;
            maxRow = this.CCproperty[index].maxR + 1;
            maxCol = this.CCproperty[index].maxC + 1;
            label = this.CCproperty[index].label;

            for (int i = minCol; i <= maxCol; i++) {
                inArr[minRow][i] = label;
                inArr[maxRow][i] = label;
            }
            for (int i = minRow; i <= maxRow; i++) {
                inArr[i][minCol] = label;
                inArr[i][maxCol] = label;
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

    private int findMin() {
        int retVal = this.nonZeroNeighborAry[0];
        for (int i = 1; i < this.nonZeroNeighborAry.length; i++) {
            if (retVal > this.nonZeroNeighborAry[i] && this.nonZeroNeighborAry[i] != -1) {
                retVal = this.nonZeroNeighborAry[i];
            }
        }
        this.nonZeroNeighborAry = minus1D(this.nonZeroNeighborAry);
        return retVal;
    }

    public void makeBorder(BufferedWriter out) throws IOException {
        for (int i = 0; i <= this.numRows + 1; i++) {
            out.write("---");
        }
        out.write("\n");
    }

}

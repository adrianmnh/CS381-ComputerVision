#include <iostream>
#include <string>

using namespace std;

// static int f(int x){
//     return 3*x/x;
// }

static void findMax(int arr[]){


//  cout << sizeof(*arr);
    // int mine[] = arr;
    // for(auto x:arr) cout<<x;
    cout<<"HI";
}

static int f(int x, int arr[]){ 
    return arr[x];
}

static bool func(int col_x, int row_y, int lx, int ly, int arr[]){

    if (f(col_x, arr) == ly - row_y) return true;
    return false;
}
static void graph(int x_axis, int y_axis, int arr[]){

    // cout << arr[1]<<endl;

    for (int row=0; row<=y_axis; row++){
        for (int col=0; col<=x_axis; col++){
            if ((row==x_axis && col==y_axis) || (col==0) || row==y_axis) cout << "* ";
            else if(row<y_axis && col>0){
                if(func(col,row,x_axis,y_axis, arr)) cout << "* ";
                else cout << "  ";
            } 
        }
        cout << endl;
    }
}

static bool funcLine(int col_x, int row_y, int lx, int ly, int arr[]){

    if (f(col_x, arr) >= ly - row_y) return true;
    return false;
}


static void graphLine(int x_axis, int y_axis, int arr[]){

    // cout << arr[1]<<endl;

    for (int row=0; row<=y_axis; row++){
        for (int col=0; col<=x_axis; col++){
            if ((row==x_axis && col==y_axis) || (col==0) || row==y_axis) cout << "* ";
            else if(row<y_axis && col>0){
                if(funcLine(col,row,x_axis,y_axis, arr)) cout << "* ";
                else cout << "  ";
            } 
        }
        cout << endl;
    }
}

// int main(int argc, const char* args[]){

//     graph(5,6);
//     return 0;

// }


// |
// |6
// |________11_____
//                      (x+col)(y-row)
// f(1) = 1 -> (1,1)    (6-1) ( x(0)+1)   (5,1)
// f(2) = 2 -> (2,2)
// f(3) = 2 -> (2,2)
// f(4) = 2 -> (2,2)
// f(5) = 2 -> (5,5)
// f(6) = 2 -> (6,6)  (6-6) (x(0)+6)  (r,c)->(0,6)
//                                            0,6,  y=6, x=11
//                                              (col)(y-row)
// (y=16)  x=5
// (x,y) -> (col,row)
// f(1) = 1 (1,1)          (col, y-row)->(15,1)
// f(2) = 4 (2,2)
// f(3) = 9 (3,9)
// f(4) = 16 (4,16)        (col, y-row)
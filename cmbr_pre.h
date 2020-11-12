#include <iostream>
#include <vector>
#include <array>
#include <bitset>
#include <cmath> 
#include<bits/stdc++.h>

// #include "Types.h" 

#define MAX_COUNT 5000

using namespace std;

struct table_row {
    int id;
    float x;
    float y;
};

// data structure to save MBR info
struct mbr {
    float x1;
    float y1;
    float x2;
    float y2;
    bool empty = true;
};

float const DIST = 200.0;

int const FMAX = 13;

// number of rows in the data file
int ROWS;

int count1 = 0, count2 = 0;

// saves all counts for features
vector<int> fcount(FMAX);

vector<vector<mbr>> mbr_array(FMAX);

// vector<mbr_t> xMBR1;
// vector<mbr_t> yMBR1; 
// vector<mbr_t> xMBR2; 
// vector<mbr_t> yMBR2;

mbr_t* xMBR1 = (mbr_t*) malloc(sizeof(mbr_t) * MAX_COUNT * 2); 
mbr_t* xMBR2 = (mbr_t*) malloc(sizeof(mbr_t) * MAX_COUNT * 2); 
mbr_t* yMBR1 = (mbr_t*) malloc(sizeof(mbr_t) * MAX_COUNT * 2); 
mbr_t* yMBR2 = (mbr_t*) malloc(sizeof(mbr_t) * MAX_COUNT * 2); 


void print_message(string str) {
    if (true)
    {
        cout << str << endl;        
    }
}

// read file data and retunr data as an object array
struct table_row *createArray(const char *fileName) {
    FILE *fp = fopen(fileName, "r");

    if (!fp) {
        print_message("Can not open file\n");
        return NULL;
    }
    
    fscanf(fp, "%d", &ROWS);
    // cout << "Total rows: " << ROWS << endl;

    struct table_row* table_rows = (struct table_row*)malloc(sizeof(struct table_row) * ROWS);
    
    for (int count = 0; count < ROWS; ++count) {
        fscanf(fp, "%d, %f, %f", &table_rows[count].id, &table_rows[count].x, &table_rows[count].y);

    }
    // float x,y;

    // fscanf(fp, "%f, %f, %f, %f", &GRID_MIN_X, &GRID_MIN_Y, &x, &y);

    // cout << GRID_ROWS << " " << GRID_COLS << " " << GRID_MIN_X << " " << GRID_MIN_Y << endl;


    fclose(fp);
    return table_rows;
}

// calculate MBR for a given datapoint
mbr getMBR(float px, float py) {
    mbr box;
    box.x1 = px - DIST;
    box.y1 = py - DIST;
    box.x2 = px + DIST;
    box.y2 = py + DIST;

    return box;
}

void getMBRList(struct table_row *data) {

    int j = 0, k = 0;
    int row, col;
    mbr temp;

    print_message("Grid set Start...");
    for (int i = 0; i < ROWS; ++i) { 
        // check if feature id changes
        if (j != (data[i].id - 1)) {
            j = (data[i].id - 1);
            k++;
        }

        temp = getMBR(data[i].x, data[i].y);

        // calculate MBR using the getMBR() and assign it to the relavant feature instance
        mbr_array[k].push_back(temp);
        // cout << k << mbr_array[k].x1;
        fcount[k] += 1; 
    }
    print_message("Grid set...");
}

// get how many decimal points
long convertFloatToLong(float num) {
    long x;
    // stringstream ss;
    // int n = (int)num;
    // ss << abs(num-n); 
    // string s; 
    // ss >> s;
    // cout << num << " " << n << " " << s << " " << num-n << endl;
    // int p = pow(10,s.length()-2);
    // x = num*p;
    x = num*10;
    // cout << num*10000 << " " << s.length()-2 << " " << x << endl;
    return x;
}

// select smaple data
void preProcessMBRArray(int fid1, int fid2) {
    count1 = 0, count2 = 0;

    for (int i = 0; i < mbr_array[fid1].size(); ++i)
    {
        xMBR1[count1*2] = convertFloatToLong(mbr_array[fid1][i].x1);
        xMBR1[count1*2+1] = convertFloatToLong(mbr_array[fid1][i].x2);
        yMBR1[count1*2] = convertFloatToLong(mbr_array[fid1][i].y1);
        yMBR1[count1*2+1] = convertFloatToLong(mbr_array[fid1][i].y2);

        count1++;
        // xMBR1.push_back(getNumDecimalDigits(mbr_array[row][col][fid1][i].x1));
        // xMBR1.push_back(getNumDecimalDigits(mbr_array[row][col][fid1][i].x2));
        // yMBR1.push_back(getNumDecimalDigits(mbr_array[row][col][fid1][i].y1));
        // yMBR1.push_back(getNumDecimalDigits(mbr_array[row][col][fid1][i].y2));
    }
    for (int i = 0; i < mbr_array[fid2].size(); ++i)
    {
        xMBR2[count2*2] = convertFloatToLong(mbr_array[fid2][i].x1);
        xMBR2[count2*2+1] = convertFloatToLong(mbr_array[fid2][i].x2);
        yMBR2[count2*2] = convertFloatToLong(mbr_array[fid2][i].y1);
        yMBR2[count2*2+1] = convertFloatToLong(mbr_array[fid2][i].y2);

        count2++;
        // xMBR2.push_back(getNumDecimalDigits(mbr_array[row][col][fid2][i].x1));
        // xMBR2.push_back(getNumDecimalDigits(mbr_array[row][col][fid2][i].x2));
        // yMBR2.push_back(getNumDecimalDigits(mbr_array[row][col][fid2][i].y1));
        // yMBR2.push_back(getNumDecimalDigits(mbr_array[row][col][fid2][i].y2));
    }
}

void printArray(mbr_t *x, int count) {
    for (int i = 0; i < count; ++i)
    {
        cout << x[i] << " ";
    }
    cout << endl;
}

// int main() {
//     // read data into a table_row structure type 1D array
//     struct table_row *dat;
//     dat = createArray("data/Point_Of_Interest_modified.csv");

//     getMBRList(dat);
//     print_message("mbr array constructed");

//     preProcessMBRArray(0,1);
//     // cout<<"Numbers after decimal point = "<<getNumDecimalDigits(12.351)<<endl; 
//     printArray(xMBR1, 10);
//     printArray(yMBR1, 10);
//     printArray(xMBR2, 10);
//     printArray(yMBR2, 10);
   
//    // convertFloatToLong(mbr_array[0][0].x1);
   
//     return 0;
// }
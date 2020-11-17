#include <iostream>
#include <vector>
#include <array>
#include <bitset>
#include <cmath> 
#include<bits/stdc++.h>

// #include "Types.h" 

#define MAX_COUNT 9000

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

long const DIST = 200;

int const FMAX = 13;

// number of rows in the data file
int ROWS;

int count1 = 0, count2 = 0;

// saves all counts for features
vector<int> fcount(FMAX);

vector<vector<mbr>> mbr_array(FMAX);

mbr_t* x_MBR1 = (mbr_t*) malloc(sizeof(mbr_t) * MAX_COUNT * 2); 
mbr_t* x_MBR2 = (mbr_t*) malloc(sizeof(mbr_t) * MAX_COUNT * 2); 
mbr_t* y_MBR1 = (mbr_t*) malloc(sizeof(mbr_t) * MAX_COUNT * 2); 
mbr_t* y_MBR2 = (mbr_t*) malloc(sizeof(mbr_t) * MAX_COUNT * 2); 


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

// get number of digits of a given number
int getDigitCount(long num) {
    int count = 0;
    while(num != 0) {
        num /= 10;
        count++;
    }
    return count;
}

// get how many decimal points
long convertFloatToLong(float num, int id, int i) {
    long x;
    int n;

    x = num*10;

    if (mbr_array[id].size() <= i)
    {
        x = 0; 
    }
    return x;
}

// select smaple data
void preProcessMBRArray(int fid1, int fid2) {
    count1 = 0, count2 = 0;

    // for (int i = 0; i < /*3190*/mbr_array[fid1].size(); ++i)
    for (int i = 0; i < MAX_COUNT /* 3190*/; ++i)
    {
        x_MBR1[count1*2] = convertFloatToLong(mbr_array[fid1][i].x1, fid1, i);
        x_MBR1[count1*2+1] = convertFloatToLong(mbr_array[fid1][i].x2, fid1, i);
        y_MBR1[count1*2] = convertFloatToLong(mbr_array[fid1][i].y1, fid1, i);
        y_MBR1[count1*2+1] = convertFloatToLong(mbr_array[fid1][i].y2, fid1, i);

        count1++;
    }
    // for (int i = 0; i < /*3899*/mbr_array[fid2].size(); ++i)
    for (int i = 0; i < MAX_COUNT /* 6000*/; ++i)
    {
        x_MBR2[count2*2] = convertFloatToLong(mbr_array[fid2][i].x1, fid2, i);
        x_MBR2[count2*2+1] = convertFloatToLong(mbr_array[fid2][i].x2, fid2, i);
        y_MBR2[count2*2] = convertFloatToLong(mbr_array[fid2][i].y1, fid2, i);
        y_MBR2[count2*2+1] = convertFloatToLong(mbr_array[fid2][i].y2, fid2, i);

        count2++;
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
//     printArray(x_MBR1, 10);
//     printArray(y_MBR1, 10);
//     printArray(x_MBR2, 10);
//     printArray(y_MBR2, 10);
   
//    // convertFloatToLong(mbr_array[0][0].x1);
   
//     return 0;
// }
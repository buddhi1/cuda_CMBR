#include <iostream>
#include <vector>
#include <array>
#include <bitset>
#include <cmath> 

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

// grid number of rows
int GRID_ROWS;

// grid number of columns
int GRID_COLS;

// grid origin
float GRID_MIN_X, GRID_MIN_Y;

// saves all counts for features
vector<int> fcount(FMAX);

vector<vector<vector<vector<mbr>>>>  mbr_array;
vector<vector<vector<int>>> instance_sum;

vector<float> xMBR1; 
vector<float> yMBR1; 
vector<float> xMBR2; 
vector<float> yMBR2;

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
	float x,y;

	fscanf(fp, "%f, %f, %f, %f", &GRID_MIN_X, &GRID_MIN_Y, &x, &y);

	GRID_MIN_X -= DIST;
	GRID_MIN_Y -= DIST;

	GRID_COLS = ceil((x - GRID_MIN_X)/(DIST * 2)) + 2;
	GRID_ROWS = ceil((y - GRID_MIN_Y)/(DIST * 2)) + 2;
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
        col = floor((temp.x1 - GRID_MIN_X) / (DIST * 2));
        row = GRID_ROWS - 1 - floor((temp.y2 - GRID_MIN_Y) / (DIST * 2));

        // calculate MBR using the getMBR() and assign it to the relavant feature instance
        mbr_array[row][col][k].push_back(temp);
        fcount[k] += 1; 
    }
    print_message("Grid set...");
    // calculate cumulative sums
    for (int i = 0; i < GRID_ROWS; ++i)
    {
        for (int j = 0; j < GRID_COLS; ++j)
        {
            for (int k = 0; k < FMAX; ++k)
            {                          
                if (j + 1 < GRID_COLS)
                { 
                    instance_sum[i][j + 1][k] = mbr_array[i][j][k].size() + instance_sum[i][j][k];          
                } else if(i + 1 < GRID_ROWS) { 
                    instance_sum[i + 1][0][k] = mbr_array[i][j][k].size() + instance_sum[i][j][k];          
                }
            }
        }
    }

}

// select smaple data
void preProcessMBRArray(int fid1, int fid2) {
	for (int row = 0; row< GRID_ROWS; ++row)
	{
		for (int col = 0; col < GRID_COLS; ++col)
		{
			for (int i = 0; i < mbr_array[row][col][fid1].size(); ++i)
			{
				xMBR1.push_back(mbr_array[row][col][fid1][i].x1);
				xMBR1.push_back(mbr_array[row][col][fid1][i].x2);
				yMBR1.push_back(mbr_array[row][col][fid1][i].y2);
				yMBR1.push_back(mbr_array[row][col][fid1][i].y1);
			}
			for (int i = 0; i < mbr_array[row][col][fid2].size(); ++i)
			{
				xMBR2.push_back(mbr_array[row][col][fid2][i].x1);
				xMBR2.push_back(mbr_array[row][col][fid2][i].x2);
				yMBR2.push_back(mbr_array[row][col][fid2][i].y2);
				yMBR2.push_back(mbr_array[row][col][fid2][i].y1);
			}
		}
	}
}

// int main() {
// 	// read data into a table_row structure type 1D array
//     struct table_row *dat;
// 	dat = createArray("data/Point_Of_Interest_modified.csv");

//     mbr_array.resize(GRID_ROWS, vector<vector<vector<mbr>>>(GRID_COLS, vector<vector<mbr>>(FMAX)));
//     print_message("mbr_array initialized..");
// 	instance_sum.resize(GRID_ROWS, vector<vector<int>>(GRID_COLS, vector<int>(FMAX, 0))); 
//     print_message("instance_sum initialized..");

// 	getMBRList(dat);
//     print_message("mbr array constructed");

//     preProcessMBRArray(0,1);

// 	return 0;
// }
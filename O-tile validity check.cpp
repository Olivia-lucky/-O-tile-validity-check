#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <new>
#include <set>
#include <iterator>

using namespace std;

// romove duplicate elements from the vector v
vector<int> unique_element_in_vector(vector<int> v) {
    vector<int>::iterator vector_iterator;
    sort(v.begin(), v.end());
    vector_iterator = unique(v.begin(), v.end());
    if (vector_iterator != v.end()) {
        v.erase(vector_iterator, v.end());
    }
    return v;
}

// find the intersection of two vectors v1 and v2
vector<int> vectors_intersection(vector<int> v1, vector<int> v2) {
    vector<int> v;
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v));
    return v;
}

// find the union of two vectors v1 and v2
vector<int> vectors_set_union(vector<int> v1, vector<int> v2) {
    vector<int> v;
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v));
    return v;
}

// determine whether element e exists in vector v
bool is_element_in_vector(vector<int> v, int e) {
    vector<int>::iterator it;
    it = find(v.begin(), v.end(), e);
    if (it != v.end()) {
        return true;
    }
    else {
        return false;
    }
}

// print all elements of the vector v
void print_vector(vector<int> v) {
    if (v.size() > 0) {
        cout << "{";
        for (int i = 0; i<int(v.size()); i++) {
            cout << v[i] << ",";
        }
        cout << "\b}.";
        cout << endl;
    }
    else {
        cout << "{ }";
    }
}

unsigned short int m, s;

FILE* fp;
char fname[20];  // filename, the content of the file is a tile structure with s-tiles in C^m otimes C^m, and the grid lines in the m*m grid are omitted for convenience.

int** Tile;  // m * m gird(i.e. the tile structure)
int* index;  // the set of the number of (valid) columns of MM and NN, e.g. MM = {{0,0,1,1},{1,2},...,{5,7,11}}, index = {4,2,...,3}

int MM[1000][1000] = { 0 };  // the row indices of s tiles(MM[] may contain duplicate elements), e.g. MM[0] = {0,0,1,1} (actually MM[0] = {0,0,1,1(0,...,0)}, but we only focus on valid columns)
int NN[1000][1000] = { 0 };  // the column indices of s tiles(NN[] may contain duplicate elements)
vector<vector<int> > M(1000);  // the row indices of s tiles(M[] contains no duplicate elements), e.g. M[0] = {0,1}(i.e. the row indices of tile t_1 are 0 and 1)
vector<vector<int> > N(1000);  // the column indices of s tiles(N[] contains no duplicate elements)  
vector<int> Trans;  
vector<int> Nums;  // tiles t_k' may form a special rectangle, where k'\in Nums
vector<int> SumM;  // the union of row indices of tiles
vector<int> SumN;  // the union of column indices of tiles

bool checkSpecialRectangle();

int main() {
    cout << "This tool will try to help you check whether a tile structure with s-tiles in C^m otimes C^m is an O-tile structrue." << endl;
    cout << "\nPlease enter m: ";
    cin >> m;
    cout << "\nPlease enter s: ";
    cin >> s;
    cout << "\nPlease enter the filename to read: ";
    cin >> fname;

    Tile = new int* [m];
    for (int i = 0; i < m; i++) {
        Tile[i] = new int[m];
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            Tile[i][j] = 0;
        }
    }

    index = new int[s];
    for (int i = 0; i < s; i++) {
        index[i] = 0;
    }

    ifstream myfile(fname);

    if (!myfile.is_open()) {
        cout << "\nSorry, could not find the file " << fname << "£¡" << endl;
        return 0;
    }

    for (int i = 0; i < m; i++) {  // read the contents of the file into Tile[m][m]
        for (int j = 0; j < m; j++) {
            myfile >> Tile[i][j];
        }
    }

    int Max = Tile[0][0];

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            if (Tile[i][j] > Max) {
                Max = Tile[i][j];
            }
        }
    }

    if (Max != s) {  // check whether the number of tiles contained in the tile structure is equal to s
        cout << "\nSorry, this tile structure does not contain s tiles. Please check agian!";
        return 0;
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            MM[Tile[i][j] - 1][index[Tile[i][j] - 1]] = i;
            NN[Tile[i][j] - 1][index[Tile[i][j] - 1]] = j;
            index[Tile[i][j] - 1]++;
        }
    }

    M = vector<vector<int>>(s, vector<int>(1));
    N = vector<vector<int>>(s, vector<int>(1));

    for (int i = 0; i < s; i++) {  // this is for the convenience of deduplication later
        M[i].assign(index[i], 0);
        N[i].assign(index[i], 0);
    }

    for (int i = 0; i < s; i++) {
        for (int j = 0; j < index[i]; j++) {
            M[i][j] = MM[i][j];
            N[i][j] = NN[i][j];
        }
    }

    for (int i = 0; i < s; i++) {  // remove duplicate elements, e.g. M[0] = {0,0,1,1} --> M[0] = {0,1}
        M[i] = unique_element_in_vector(M[i]);
        N[i] = unique_element_in_vector(N[i]);
    }

    myfile.close();

    for (int i = 0; i < s; i++) {  // check whether each tile in the tile structure is a (possibly separated) rectangle
        if (index[i] != M[i].size() * N[i].size()) {
            cout << "\nTile " << i + 1 << " is not a rectangle!" << endl;
            return 0;
        }
    }

    // check whether the tile structure contains other special rectangles in addition to the whole grid itself
    if (checkSpecialRectangle()) {
        cout << "\nCongratulations to you! This tile structure is an O-tile structure!\n" << endl;
    }
    else {
        cout << "\nSorry, this tile structure is not an O-tile structure." << endl;
        cout << "The special rectangle contains tiles ";
        sort(Nums.begin(), Nums.end());
        for (int v = 0; v < Nums.size(); v++) {
            cout << Nums[v] << ",";
        }
        cout << "\b." << endl;
        cout << "The row indices of the special rectangle are ";
        print_vector(SumM);
        cout << "The column indices of the special rectangle are ";
        print_vector(SumN);
    }

    delete[]index;
    for (int i = 0; i < m; i++) {
        delete[]Tile[i];
    }
    delete[]Tile;

    return 0;
}

/*
   This function check whether the tile structure contains other special rectangles in addition to the whole grid itself
*/
bool checkSpecialRectangle() {
    int k, temp;

    for (int i = 0; i < s - 1; i++) {  // loop through all possible values of (i,j)
        for (int j = i + 1; j < s; j++) {
            Trans.emplace_back(i + 1);  // transition
            Trans.emplace_back(j + 1);

            while (!Trans.empty()) {
                temp = Trans.back();
                Trans.pop_back();
                Nums.emplace_back(temp);  
                SumM = vectors_set_union(SumM, M[temp - 1]);  
                SumN = vectors_set_union(SumN, N[temp - 1]);  
                if (SumM.size() == m && SumN.size() == m) {  
                    goto nextpair;
                }

                for (vector<int>::iterator it = SumM.begin(); it < SumM.end(); it++) {
                    for (vector<int>::iterator vit = SumN.begin(); vit < SumN.end(); vit++) {
                        k = Tile[(*it)][(*vit)] - 1;
                        if ((k < i) || (k != i && k < j)) {  // the pair (i,e) or (e,i) is checked before.
                            goto nextpair;
                        }
                        if ((is_element_in_vector(Nums, k + 1) == false) && (is_element_in_vector(Trans, k + 1) == false)) {
                            Trans.emplace_back(k + 1);
                        }
                    }
                }
            }

            if (SumM.size() < m || SumN.size() < m) {  
                return false;
            }

        nextpair:
            Trans.clear();
            Trans.shrink_to_fit();
            Nums.clear();
            Nums.shrink_to_fit();
            SumM.clear();
            SumM.shrink_to_fit();
            SumN.clear();
            SumN.shrink_to_fit();
        }
    }
    return true; 
}


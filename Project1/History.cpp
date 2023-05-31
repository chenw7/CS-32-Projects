//
//  History.cpp
//  Proj1New
//
//  Created by Simba Chen on 1/14/23.
//

#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols){
    for(int i = 0; i < nRows; i++){
        for(int j = 0; j < nCols; j++){
            m_grid[i][j] = 0;
        }
    }
    
    m_rows = nRows;
    m_cols = nCols;
}

bool History::record(int r, int c){
    if(r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols){
        m_grid[r-1][c-1]++;
        return true;
    }
    return false;
}

void History::display() const{
    clearScreen();
    
    //display grid
    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_cols; j++){
            if(m_grid[i][j] == 0){
                cout << ".";
            }
            else if(m_grid[i][j] <= 26){
                cout << char(m_grid[i][j]+64);
            }
            else{
                cout << "Z";
            }
        }
        cout << endl;
    }
    cout << endl;
}


//
//  History.hpp
//  Proj1New
//
//  Created by Simba Chen on 1/14/23.
//

#ifndef History_h
#define History_h

#include <stdio.h>
#include "globals.h"

class History
{
    public:
        History(int nRows, int nCols);
        bool record(int r, int c);
        void display() const;
    private:
        int m_grid[MAXROWS][MAXCOLS];
        int m_rows;
        int m_cols;
};


#endif /* History_h */

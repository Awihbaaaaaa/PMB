#include <iostream>
#include <vector>
#include <limits>
#include <numeric>
#include <cmath>
#include <algorithm>

void shortestPath(int curUnassRow, std::vector<int>& pred, std::vector<int>& u, std::vector<int>& v,
                  const std::vector<std::vector<int>>& C, const std::vector<int>& col4row, const std::vector<int>& row4col,
                  int& sink);

void assign2DByCol(std::vector<std::vector<int>>& C, bool maximize,
                   std::vector<int>& col4row, std::vector<int>& row4col, int& gain,
                   std::vector<int>& u, std::vector<int>& v);

void shortestPath(int curUnassRow, std::vector<int>& pred, std::vector<int>& u, std::vector<int>& v,
                  const std::vector<std::vector<int>>& C, const std::vector<int>& col4row, const std::vector<int>& row4col,
                  int& sink) {
    size_t numRow = C.size();
    size_t numCol = C[0].size();

    std::vector<int> ScannedRows(numRow, 0);
    std::vector<int> ScannedCol(numCol, 0);
    std::vector<int> Col2Scan(numCol);
    std::iota(Col2Scan.begin(), Col2Scan.end(), 0);  // Fill Col2Scan with values 0 to numCol-1
    std::cout << "columns to scan are:" << std::endl;
    for(int intttt:Col2Scan){
        std::cout << intttt << std::endl;
    }
    size_t numCol2Scan = numCol;

    sink = -1;
    int delta = 0;
    int curRow = curUnassRow;
    std::vector<int> shortestPathCost(numCol, std::numeric_limits<int>::max());

    while (sink == -1) {
        ScannedRows[curRow] = 1;

        int minVal = std::numeric_limits<int>::max();
        size_t closestColScan = 0;

        for (size_t curColScan = 0; curColScan < numCol2Scan; ++curColScan) {
            size_t curCol = Col2Scan[curColScan];
            int reducedCost = delta + C[curRow][curCol] - u[curRow] - v[curCol];
            std::cout << "delta is " << delta << ", C["<< curRow << "][" << curCol << "] is " << C[curRow][curCol];
            std::cout << ", u[" << curRow << "] is " << u[curRow] << " and v["<<curCol<<"] is " << v[curCol] << std::endl;
            /* if (pred.size() <= curCol) {
                pred.resize(curCol + 1);
            } */

            if (reducedCost < shortestPathCost[curCol]) {
                std::cout << "pred[" << curCol << "] = " << static_cast<int>(curRow) << std::endl;
                pred[curCol] = static_cast<int>(curRow);
                std::cout << "The reduced cost is : " << reducedCost << std::endl;
                shortestPathCost[curCol] = reducedCost;
            }

            if (shortestPathCost[curCol] < minVal) {
                minVal = shortestPathCost[curCol];
                closestColScan = curColScan;
            }
        }

        if (!std::isfinite(minVal)) {
            sink = -1;
            std::cout << "Infeasable problem.\n";
            return;
        }

        size_t closestCol = Col2Scan[closestColScan];
        ScannedCol[closestCol] = 1;
        numCol2Scan--;
        Col2Scan.erase(Col2Scan.begin() + closestColScan);

        delta = shortestPathCost[closestCol];
        
        // If we reached an unassigned row
        std::cout << "row4col[closestCol]: " << row4col[closestCol] << std::endl;
        if (row4col[closestCol] == -1) {
            sink = static_cast<int>(closestCol);
        } else {
            curRow = static_cast<int>(row4col[closestCol]);
        }
    }

    u[curUnassRow] += delta;
    
    for (size_t i = 0; i < numRow; ++i) {
        std::cout << "u[" << i << "] = " << u[i] << std::endl;
        if (i != static_cast<size_t>(curUnassRow) && col4row[i] != -1 ) {
            u[i] += delta - shortestPathCost[col4row[i]];
            std::cout << "after update u[" << i << "] = " << u[i] << std::endl;
        }
    }

    for (size_t j = 0; j < numCol; ++j) {
        std::cout << "v[" << j << "] = " << v[j] << std::endl;
        if(ScannedCol[j] != 0){
            std::cout << "delta is " << delta << std::endl;
            std::cout << "shortestPath "<< j <<" is " << shortestPathCost[j] << std::endl;
            v[j] = v[j] - delta + shortestPathCost[j];
            std::cout << "v[" << j << "] is updated to " << v[j] << std::endl;
        }
    }
}

void assign2DByCol(std::vector<std::vector<int>>& C, bool maximize,
                   std::vector<int>& col4row, std::vector<int>& row4col, int& gain,
                   std::vector<int>& u, std::vector<int>& v) {
    size_t numRow = C.size();
    size_t numCol = C[0].size();

    int CDelta;
    if (maximize) {
        int max = std::numeric_limits<int>::min();
        for (const std::vector<int>& row : C) {
            int CDelta = *std::max_element(row.begin(), row.end());
            if (CDelta > max) {
                max = CDelta;
            }
        }
        std::cout << "Cost matrix:" << std::endl;
        for (size_t i = 0; i < numRow; ++i) {
            for (size_t j = 0; j < numCol; ++j) {
                C[i][j] = -C[i][j] + max;
                std::cout<< C[i][j] << " ";
            }
            std::cout << std::endl;
        }
    } else {
        int min = std::numeric_limits<int>::max();
        for (const std::vector<int>& row : C) {
            int CDelta = *std::min_element(row.begin(), row.end());
            if (CDelta < min) {
                min = CDelta;
            }
        }
        std::cout << "Cost matrix:" << std::endl;
        for (size_t i = 0; i < numRow; ++i) {
            for (size_t j = 0; j < numCol; ++j) {
                C[i][j] = C[i][j] - min;
                std::cout<< C[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    bool didFlip = false;
    if (numRow > numCol) {
        std::vector<std::vector<int>> CT(numCol, std::vector<int>(numRow));
        for (size_t i = 0; i < numRow; ++i) {
            for (size_t j = 0; j < numCol; ++j) {
                CT[j][i] = C[i][j];
            }
        }
        C = CT;
        std::swap(numRow, numCol);
        didFlip = true;
    }

    col4row.resize(numRow);
    row4col.resize(numCol);
    u.resize(numRow);
    v.resize(numCol);

    for (int curUnassRow = 0; curUnassRow < static_cast<int>(numRow); ++curUnassRow) {
        int sink;
        std::vector<int> pred(numCol);
        shortestPath(curUnassRow, pred, u, v, C, col4row, row4col, sink);
        
        if (sink == -1) {
            col4row.clear();
            row4col.clear();
            gain = -1;
            return;
        }

        int j = sink;
        while (true) {
            int i = pred[j];

            // A check to prevent index out of bounds
            if (i < 0 || i >= static_cast<int>(numRow) || j < 0 || j >= static_cast<int>(numCol)) {
                std::cerr << "Error: Index out of bounds." << std::endl;
                exit(EXIT_FAILURE);
            }
            row4col[j] = i;
            int h = col4row[i];
            
            // A check to prevent index out of bounds
            if (h < -1 || h >= static_cast<int>(numCol)) {
                std::cerr << "Error: Index out of bounds." << std::endl;
                exit(EXIT_FAILURE);
            }
            col4row[i] = j;
            j = h;

            if (i == curUnassRow) {
                break;
            }
        }
    }

    gain = 0;
    for (size_t curRow = 0; curRow < numRow; ++curRow) {
        gain += C[curRow][col4row[curRow]];
    }

    if (maximize) {
        gain = -gain + static_cast<int>(CDelta * numRow);
    } else {
        gain += static_cast<int>(CDelta * numRow);
    }

    if (didFlip) {
        std::swap(row4col, col4row);
        std::swap(u, v);
    }
    for(int in=0;in<3;in++){
        std::cout << u[in] << std::endl;
    }
    for(int in=0;in<3;in++){
        std::cout << v[in] << std::endl;
    }
    for(int in=0;in<3;in++){
        std::cout << col4row[in] << std::endl;
    }
    for(int in=0;in<3;in++){
        std::cout << row4col[in] << std::endl;
    }
}



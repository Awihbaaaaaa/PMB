#ifndef ASSIGN2DBYCOL_H
#define ASSIGN2DBYCOL_H

#include <vector>

/**
 * @brief Solve the two-dimensional assignment problem with a
 * rectangular cost matrix C, scanning column-wise.
 *
 * @param C Cost matrix.
 * @param maximize If true, maximize the assignment (default is false).
 * @return std::vector<int> col4row, std::vector<int> row4col, int gain, std::vector<int> u, std::vector<int> v.
 */
std::tuple<std::vector<int>, std::vector<int>, int, std::vector<int>, std::vector<int>> assign2DByCol(std::vector<std::vector<int>>& C, bool maximize = false);

#endif // ASSIGN2DBYCOL_H

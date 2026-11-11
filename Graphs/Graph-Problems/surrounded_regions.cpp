/**
 * Capture 'O' regions surrounded by 'X' on a rectangular board.
 * Reverse the question: boundary-connected O cells cannot be captured.
 * Multi-source BFS marks them safe; flip only unmarked O cells.
 * O(rows*columns) time and space. Iterative traversal avoids deep DFS stacks.
 */

#include <array>
#include <cassert>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
using namespace std;

void captureRegions(vector<string> &board) {
    if (board.empty()) {
        return;
    }
    int rows = static_cast<int>(board.size()),
        columns = static_cast<int>(board[0].size());
    for (const auto &row : board) {
        if (static_cast<int>(row.size()) != columns) {
            throw invalid_argument("ragged board");
        }
        for (char cell : row) {
            if (cell != 'O' && cell != 'X') {
                throw invalid_argument("expected O or X");
            }
        }
    }
    if (columns == 0) {
        return;
    }
    vector<vector<bool>> safe(rows, vector<bool>(columns));
    queue<pair<int, int>> frontier;
    auto add = [&](int row, int column) {
        if (!safe[row][column] && board[row][column] == 'O') {
            safe[row][column] = true;  // mark on enqueue to avoid duplicates
            frontier.emplace(row, column);
        }
    };
    for (int row = 0; row < rows; ++row) {
        add(row, 0);
        add(row, columns - 1);
    }
    for (int column = 0; column < columns; ++column) {
        add(0, column);
        add(rows - 1, column);
    }
    const array<int, 5> direction{1, 0, -1, 0, 1};
    while (!frontier.empty()) {
        auto [row, column] = frontier.front();
        frontier.pop();
        for (int d = 0; d < 4; ++d) {
            int nextRow = row + direction[d],
                nextColumn = column + direction[d + 1];
            if (nextRow >= 0 && nextRow < rows && nextColumn >= 0 &&
                nextColumn < columns) {
                add(nextRow, nextColumn);
            }
        }
    }
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            if (board[row][column] == 'O' && !safe[row][column]) {
                board[row][column] = 'X';
            }
        }
    }
}

int main() {
    vector<string> board{"XXXX", "XOOX", "XXOX", "XOXX"};
    captureRegions(board);
    assert((board == vector<string>{"XXXX", "XXXX", "XXXX", "XOXX"}));
    vector<string> single{"O"};
    captureRegions(single);
    assert(single[0] == "O");
    vector<string> empty;
    captureRegions(empty);
    for (const auto &row : board) {
        cout << row << '\n';
    }
}

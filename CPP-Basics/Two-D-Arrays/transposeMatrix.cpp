// Write a program to display transpose of matrix entered by the user.

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int rows, columns;
    cout << "Enter the no of rows: ";
    cin >> rows;

    cout << "Enter the no of columns: ";
    cin >> columns;

    vector<vector<int>> matrix(rows, vector<int>(columns));

    cout << "Enter the element of matrix " << endl;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            cin >> matrix[i][j];
        }
    }

    vector<vector<int>> transpose(columns, vector<int>(rows));

    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            transpose[i][j] = matrix[j][i];
        }
    }

    cout << "Transpose of entered matrix is: " << endl;

    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            cout << transpose[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
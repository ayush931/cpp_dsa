// Omitting parts of loop

// 1. Omitting init statement

#include <iostream>
using namespace std;

int main() {
    int i = 0;

    for (; i <= 5; i++) {
        cout << i << endl;
    }

    return 0;
}

// 2. Omitting condition

/* for (int i = 1;  ; i++) {
    code -> should include my stopping condition
}
*/

// 3. Omitting final expression

/* for (int i = 1; i < 5;  ) {
    cout << i;
    i++;
}
*/
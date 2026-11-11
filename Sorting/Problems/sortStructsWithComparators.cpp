// Sorting structs with comparators, and stability
//
// A comparator must define a strict weak ordering:
//   - comp(a, a) is false
//   - if comp(a, b) is true, then comp(b, a) is false
//   - the relation is transitive
// Using <= or >= as the comparator breaks the first rule and can make
// std::sort read out of bounds.
//
// std::sort is not stable. std::stable_sort keeps equal elements in their
// original order, at the cost of O(n) extra memory.
//
// Example:
//   students = {Zara 90, Aman 90, Mia 85, Bob 85, Esha 70}
//   stable sort by score descending -> Zara Aman Mia Bob Esha
//   sort by score descending, then name ascending -> Aman Zara Bob Mia Esha

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Student {
    std::string name;
    int score;
};

// Equal scores compare as equivalent: neither is "less" than the other.
bool byScoreDescending(const Student &a, const Student &b) {
    return a.score > b.score;
}

// Break ties by name so the order is deterministic.
bool byScoreThenName(const Student &a, const Student &b) {
    if (a.score != b.score) {
        return a.score > b.score;
    }
    return a.name < b.name;
}

bool byName(const Student &a, const Student &b) {
    return a.name < b.name;
}

void print(const std::vector<Student> &students) {
    for (int i = 0; i < (int)students.size(); i++) {
        std::cout << students[i].name << " " << students[i].score << "  ";
    }
    std::cout << "\n";
}

int main() {
    std::vector<Student> students = {
        {"Zara", 90}, {"Aman", 90}, {"Mia", 85}, {"Bob", 85}, {"Esha", 70},
    };

    std::vector<Student> stable = students;
    std::stable_sort(stable.begin(), stable.end(), byScoreDescending);
    print(stable);  // Zara 90  Aman 90  Mia 85  Bob 85  Esha 70

    std::vector<Student> byScore = students;
    std::sort(byScore.begin(), byScore.end(), byScoreThenName);
    print(byScore);  // Aman 90  Zara 90  Bob 85  Mia 85  Esha 70

    std::vector<Student> alphabetical = students;
    std::sort(alphabetical.begin(), alphabetical.end(), byName);
    print(alphabetical);  // Aman 90  Bob 85  Esha 70  Mia 85  Zara 90

    return 0;
}

// Output:
// Zara 90  Aman 90  Mia 85  Bob 85  Esha 70
// Aman 90  Zara 90  Bob 85  Mia 85  Esha 70
// Aman 90  Bob 85  Esha 70  Mia 85  Zara 90
//
// Time complexity: O(n log n) for both sort and stable_sort.
// Space complexity:
//   std::sort        -> O(log n) recursion stack
//   std::stable_sort -> O(n) auxiliary storage
//
// Edge cases: all keys equal (a stable sort preserves the input order),
// duplicate names, an empty vector, and one element. Comparators should take
// const references to avoid copying each element.

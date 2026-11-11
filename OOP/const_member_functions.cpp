// Const member functions
//
// A const member function promises not to modify the object. It can be called
// on const objects and on const references. Inside it, `this` has type
// `const ClassName*`, so data members are read-only.
//
// Overloading on const: provide both a const and a non-const version of an
// accessor. The compiler picks the const one for const objects and the
// non-const one for mutable objects.
//
// `mutable` lets a specific member change inside a const function. Use it for
// caches and counters, not for real state.

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Playlist {
    string name;
    vector<string> songs;
    mutable int readCount;  // bookkeeping is allowed to change

   public:
    Playlist(string playlistName) {
        name = playlistName;
        readCount = 0;
    }

    void addSong(string song) {
        songs.push_back(song);
    }

    // const: does not modify the playlist.
    int size() const {
        return songs.size();
    }

    string getName() const {
        return name;
    }

    // Const overload: read-only access for const objects.
    const string &getSong(int index) const {
        readCount++;  // mutable member
        return songs[index];
    }

    // Non-const overload: allows modification through the returned reference.
    string &getSong(int index) {
        readCount++;
        return songs[index];
    }

    void print() const {
        cout << name << " (" << songs.size() << " songs, reads=" << readCount
             << ")" << endl;

        for (const string &song : songs) {
            cout << "  " << song << endl;
        }
    }
};

// A const reference parameter avoids a copy and forbids modification.
void printLength(const string &text) {
    cout << text.size() << endl;
    // text += "!";   // error: text is const
}

int main() {
    Playlist playlist("Road Trip");
    playlist.addSong("Highway Star");
    playlist.addSong("Born to Run");

    playlist.getSong(0) = "Highway Star (Live)";  // non-const overload

    const Playlist &view = playlist;
    cout << view.size() << endl;      // 2
    cout << view.getName() << endl;   // Road Trip
    cout << view.getSong(0) << endl;  // Highway Star (Live)
    // view.addSong("X");                // error: addSong is not const

    playlist.print();
    // Road Trip (2 songs, reads=2)

    printLength("const reference");

    return 0;
}

// Special cases:
//   - a const object can only call const member functions
//   - `const` is part of the function signature for overloading, but it does
//     not change the mangled name of the return type
//   - returning a non-const reference from a const function would break
//     const-correctness, so return `const T&` or a copy
//   - `mutable` members can change in const functions and are not part of the
//     logical const state
//   - mark every member function that does not modify the object as const;
//     this makes the class usable with const references and containers

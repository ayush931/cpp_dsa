/**
 * Chess
 *
 * A validator for the moves listed below on an 8x8 board, plus check,
 * checkmate, and stalemate detection.
 *
 * Board representation:
 *   row 0 is rank 8 (black's back rank), row 7 is rank 1 (white's back rank)
 *   column 0 is the a-file, column 7 is the h-file
 *   uppercase is white (K Q R B N P), lowercase is black (k q r b n p)
 *   '.' is an empty square
 *
 * Move pipeline:
 *   1. parse the squares
 *   2. check the piece belongs to the side to move
 *   3. check the piece's movement pattern (pseudo-legal)
 *   4. make the move on a copy and verify the mover's king is not in check
 *   5. commit and switch the turn
 *
 * The king-safety check is what separates pseudo-legal from legal moves. It
 * rejects moves that expose the king, moves that ignore a check, and king moves
 * onto an attacked square.
 *
 * Implemented:
 *   - pawn single and double pushes, diagonal captures
 *   - knight, bishop, rook, queen, king movement
 *   - path blocking for sliding pieces
 *   - capture of enemy pieces
 *   - check, checkmate, stalemate
 *   - pawn promotion to a queen
 *
 * Not implemented (documented extensions):
 *   - castling, en passant, underpromotion
 *   - threefold repetition and the fifty-move rule
 *   - FEN/PGN parsing, clocks, and move history
 *
 * Attack detection is separate from movement because a pawn attacks
 * diagonally but moves straight ahead. Reusing the movement rule for attacks
 * would miss pawn checks and would wrongly let a pawn "attack" the square in
 * front of it.
 */

#include <array>
#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

using Board = array<array<char, 8>, 8>;

struct Square {
    int row;
    int column;
};

char upperChar(char value) {
    if (value >= 'a' && value <= 'z') {
        return value - 'a' + 'A';
    }
    return value;
}

bool isWhitePiece(char piece) {
    return piece >= 'A' && piece <= 'Z';
}

bool isBlackPiece(char piece) {
    return piece >= 'a' && piece <= 'z';
}

bool sameColor(char first, char second) {
    if (first == '.' || second == '.') {
        return false;
    }

    return isWhitePiece(first) == isWhitePiece(second);
}

class ChessGame {
    Board board;
    bool whiteToMove = true;

    static Square parse(const string &square) {
        if (square.size() != 2) {
            return {-1, -1};
        }

        int column = square[0] - 'a';
        int row = '8' - square[1];

        if (column < 0 || column > 7 || row < 0 || row > 7) {
            return {-1, -1};
        }

        return {row, column};
    }

    bool inside(int row, int column) const {
        return row >= 0 && row < 8 && column >= 0 && column < 8;
    }

    bool pathClear(int fromRow, int fromColumn, int toRow, int toColumn) const {
        int rowStep = (toRow > fromRow) - (toRow < fromRow);
        int columnStep = (toColumn > fromColumn) - (toColumn < fromColumn);

        int row = fromRow + rowStep;
        int column = fromColumn + columnStep;

        while (row != toRow || column != toColumn) {
            if (board[row][column] != '.') {
                return false;
            }

            row += rowStep;
            column += columnStep;
        }

        return true;
    }

    // Does `piece` on (fromRow, fromColumn) attack (toRow, toColumn)? This
    // ignores whose turn it is and ignores king safety.
    bool attacks(int fromRow, int fromColumn, int toRow, int toColumn,
                 char piece) const {
        if (piece == '.') {
            return false;
        }

        int rowDelta = toRow - fromRow;
        int columnDelta = toColumn - fromColumn;
        char type = upperChar(piece);

        if (type == 'P') {
            int direction = isWhitePiece(piece) ? -1 : 1;
            return rowDelta == direction && abs(columnDelta) == 1;
        }

        if (type == 'N') {
            int rowStep = abs(rowDelta);
            int columnStep = abs(columnDelta);

            return (rowStep == 2 && columnStep == 1) ||
                   (rowStep == 1 && columnStep == 2);
        }

        if (type == 'B') {
            return abs(rowDelta) == abs(columnDelta) && rowDelta != 0 &&
                   pathClear(fromRow, fromColumn, toRow, toColumn);
        }

        if (type == 'R') {
            bool straight = rowDelta == 0 || columnDelta == 0;

            return straight && (rowDelta != 0 || columnDelta != 0) &&
                   pathClear(fromRow, fromColumn, toRow, toColumn);
        }

        if (type == 'Q') {
            bool diagonal = abs(rowDelta) == abs(columnDelta) && rowDelta != 0;
            bool straight = rowDelta == 0 || columnDelta == 0;

            return (diagonal ||
                    (straight && (rowDelta != 0 || columnDelta != 0))) &&
                   pathClear(fromRow, fromColumn, toRow, toColumn);
        }

        if (type == 'K') {
            return max(abs(rowDelta), abs(columnDelta)) == 1;
        }

        return false;
    }

    bool pseudoLegal(int fromRow, int fromColumn, int toRow, int toColumn,
                     bool white) const {
        if (!inside(fromRow, fromColumn) || !inside(toRow, toColumn)) {
            return false;
        }

        char piece = board[fromRow][fromColumn];
        char target = board[toRow][toColumn];

        if (piece == '.') {
            return false;
        }
        if (isWhitePiece(piece) != white) {
            return false;
        }
        if (sameColor(piece, target)) {
            return false;
        }

        int rowDelta = toRow - fromRow;
        int columnDelta = toColumn - fromColumn;
        char type = upperChar(piece);

        if (type == 'P') {
            int direction = white ? -1 : 1;
            int startRow = white ? 6 : 1;

            if (columnDelta == 0 && rowDelta == direction && target == '.') {
                return true;
            }

            if (columnDelta == 0 && rowDelta == 2 * direction &&
                fromRow == startRow && target == '.' &&
                board[fromRow + direction][fromColumn] == '.') {
                return true;
            }

            if (abs(columnDelta) == 1 && rowDelta == direction &&
                target != '.' && !sameColor(piece, target)) {
                return true;
            }

            return false;
        }

        if (type == 'N' || type == 'K') {
            return attacks(fromRow, fromColumn, toRow, toColumn, piece);
        }

        // Bishops, rooks, and queens must also have a clear path. The attacks
        // function already checks it, and it rejects zero-length moves.
        return attacks(fromRow, fromColumn, toRow, toColumn, piece);
    }

    // Applies the move to the real board. Assumes the move is pseudo-legal.
    void applyMove(int fromRow, int fromColumn, int toRow, int toColumn) {
        char piece = board[fromRow][fromColumn];

        board[toRow][toColumn] = piece;
        board[fromRow][fromColumn] = '.';

        if (upperChar(piece) == 'P' && (toRow == 0 || toRow == 7)) {
            board[toRow][toColumn] = isWhitePiece(piece) ? 'Q' : 'q';
        }
    }

    // A move is legal when it is pseudo-legal and leaves the mover's king safe.
    bool legalMove(int fromRow, int fromColumn, int toRow, int toColumn,
                   bool white) {
        if (!pseudoLegal(fromRow, fromColumn, toRow, toColumn, white)) {
            return false;
        }

        Board backup = board;
        applyMove(fromRow, fromColumn, toRow, toColumn);

        bool safe = !inCheck(white);
        board = backup;

        return safe;
    }

   public:
    ChessGame() {
        for (array<char, 8> &row : board) {
            row.fill('.');
        }

        string backRank = "RNBQKBNR";
        string pawns = "PPPPPPPP";

        for (int column = 0; column < 8; column++) {
            board[0][column] = static_cast<char>(backRank[column] - 'A' + 'a');
            board[1][column] = static_cast<char>(pawns[column] - 'A' + 'a');
            board[6][column] = pawns[column];
            board[7][column] = backRank[column];
        }
    }

    // Test helper: replace the board and set the side to move.
    void setup(const vector<pair<string, char>> &pieces, bool whiteTurn) {
        for (array<char, 8> &row : board) {
            row.fill('.');
        }

        for (const auto &[square, piece] : pieces) {
            Square position = parse(square);

            if (position.row >= 0) {
                board[position.row][position.column] = piece;
            }
        }

        whiteToMove = whiteTurn;
    }

    bool inCheck(bool white) const {
        int kingRow = -1;
        int kingColumn = -1;
        char king = white ? 'K' : 'k';

        for (int row = 0; row < 8; row++) {
            for (int column = 0; column < 8; column++) {
                if (board[row][column] == king) {
                    kingRow = row;
                    kingColumn = column;
                }
            }
        }

        if (kingRow == -1) {
            return false;
        }

        for (int row = 0; row < 8; row++) {
            for (int column = 0; column < 8; column++) {
                char piece = board[row][column];

                if (piece == '.') {
                    continue;
                }
                if (isWhitePiece(piece) == white) {
                    continue;
                }

                if (attacks(row, column, kingRow, kingColumn, piece)) {
                    return true;
                }
            }
        }

        return false;
    }

    bool hasAnyLegalMove(bool white) {
        for (int fromRow = 0; fromRow < 8; fromRow++) {
            for (int fromColumn = 0; fromColumn < 8; fromColumn++) {
                char piece = board[fromRow][fromColumn];

                if (piece == '.') {
                    continue;
                }
                if (isWhitePiece(piece) != white) {
                    continue;
                }

                for (int toRow = 0; toRow < 8; toRow++) {
                    for (int toColumn = 0; toColumn < 8; toColumn++) {
                        if (legalMove(fromRow, fromColumn, toRow, toColumn,
                                      white)) {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    bool isCheckmate(bool white) {
        if (white != whiteToMove) {
            return false;
        }

        return inCheck(white) && !hasAnyLegalMove(white);
    }

    bool isStalemate(bool white) {
        if (white != whiteToMove) {
            return false;
        }

        return !inCheck(white) && !hasAnyLegalMove(white);
    }

    // Returns true when the move was legal and committed.
    bool move(const string &from, const string &to) {
        Square start = parse(from);
        Square end = parse(to);

        if (start.row < 0 || end.row < 0) {
            return false;
        }

        if (!legalMove(start.row, start.column, end.row, end.column,
                       whiteToMove)) {
            return false;
        }

        applyMove(start.row, start.column, end.row, end.column);
        whiteToMove = !whiteToMove;

        return true;
    }

    char at(const string &square) const {
        Square position = parse(square);
        if (position.row < 0) {
            return '?';
        }

        return board[position.row][position.column];
    }

    bool turnIsWhite() const {
        return whiteToMove;
    }

    void print() const {
        cout << "  a b c d e f g h" << endl;

        for (int row = 0; row < 8; row++) {
            cout << 8 - row << " ";

            for (int column = 0; column < 8; column++) {
                cout << board[row][column] << " ";
            }

            cout << 8 - row << endl;
        }

        cout << "  a b c d e f g h" << endl;
        cout << (whiteToMove ? "white" : "black") << " to move" << endl;
    }
};

int main() {
    ChessGame game;

    game.print();

    // A pawn cannot jump three squares.
    cout << game.move("e2", "e5") << endl;  // 0
    cout << game.move("e2", "e4") << endl;  // 1
    cout << game.move("e7", "e5") << endl;  // 1

    // Knights can jump over pieces.
    cout << game.move("g1", "f3") << endl;  // 1
    cout << game.move("b8", "c6") << endl;  // 1

    // Bishops need a clear diagonal.
    cout << game.move("f1", "b5") << endl;  // 1
    cout << game.move("g8", "f6") << endl;  // 1

    // A capture: the knight takes the pawn on e5.
    cout << game.move("f3", "e5") << endl;  // 1
    cout << game.move("c6", "e5") << endl;  // 1 (black recaptures)

    // White's d-pawn is free to advance.
    cout << game.move("d2", "d4") << endl;  // 1
    // The d7 pawn is pinned by the bishop on b5: moving it exposes the king.
    cout << game.move("d7", "d6") << endl;  // 0 (pinned)
    // It is still black's turn, so a white move is rejected.
    cout << game.move("d2", "d4") << endl;  // 0 (wrong side)

    cout << game.inCheck(true) << " " << game.inCheck(false) << endl;  // 0 0
    cout << game.isCheckmate(true) << " " << game.isCheckmate(false) << endl;
    // 0 0

    game.print();

    // A pinned piece cannot step off the line between its king and the
    // attacker. White rook e2 is pinned by the black rook e8; moving it
    // sideways exposes the white king on e1. Moving along the e-file is still
    // legal.
    ChessGame pin;
    pin.setup({{"e1", 'K'}, {"e2", 'R'}, {"e8", 'r'}, {"h8", 'k'}}, true);

    cout << pin.move("e2", "d2") << endl;  // 0 (leaves the king in check)
    cout << pin.move("e2", "e3") << endl;  // 1 (stays on the pin line)
    cout << pin.at("e2") << " " << pin.at("e3") << endl;  // . R

    // Fool's mate: 1. f3 e5 2. g4 Qh4#.
    ChessGame fool;
    cout << fool.move("f2", "f3") << endl;  // 1
    cout << fool.move("e7", "e5") << endl;  // 1
    cout << fool.move("g2", "g4") << endl;  // 1
    cout << fool.move("d8", "h4") << endl;  // 1

    cout << fool.inCheck(true) << " " << fool.isCheckmate(true) << endl;
    // 1 1
    cout << fool.hasAnyLegalMove(true) << endl;  // 0

    fool.print();

    // A classic stalemate: black king a8, white king c6, white queen b6, black
    // to move. The king is not in check but has no legal square.
    ChessGame stale;
    stale.setup({{"a8", 'k'}, {"c6", 'K'}, {"b6", 'Q'}}, false);

    cout << stale.inCheck(false) << " " << stale.isStalemate(false) << endl;
    // 0 1
    cout << stale.hasAnyLegalMove(false) << endl;  // 0
    cout << stale.isCheckmate(false) << endl;      // 0

    // The same position with white to move is not stalemate for black; it is
    // simply white's turn and black is not in check.
    stale.setup({{"a8", 'k'}, {"c6", 'K'}, {"b6", 'Q'}}, true);
    cout << stale.isStalemate(false) << endl;  // 0

    // Promotion: a white pawn on a7 promotes to a queen on a8.
    ChessGame promotion;
    promotion.setup({{"a7", 'P'}, {"e1", 'K'}, {"e8", 'k'}}, true);
    cout << promotion.move("a7", "a8") << endl;  // 1
    cout << promotion.at("a8") << endl;          // Q

    // Invalid square strings are rejected.
    cout << promotion.move("a9", "a8") << endl;  // 0
    cout << promotion.at("z1") << endl;          // ?

    return 0;
}

/**
 * Time complexity:
 *   move -> O(64) for the path scan plus O(64) for the check scan, so O(64)
 *   hasAnyLegalMove -> O(64 * 64 * 64) worst case: for every piece and target,
 *   copy the board and scan for attacks. On a fixed 8x8 board this is constant.
 *   inCheck -> O(64)
 * Space complexity -> O(64) for the board, plus one board copy per legality
 * check
 *
 * Special cases:
 *   - an empty square cannot move
 *   - a move by the side not to move is rejected
 *   - capturing your own piece is rejected
 *   - a sliding piece cannot jump over a piece
 *   - a knight can jump
 *   - a pawn's double push requires both squares to be empty and the pawn to
 *     be on its starting rank
 *   - a pawn captures diagonally, not straight ahead
 *   - a move that leaves the king in check is rejected
 *   - a pinned piece can still move along the pin line
 *   - checkmate: the side to move is in check and has no legal move
 *   - stalemate: the side to move is not in check and has no legal move
 *   - checkmate and stalemate are false for the side that is not to move
 *   - a missing king is treated as not in check (useful for test positions)
 *   - pawn promotion always becomes a queen
 *   - castling, en passant, and underpromotion are not supported
 *
 * Why legality needs a simulation: pseudo-legal rules only describe how a
 * piece moves. They do not know that the king would be exposed. Copying the
 * board, making the move, and testing `inCheck` is the simplest correct
 * approach. A production engine uses make/unmake with incremental attack maps
 * and pin detection, but the copy-based version is fast enough on 64 squares.
 *
 * Checkmate is not "the king is attacked". It is "the king is attacked and
 * every response fails": moving the king, capturing the attacker, or blocking
 * the line. `hasAnyLegalMove` covers all three because it tries every move.
 */

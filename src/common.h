#ifndef _COMMON_
#define _COMMON_

#include <cstdint>
#include <cassert>

namespace mockfish {

typedef uint64_t Bitboard;

enum CastlingRights {
    NO_CASTLING,
    WHITE_OO,
    WHITE_OOO = WHITE_OO << 1,
    BLACK_OO = WHITE_OO << 2,
    BLACK_OOO = WHITE_OO << 3,

    KINGSIDE = WHITE_OO | BLACK_OO,
    QUEENSIDE = WHITE_OOO | BLACK_OOO,
    WHITE_CASTLING = WHITE_OO | WHITE_OOO,
    BLACK_CASTLING = BLACK_OO | BLACK_OOO,

    CASTLING_RIGHTS_NB = 16
};

enum PieceType {
    NO_PIECE_TYPE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
    ALL_PIECES = 0,
    PIECE_TYPE_NB = 8
};

enum Piece {
    NO_PIECE, 
    W_PAWN = PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN = PAWN + 8, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
    PIECE_NB = 16
};

enum Color {
    WHITE, BLACK, 
    COLOR_NB
};

enum Rank {
    RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8,
    RANK_NB
};

enum File {
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, 
    FILE_NB
};

enum Square : int {
    A1, B1, C1, D1, E1, F1, G1, H1, 
    A2, B2, C2, D2, E2, F2, G2, H2, 
    A3, B3, C3, D3, E3, F3, G3, H3, 
    A4, B4, C4, D4, E4, F4, G4, H4, 
    A5, B5, C5, D5, E5, F5, G5, H5, 
    A6, B6, C6, D6, E6, F6, G6, H6, 
    A7, B7, C7, D7, E7, F7, G7, H7, 
    A8, B8, C8, D8, E8, F8, G8, H8, 
    NO_SQUARE, 
    SQUARE_NB = 64
};

enum Direction : int {
    NORTH = 8,
    EAST = 1,
    SOUTH = -NORTH,
    WEST = -EAST,

    NORTHEAST = NORTH + EAST,
    SOUTHEAST = SOUTH + EAST,
    NORTHWEST = NORTH + WEST,
    SOUTHWEST = SOUTH + WEST
};

#define ENABLE_BASE_OPERATORS_ON(T)                                \
constexpr T operator+(T d1, int d2) { return T(int(d1) + d2); }    \
constexpr T operator-(T d1, int d2) { return T(int(d1) - d2); }    \
constexpr T operator-(T d) { return T(-int(d)); }                  \
inline T& operator+=(T& d1, int d2) { return d1 = d1 + d2; }       \
inline T& operator-=(T& d1, int d2) { return d1 = d1 - d2; }

#define ENABLE_INCR_OPERATORS_ON(T)                      \
inline T& operator--(T& t) { return t = T(int(t) - 1); } \
inline T& operator++(T& t) { return t = T(int(t) + 1); }

#define ENABLE_FULL_OPERATORS_ON(T)                                \
ENABLE_BASE_OPERATORS_ON(T)                                        \
constexpr T operator*(int i, T d) { return T(i * int(d)); }        \
constexpr T operator*(T d, int i) { return T(int(d) * i); }        \
constexpr T operator/(T d, int i) { return T(int(d) / i); }        \
constexpr int operator/(T d1, T d2) { return int(d1) / int(d2); }  \
inline T& operator*=(T& d, int i) { return d = T(int(d) * i); }    \
inline T& operator/=(T& d, int i) { return d = T(int(d) / i); }

ENABLE_INCR_OPERATORS_ON(Rank)
ENABLE_INCR_OPERATORS_ON(File)
ENABLE_INCR_OPERATORS_ON(Square)
ENABLE_INCR_OPERATORS_ON(Piece)
ENABLE_INCR_OPERATORS_ON(PieceType)

ENABLE_FULL_OPERATORS_ON(Direction)

#undef ENABLE_FULL_OPERATORS_ON
#undef ENABLE_INCR_OPERATORS_ON
#undef ENABLE_BASE_OPERATORS_ON

constexpr Square operator+(Square s, Direction d) { return Square(int(s) + int(d)); }
constexpr Square operator-(Square s, Direction d) { return Square(int(s) - int(d)); }
inline Square& operator+=(Square& s, Direction d) { return s = s + d; }
inline Square& operator-=(Square& s, Direction d) { return s = s - d; }

constexpr PieceType type_of(Piece p) {
    return PieceType(p & 7);
}

constexpr Color color_of(Piece p) {
    assert(p != NO_PIECE);
    return Color(p >> 3);
}

constexpr File file_of(Square s) {
    return File(s & 7);
}

constexpr Rank rank_of(Square s) {
    return Rank(s >> 3);
}

constexpr Square make_square(File file, Rank rank) {
    return Square((rank << 3) + file);
}

}

#endif
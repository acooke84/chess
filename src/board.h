#ifndef _BOARD_
#define _BOARD_

#include <string>

#include "common.h"

namespace mockfish {

class Board {
public:
    Board() = default;
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    Board& build(const std::string& fen);

    void put_piece(Piece p, Square s);
    Piece remove_piece(Square s);
    Piece move_piece(Square start, Square target);

    Bitboard pieces(PieceType type) const;
    Bitboard pieces(PieceType type1, PieceType type2) const;
    Bitboard pieces(Color color) const;
    Bitboard pieces(PieceType type, Color color) const;
    Bitboard pieces(PieceType type1, PieceType type2, Color c) const;
    Piece piece_on(Square s) const;
    bool empty(Square s) const;
    Color turn_color() const;
    Square en_passant_square() const;
    bool can_castle(CastlingRights rights) const;
    int halfmoves() const;
    int fullmoves() const;

    std::string get_fen() const;
    std::string string_representation() const;

private:
    Bitboard byTypeBB[PIECE_TYPE_NB];
    Bitboard byColorBB[COLOR_NB];

    Piece board[SQUARE_NB];

    Color turnColor;
    Square enPassantSquare;
    int castlingRights;
};

inline void Board::put_piece(Piece p, Square s) {
    board[s] = p;
    byTypeBB[ALL_PIECES] |= byTypeBB[type_of(p)] |= s;
    byColorBB[color_of(p)] |= s;
}

inline Piece Board::remove_piece(Square s) {
    Piece p = board[s];
    byTypeBB[ALL_PIECES] ^= s;
    byTypeBB[type_of(p)] ^= s;
    board[s] = NO_PIECE;
    return p;
}

inline Piece Board::move_piece(Square start, Square target) {
    Piece p = board[start];
    Bitboard fromTo = start | target;
    byTypeBB[ALL_PIECES] ^= fromTo;
    byTypeBB[type_of(p)] ^= fromTo;
    byColorBB[color_of(p)] ^= fromTo;
    board[start] = NO_PIECE;
    board[target] = p;
    return p;
}

inline Bitboard Board::pieces(PieceType type) const {
    return byTypeBB[type];
}

inline Bitboard Board::pieces(PieceType type1, PieceType type2) const {
    return byTypeBB[type1] | byTypeBB[type2];
}

inline Bitboard Board::pieces(Color color) const {
    return byColorBB[color];
}

inline Bitboard Board::pieces(PieceType type, Color color) const {
    return byColorBB[color] & byTypeBB[type];
}

inline Bitboard Board::pieces(PieceType type1, PieceType type2, Color color) const {
    return byColorBB[color] & (byTypeBB[type1] | byTypeBB[type2]);
}

inline Piece Board::piece_on(Square s) const {
    return board[s];
}

inline bool Board::empty(Square s) const {
    return (board[s] == NO_PIECE);
}

inline Color Board::turn_color() const {
    return turnColor;
}

inline Square Board::en_passant_square() const {
    return NO_SQUARE;
}

inline bool Board::can_castle(CastlingRights rights) const {
    return castlingRights & rights;
}

inline int Board::halfmoves() const {
    return 0;
}

inline int Board::fullmoves() const {
    return 0;
}

}

#endif
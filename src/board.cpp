
#include <sstream>

#include "board.h"
#include "uci.h"

using std::string;

namespace mockfish {

namespace {
const string PieceToString = (" PNBRQK  pnbrqk");
const string ASCII_WHITE = "\u001b[37m";
const string ASCII_BLACK = "\u001b[30m";
const string ASCII_RESET = "\u001b[0m";

constexpr Piece Pieces[] = { W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
                             B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING };
}

Board& Board::build(const string& fen) {
    unsigned char token, hm, fm;
    size_t idx;
    Square sq = A8;

    std::istringstream fenStream(fen);

    std::memset(this, 0, sizeof(Board));

    fenStream >> std::noskipws;

    while ((fenStream >> token) && !isspace(token)) {
        if (isdigit(token)) {
            for (int i = 0; i < token - '0'; i++) {
                board[sq] = NO_PIECE;
                sq += EAST;
            }
        } else if (token == '/') {
            sq += 2 * SOUTH;
        } else if ((idx = PieceToString.find(token)) != string::npos) {
            put_piece(Piece(idx), sq);
            ++sq;
        }
    }

    fenStream >> token;
    turnColor = (token == 'w' ? WHITE : BLACK);
    fenStream >> token;

    castlingRights = 0;
    while ((fenStream >> token) && !isspace(token)) {
        if (token == 'K') {
            castlingRights |= WHITE_OO;
        } else if (token == 'Q') {
            castlingRights |= WHITE_OOO;
        } else if (token == 'k') {
            castlingRights |= BLACK_OO;
        } else if (token == 'q') {
            castlingRights |= BLACK_OOO;
        }
    }

    fenStream >> token;
    // handle en passant sq
    enPassantSquare = NO_SQUARE;

    fenStream >> std::skipws >> hm >> fm;
    // handle halfmoves and fullmoves

    return *this;
}

string Board::get_fen() const {
    std::ostringstream fen;

    for (Rank rank = RANK_8; rank >= RANK_1; --rank) {
        int cntr = 0;
        for (File file = FILE_A; file <= FILE_H; ++file) {
            Piece p = piece_on(make_square(file, rank));
            if (p == NO_PIECE) {
                ++cntr;
            } else {
                if (cntr > 0)
                    fen << cntr;
                fen << PieceToString[p];
                cntr = 0;
            }
        }
        if (cntr > 0)
            fen << cntr;
        if (rank != RANK_1) 
            fen << "/";
    }

    fen << ((turn_color() == WHITE) ? " w " : " b ");

    if (can_castle(WHITE_OO))
        fen << "K";
    if (can_castle(WHITE_OOO))
        fen << "Q";
    if (can_castle(BLACK_OO))
        fen << "k";
    if (can_castle(BLACK_OOO))
        fen << "q";
    
    fen << " ";

    fen << ((en_passant_square() == NO_SQUARE) ? "- " : uci::square(en_passant_square()) + " ");

    fen << halfmoves();

    fen << " ";

    fen << fullmoves();

    return fen.str();
}

string Board::string_representation() const {
    std::ostringstream str;

    str << " +---+---+---+---+---+---+---+---+ \n";

    for (Rank rank = RANK_8; rank >= RANK_1; --rank) {
        str << " | ";
        for (File file = FILE_A; file <= FILE_H; ++file) {
            Square s = make_square(file, rank);
            if (empty(s)) {
                str << " ";
            } else {
                Piece p = piece_on(s);
                Color c = color_of(p);
                if (c == WHITE) {
                    str << ASCII_WHITE;
                } else {
                    str << ASCII_BLACK;
                }
                str << PieceToString[p];
                str << ASCII_RESET;
            }
            str << " | ";
        }
        str << (rank + 1);
        str << "\n +---+---+---+---+---+---+---+---+ \n";
    }

    str << " | A | B | C | D | E | F | G | H | \n";

    return str.str();
}

}
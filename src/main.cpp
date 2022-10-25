#include <iostream>

#include "board.h"
#include "uci.h"

using namespace mockfish;

const char* StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main(int argc, char** argv) {

    Board board;
    board.build(StartFEN);
    std::cout << board.string_representation();
    std::cout << board.get_fen();
    
    board.move_piece(E2, E4);
    std::cout << board.string_representation();
    std::cout << board.get_fen();
    std::cout << std::endl;
}
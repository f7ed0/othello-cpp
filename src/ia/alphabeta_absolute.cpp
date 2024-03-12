#include "ia/alphabeta_absolute.hpp"

using namespace IA;

AlphaBeta_Absolute::AlphaBeta_Absolute(int __depth): AlphaBeta(__depth) {}

int AlphaBeta_Absolute::heuristics(const othello::Board& current_board, const othello::pawn team, const othello::pawn player) {
        int result = 0;
    for(int i=0 ; i < othello::Board::length ; i++) {
        if(current_board.getCase(i) == team) {
            result ++;
        } else {
            result --;
        }
    }
    return result;
}
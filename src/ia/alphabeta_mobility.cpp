#include "ia/alphabeta_mobility.hpp"

using namespace IA;

AlphaBeta_Mobility::AlphaBeta_Mobility(int __depth): AlphaBeta(__depth) {}

int AlphaBeta_Mobility::heuristics(const othello::Board& current_board, const othello::pawn team, const othello::pawn player) {
    
    std::vector plays = current_board.listAllPlay(player);
    std::vector plays = current_board.listAllPlay(player);
    if(team == player) {
        return plays.size();
    } else {
        int arr[3];
        current_board.getPawnNumbers(arr);
        return arr[othello::pawn::empty]-plays.size();
    }
}
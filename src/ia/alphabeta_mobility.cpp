#include "ia/alphabeta_mobility.hpp"

using namespace IA;

AlphaBeta_Mobility::AlphaBeta_Mobility(int __depth): AlphaBeta(__depth) {}

int AlphaBeta_Mobility::heuristics(const othello::Board& current_board, const othello::pawn team ) {
    
    std::vector plays = current_board.listAllPlay(team);

}
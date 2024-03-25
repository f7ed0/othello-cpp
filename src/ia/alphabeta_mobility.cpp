#include "ia/alphabeta_mobility.hpp"

using namespace IA;

AlphaBeta_Mobility::AlphaBeta_Mobility(int __depth): AlphaBeta(__depth) {}

int AlphaBeta_Mobility::heuristics(const othello::Board& current_board, const othello::pawn team ) {

    int diag_score = 0;
    if(current_board.getCase("a1") == team) {
        diag_score += 10;
    }
    if(current_board.getCase("a8") == team) {
        diag_score += 10;
    }
    if(current_board.getCase("h1") == team) {
        diag_score += 10;
    }
    if(current_board.getCase("h8") == team) {
        diag_score += 10;
    }
    
    std::vector plays = current_board.listAllPlay(team);
    std::vector nmi_plays = current_board.listAllPlay(othello::other_team(team));

    return plays.size() - nmi_plays.size();

}
#include "ia/alphabeta_mixte.hpp"

using namespace IA;

AlphaBeta_Mixte::AlphaBeta_Mixte(int __depth): AlphaBeta(__depth) {}

int AlphaBeta_Mixte::heuristics(const othello::Board& current_board, const othello::pawn team ) {
    int arr[3];
    current_board.getPawnNumbers(arr);
    if (arr[othello::pawn::empty] > 39) { // 64 - 25 = 39
        return this->positional(current_board,team);
    }
    if (arr[othello::pawn::empty] < 16) {
        return this->absolute(current_board,team);
    }
    return this->mobility(current_board,team);
}

int AlphaBeta_Mixte::positional(const othello::Board& current_board, const othello::pawn team ) {
    int result = 0;
    for(int i=0 ; i < othello::Board::length ; i++) {
        if(current_board.getCase(i) == team) {
            result += AlphaBeta::payoff_matrix[i];
        } else {
            result -= AlphaBeta::payoff_matrix[i];
        }
    }
    return result;
}

int AlphaBeta_Mixte::absolute(const othello::Board& current_board, const othello::pawn team ) {
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

int AlphaBeta_Mixte::mobility(const othello::Board& current_board, const othello::pawn team ) {
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

    return plays.size() - nmi_plays.size() + diag_score;
}
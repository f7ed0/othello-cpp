#include <random>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include "ia/minmax.hpp"


using namespace IA;


int MinMax::makeAMove(const othello::Board& current_board, othello::pawn team) {
    std::vector<int> choices = current_board.listAllPlay(team);
    return choices.at(random()%choices.size());
}

int MinMax::minmax(const othello::Board& current_board,othello::pawn player, const int depth, othello::pawn team) {
    std::vector<int> choices = current_board.listAllPlay(player);
    
    if (depth == 0 || choices.size() == 0 ) {

        return 0;
    } if (player == team){
        const int value = INT64_MIN;
        for (int i=0 ; i < choices.size() ; i++) {
            othello::Board* next_move = new othello::Board(current_board);
            if(next_move->placePawn(choices.at(i),player) == 0) {
                throw -1;
            }
            const int value = std::max(value, minmax(*next_move, player, depth-1, team));
            return value;
            
        }
    } else {
        const int value = INT64_MAX;
        for (int i=0 ; i < choices.size() ; i++) {
            othello::Board* next_move = new othello::Board(current_board);
            if(next_move->placePawn(choices.at(i),player) == 0) {
                throw -1;
            }
            const int value = std::min(value, minmax(*next_move, player, depth-1, team));
            return value;
        }
    }

    return 0;
}


void MinMax::resetAI() {
    return;
}
#include <random>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include "ia/minmax.hpp"


using namespace IA;


int MinMax::makeAMove(const othello::Board& current_board, othello::pawn team) {
    std::vector<int> choices = current_board.listAllPlay(team);
    int index = 0;
    int max = INT32_MIN;
    int val;
    for(int i = 0; i < (int) choices.size() ; i++) {
        othello::Board* next_move = new othello::Board(current_board);
        if(next_move->placePawn(choices.at(i),team) == 0) {
            throw -1;
        }
        val = this->minmax(*next_move, MinMax::switchTeam(team), 5, team);
        if(val > max) {
            index = i;
            max = val;
        }
    }
    return choices.at(index);
}
    

int MinMax::minmax(const othello::Board& current_board,othello::pawn player, const int depth, othello::pawn team) {
    std::vector<int> choices = current_board.listAllPlay(player);
    int pawn_place;

    if (depth == 0 || othello::Board(current_board).isFinished()) {
        return 0;
    } 
    if(choices.size() == 0) {
        player = MinMax::switchTeam(player);
        choices = current_board.listAllPlay(player);
    }
    if (player == team){
        const int value = INT32_MIN;
        for (int i=0 ; i < (int) choices.size() ; i++) {
            othello::Board* next_move = new othello::Board(current_board);
            pawn_place = next_move->placePawn(choices.at(i),player);
            if(pawn_place == 0) {
                throw -1;
            }
            const int value = std::max(value, this->minmax(*next_move, player, depth-1, team));          
        }
        return pawn_place + value;

    } else {
        const int value = INT32_MAX;
        for (int i=0 ; i < (int) choices.size() ; i++) {
            othello::Board* next_move = new othello::Board(current_board);
            pawn_place = next_move->placePawn(choices.at(i),player);
            if(pawn_place == 0) {
                throw -1;
            }
            const int value = std::min(value, this->minmax(*next_move, player, depth-1, team));
        }
        return pawn_place + value;
    }
}


void MinMax::resetAI() {
    return;
}
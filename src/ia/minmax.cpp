#include <random>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include <thread>
#include <atomic>
#include "ia/minmax.hpp"


using namespace IA;


int MinMax::makeAMove(const othello::Board& current_board, othello::pawn team) {
    std::vector<int> choices = current_board.listAllPlay(team);
    std::thread** threadArray = (std::thread**) malloc(choices.size()*sizeof(std::thread*));
    int index = 0;
    int max = INT32_MIN;
    int* val = (int*) calloc(choices.size(),sizeof(int));

    for(int i = 0; i < (int) choices.size() ; i++) {
        othello::Board* next_move = new othello::Board(current_board);
        if(next_move->placePawn(choices.at(i),team) == 0) {
            throw -1;
        }
        threadArray[i] =  new std::thread(MinMax::launchMinmax,this,*next_move,MinMax::switchTeam(team),this->depth-1,team,val+i);
        delete next_move;
    }
    for(int i = 0; i < (int) choices.size() ; i++) {
        if(threadArray[i]->joinable()) {
            threadArray[i]->join();
            delete threadArray[i];
        }
        if(val[i] > max) {
                index = i;
                max = val[i];
        }
    }
    free(threadArray);
    return choices.at(index);
    
}
    

int MinMax::minmax(const othello::Board& current_board,othello::pawn player, const int depth, othello::pawn team) {
    if (depth == 0) {
        return this->euristics(current_board,player);
    }
    std::vector<int> choices = current_board.listAllPlay(player);
    if(choices.size() == 0) {
        player = MinMax::switchTeam(player);
        choices = current_board.listAllPlay(player);

        if(choices.size() == 0) {
            int score[3] = {0,0,0};
            current_board.getPawnNumbers(score);
            if(score[player] > 32) {
                return INT32_MAX;
            } 
            if (score[player] < 32){
                return INT32_MIN;
            }
            return 0;
        }
    }

    int possibility = (int) choices.size();

    if (player == team){
        int value = INT32_MIN;
        for (int i=0 ; i < possibility ; i++) {
            othello::Board* next_move = new othello::Board(current_board);
            if(next_move->placePawn(choices.at(i),player) == 0) {
                throw -1;
            }
            value = std::max(value,this->minmax(*next_move,MinMax::switchTeam(player),depth-1,team));  
            delete next_move;     
        }
        return value;
    } else {
        int value = INT32_MAX;
        for (int i=0 ; i < possibility ; i++) {
            othello::Board* next_move = new othello::Board(current_board);
            if(next_move->placePawn(choices.at(i),player) == 0) {
                throw -1;
            }
            value = std::min(value,this->minmax(*next_move,MinMax::switchTeam(player),depth-1,team));
            delete next_move;
        }
        return value;
    }
}


void MinMax::resetAI() {
    return;
}

int MinMax::euristics(const othello::Board& current_board, const othello::pawn team) {
    int result = 0;
    for(int i=0;i < othello::Board::length ; i++) {
        if(current_board.getCase(i) == team) {
            result += MinMax::payoff_matrix[i];
        } else {
            result -= MinMax::payoff_matrix[i];
        }
    }
    return result;
}

MinMax::MinMax(int __depth): IAInterface() {
    this->depth = __depth;
}

void MinMax::launchMinmax(MinMax* IA,const othello::Board& board,othello::pawn player,int depth, othello::pawn team, int* result ) {
    *result = IA->minmax(board, player, depth, team);
}

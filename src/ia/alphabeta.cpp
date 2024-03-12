#include <random>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include <thread>
#include "ia/alphabeta.hpp"


using namespace IA;


int AlphaBeta::makeAMove(const othello::Board& current_board, othello::pawn team) {
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
        threadArray[i] =  new std::thread(AlphaBeta::launchAlphabeta,this,*next_move,AlphaBeta::switchTeam(team),this->depth-1,team,val+i);
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
    

int AlphaBeta::alphabeta(const othello::Board& current_board,othello::pawn player, const int depth, othello::pawn team, int _alpha, int _beta) {
    if (depth == 0) {
        return this->heuristics(current_board,team,player);
    }
    std::vector<int> choices = current_board.listAllPlay(player);
    if(choices.size() == 0) {
        player = AlphaBeta::switchTeam(player);
        choices = current_board.listAllPlay(player);

        if(choices.size() == 0) {
            int score[3] = {0,0,0};
            current_board.getPawnNumbers(score);
            if(score[team] > 32) {
                return 32000 + this->heuristics(current_board,team,player);
            } 
            if (score[team] < 32){
                return -32000 + this->heuristics(current_board,team,player);
            }
            return 0;
        }
    }

    int possibility = (int) choices.size();

    int value;
    if (player == team){
        value = INT32_MIN;
        for (int i=0 ; i < possibility ; i++) {
            othello::Board* next_move = new othello::Board(current_board);
            if(next_move->placePawn(choices.at(i),player) == 0) {
                throw -1;
            }
            value = std::max(value,this->alphabeta(*next_move,AlphaBeta::switchTeam(player),depth-1,team,_alpha,_beta));
            delete next_move;
            if(value >= _beta) { // Coupure betamake
            
                return value;
            }
            _alpha = std::max(_alpha,value);
                 
        }
    } else {
        value = INT32_MAX;
        for (int i=0 ; i < possibility ; i++) {
            othello::Board* next_move = new othello::Board(current_board);
            if(next_move->placePawn(choices.at(i),player) == 0) {
                throw -1;
            }
            value = std::min(value,this->alphabeta(*next_move,AlphaBeta::switchTeam(player),depth-1,team,_alpha,_beta));
            delete next_move;
            if(_alpha >= value) { // Coupure alpha
                return value;
            }
            _beta = std::min(_beta,value);
            
        }
    }
    return value;
}


void AlphaBeta::resetAI() {
    return;
}

int AlphaBeta::heuristics(const othello::Board& current_board, const othello::pawn team, const othello::pawn player) {
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

AlphaBeta::AlphaBeta(int __depth): IAInterface() {
    this->depth = __depth;
}

void AlphaBeta::launchAlphabeta(AlphaBeta* IA,const othello::Board& board,othello::pawn player,int depth, othello::pawn team, int* result ) {
    *result = IA->alphabeta(board, player, depth, team, INT32_MIN, INT32_MAX);
}
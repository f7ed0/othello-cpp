#include <random>
#include <vector>
#include <unistd.h>
#include "ia/random.hpp"


using namespace IA;


int Random::makeAMove(const othello::Board& current_board, othello::pawn team) {
    std::vector<int> choices = current_board.listAllPlay(team);
    return choices.at(random()%choices.size());
}

void Random::resetAI() {
    return;
}
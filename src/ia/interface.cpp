#include "ia/interface.hpp"
#include "ia/random.hpp"
#include "ia/minmax.hpp"
#include "ia/alphabeta.hpp"
#include "errors/errors.hpp"

using namespace IA;

IAInterface* IAInterface::selectByName(std::string name) {
    if(name == "random") {
        return (new Random());
    } else if(name == "minmax3"){
        return (new MinMax(3));
    } else if(name == "minmax5"){
        return (new MinMax(5));
    } else if(name == "minmax7"){
        return (new MinMax(7));
    } else if(name == "minmax10"){
        return (new MinMax(10));
    } else if(name == "alphabeta5"){
        return (new AlphaBeta(5));
    } else if(name == "alphabeta7"){
        return (new AlphaBeta(7));
    } else if(name == "alphabeta10"){
        return (new AlphaBeta(10));
    } else {
        std::cout << "No IA named " << name << "." << std::endl;
        throw errors::OutOfBoundError();
    }
}

const int IAInterface::payoff_matrix[64] = {
    500,    -150,   30,     10,     10,     30,     -150,   500,
    -150,   -250,   0,      0,      0,      0,      -250,   -150,
    30,     0,      1,      2,      2,      1,      0,      30,
    10,     0,      2,      16,     16,     2,      0,      10,
    10,     0,      2,      16,     16,     2,      0,      10,
    30,     0,      1,      2,      2,      1,      0,      30,
    -150,   -250,   0,      0,      0,      0,      -250,   -150,
    500,    -150,   30,     10,     10,     30,     -150,   500,
};


othello::pawn IAInterface::switchTeam(othello::pawn team) {
    if(team == othello::pawn::black) {
        return othello::pawn::white;
    }
    return othello::pawn::black;
}
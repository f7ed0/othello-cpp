#include "ia/interface.hpp"
#include "ia/random.hpp"
#include "ia/minmax.hpp"
#include "ia/alphabeta.hpp"
#include "ia/alphabeta_absolute.hpp"
#include "errors/errors.hpp"
#include <bits/stdc++.h>

using namespace IA;

IAInterface* IAInterface::selectByName(std::string name) {
    
    std::vector <std::string> tokens;
    std::string intermediate;
    std::stringstream stream(name);

    while(getline(stream, intermediate, '=')) {
        tokens.push_back(intermediate);
    }

    if(tokens[0] == "random") {
        return (new Random());
    } else if(tokens[0] == "minmax"){
        return (new MinMax(std::stoi(tokens[1])));
    } else if(tokens[0] == "alphabeta"){
        return (new AlphaBeta(std::stoi(tokens[1])));
    } else if(tokens[0] == "alphabeta_absolute"){
        return (new AlphaBeta_Absolute(std::stoi(tokens[1])));
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
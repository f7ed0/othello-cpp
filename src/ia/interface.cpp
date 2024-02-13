#include "ia/interface.hpp"
#include "ia/random.hpp"
#include "ia/minmax.hpp"
#include "errors/errors.hpp"

using namespace IA;

IAInterface* IAInterface::selectByName(std::string name) {
    if(name == "random") {
        return (new IA::Random());
    } else if(name == "minmax_robin"){
        return (new IA::MinMax());
    } else {
        throw errors::OutOfBoundError();
    }
}

const int IAInterface::payoff_matrix[64] = {500,-150,30,10,10,30,-150,500,-150,-250,0,0,0,0,-250,-150,30,0,1,2,2,1,0,30,10,0,2,16,16,2,0,10,10,0,2,16,16,2,0,10,30,0,1,2,2,1,0,30,-150,-250,0,0,0,0,-250,-150,500,-150,30,10,10,30,-150,500};

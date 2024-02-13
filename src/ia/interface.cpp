#include "ia/interface.hpp"
#include "ia/random.hpp"
#include "errors/errors.hpp"

using namespace IA;

IAInterface* IAInterface::selectByName(std::string name) {
    if(name == "random") {
        return (new IA::Random());
    } else {
        throw errors::OutOfBoundError();
    }
}
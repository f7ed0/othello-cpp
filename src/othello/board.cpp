#include "othello/board.hpp"
#include <cstdlib>
#include <sstream>

using namespace othello;

// ----------------------------------------------------------------------------------

Board::Board() {
    this->cases = (pawn *) calloc(64,sizeof(pawn));
    // TODO : create proper error type
    if(this->cases == NULL) {
        throw -1;
    }
}

Board::Board(const Board& original) {
    this->cases = (pawn *) calloc(64,sizeof(pawn));
    // TODO : create proper error type
    if(this->cases == NULL) {
        throw -1;
    }
    for(int i = 0 ; i < Board::length ; i++){
        this->cases[i] = original.cases[i];
    }
}

// ----------------------------------------------------------------------------------

Board::~Board() { 
    free(this->cases);
}

// ----------------------------------------------------------------------------------

pawn Board::getCase(int index) const {
    if(index >= 0 && index < 64) {
        return this->cases[index];
    }
    // TODO : type this
    throw -1;
}

pawn Board::getCase(const std::string& coordinate) const {
    return this->getCase(Board::coord2Index(coordinate));
}

std::string Board::pretty() const {
    const char conv[3] = {' ','#','O'};
    std::stringstream ss;
    ss << "  a b c d e f g h" << std::endl;
    for(int i = 0 ; i < Board::side ; i ++) {
        ss << i+1 << " ";
        for(int j = 0 ; j < Board::side ; j++) {
            ss << conv[this->cases[i*Board::side+j]] << " "; 
        }
        ss << std::endl;
    }
    return ss.str();
}

std::string Board::prettyPlay(pawn team) const {
    const char conv[3] = {' ','#','O'};
    std::stringstream ss;
    std::vector<int> playable = listAllPlay(team);
    ss << "  a b c d e f g h" << std::endl;
    for(int i = 0 ; i < Board::side ; i ++) {
        ss << i+1 << " ";
        for(int j = 0 ; j < Board::side ; j++) {
            bool ok = false;
            for(int k = 0 ; k < (int) playable.size() ; k++) {
                if(playable.at(k) == i*Board::side+j) {
                    ss << ". ";
                    ok = true;
                    break;
                }
            }
            if(!ok) {
                ss << conv[this->cases[i*Board::side+j]] << " "; 
            }
        }
        ss << std::endl;
    }
    return ss.str();
}

void Board::setCase(int index,pawn value) {
    if(index >= 0 && index < 64) {
        this->cases[index] = value;
    }
    // TODO handle error
}

void Board::setCase(const std::string& coordinate, pawn value) {
    return this->setCase(Board::coord2Index(coordinate),value);
}

std::vector<int> Board::listAllPlay(pawn team) const {
    std::vector<int> ret;

    for(int i = 0; i < Board::length ; i++) {
        if(this->canPlaceHere(i,team)) {
            ret.push_back(i);
        }
    }

    return ret;
}

void Board::getPawnNumbers(int arr[3]) const {
    for(int i = 0 ; i < Board::length ; i ++) {
        arr[this->getCase(i)] ++;
    }
}

void Board::newGame() {
    for(int i = 0 ; i < Board::length ; i++){
        this->cases[i] = pawn::empty;
    }
    this->setCase("d4",othello::pawn::white);
    this->setCase("e5",othello::pawn::white);
    this->setCase("e4",othello::pawn::black);
    this->setCase("d5",othello::pawn::black);
}

// ------------------------------------------------------------------------------------

int Board::coord2Index(const std::string& coordinate) {
    return ((int) coordinate[1] - '1')*Board::side+((int) coordinate[0] - 'a');
}

std::string Board::index2Coord(int index) {
    std::string s = "";
    s += ('a' + (index%Board::side));
    s += ('1' + (index / Board::side));
    return s;
}

// ----------------------------------------------------------------------------------

std::ostream& othello::operator<<(std::ostream& os, const Board& item) {
    os << "[ ";
    for(int i = 0 ; i < Board::length; i++) {
        os << item.cases[i] << " ";
    }
    return os << "]";
}

// ---------------------------------------------------------------------------------------

int max(int a, int b) {
    if(a > b) {
        return a;
    }
    return b;
}

int min(int a, int b) {
    if(a > b) {
        return b;
    }
    return a;
}

// ------------------------------------------------------------------------------------------


#include "othello/board.hpp"

using namespace othello;

direction Board::canPlaceHere(int index,pawn team) const {
    if(team == pawn::empty || team > 2){
        return NODIR;
    }
    try {
        if(this->getCase(index) != pawn::empty) {
            return NODIR;
        }
    } catch(int err) {
        return NODIR;
    }

    direction result = NODIR;
    int posx = index % Board::side;
    int posy = index / Board::side;
    bool enemy_between;

    // Checking right
    enemy_between = false;
    for(int i = posx-1 ; i >= 0  ; i--) {
        if(this->getCase(i+posy*Board::side) == pawn::empty) {
            break;
        }
        if(!enemy_between) {
            if(this->getCase(i+posy*Board::side) == team) {
                break;
            }
            enemy_between = true;
        } else if(this->getCase(i+posy*Board::side) == team) {
            result |= dirs::RIGHT;
            break;
        }
    }

    // Checking left
    enemy_between = false;
    for(int i = posx+1 ; i < Board::side ; i++) {
        if(this->getCase(i+posy*Board::side) == pawn::empty) {
            break;
        }
        if(!enemy_between) {
            if(this->getCase(i+posy*Board::side) == team) {
                break;
            }
            enemy_between = true;
        } else if(this->getCase(i+posy*Board::side) == team) {
            result |= dirs::LEFT;
            break;
        }
    }

    // Checking top
    enemy_between = false;
    for(int i = posy-1 ; i >= 0  ; i--) {
        if(this->getCase(posx+i*Board::side) == pawn::empty) {
            break;
        }
        if(!enemy_between) {
            if(this->getCase(posx+i*Board::side) == team) {
                break;
            }
            enemy_between = true;
        } else if(this->getCase(posx+i*Board::side) == team) {
            result |= dirs::TOP;
            break;
        }
    }

    // Checking bottom
    enemy_between = false;
    for(int i = posy+1 ; i < Board::side ; i++) {
        if(this->getCase(posx+i*Board::side) == pawn::empty) {
            break;
        }
        if(!enemy_between) {
            if(this->getCase(posx+i*Board::side) == team) {
                break;
            }
            enemy_between = true;
        } else if(this->getCase(posx+i*Board::side) == team) {
            result |= dirs::BOTTOM;
            break;
        }
    }

    // checking diagonal top left
    enemy_between = false;
    for(int i = 1; posx-i >= 0 && posy-i >= 0 ; i++) {
        if(this->getCase((posx-i)+(posy-i)*Board::side) == pawn::empty) {
            break;
        }
        if(!enemy_between) {
            if(this->getCase((posx-i)+(posy-i)*Board::side) == team) {
                break;
            }
            enemy_between = true;
        } else if(this->getCase((posx-i)+(posy-i)*Board::side) == team) {
            result |= dirs::DTL;
            break;
        }
    }

    // checking diagonal top right
    enemy_between = false;
    for(int i = 1; posx+i <= Board::side && posy-i >= 0 ; i++) {
        if(this->getCase((posx+i)+(posy-i)*Board::side) == pawn::empty) {
            break;
        }
        if(!enemy_between) {
            if(this->getCase((posx+i)+(posy-i)*Board::side) == team) {
                break;
            }
            enemy_between = true;
        } else if(this->getCase((posx+i)+(posy-i)*Board::side) == team) {
            result |= dirs::DTR;
            break;
        }
    }

    // checking diagonal bottom left
    enemy_between = false;
    for(int i = 1; posx-i >= 0 && posy+i < Board::side ; i++) {
        if(this->getCase((posx-i)+(posy+i)*Board::side) == pawn::empty) {
            break;
        }
        if(!enemy_between) {
            if(this->getCase((posx-i)+(posy+i)*Board::side) == team) {
                break;
            }
            enemy_between = true;
        } else if(this->getCase((posx-i)+(posy+i)*Board::side) == team) {
            result |= dirs::DBL;
            break;
        }
    }

    // checking diagonal bottom right
    enemy_between = false;
    for(int i = 1; posx+i < Board::side && posy+i < Board::side ; i++) {
        if(this->getCase((posx+i)+(posy+i)*Board::side) == pawn::empty) {
            break;
        }
        if(!enemy_between) {
            if(this->getCase((posx+i)+(posy+i)*Board::side) == team) {
                break;
            }
            enemy_between = true;
        } else if(this->getCase((posx+i)+(posy+i)*Board::side) == team) {
            result |= dirs::DBR;
            break;
        }
    }

    return result;
}
#include "othello/board.hpp"

using namespace othello;

int Board::placePawn(int index, pawn team) {
    direction directions = canPlaceHere(index,team);

    if(directions == dirs::NODIR) {
        return 0;
    }
    
    int count = 0;
    int posx = index % Board::side;
    int posy = index / Board::side;

    // filling right
    if(directions & dirs::RIGHT) {
        for(int i = posx-1 ; i >= 0  ; i--) {
            if(this->getCase(i+posy*8) == pawn::empty) {
                throw -1;
            }
            if(this->getCase(i+posy*8) == team) {
               break;
            }
            this->setCase(i+posy*8,team);
            count ++;
        }
    }

    // Checking left
    if(directions & dirs::LEFT) {
        for(int i = posx+1 ; i < Board::side ; i++) {
            if(this->getCase(i+posy*8) == pawn::empty) {
                throw -1;
            }
            if(this->getCase(i+posy*8) == team) {
                break;
            }
            this->setCase(i+posy*8,team);
            count ++;
        }
    }
    
    // Checking top
    if(directions & dirs::TOP) {
        for(int i = posy-1 ; i >= 0  ; i--) {
            if(this->getCase(posx+i*8) == pawn::empty) {
                throw -1;
            }
            if(this->getCase(posx+i*8) == team) {
                break;
            }
            this->setCase(posx+i*8,team);
            count ++;
        }
    }

    // Checking bottom
    if(directions & dirs::BOTTOM) {
        for(int i = posy+1 ; i < Board::side ; i++) {
            if(this->getCase(posx+i*8) == pawn::empty) {
                throw -1;
            }
            if(this->getCase(posx+i*8) == team) {
                break;
            }
            this->setCase(posx+i*8,team);
            count ++;
        }
    }

    // checking diagonal top left
    if(directions & dirs::DTL) {
        for(int i = 1; posx-i >= 0 && posy-i >= 0 ; i++) {
            if(this->getCase((posx-i)+(posy-i)*8) == pawn::empty) {
                throw -1;
            }
            if(this->getCase((posx-i)+(posy-i)*8) == team) {
                break;
            }
            this->setCase((posx-i)+(posy-i)*8,team);
            count ++;
        }
    }

    // checking diagonal top right
    if(directions & dirs::DTR) {
        for(int i = 1; posx+i <= Board::side && posy-i >= 0 ; i++) {
            if(this->getCase((posx+i)+(posy-i)*8) == pawn::empty) {
                throw -1;
            }
            if(this->getCase((posx+i)+(posy-i)*8) == team) {
                break;
            }
            this->setCase((posx+i)+(posy-i)*8,team);
            count ++;
        }
    }

    // checking diagonal bottom left
    if(directions & dirs::DBL) {
        for(int i = 1; posx-i >= 0 && posy+i < Board::side ; i++) {
            if(this->getCase((posx-i)+(posy+i)*8) == pawn::empty) {
                throw -1;
            }
            else if(this->getCase((posx-i)+(posy+i)*8) == team) {
                break;
            }
            this->setCase((posx-i)+(posy+i)*8,team);
            count ++;
        }
    }

    // checking diagonal bottom right
    if(directions & dirs::DBR) {
        for(int i = 1; posx+i < Board::side && posy+i < Board::side ; i++) {
            if(this->getCase((posx+i)+(posy+i)*8) == pawn::empty) {
                throw -1;
            }
            if(this->getCase((posx+i)+(posy+i)*8) == team) {
                break;
            }
            this->setCase((posx+i)+(posy+i)*8,team);
            count ++;
        }
    }

    if(count >= 1) {
        this->setCase(index,team);
    }

    return count;
}
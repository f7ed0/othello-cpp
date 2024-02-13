#ifndef IA_INTERFACE_HPP
#define IA_INTERFACE_HPP

#include "othello/board.hpp"

namespace IA {
    /**
     * Virtual class for creating AI for the othello game.
    */
    class IAInterface {
        public :
            virtual int makeAMove(const othello::Board& current_board,othello::pawn team) = 0;

            virtual void resetAI() {};

            static IAInterface* selectByName(std::string name);
    };
}

#endif
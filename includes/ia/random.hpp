#ifndef IA_RANDOM_HPP
#define IA_RANDOM_HPP

#include "ia/interface.hpp"

namespace IA {
    class Random : public IAInterface {
        public :
            int makeAMove(const othello::Board& current_board,othello::pawn team) override;

            void resetAI() override;
    };
}


#endif
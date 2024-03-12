#ifndef IA_RANDOM_HPP
#define IA_RANDOM_HPP

#include "ia/interface.hpp"

namespace IA {
    class Random : public IAInterface {
        protected :
            int heuristics(const othello::Board& current_board, const othello::pawn team, const othello::pawn player) override;
        public :
            int makeAMove(const othello::Board& current_board,othello::pawn team) override;

            void resetAI() override;
    };
}


#endif
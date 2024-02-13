#ifndef IA_MINMAX_HPP
#define IA_MINMAX_HPP

#include "ia/interface.hpp"

namespace IA {
    class MinMax : public IAInterface {
        private :
            int depth;
        protected :
            int euristics(const othello::Board& current_board, const othello::pawn team) override;
        public :
            int makeAMove(const othello::Board& current_board,othello::pawn team) override;

            int minmax(const othello::Board& current_board,othello::pawn player, const int depth, othello::pawn team);

            void resetAI() override;

            MinMax(int __depth);
    };
}


#endif
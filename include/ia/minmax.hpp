#ifndef IA_MINMAX_HPP
#define IA_MINMAX_HPP

#include "ia/interface.hpp"

namespace IA {
    class MinMax : public IAInterface {
        protected :
            int depth;

            int heuristics(const othello::Board& current_board, const othello::pawn team) override;

            static void launchMinmax(MinMax* IA,const othello::Board& board,othello::pawn player,int depth, othello::pawn team, int* result);
            
        public :
            int makeAMove(const othello::Board& current_board,othello::pawn team) override;

            int minmax(const othello::Board& current_board,othello::pawn player, const int depth, othello::pawn team);

            void resetAI() override;

            MinMax(int __depth);

    };
}


#endif
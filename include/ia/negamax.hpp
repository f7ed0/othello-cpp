#ifndef IA_NEGAMAX_HPP
#define IA_NEGAMAX_HPP

#include "ia/interface.hpp"

namespace IA {
    class NegaMax : public IAInterface {
        private :
            int depth;
        protected :
            int heuristics(const othello::Board& current_board, const othello::pawn team) override;

            static void launchNegamax(NegaMax* IA,const othello::Board& board,othello::pawn player,int depth, othello::pawn team, int* result);
            
        public :
            int makeAMove(const othello::Board& current_board,othello::pawn team) override;

            int negamax(const othello::Board& current_board,othello::pawn player, const int depth, othello::pawn team);

            void resetAI() override;

            NegaMax(int __depth);

    };
}


#endif
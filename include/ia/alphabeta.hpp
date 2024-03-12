#ifndef IA_ALPHABETA_HPP
#define IA_ALPHABETA_HPP

#include "ia/interface.hpp"

namespace IA {
    class AlphaBeta : public IAInterface {
        protected :
            int depth;

            int heuristics(const othello::Board& current_board, const othello::pawn team) override;

            static void launchAlphabeta(AlphaBeta* IA,const othello::Board& board,othello::pawn player,int depth, othello::pawn team, int* result);
            
        public :
            int makeAMove(const othello::Board& current_board,othello::pawn team) override;

            int alphabeta(const othello::Board& current_board,othello::pawn player, const int depth, othello::pawn team, int _alpha, int _beta);

            void resetAI() override;

            AlphaBeta(int __depth);

    };
}


#endif
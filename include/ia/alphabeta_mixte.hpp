#ifndef IA_ALPHABETA_MIXTE_HPP
#define IA_ALPHABETA_MIXTE_HPP

#include "ia/alphabeta.hpp"

namespace IA {
    class AlphaBeta_Mixte : public AlphaBeta {
        protected :
            int heuristics(const othello::Board& current_board, const othello::pawn team ) override;

            int positional(const othello::Board& current_board, const othello::pawn team );
            int absolute(const othello::Board& current_board, const othello::pawn team );
            int mobility(const othello::Board& current_board, const othello::pawn team );
        public :
            AlphaBeta_Mixte(int __depth);
    };
}


#endif
#ifndef IA_ALPHABETA_HPP
#define IA_ALPHABETA_HPP

#include "ia/alphabeta.hpp"

namespace IA {
    class AlphaBeta_Absolute : public AlphaBeta {
        protected :
            int heuristics(const othello::Board& current_board, const othello::pawn team) override;
        public :
            AlphaBeta_Absolute(int __depth);
    };
}


#endif
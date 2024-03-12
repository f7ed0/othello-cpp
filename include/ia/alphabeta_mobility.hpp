#ifndef IA_ALPHABETA_MOB_HPP
#define IA_ALPHABETA_MOB_HPP

#include "ia/alphabeta.hpp"

namespace IA {
    class AlphaBeta_Mobility : public AlphaBeta {
        protected :
            int heuristics(const othello::Board& current_board, const othello::pawn team, const othello::pawn player) override;
        public :
            AlphaBeta_Mobility(int __depth);
    };
}


#endif
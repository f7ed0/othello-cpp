#ifndef GUI_WINDOW_HPP
#define GUI_WINDOW_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <thread>
#include <atomic>
#include "othello/board.hpp"
#include "ia/interface.hpp"

namespace gui {

    class Window {
        private :
            SDL_Window* w;
            SDL_Renderer* r;
            othello::Board* board;

            SDL_Texture* textures[3];

            TTF_Font* font;

            static const int BLACK_PAWN_TEXTURE = 0;
            static const int WHITE_PAWN_TEXTURE = 1;
            static const int WOOD_BOARD_TEXTURE = 2;

            bool gameEnded;
            bool exited;

            int height;
            int width;

            othello::pawn current_player;

            int mousePosX;
            int mousePosY;

            int* score;

            static constexpr float aspectRatio = 16.0f/9.0f;

            bool isPlayer1AI;
            bool isPlayer2AI;

            IA::IAInterface* IA1;
            IA::IAInterface* IA2;

            std::atomic<bool> IA_thinking;
            std::atomic<int> IA_result;

            bool IA_launched;

            std::thread* IAthread;


        protected :
            int eventPolling();
            void rendering();

            static void IAPlay(IA::IAInterface* ia,const othello::Board& board, othello::pawn team, std::atomic<bool>* thinking, std::atomic<int>* result);

            void placePawn(int posX, int posY);

            void placePawn(int index);

            // Graphical Board boundaries
            int minDimention() const;

            float getBoardSize() const;
            float getCaseSize() const;
            float getBoardAnchorX() const;
            float getBoardAnchorY() const;

            int mouseBoardX() const;
            int mouseBoardY() const;

            /**
             * UNSAFE
            */
            int mouseToBoardX(int posX) const;

            /**
             * UNSAFE
            */
            int mouseToBoardY(int posY) const;

            void switchPlayer();

        public :
            static void SDLInit();

            static void SDLQuit();

            Window();
            Window(bool player1IA, bool player2IA, std::string IA1, std::string IA2);
            ~Window();

            int mainLoop();

    };

}

#endif
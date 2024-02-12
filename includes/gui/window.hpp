#ifndef GUI_WINDOW_HPP
#define GUI_WINDOW_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "othello/board.hpp"

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

            bool currently_resizing;

            int* score;

            static constexpr float aspectRatio = 16.0f/9.0f;

        protected :
            int eventPolling();
            void rendering();

            void placePawn(int posX, int posY);

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
            ~Window();

            int mainLoop();

    };

}

#endif
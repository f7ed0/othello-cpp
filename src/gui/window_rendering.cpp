#include <vector>
#include <sstream>

#include "gui/window.hpp"
#include "SDL2/SDL_image.h"


using namespace gui;

void Window::rendering() {
    // Rendering backscreen
    SDL_SetRenderDrawColor(this->r, 254, 247, 240, 255);
    SDL_RenderFillRect(this->r,NULL);

    float dim = this->getBoardSize();
    float sep = this->getCaseSize();

    float minx = this->getBoardAnchorX();
    float miny = this->getBoardAnchorY();

    SDL_FRect r;

    // Rendering wood back box
    r = {minx-sep*.3f,miny-sep*.3f,dim+sep*.6f,dim+sep*.6f};
    SDL_RenderCopyF(this->r,this->textures[Window::WOOD_BOARD_TEXTURE],NULL,&r);

    // Rendering Board
    for(int i=0; i < othello::Board::side; i++) {
        for(int j=0; j < othello::Board::side; j++) {
            if((i+j)%2 == 0) {
                SDL_SetRenderDrawColor(this->r,147, 163, 146, 255);
            } else {
                SDL_SetRenderDrawColor(this->r, 173, 191, 151, 255);
            }

            r = {minx+sep*((float) i),miny+sep*((float) j),sep,sep};
            SDL_RenderFillRectF(this->r, &r);
        }
    }

    // Rendering pawns
    for(int i=0; i < othello::Board::side; i++) {
        for(int j=0; j < othello::Board::side; j++) {
            if(this->board->getCase(j*othello::Board::side+i) == othello::pawn::empty) {
                continue;
            }
            r = {minx+sep*((float) i)+sep*0.1f,miny+sep*((float) j)+sep*0.1f,sep*0.8f,sep*0.8f};
            if(this->board->getCase(j*othello::Board::side+i) == othello::pawn::black) {
                SDL_RenderCopyF(this->r,this->textures[Window::BLACK_PAWN_TEXTURE],NULL,&r);
            } else if(this->board->getCase(j*othello::Board::side+i) == othello::pawn::white) {
                SDL_RenderCopyF(this->r,this->textures[Window::WHITE_PAWN_TEXTURE],NULL,&r);
            }
        }
    }

    // Rendering possible plays
    std::vector<int> plays = this->board->listAllPlay(this->current_player);
    SDL_SetRenderDrawColor(this->r, 0, 0, 0, 100);
    for(int i = 0 ; i < ((int) plays.size()) ; i++) {
        int x = plays.at(i)%8;
        int y = plays.at(i)/8;

        r = {minx+sep*((float) x)+sep*0.3f,miny+sep*((float) y)+sep*0.3f,sep*0.4f,sep*0.4f};

        SDL_RenderFillRectF(this->r, &r);
    }


    // rendering the mouse cursor
    int x = this->mouseBoardX();
    int y = this->mouseBoardY();
    if( x >= 0 && y >= 0 ){
        r = {minx+sep*x,miny+sep*y,sep,sep};
        SDL_SetRenderDrawColor(this->r, 255, 255, 255, 100);
        SDL_RenderFillRectF(this->r, &r);
    }

    // ---------------- TEXT ------------------------

    std::stringstream ss;

    ss << this->score[othello::pawn::black] << " - " << this->score[othello::pawn::white];

    SDL_Color front = {0,0,0,255};

    SDL_Texture* textTexture;


    //Rendering score
    SDL_Surface* textsurf = TTF_RenderText_Blended_Wrapped(this->font,ss.str().c_str(),front,this->minDimention()/5);
    if(textsurf == NULL) {
        std::cout << SDL_GetError() << std::endl;
        throw SDL_GetError();
    }

    textTexture = SDL_CreateTextureFromSurface(this->r,textsurf);
    if(textTexture == NULL) {
        throw -1;
    }

    SDL_FreeSurface(textsurf);

    r = {minx-((float) (this->minDimention()/5))*2,((float) this->height)/2 - ((float) (this->minDimention()/5))*0.25f,(float) (this->minDimention()/5),(float) (this->minDimention()/5)*0.5f};

    SDL_RenderCopyF(this->r,textTexture,NULL,&r);

    SDL_DestroyTexture(textTexture);

    r = {minx-((float) (this->minDimention()/5))*2-sep*0.7f-10,((float) this->height)/2 - sep*0.35f,sep*0.7f,sep*0.7f};
    SDL_RenderCopyF(this->r,this->textures[BLACK_PAWN_TEXTURE],NULL,&r);

    r = {minx-((float) (this->minDimention()/5))+10,((float) this->height)/2 - sep*0.35f,sep*0.7f,sep*0.7f};
    SDL_RenderCopyF(this->r,this->textures[WHITE_PAWN_TEXTURE],NULL,&r);

    // ----------------------------------------------

    SDL_RenderPresent(this->r);
}
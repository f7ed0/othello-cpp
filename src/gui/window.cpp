#include "gui/window.hpp"
#include "SDL2/SDL_image.h"
#include <unistd.h>

using namespace gui;


Window::Window() {
    if(SDL_CreateWindowAndRenderer(1280,720,SDL_WINDOW_RESIZABLE,&this->w,&this->r) != 0) {
        throw -1; // TODO make an error type
    }
    if(this->w == NULL) {
        throw -1;
    }
    if(this->r == NULL) {
        throw -1;
    }
    SDL_SetWindowTitle(this->w,"Othello SDL2");
    SDL_SetWindowMinimumSize(this->w, 640, 480);
    SDL_GetWindowSize(this->w,&this->width,&this->height);

    this->mousePosX = 0;
    this->mousePosY = 0;

    SDL_SetRenderDrawBlendMode(this->r, SDL_BLENDMODE_BLEND);

    this->textures[Window::BLACK_PAWN_TEXTURE] = IMG_LoadTexture(this->r,"assets/black.png");
    if(this->textures[Window::BLACK_PAWN_TEXTURE] == NULL) {
        throw -1;
    }
    this->textures[Window::WHITE_PAWN_TEXTURE] = IMG_LoadTexture(this->r,"assets/white.png");
    if(this->textures[Window::WHITE_PAWN_TEXTURE] == NULL) {
        throw -1;
    }
    this->textures[Window::WOOD_BOARD_TEXTURE] = IMG_LoadTexture(this->r,"assets/wood.png");
    if(this->textures[Window::WOOD_BOARD_TEXTURE] == NULL) {
        throw -1;
    }

    this->font = TTF_OpenFont("assets/VT323-Regular.ttf",40);

    this->board = new othello::Board();
    this->board->newGame();
    this->current_player = othello::pawn::black;

    this->score = (int*) calloc(3,sizeof(int));

    this->board->getPawnNumbers(this->score);
}

Window::~Window() {
    SDL_DestroyTexture(this->textures[Window::BLACK_PAWN_TEXTURE]);
    SDL_DestroyTexture(this->textures[Window::WHITE_PAWN_TEXTURE]);
    SDL_DestroyTexture(this->textures[Window::WOOD_BOARD_TEXTURE]);

    TTF_CloseFont(this->font);

    free(this->score);
}

void Window::SDLInit() {
    if(SDL_Init(0) != 0) {
        throw -1; // TODO make an error type
    }

    if(TTF_Init() != 0){
        throw -1;
    } 

    /*
    bool value = SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "2" );
    if(!value) {
        std::cout << "Failed setting the anti-alisasing" << std::endl;
    }
    */
}

void Window::SDLQuit() {
    TTF_Quit();
    SDL_Quit();
}

int Window::mainLoop() {
    int sleep_result;
    while(!this->exited) {
        // Event polling
        this->eventPolling();

        // Rendering
        this->rendering();


        // Yielding
        sleep_result = usleep(16000);
        if(sleep_result != 0) {
            throw -1;
        }
    }

    return 0;
}

int Window::eventPolling() {
    SDL_Event e;
    int evt = 0;
    bool isResizing = false;
    while(SDL_PollEvent(&e)) {
        
        switch (e.type) {
            case SDL_QUIT:
                this->exited = true;
                break;
            case SDL_WINDOWEVENT :
                switch (e.window.event) {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        SDL_GetWindowSize(this->w,&this->width,&this->height);
                        isResizing = true;
                        break;
                }
                break;
            case SDL_MOUSEMOTION :
                this->mousePosX = e.motion.x;
                this->mousePosY = e.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN :
                switch (e.button.button){
                    case SDL_BUTTON_LEFT:
                        this->placePawn(e.button.x,e.button.y);
                        break;
                }
                break;
        }
        evt ++;
    }
    if(this->currently_resizing && !isResizing) {
        float h = (float) this->height;
        float w = (float) this->width;
        float current_ratio = w/h;
        if(current_ratio < Window::aspectRatio) {
            w = aspectRatio*h;
        }
        if(current_ratio > Window::aspectRatio) {
            h = (1.0f/aspectRatio)*w;
        }
        SDL_SetWindowSize(this->w,(int)w,(int)h);
    }
    this->currently_resizing = isResizing;

    return evt;
}

int Window::minDimention() const {
    if(this->width < this->height) {
        return this->width;
    }
    return this->height;
}

float Window::getBoardSize() const {
    return ((float) this->minDimention())*0.8-50;
}

float Window::getCaseSize() const {
    return ((float) this->getBoardSize())/( (float) othello::Board::side);
}

float Window::getBoardAnchorX() const {
    return ((float)this->width)/2 - this->getBoardSize()/2;
}

float Window::getBoardAnchorY() const {
    return ((float)this->height)/2 - this->getBoardSize()/2 + 50;
}

int Window::mouseBoardX() const {
    int x = this->mouseToBoardX(this->mousePosX);
    if (x >= 0 && x < othello::Board::side) {
        return x;
    }
    return -1;
}

int Window::mouseBoardY() const {
    int y = this->mouseToBoardY(this->mousePosY);
    if(y >= 0 && y < othello::Board::side) {
        return y;
    }
    return -1;
}

int Window::mouseToBoardX(int posX) const {
    return (posX - ((int) this->getBoardAnchorX()))/((int) this->getCaseSize());
}

int Window::mouseToBoardY(int posY) const {
    return (posY - ((int) this->getBoardAnchorY()))/((int) this->getCaseSize());
}

void Window::placePawn(int posX, int posY) {
    int x = this->mouseToBoardX(posX);
    int y = this->mouseToBoardY(posY);

    if(this->board->placePawn(x+y*othello::Board::side,this->current_player)) {
        this->switchPlayer();
        if(this->board->listAllPlay(this->current_player).size() == 0) {
            this->switchPlayer();
            if(this->board->listAllPlay(this->current_player).size() == 0) {
                this->gameEnded = true;
            }
        }
    }

    this->board->getPawnNumbers(this->score);
}

void Window::switchPlayer() {
    if (this->current_player == othello::pawn::black){
        this->current_player = othello::pawn::white;
        return;
    }
    this->current_player = othello::pawn::black;
}
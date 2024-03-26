
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include <random>
#include <ctime>

#include "gui/window.hpp"
#include "ia/random.hpp"
#include "errors/errors.hpp"


using namespace gui;


Window::Window() {
    if(SDL_CreateWindowAndRenderer(1280,720,SDL_WINDOW_RESIZABLE,&this->w,&this->r) != 0) {
        throw errors::SDLError(SDL_GetError());
    }
    if(this->w == NULL) {
        throw errors::SDLError(SDL_GetError());
    }
    if(this->r == NULL) {
        throw errors::SDLError(SDL_GetError());
    }
    SDL_SetWindowTitle(this->w,"Othello SDL2");
    SDL_SetWindowMinimumSize(this->w, 640, 480);
    SDL_GetWindowSize(this->w,&this->width,&this->height);

    this->mousePosX = 0;
    this->mousePosY = 0;

    SDL_SetRenderDrawBlendMode(this->r, SDL_BLENDMODE_BLEND);

    this->textures[BLACK_PAWN_TEXTURE] = IMG_LoadTexture(this->r,"assets/black.png");
    if(this->textures[BLACK_PAWN_TEXTURE] == NULL) {
        throw -1;
    }
    this->textures[WHITE_PAWN_TEXTURE] = IMG_LoadTexture(this->r,"assets/white.png");
    if(this->textures[WHITE_PAWN_TEXTURE] == NULL) {
        throw -1;
    }
    this->textures[WOOD_BOARD_TEXTURE] = IMG_LoadTexture(this->r,"assets/wood.png");
    if(this->textures[WOOD_BOARD_TEXTURE] == NULL) {
        throw -1;
    }

    this->font = TTF_OpenFont("assets/VT323-Regular.ttf",40);

    this->board = new othello::Board();
    this->board->newGame();
    this->current_player = othello::pawn::black;

    this->score = (int*) calloc(3,sizeof(int));

    this->board->getPawnNumbers(this->score);

    this->isPlayer1AI = false;
    this->isPlayer2AI = false;

    this->IA1 = NULL;
    this->IA2 = NULL;

    IA_thinking = false;
    IA_launched = false;

    IAthread = NULL;
}

Window::Window(bool player1IA, bool player2IA, std::string IA1, std::string IA2): Window::Window() {
    this->isPlayer1AI = player1IA;
    this->isPlayer2AI = player2IA;

    if(this->isPlayer1AI) {
        this->IA1 = IA::IAInterface::selectByName(IA1);
    }

    if(this->isPlayer2AI) {
        this->IA2 = IA::IAInterface::selectByName(IA2);
    }
}

Window::~Window() {

    SDL_DestroyTexture(this->textures[BLACK_PAWN_TEXTURE]);
    SDL_DestroyTexture(this->textures[WHITE_PAWN_TEXTURE]);
    SDL_DestroyTexture(this->textures[WOOD_BOARD_TEXTURE]);

    TTF_CloseFont(this->font);

    delete this->board;
    if(this->IA1 != NULL && this->isPlayer1AI) {
        delete this->IA1;
    }
    if(this->IA2 != NULL && this->isPlayer2AI) {
        delete this->IA2;
    }

    free(this->score);
}

void Window::SDLInit() {
    if(SDL_Init(0) != 0) {
        throw errors::SDLError(SDL_GetError());
    }

    if(TTF_Init() != 0){
        throw errors::SDLError(TTF_GetError());
    } 

    srand(time(NULL));
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

        if(!gameEnded && ((current_player == othello::pawn::black && isPlayer1AI) || (current_player == othello::pawn::white && isPlayer2AI))) {
            if(!IA_launched) {
                IA_thinking = true;
                IA_launched = true;
                if(this->current_player == othello::pawn::black){
                    this->IAthread = new std::thread(Window::IAPlay,this->IA1,*(this->board),this->current_player,&IA_thinking,&IA_result);
                } else {
                    this->IAthread = new std::thread(Window::IAPlay,this->IA2,*(this->board),this->current_player,&IA_thinking,&IA_result);
                }
            } else {
                if(!IA_thinking) {
                    if(this->IAthread->joinable()) {
                        this->IAthread->join();
                        delete this->IAthread;
                    }
                    IA_launched = false;
                    this->placePawn(IA_result);
                }
            }
        }

        // Yielding
        sleep_result = usleep(10000);
        if(sleep_result != 0) {
            throw -1;
        }
    }

    return 0;
}

int Window::eventPolling() {
    SDL_Event e;
    int evt = 0;
    while(SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                this->exited = true;
                break;
            case SDL_WINDOWEVENT :
                switch (e.window.event) {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        SDL_GetWindowSize(this->w,&this->width,&this->height);
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
    if(current_player == othello::pawn::black && isPlayer1AI) {
        return;
    }
    if(current_player == othello::pawn::white && isPlayer2AI) {
        return;
    }

    int x = this->mouseToBoardX(posX);
    int y = this->mouseToBoardY(posY);

    try {
        this->placePawn(x+y*othello::Board::side);
    } catch(errors::NotPlayableMoveError err) {

    }
    
}

void Window::placePawn(int index) {
    if(this->board->placePawn(index,this->current_player)) {
        this->switchPlayer();
        if(this->board->listAllPlay(this->current_player).size() == 0) {
            this->switchPlayer();
            if(this->board->listAllPlay(this->current_player).size() == 0) {
                this->gameEnded = true;
            }
        }
    } else {
        throw errors::NotPlayableMoveError();
    }

    this->board->getPawnNumbers(this->score);
}

void Window::IAPlay(IA::IAInterface* ia, const othello::Board& board, othello::pawn team, std::atomic<bool>* thinking, std::atomic<int>* result) {
    int i = ia->makeAMove(board,team);

    *result = i;
    *thinking = false;
}

void Window::switchPlayer() {
    if (this->current_player == othello::pawn::black){
        this->current_player = othello::pawn::white;
        return;
    }
    this->current_player = othello::pawn::black;
}
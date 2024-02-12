#include <iostream>
#include <chrono>

#include "othello/board.hpp"
#include "gui/window.hpp"

using namespace std;

int playNoGui();
void showHelp();
int playGUI();


int main(int argc, char *argv[]) {
    // reading command line arguments
    bool gui = false;
    for(int i = 1 ; i < argc ; i++) {
        string stringed = (string) argv[i];
        if(stringed == "--gui") {
            gui = true;
            continue;
        }
        if(stringed == "--no-gui") {
            gui = false;
            continue;
        }
        if(stringed == "-h" || stringed == "--help") {
            showHelp();
            return 0;
        }
        cout << "argument \"" << stringed << "\" is not recognised." << endl;
        return 1;
    }
    if(gui) {
        return playGUI();
    } else {
        return playNoGui();
    }
}

int playGUI() {
    gui::Window::SDLInit();

    gui::Window *w = new gui::Window();

    w->mainLoop();

    gui::Window::SDLQuit();

    return 0;
}

int playNoGui() {
    othello::Board a;
    a.newGame();
    string play = "";
    othello::pawn players[2] = {othello::pawn::black,othello::pawn::white};
    chrono::_V2::system_clock::time_point t1,t2;
    chrono::microseconds timing;
    int player_index = 0;
    bool end_flag[2] = {0,0};
    int score[3];
    while(true){
        
        cout << "------------------" << endl << a.prettyPlay(players[player_index]) << "------------------" << endl;

        a.getPawnNumbers(score);
        cout << "vide : " << score[othello::pawn::empty] << "\tnoir : " << score[othello::pawn::black] << "\tblanc : " << score[othello::pawn::white] << endl;

        vector<int> plays = a.listAllPlay(players[player_index]);

        if(plays.size() <= 0) {
            end_flag[player_index] = true;
            player_index = (player_index + 1)%2;
            if(end_flag[0] && end_flag[1]){
                break;
            }
            continue;
        } else {
            end_flag[player_index] = false;
        }

        t1 = chrono::high_resolution_clock::now();

        for(int i = 0 ; i < 1000 ; i++) {
             a.listAllPlay(players[player_index]);
        }

        t2 = chrono::high_resolution_clock::now();

        for(int i = 0 ; i < (int) plays.size() ; i ++) {
            cout << othello::Board::index2Coord(plays.at(i)) << " ";
        }
        cout << endl;

        timing = chrono::duration_cast<chrono::microseconds>(t2-t1);

        cout << "Plays fetched in " << ((float) timing.count())/1000.0f << "µs." << endl;

        if(players[player_index] == othello::pawn::black) {
            cout << "Joueur : Noir #" << endl;
        } else {
            cout << "Joueur : Blanc O" << endl;
        }

        while(!a.canPlaceHere(othello::Board::coord2Index(play),players[player_index])) {
            cin >> play;
        }
        
        cout << a.placePawn(othello::Board::coord2Index(play),players[player_index]) << " pawns taken."  << endl;
        
        player_index = (player_index + 1)%2;
    }

    a.getPawnNumbers(score);

    if(score[othello::pawn::black] < score[othello::pawn::white]) {
        cout << "Victoire des blancs." << endl;
    } else if(score[othello::pawn::black] > score[othello::pawn::white]) {
         cout << "Victoire des noirs." << endl;
    } else {
        cout << "Match nul." << endl;
    }

    return 0;
}

void showHelp() {
    cout << " ohtello by 0xf7ed0 & kappacino " << endl;
    cout << "---------------------------------" << endl << endl;
    cout << "usage : othello [--gui | --no-gui]" << endl << endl;
    cout << "---------------------------------" << endl << endl;
    cout << "--gui      enable the GUI to play" << endl;
    cout << "--no-gui   play in CLI (default) " << endl;
}
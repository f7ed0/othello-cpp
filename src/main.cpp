#include <iostream>
#include <chrono>

#include "othello/board.hpp"



using namespace std;


int main(int argc, char *argv[]) {
    othello::Board a;

    a.setCase("d4",othello::pawn::white);
    a.setCase("e5",othello::pawn::white);
    a.setCase("e4",othello::pawn::black);
    a.setCase("d5",othello::pawn::black);
    /*
        a.setCase("c6",othello::pawn::white);
        a.setCase("f3",othello::pawn::white);
    */

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

        cout << "Plays fetched in " << ((float) timing.count())/1000 << "µs." << endl;

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

    return 0;
}
#include <iostream>
#include <chrono>

#include "othello/board.hpp"
#include "gui/window.hpp"

using namespace std;

int playNoGui(bool IA1, bool IA2, string name1, string name2,int gamecount, bool progress, bool noresult);
void showHelp();
int playGUI(bool IA1, bool IA2, string name1, string name2);


int main(int argc, char *argv[]) {
    // reading command line arguments
    bool gui = false;

    bool IA1 = false;
    bool IA2 = false;

    bool progress = false;

    bool no_result = false;

    string IA1_name = "";
    string IA2_name = "";

    int gcount = 1;
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
        if(stringed == "--IA1") {
            IA1 = true;
            i++;
            IA1_name = (string) argv[i];
            continue;
        }
        if(stringed == "--IA2") {
            IA2 = true;
            i++;
            IA2_name = (string) argv[i];
            continue;
        }
        if(stringed == "--gamecount") {
            IA2 = true;
            i++;
            char* end;
            gcount = strtol(argv[i],&end,10);
            if(gcount < 0) {
                cout << "gamecount needs to be >= 0." << endl;
                return 1;
            }
            continue;
        }
        if(stringed == "--progress") {
            progress = true;
            continue;
        }
        if(stringed == "--no-result") {
            no_result = true;
            continue;
        }
        cout << "argument \"" << stringed << "\" is not recognised." << endl;
        return 1;
    }
    if(gui) {
        return playGUI(IA1,IA2,IA1_name,IA2_name);
    } else {
        return playNoGui(IA1,IA2,IA1_name,IA2_name,gcount,progress,no_result);
    }
}

int playGUI(bool IA1, bool IA2, string name1, string name2) {
    gui::Window::SDLInit();

    gui::Window *w = new gui::Window(IA1,IA2,name1,name2);

    w->mainLoop();

    delete w;

    gui::Window::SDLQuit();    

    return 0;
}

int playNoGui(bool IA1, bool IA2, string name1, string name2,int gamecount,bool progress, bool no_result) {
    IA::IAInterface* IA_black = NULL;
    IA::IAInterface* IA_white = NULL;
    if(IA1) {
        IA_black = IA::IAInterface::selectByName(name1);
    }
    if(IA2) {
        IA_white = IA::IAInterface::selectByName(name2);
    }

    int winloss[3] = {0,0,0};

    srand(time(NULL));

    chrono::_V2::system_clock::time_point t1,t2;
    double mean_time_move1 = 0;
    double mean_time_move2 = 0;

    long move_count1 = 0;
    long move_count2 = 0;

    for(int i =0 ; i < gamecount ; i++) {
        cout << "game " << i+1 << " of " << gamecount << endl;
        othello::Board a;
        a.newGame();
        othello::pawn players[2] = {othello::pawn::black,othello::pawn::white};
        int player_index = 0;
        bool end_flag[2] = {0,0};
        int score[3];
        int count = 1;
        while(true){
            vector<int> plays = a.listAllPlay(players[player_index]);
            string play = "";

            if(!(((players[player_index] == othello::pawn::black) && IA1) || ((players[player_index] == othello::pawn::white) && IA2))) {
                cout << "------------------" << endl << a.prettyPlay(players[player_index]) << "------------------" << endl;
                a.getPawnNumbers(score);
                cout << "vide : " << score[othello::pawn::empty] << "\tnoir : " << score[othello::pawn::black] << "\tblanc : " << score[othello::pawn::white] << endl;
                cout << endl;

                for(int i = 0 ; i < (int) plays.size() ; i ++) {
                    cout << othello::Board::index2Coord(plays.at(i)) << " ";
                }

                if(players[player_index] == othello::pawn::black) {
                    cout << "Joueur : Noir #" << endl;
                } else {
                    cout << "Joueur : Blanc O" << endl;
                }
            }

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

            if((players[player_index] == othello::pawn::black) && IA1) {
                t1 = chrono::high_resolution_clock::now();
                int move = IA_black->makeAMove(a,othello::black);
                t2 = chrono::high_resolution_clock::now();
                mean_time_move1 += (double) chrono::duration_cast<chrono::microseconds>(t2-t1).count();
                move_count1 ++;
                if(a.placePawn(move,players[player_index]) == 0) {
                    cout << "can't place here" << endl;
                    throw 1;
                }
            } else if((players[player_index] == othello::pawn::white) && IA2) {
                t1 = chrono::high_resolution_clock::now();
                int move = IA_white->makeAMove(a,othello::white);
                t2 = chrono::high_resolution_clock::now();
                mean_time_move2 += (double) chrono::duration_cast<chrono::microseconds>(t2-t1).count();
                move_count2 ++;
                if(a.placePawn(move,players[player_index]) == 0) {
                    cout << "can't place here" << endl;
                    throw 1;
                }
            } else {
                while(!a.canPlaceHere(othello::Board::coord2Index(play),players[player_index])) {
                    cin >> play;
                }
                cout << a.placePawn(othello::Board::coord2Index(play),players[player_index]) << " pawns taken."  << endl;
            }

            if(IA1 && IA2 && progress) {
                if(count%8==0) {
                    cout << count;
                } else {
                    cout << ".";
                }
                cout.flush();
            }
            
            player_index = (player_index + 1)%2;
            count ++;
        }

        a.getPawnNumbers(score);

        if(IA1 && IA2 && progress) {
            cout << endl;
        }

        if(!no_result) {
            cout << "------------------" << endl << a.prettyPlay(players[player_index]) << "------------------" << endl;
        }

        a.getPawnNumbers(score);
        cout << "vide : " << score[othello::pawn::empty] << "\tnoir : " << score[othello::pawn::black] << "\tblanc : " << score[othello::pawn::white] << endl;
        cout << endl;

        if(score[othello::pawn::black] < score[othello::pawn::white]) {
            cout << "Victoire des blancs." << endl;
            winloss[othello::pawn::white] ++;
        } else if(score[othello::pawn::black] > score[othello::pawn::white]) {
            cout << "Victoire des noirs." << endl;
            winloss[othello::pawn::black] ++;
        } else {
            cout << "Match nul." << endl;
            winloss[othello::pawn::empty] ++;
        }

        if(IA1) {
            IA_black->resetAI();
        }
        if(IA2) {
            IA_white->resetAI();
        }
    }


    cout << winloss[othello::pawn::black] << " match(s) gagné par les Noirs." << endl;
    cout << winloss[othello::pawn::white] << " match(s) gagné par les Blanc." << endl;
    cout << winloss[othello::pawn::empty] << " match(s) nul(s)." << endl;

    if(move_count1 > 0) {
        cout << "IA1 (" + name1 + ") mean calculation time per move : " << (mean_time_move1 / (((double) move_count1)*1000)) << " ms (" << move_count1 << " moves played)" << endl;
    }

    if(move_count2 > 0) {
        cout << "IA2 (" + name2 + ") mean calculation time per move : " << (mean_time_move2 / (((double) move_count2)*1000)) << " ms (" << move_count2 << " moves played)"  << endl;
    }

    if(gamecount == 0) {
        cout << "SKIBITOILET." << endl;
    }

    return 0;
}

void showHelp() {
    cout << "                             othello by 0xf7ed0 & Kappaccino                             " << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl << endl;
    cout << " usage : othello [--gui | --no-gui] [--IA1 <name>=<depth>] [--IA2 <name>=<depth>] [args] " << endl << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl << endl;
    cout << "  --gui                     enable the GUI to play" << endl;
    cout << "  --no-gui                  play in CLI (default) " << endl;
    cout << "  --progress                show real-time game progres (CLI only) " << endl;
    cout << "  --no-result               don't show game results (CLI only)" << endl;
    cout << "  --gamecount <count>       set the number of games to play" << endl;
    cout << "  --IA1 <name>=<depth>      set player1 (black) to AI with depth" << endl;
    cout << "  --IA2 <name>=<depth>      set player2 (white) to AI with depth" << endl;
}
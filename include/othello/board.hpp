#ifndef OTHELLO_BOARD_HPP
#define OTHELLO_BOARD_HPP

#include <iostream>
#include <vector>

namespace othello {

    // Type pawn used by board in the othello representation
    typedef enum : unsigned short {
        empty = 0,
        black = 1,
        white = 2,
    } pawn;

    //
    typedef unsigned short direction;

    typedef enum : direction {
        NODIR = 0,
        TOP = 1,
        BOTTOM = 2,
        LEFT = 4,
        RIGHT = 8,
        DTL = 16,
        DTR = 32,
        DBL = 64,
        DBR = 128,
    } dirs;

    class Board {
    
        friend std::ostream& operator<<(std::ostream &os,const Board& item);

        private :
            // 64 unsigned short allocated array : contains the state of the board
            pawn *cases;
        
        protected :
            /**
             * Setter for case by array index
            */
            void setCase(int index,pawn value);

            /**
             * Setter for case by coordinate
             * 
             * uses : int setCase(int index, pawn value);
            */
            void setCase(const std::string& coordinate,pawn value);
        
        public :
            // Size of a row or column of the board
            const static int side = 8;
            // Number of cases of the board
            const static int length = 64;

            static std::vector<int> diff(const Board& a, const Board& b);

            /**
             * convert coordinate into index of the array representation
             * UNSAFE : does not check if the coordinate is in the board or not.
            */
            static int coord2Index(const std::string& coordinate);

            /**
             * convert index of the array representation into coordinate
             * UNSAFE : does not check if the coordinate is in the board or not.
            */
            static std::string index2Coord(int index);

            /**
             * default constructor
            */
            Board();

            /**
             * Copy constructor
            */
            Board(const Board& original);

            /**
             * destructor
            */
            ~Board();

            /**
             * Resets the board to its start state
            */
            void newGame();

            /**
             * Getter for case by array index
            */
            pawn getCase(int index) const;

            /**
             * Getter for case by coordinate
             * 
             * uses : int getCase(int index) const;
            */
            pawn getCase(const std::string& coordinate) const;

            /**
             * Check for a case if you can place a token
             * uses : int getCase(int index), 
            */
            direction canPlaceHere(int index, pawn team) const;

            /**
             * 
            */
            std::vector<int> listAllPlay(pawn team) const;

            int placePawn(int index, pawn team);

            /**
             * return the board in a string formatted a pretty way !
            */
            std::string pretty() const;

            /**
             * return the board in a string formatted a pretty way !
             * Shows the possible plays of the team with '.'
            */
            std::string prettyPlay(pawn team) const;

            /**
             * return an array of 3 int representing the number of empty case, team black and team white case.
            */
            void getPawnNumbers(int arr[3]) const;

            bool isFinished() const;

            /**
             * Realy unoptimised : do not use if you already called listAllPlay for one of the player.
            */
            int filledCount() const;
    };

    std::ostream& operator<<(std::ostream& os,const Board& item);
};

#endif
#ifndef GAME_H
#define GAME_H

#include "other_includes.h"
#include <iostream>

using namespace std;

enum HandState { won, lost, tie};

class Player {
    public:
        int pot;
        int cur_bet;
        Player(string nm);
        int bet(int amount);
        int addCard(int card, bool is_visible=true);
        int showHand();
        void endOfHand(HandState state);
        int hand; // FIXME this should be private

        string getName(){ return name;}

    private:
        string name="player";
};

class Deck{
    public:
            // map to keep track of numbers of cards
            Deck();
            Deck(int num_decks = 1);
            void shuffle();
            int dealCard();
    private:
    // Each value stores number of instances of each card
        map<char, int> original_deck;
        map<char,int> cur_deck;
};

#endif
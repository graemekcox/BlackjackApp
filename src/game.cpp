#include "../include/game.h"

Player::Player(string nm){
    pot = 50;
    cur_bet = 0;
    hand = 0; 
    name = nm;
    // name = nm;
};

void Player::endOfHand(HandState state){
    switch(state){
        case HandState::won:
            pot+= cur_bet;
            break;
        case HandState::lost:
            pot -= cur_bet;
            break;
        case HandState::tie:
            break;
    };
    cur_bet = 0;
    hand = 0;
    cout << name<< "'s pot is now $"<<pot<<endl;
}

int Player::bet(int amount){
    if (amount > pot)
        return -1;
    
    cur_bet = amount;
    cout<< name <<" has bet $"<<amount<<endl;

    return 0;
};

int Player::addCard(int card, bool is_visible){
    hand += card;
    if (hand > 21) // FIXME
        return -1;

    if (is_visible)
        cout << name<< "'s hand is now "<<hand << endl;
    return 0;
}

int Player::showHand(){
    cout<< name<<"'s hand is "<< hand <<endl;
    return hand;
}

Deck::Deck(){
    int num_decks = 1;
    original_deck['2'] = 4 * num_decks;
    original_deck['3'] = 4 * num_decks;
    original_deck['4'] = 4 * num_decks;
    original_deck['5'] = 4 * num_decks;
    original_deck['6'] = 4 * num_decks;
    original_deck['7'] = 4 * num_decks;
    original_deck['8'] = 4 * num_decks;
    original_deck['9'] = 4 * num_decks;
    original_deck['10'] = 4 * num_decks;
    original_deck['J'] = 4 * num_decks;
    original_deck['Q'] = 4 * num_decks;
    original_deck['K'] = 4 * num_decks;
    original_deck['A'] = 4 * num_decks;

    cur_deck = original_deck;
}
Deck::Deck(int num_decks){
    original_deck['2'] = 4 * num_decks;
    original_deck['3'] = 4 * num_decks;
    original_deck['4'] = 4 * num_decks;
    original_deck['5'] = 4 * num_decks;
    original_deck['6'] = 4 * num_decks;
    original_deck['7'] = 4 * num_decks;
    original_deck['8'] = 4 * num_decks;
    original_deck['9'] = 4 * num_decks;
    original_deck['10'] = 4 * num_decks;
    original_deck['J'] = 4 * num_decks;
    original_deck['Q'] = 4 * num_decks;
    original_deck['K'] = 4 * num_decks;
    original_deck['A'] = 4 * num_decks;

    cur_deck = original_deck;
}

void Deck::shuffle()
{
    // Reset current deck to the template
    cur_deck = original_deck;
}

int Deck::dealCard()
{
    auto it = cur_deck.begin();
    std::advance(it, rand() % cur_deck.size());

    char rand_key = it->first;
    int card_count = it->second;

    if (cur_deck[rand_key] <= 1)
        cur_deck.erase(it);
    else
        cur_deck[rand_key] -= 1; // Decrement out deck counter

    int card_val;
    if (rand_key == 'K' || rand_key == 'Q' || rand_key == 'J' || rand_key == 'A')
        card_val = 10;
    else
        card_val = int(rand_key) - '0';

    // cout << "Key="<<rand_key << " Card val = "<< card_val<<endl;
    
    return card_val;
}
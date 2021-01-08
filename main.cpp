#include "include/other_includes.h"
#include "include/handler.h"

#include <cpprest/filestream.h>

// using namespace std;
// using namespace web;
// using namespace http;
// using namespace utility;
// using namespace http::experimental::listener;

using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;   
std::unique_ptr<handler> g_httpHandler;
// http_listener to host table and act as dealer
// http_client to join tables and play

void on_initialize(const string_t& address)
{
    uri_builder uri(address);

    auto addr = uri.to_uri().to_string();
    g_httpHandler = std::unique_ptr<handler>(new handler(addr));

    g_httpHandler = std::unique_ptr<handler>(new handler(addr));
    g_httpHandler->open().wait();

    ucout << utility::string_t(U("Listening for requests at: ")) << addr <<std::endl;
    return;
}

void on_shutdown()
{
    g_httpHandler->close().wait();
    return;
}

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
        int hand;

    private:
        string name="player";
};

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
    cout << name<< "'s pot is now $"<<pot<<endl;
}

int Player::bet(int amount){
    if (amount < pot)
        return -1;
    
    cur_bet = amount;
    cout<< name <<" has bet $"<<amount<<endl;

    return 0;
};

int Player::addCard(int card, bool is_visible){
    hand += card;
    if (hand > 21)
        return -1;

    if (is_visible)
        cout << name<< "'s hand is now "<<hand << endl;
    return 0;
}

int Player::showHand(){
    cout<< name<<"'s hand is "<<hand<<endl;
    return hand;
}

bool is_number(const string& str)
{
    return !str.empty() && find_if(str.begin(), str.end(), [](unsigned char c){
        return !isdigit(c);
    }) == str.end();
}

int main(int argc, char *argv[])
{
    Player p1("G");
    Player dealer("Dealer");

    cout << "Starting pot is = " << p1.pot << endl;

    // do{
    //     cout<< "\n Press a key to continue" << endl;
    // } while ( cin.get() != 'q');
    bool game_on = true;
    bool start_of_hand = true;

    char in_buf;
    while (game_on){
        if (start_of_hand){
            cout<<"\nPress 'q' to quit. Otherwise bet amount <$100."<<endl;
            cin.get(in_buf);

            switch(in_buf){
                case ('q'):
                    //game_on = false;
                    break;
                case('Q'):
                    //game_on = false;
                    break;
                default:
                    p1.bet(10);
                    // p1.bet(int(in_buf)); // FIXME check input is actually a num
                    p1.addCard(10); // FIXME check for splits
                    p1.addCard(6);
                    dealer.addCard(10);
                    dealer.addCard(5, false);
            };
            start_of_hand = false;
        } else {
            cout<<"\nPress 'h' to hit. 's' to stay"<<endl;
            // cin.get(in_buf);
            cin >> in_buf;

            switch(in_buf){
                case('h'):
                    if (p1.addCard(2) == -1){
                        cout<<"You busted!"<< endl;
                        start_of_hand = true; // New hand
                    }
                    break;
                case('s'):
                    while (dealer.showHand() < 17){
                        dealer.addCard(4, false);
                    }

                    if (dealer.hand == p1.hand){
                        p1.endOfHand(HandState::tie);   
                    } else if (dealer.hand < p1.hand){
                        p1.endOfHand(HandState::won);
                    } else
                        p1.endOfHand(HandState::lost);
                    break;
                case('q'):
                    game_on = false;
                    break;
                default:
                    cout << "\nInvalid input"<<endl;
                    break;
            }
        }
    }

    cout << "You walked away with $"<< p1.pot << endl;

    return 0;
}
#include "include/other_includes.h"
#include "include/handler.h"
#include "include/game.h"

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

// void on_initialize(const string_t& address)
// {
//     uri_builder uri(address);

//     auto addr = uri.to_uri().to_string();
//     g_httpHandler = std::unique_ptr<handler>(new handler(addr));

//     g_httpHandler = std::unique_ptr<handler>(new handler(addr));
//     g_httpHandler->open().wait();

//     ucout << utility::string_t(U("Listening for requests at: ")) << addr <<std::endl;
//     return;
// }

// void on_shutdown()
// {
//     g_httpHandler->close().wait();
//     return;
// }

// bool is_number(const string& str)
// {
//     return !str.empty() && find_if(str.begin(), str.end(), [](unsigned char c){
//         return !isdigit(c);
//     }) == str.end();
// }


int main(int argc, char *argv[])
{
    Player p1("G");
    Player dealer("Dealer");

    cout << "Starting pot is = " << p1.pot << endl;

    bool game_on = true;
    bool start_of_hand = true;

    Deck deck(1);

    string in_buf;
    int bet_amount;

    while (game_on){
        if (start_of_hand){
            if (p1.pot == 0) { // If player is out of money
                cout<<"Sorry "<<p1.getName()<< ", but you're out of money!"<<endl;
                game_on = false;
                break;
            }

            cout<<"\nPress 'q' to quit. Otherwise bet amount <$100."<<endl;
            cin >> in_buf;

            deck.shuffle(); // FIXME Currently shuffling each hand, but maybe we should allow card counting

            if (in_buf == "q" || in_buf == "Q"){
                game_on = false;

            } else {
                std::stringstream(in_buf) >> bet_amount;
                if (p1.bet( bet_amount) == -1) // FIXME check input is actually a num
                    continue; // If bet is more than our pot, re-ask for inputs
                p1.addCard( deck.dealCard(),false); // FIXME check for splits
                p1.addCard( deck.dealCard(), false);
                p1.showHand();
                dealer.addCard( deck.dealCard());
                dealer.addCard( deck.dealCard(), false);
            }
            start_of_hand = false;
        } else {
            cout<<"\nPress 'h' to hit. 's' to stay"<<endl;
            cin >> in_buf;

            if (in_buf == "h"){
                if (p1.addCard(deck.dealCard()) == -1){
                    cout<<"You busted. Your hand was "<<p1.hand << endl;
                    p1.endOfHand(HandState::lost);
                    dealer.hand = 0;
                    start_of_hand = true; // New hand
                }               
            } else if (in_buf == "s"){
                while (dealer.showHand() < 17){
                    dealer.addCard( deck.dealCard(), false);
                }

                if (dealer.hand == p1.hand){
                    p1.endOfHand(HandState::tie);  
                    
                } else if (dealer.hand < p1.hand || dealer.hand > 21){
                    p1.endOfHand(HandState::won);
                } else
                    p1.endOfHand(HandState::lost);

                dealer.hand = 0;
                start_of_hand = true;           
            } else if ( in_buf == "q"){
                game_on = false;
            }
        }
    }

    cout << "You walked away with $"<< p1.pot << endl;

    return 0;
}
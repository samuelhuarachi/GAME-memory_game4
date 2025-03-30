#include <string>
#include <algorithm>
#include <array>
#include <iostream>
#include <ctime>

using namespace std;
using std::string;
#include "Cards.h"



int int_random (int i) { return std::rand()%i;}

Cards::Cards()
{
    std::srand ( unsigned ( std::time(0) ) );
    random_shuffle(std::begin(cards_deck), std::end(cards_deck), int_random);

}

string Cards::get_card_by_index(int index) {
    return cards_deck[index];
}

void Cards::shuffle_cards() {
    std::srand ( unsigned ( std::time(0) ) );
    random_shuffle(std::begin(cards_deck), std::end(cards_deck), int_random);
    random_shuffle(std::begin(cards), std::end(cards), int_random);
}

Cards::~Cards()
{
    //dtor
}
string Cards::cards_deck[] = {"dog", "cachorro", "cat", "gato", "house", "casa", "arvore", "tree", "bola",
"ball", "livro", "book", "carro", "car", "aviao", "airplane", "escola", "school", "sol", "sun"};

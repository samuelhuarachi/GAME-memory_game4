#include "Player.h"

Player::Player()
{
    //ctor
}

Player::~Player()
{
    //dtor
}

void Player::setName(const std::string& name)
{
    this->name = name;
}

void Player::setScore(int &score)
{
    this->score = score;
}


std::string Player::getName() const
{
    return name;
}

int Player::getScore() const
{
    return score;
}

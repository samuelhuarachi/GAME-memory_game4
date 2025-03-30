#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <iostream>

class Player
{
    public:
        Player();
        virtual ~Player();
        void setName(const std::string& name);
        std::string getName() const;
        void setScore(int &score);
        int getScore() const;

    protected:

    private:
        std::string name = "no name";
        int score = 0;
};

#endif // PLAYER_H

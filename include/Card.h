#ifndef CARD_H
#define CARD_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <string>
#include <iostream>

using namespace std;

class Card
{
    public:
        Card(const std::string& name, const int& identifier);
        virtual ~Card();
        void setName(const std::string& name);
        std::string getName() const;
        ALLEGRO_BITMAP* getImg() const;
        int getWidth() const;
        int getHeight() const;
        int getIdentifier() const;
        bool isFaceDown = false;

    protected:

    private:
        std::string name;
        ALLEGRO_BITMAP* some_img;
        const int identifier;
        int width = 100;
        int height = 100;

};

#endif // CARD_H

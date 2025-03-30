#include "Card.h"



Card::Card(const std::string& name, const int& identifier) : name(name), identifier(identifier)
{
    const char* basePath = "./images/";
    const char* extension = ".jpg";
    std::string tempPath = std::string(basePath) + name + extension;
    const char* fullPath = tempPath.c_str();

    some_img = al_load_bitmap(fullPath);
}

Card::~Card()
{
    al_destroy_bitmap(some_img);
}


void Card::setName(const std::string& name)
{
    this->name = name;
}


std::string Card::getName() const
{
    return name;
}

int Card::getIdentifier() const
{
    return identifier;
}

int Card::getWidth() const
{
    return width;
}

int Card::getHeight() const
{
    return height;
}


ALLEGRO_BITMAP* Card::getImg() const
{
    return some_img;
}

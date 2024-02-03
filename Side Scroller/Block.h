#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>


class Block
{
public:
    //Give data to create transparent block
    //X Y WIDTH HEIGHT FunctionID
    Block(int, int, int, int, int);
    //Give data to create block
    //X Y TextureLocation FunctionID
    Block(int, int, const std::string&, int);
    Block(int, int, const sf::Texture&, int);
    Block();
    ~Block();

    int collisionCheck(const sf::Sprite&);
    sf::Sprite getSprite();

    void draw(sf::RenderWindow&);

    sf::Sprite block;
private:
    sf::Texture t;
    //sf::Sprite block;
    int functionID;
};
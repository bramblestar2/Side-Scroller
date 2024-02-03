#include "Block.h"

Block::Block(int x, int y, int width, int height, int fID)
{
    functionID = fID;
    block.setPosition(x, y);

    if (fID == 1)
    {
        sf::Image image;
        image.create(width, height);
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
            {
                image.setPixel(x, y, sf::Color::Transparent);
            }
        t.loadFromImage(image);
    }
    block.setTexture(t);
}

Block::Block(int x, int y, const std::string& textureLocation, int fID)
{
    functionID = fID;
    block.setPosition(x, y);
    t.loadFromFile(textureLocation);
    block.setTexture(t);
}

Block::Block(int x, int y, const sf::Texture& texture, int fID)
{
    functionID = fID;
    block.setPosition(x, y);
    t = texture;
    block.setTexture(t);
}

Block::Block()
{
    functionID = 0;
}

Block::~Block() {

}

int Block::collisionCheck(const sf::Sprite& sprite)
{
    //Points of sprite
    sf::Vector2f a, b, c, d;

    //Top left
    a = sprite.getPosition();
    //Top right
    b = { sprite.getPosition().x + sprite.getGlobalBounds().width, sprite.getPosition().y };
    //Bottom left
    c = { sprite.getPosition().x, sprite.getPosition().y + sprite.getGlobalBounds().height };
    //Bottom right
    d = { sprite.getPosition().x + sprite.getGlobalBounds().width,
         sprite.getPosition().y + sprite.getGlobalBounds().height };

    int collision = -1;

    sf::Vector2f altSpritePos = { sprite.getPosition().x + (sprite.getGlobalBounds().width / 2),
                                 sprite.getPosition().y + sprite.getGlobalBounds().height };

    //Floor detection
    if (block.getGlobalBounds().contains(c) &&
        block.getGlobalBounds().contains(d) ||

        block.getGlobalBounds().contains(d) &&
        altSpritePos.y < block.getPosition().y + (block.getGlobalBounds().height / 10) ||

        block.getGlobalBounds().contains(c) &&
        altSpritePos.y < block.getPosition().y + (block.getGlobalBounds().height / 10))
    {
        collision = 0;
    }

    //Ceiling detection
    else if (block.getGlobalBounds().contains(a) &&
        block.getGlobalBounds().contains(b) || 

        block.getGlobalBounds().contains(b) &&
        (double)sprite.getPosition().y + sprite.getGlobalBounds().height >
        block.getPosition().y + (block.getGlobalBounds().height + (sprite.getGlobalBounds().height / 1.2)) ||

        block.getGlobalBounds().contains(a) &&
        (double)sprite.getPosition().y + sprite.getGlobalBounds().height >
        block.getPosition().y + (block.getGlobalBounds().height + (sprite.getGlobalBounds().height / 1.2)))
    {
        collision = 1;
    }

    //Right collision
    else if (block.getGlobalBounds().contains(b) &&
        block.getGlobalBounds().contains(d) ||
        block.getGlobalBounds().contains(d) ||
        block.getGlobalBounds().contains(b))
    {
        collision = 2;
    }

    //Left collision
    else if (block.getGlobalBounds().contains(a) &&
        block.getGlobalBounds().contains(c) || 
        block.getGlobalBounds().contains(c) ||
        block.getGlobalBounds().contains(a))
    {
        collision = 3;
    }

    //Death block
    if (block.getGlobalBounds().intersects(sprite.getGlobalBounds()) && functionID == 1)
    {
        collision = 4;
    }

    return collision;
}


sf::Sprite Block::getSprite()
{
    return block;
}


void Block::draw(sf::RenderWindow& window)
{
    window.draw(block);
}
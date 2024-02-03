#include "functions.h"

const int MAXSPEED = 7;
const int MAXJUMP = 9;
const int DASHSPEED = 5;

const int PLAYER_TEXTURE_WIDTH = 32;
const int PLAYER_TEXTURE_HEIGHT = 32;
const int PLAYER_TEXTURE_MOVE = 32;
const string BLOCK_INFO_LOCATION = "info/blocks.txt";

Texture blankTexture(int width, int height, Color color)
{
    Texture t;

    Image image;
    image.create(width, height);
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
        {
            image.setPixel(x, y, color);
        }

    t.loadFromImage(image);

    return t;
}

void movement(Sprite& s, double& vX, double& vY)
{
    double speedUp = (double)MAXSPEED / 30;
    float lerpSpeed = 0.5f;

    if (Keyboard::isKeyPressed(Keyboard::S))
    {
        s.move(0, 1);
    }
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        if (vX > 0)
            vX = lerp(vX, -MAXSPEED, lerpSpeed);
            //vX -= speedUp * 2;
        else if (vX > -MAXSPEED)
            vX = lerp(vX, -MAXSPEED, lerpSpeed);
            //vX -= speedUp;
    }
    else if (Keyboard::isKeyPressed(Keyboard::D))
    {
        if (vX < 0)
            vX = lerp(vX, MAXSPEED, lerpSpeed);
            //vX += speedUp * 2;
        else if (vX < MAXSPEED)
            vX = lerp(vX, MAXSPEED, lerpSpeed);
            //vX += speedUp;
    }
    else
    {
        vX = lerp(vX, 0, 0.2f);
        //vY = lerp(vY, 0, 0.2f);

        //if (vX > 0)
        //    vX -= 0.2f;
        //else if (vX < 0)
        //    vX += 0.2f;
        
        //if (vX > -0.5 && vX < 0.5)
        //    vX = 0;
    }
}

float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

void scale(Sprite& s, double x, double y)
{
    s.setScale(x / s.getTexture()->getSize().x, y / s.getTexture()->getSize().y);
}

vector<Block*> getMap()
{
    fstream map;
    map.open(BLOCK_INFO_LOCATION, ios::in);

    vector<Block*> blocks;

    if (map.is_open())
    {
        while (!map.eof())
        {
            int x, y, width, height, functionID;
            string image;
            map >> x >> y >> width >> height >> functionID >> image;
            blocks.push_back(new Block(x, y, image, functionID));

            scale(blocks.at(blocks.size() - 1)->block, width, height);
        }
    }

    return blocks;
}

bool animatePlayer(double vX, double vY, Texture& playerTex, Sprite& player)
{
    string jumpImgLocation = "Player Sprite/Owlet/Owlet_Monster_Jump_8.png";
    string walkImgLocation = "Player Sprite/Owlet/Owlet_Monster_Walk_6.png";
    string runImgLocation = "Player Sprite/Owlet/Owlet_Monster_Run_6.png";
    string idleImgLocation = "Player Sprite/Owlet/Owlet_Monster_Idle_4.png";

    static Clock clock;
    Time time = clock.getElapsedTime();

    bool frameChanged = false;

    static int FRAME = 0;
    static bool JUMPING = true;
    static bool IDLE = false;

    static bool FLIP_IDLE = false;

    if (vY < -MAXJUMP + 0.5)
        FRAME = 0;

    if ((vX > 0.2 || vX < -0.2) && !JUMPING)
    {
        if (IDLE)
        {
            IDLE = false;
            FRAME = 0;
        }
        //160 MAX
        if (time.asSeconds() > 0.1)
        {
            if (abs(vX) < MAXSPEED / 1.5)
            {
                if (!playerTex.loadFromFile(walkImgLocation, IntRect(FRAME, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT)))
                    playerTex = blankTexture(PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT, Color::White);
            }
            else
            {
                if (!playerTex.loadFromFile(runImgLocation, IntRect(FRAME, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT)))
                    playerTex = blankTexture(PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT, Color::White);
            }


            FRAME += PLAYER_TEXTURE_MOVE;

            if (FRAME >= 160)
                FRAME = 0;
            clock.restart();
        }
    }
    else
    {
        if (!JUMPING)
        {
            if (!IDLE)
            {
                FLIP_IDLE = false;
                IDLE = true;
                FRAME = 0;

                if (!playerTex.loadFromFile(idleImgLocation, IntRect(FRAME, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT)))
                    playerTex = blankTexture(PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT, Color::White);
            }
            else
            {
                if (time.asSeconds() > 0.3)
                {
                    if (!playerTex.loadFromFile(idleImgLocation, IntRect(FRAME, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT)))
                        playerTex = blankTexture(PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT, Color::White);

                    if (!FLIP_IDLE)
                        FRAME += PLAYER_TEXTURE_MOVE;
                    else
                        FRAME -= PLAYER_TEXTURE_MOVE;

                    if (FRAME >= 96)
                        FLIP_IDLE = true;
                    else if (FRAME <= 0)
                        FLIP_IDLE = false;
                    //FRAME = 0;

                    clock.restart();
                }
            }
        }
    }

    //Jump animation
    if (vY != 0)
    {
        if (!JUMPING)
            FRAME = 0;
        JUMPING = true;
        if (time.asSeconds() > 0.1)
        {
            if (!playerTex.loadFromFile(jumpImgLocation, IntRect(FRAME, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT)))
                playerTex = blankTexture(PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT, Color::White);

            if (FRAME < 64 && vY < 0)
                FRAME += PLAYER_TEXTURE_MOVE;
            else if (vY > 0 && FRAME < 192)
                FRAME += PLAYER_TEXTURE_MOVE;

            if (vY > 0 && FRAME < 160)
                FRAME += PLAYER_TEXTURE_MOVE;

            if (FRAME < 160 && vY > 0)
                FRAME = 160;

            clock.restart();
        }
    }
    else
    {
        if (JUMPING)
        {
            JUMPING = false;
            FRAME = 0;

            if (!playerTex.loadFromFile(jumpImgLocation, IntRect(FRAME, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT)))
            {
                playerTex = blankTexture(PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT, Color::White);
            }
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        player.setTextureRect(IntRect(PLAYER_TEXTURE_WIDTH, 0, -PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT));
        //player.setScale(-abs(player.getScale().x), player.getScale().y);
    }
    else if (Keyboard::isKeyPressed(Keyboard::D))
    {
        player.setTextureRect(IntRect(0, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT));
        //player.setScale(abs(player.getScale().x), player.getScale().y);
    }

    if (frameChanged)
    {
        //player.setOrigin(playerTex.getSize().x/2, playerTex.getSize().y / 2);
        player.setTexture(playerTex);

    }

    return frameChanged;
}

void editor(RenderWindow& window, vector<Block*>& blocks)
{
    static bool leftMousePressed = false;
    static bool middleMousePressed = false;
    static Vector2f topLeftPoint, bottomRightPoint, offset;

    Vector2i mouse = Mouse::getPosition(window);
    Vector2f mouse_world = window.mapPixelToCoords(mouse);

    if (Mouse::isButtonPressed(Mouse::Left))
    {
        if (!leftMousePressed)
        {
            leftMousePressed = true;
            topLeftPoint = mouse_world;
        }

        bottomRightPoint = Vector2f(mouse_world - topLeftPoint);

        RectangleShape block;
        block.setPosition(topLeftPoint);
        block.setSize(bottomRightPoint);
        block.setFillColor(Color::White);

        window.draw(block);
    }
    else if (leftMousePressed)
    {
        //cout << topLeftPoint.x << " - " << topLeftPoint.y << endl;
        //cout << bottomRightPoint.x << " - " << bottomRightPoint.y << endl;

        fstream info;
        info.open(BLOCK_INFO_LOCATION, ios::app);

        Vector2i topLeft = Vector2i(topLeftPoint);
        Vector2i bottomRight = Vector2i(bottomRightPoint);

        if (abs(bottomRight.x) > 32 && abs(bottomRight.y) > 10 || abs(bottomRight.x) > 10 && abs(bottomRight.y) > 32)
            info << endl << topLeft.x << " " << topLeft.y << " " << bottomRight.x << " " << bottomRight.y << "  0 Textures/block.png";

        info.close();

        blocks = getMap();

        leftMousePressed = false;
    }
    if (Mouse::isButtonPressed(Mouse::Middle))
    {
        //detect if mouse is inside a block
        for (int i = 0; i < blocks.size(); i++)
        {
            if (blocks.at(i)->block.getGlobalBounds().contains(mouse_world))
            {
                vector<Vector2i> xy, widthHeight;
                vector<int> vfID;
                vector<string> vtextureLocation;

                fstream info;
                info.open(BLOCK_INFO_LOCATION, ios::in);

                if (info.is_open())
                {
                    while (!info.eof())
                    {
                        int x, y, width, height, fID;
                        string textureL;

                        info >> x >> y >> width >> height >> fID >> textureL;

                        xy.push_back(Vector2i(x, y));
                        widthHeight.push_back(Vector2i(width, height));
                        vtextureLocation.push_back(textureL);
                        vfID.push_back(fID);

                        //UNFINISHED
                        if (blocks.at(i)->block.getPosition().x == x && blocks.at(i)->block.getPosition().y == y &&
                            blocks.at(i)->block.getGlobalBounds().width == width && blocks.at(i)->block.getGlobalBounds().height == height)
                        {
                            xy.erase(xy.begin() + i);
                            widthHeight.erase(widthHeight.begin() + i);
                            vtextureLocation.erase(vtextureLocation.begin() + i);
                            vfID.erase(vfID.begin() + i);
                        }
                    }
                }
                info.close();

                info.open(BLOCK_INFO_LOCATION, ios::out);

                for (int k = 0; k < xy.size(); k++)
                {
                    info << xy.at(k).x << " " << xy.at(k).y << " " <<
                        widthHeight.at(k).x << " " << widthHeight.at(k).y << " " <<
                        vfID.at(k) << " " << vtextureLocation.at(k);

                    if (k != xy.size() - 1)
                        info << endl;
                }

                info.close();

                blocks = getMap();
            }
        }
    }
}

void deathState(Sprite& sprite, Texture& texture, bool& isDead)
{
    string deathImgLoc = "Player Sprite/Owlet/Owlet_Monster_Death_8.png";

    static Clock deathClock;
    Time time = deathClock.getElapsedTime();

    static int frame = 0;

    bool frameChanged = false;

    if (!isDead)
    {
        if (sprite.getColor().a != 255)
            sprite.setColor(Color(255, 255, 255, 255));

        if (sprite.getPosition().y > 1500)
            isDead = true;

        frame = 0;
    }
    else
    {
        if (time.asSeconds() > 0.2)
        {
            texture.loadFromFile(deathImgLoc, IntRect(frame, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT));

            //224 MAX
            if (frame <= 224)
                frame += PLAYER_TEXTURE_MOVE;

            frameChanged = true;
            deathClock.restart();
        }

        if (frame == 256 && time.asSeconds() > 0.1)
        {
            sprite.setColor(Color(255, 255, 255, lerp(sprite.getColor().a, 0, 0.3f)));
            deathClock.restart();
        }

        if (frameChanged)
        {
            sprite.setTexture(texture);
        }
    }
}

bool collisionCheck(Sprite& spriteOne, Block& spriteTwo)
{
    bool onGround = false;

    int collision = spriteTwo.collisionCheck(spriteOne);

    if (collision == 0) // Floor
    {
        spriteOne.setPosition(spriteOne.getPosition().x,
            spriteTwo.getSprite().getPosition().y - spriteOne.getGlobalBounds().height);
        onGround = true;
    }
    else if (collision == 1) // Ceiling
    {
        spriteOne.setPosition(spriteOne.getPosition().x, spriteTwo.getSprite().getPosition().y +
            spriteTwo.getSprite().getGlobalBounds().height);
    }
    else if (collision == 2) // Left
    {
        spriteOne.setPosition(spriteTwo.getSprite().getPosition().x -
            spriteOne.getGlobalBounds().width,
            spriteOne.getPosition().y);
    }
    else if (collision == 3) // Right
    {
        spriteOne.setPosition(spriteTwo.getSprite().getPosition().x +
            spriteTwo.getSprite().getGlobalBounds().width, spriteOne.getPosition().y);
    }
    return onGround;
}
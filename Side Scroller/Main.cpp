#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <fstream>
#include <string>
#include "Block.h"
#include "functions.h"

const int PLAYER_TEXTURE_WIDTH = 32;
const int PLAYER_TEXTURE_HEIGHT = 32;
const int PLAYER_TEXTURE_MOVE = 32;
const std::string BLOCK_INFO_LOCATION = "info/blocks.txt";

using namespace std;
using namespace sf;

const int MAXSPEED = 7;
const int MAXJUMP = 9;
const int DASHSPEED = 5;

int main()
{
    RenderWindow window(VideoMode(800, 600), "Platformer");
    window.setFramerateLimit(60);

    Image image;
    image.loadFromFile("icon.png");
    
    window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

    Sprite player;
    Texture playerS = blankTexture(PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT, Color::White);
    player.setTexture(playerS);
    player.setScale(1.5, 1.5);

    vector<Block*> blocks = getMap();
    //x y width height functionID IMAGE

    //Foreground blocks

    double vY = 0, vX = 0;
    double dashVX = 0;
    
    bool dashX = false;
    bool playerFalling = true;
    bool canJump = false;

    //this will replace canJump
    const int MAXJUMPS = 2;
    int jumpCount = 0;
    Clock jumpCounter;

    View v1, v2, v3;
    v1.setSize(window.getSize().x, window.getSize().y);
    v1.setCenter(player.getPosition().x + (player.getGlobalBounds().width / 2),
                 player.getPosition().y + (player.getGlobalBounds().height / 2));

    v2.setSize(window.getSize().x, window.getSize().y);

    v3.setSize(window.getSize().x, window.getSize().y);
    Vector2f cameraThreePosition = {500, 450};

    double zoom = 0.5;

    bool isDead = false;

    int viewNumber = 1;

    Clock dashTime;

    //DASHING
    Sprite dashSprite;
    Texture dashTexture;
    string dashLocation = "Player Sprite/Owlet/Walk_Run_Push_Dust_6.png";
    int dashFrame = 0;
    bool dashIsVisible = false;
    Clock dashFrameTime;
    bool dashSpriteMovingLeft = false;

    //160 MAX
    dashTexture.loadFromFile(dashLocation, IntRect(dashFrame, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT));
    dashSprite.setTexture(dashTexture);
    dashSprite.setColor(Color(255, 255, 255, 0));
    dashSprite.setScale(player.getScale().x, player.getScale().y);

    //JUMPING SPRITE
    Sprite jumpSprite;
    Texture jumpTexture;
    string jumpLocation = "Player Sprite/Owlet/Double_Jump_Dust_5.png";
    int jumpFrame = 0;
    bool jumpDustIsVisible = false;
    Clock jumpFrameTime;
    
    //128 MAX
    jumpTexture.loadFromFile(jumpLocation, IntRect(jumpFrame, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT));
    jumpSprite.setTexture(jumpTexture);
    jumpSprite.setColor(Color(255, 255, 255, 0));
    jumpSprite.setScale(player.getScale().x, player.getScale().y);


    //EDITOR
    bool editorEnabled = false;

    while (window.isOpen())
    {
        
        float time = 0.1f;
        float interpSpeed = 0.1f;

        v1.setCenter(lerp(v1.getCenter().x, player.getPosition().x + (player.getGlobalBounds().width / 2), time),
                     lerp(v1.getCenter().y, player.getPosition().y + (player.getGlobalBounds().height / 2), time));
        v2.setCenter(player.getPosition().x + (player.getGlobalBounds().width / 2), 
                     player.getPosition().y + (player.getGlobalBounds().height / 2));
        v3.setCenter(lerp(v3.getCenter().x, cameraThreePosition.x, time*2),
                     lerp(v3.getCenter().y, cameraThreePosition.y, time*2));

        v1.setSize(lerp(v1.getSize().x, window.getSize().x * zoom, interpSpeed), 
                   lerp(v1.getSize().y, window.getSize().y * zoom, interpSpeed));
        v2.setSize(lerp(v2.getSize().x, window.getSize().x * zoom, interpSpeed), 
                   lerp(v2.getSize().y, window.getSize().y * zoom, interpSpeed));

        if (player.getPosition().x + (player.getTexture()->getSize().x / 1.5) > (double)v3.getCenter().x + (v3.getSize().x / 2))
            cameraThreePosition.x += v3.getSize().x;
        //v3.move(v3.getSize().x, 0);
        else if (player.getPosition().x + (player.getTexture()->getSize().x / 1.5) < (double)v3.getCenter().x - (v3.getSize().x / 2))
            cameraThreePosition.x -= v3.getSize().x;
            //v3.move(-v3.getSize().x, 0);
        if (player.getPosition().y > (double)v3.getCenter().y + (v3.getSize().y / 2))
            cameraThreePosition.y += v3.getSize().y;
            //v3.move(0, v3.getSize().y);
        else if (player.getPosition().y < (double)v3.getCenter().y - (v3.getSize().y / 2))
            cameraThreePosition.y -= v3.getSize().y;
            //v3.move(0, -v3.getSize().y);

        if (viewNumber == 1)
            window.setView(v1);
        else if (viewNumber == 2)
            window.setView(v2);
        else if (viewNumber == 3)
            window.setView(v3);

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed)
            {
                if (Keyboard::isKeyPressed(Keyboard::Escape))
                    window.close();
                if (Keyboard::isKeyPressed(Keyboard::R))
                {
                    player.setPosition(0, 0);
                    isDead = false;
                    vX = 0;
                    vY = 0;
                }
                if (Keyboard::isKeyPressed(Keyboard::Num1))
                    blocks = getMap();
                if (Keyboard::isKeyPressed(Keyboard::Num2))
                {
                    int x = 0, y = 0;
                    cout << "\nPlease enter X Y coordinates: ";
                    cin >> x >> y;
                    player.setPosition(x, y);

                    
                    window.requestFocus();
                }
                if (Keyboard::isKeyPressed(Keyboard::Num9))
                {
                    if (!editorEnabled)
                    {
                        editorEnabled = true;
                        cout << "Entered Editor" << endl;
                    }
                    else
                    {
                        editorEnabled = false;
                        cout << "Exited Editor" << endl;
                    }
                }
                if (Keyboard::isKeyPressed(Keyboard::Up))
                    zoom += 0.1;
                else if (Keyboard::isKeyPressed(Keyboard::Down) && zoom >= 0.2)
                    zoom -= 0.1;
                else if (Keyboard::isKeyPressed(Keyboard::Left))
                    zoom = 1;

                if (Keyboard::isKeyPressed(Keyboard::Numpad1))
                    viewNumber = 1;
                else if (Keyboard::isKeyPressed(Keyboard::Numpad2))
                    viewNumber = 2;
                else if (Keyboard::isKeyPressed(Keyboard::Numpad3))
                    viewNumber = 3;
            }
            if (event.type == Event::MouseWheelMoved)
            {
                if (event.mouseWheel.delta == -1)
                    zoom += 0.1;
                else if (event.mouseWheel.delta == 1 && zoom >= 0.2)
                    zoom -= 0.1;
            }
        }

        for (int i = 0; i < blocks.size() && !playerFalling; i++)
        {
            if (!collisionCheck(player, *blocks.at(i)) && !playerFalling)
            {
                playerFalling = true;
            }
        }
        for (int i = 0; i < blocks.size(); i++)
        {
            if (collisionCheck(player, *blocks.at(i)) && playerFalling)
            {
                if (vY != 0)
                    vY = 0;
                playerFalling = false;
                //canJump = true;
                jumpCount = MAXJUMPS;
            }
        }

        //////////////////////
        /// Main Functions ///
        //////////////////////
        if (window.hasFocus() && !isDead)
        {
            movement(player, vX, vY);

            animatePlayer(vX, vY, playerS, player);

            Time time = jumpCounter.getElapsedTime();

            if (Keyboard::isKeyPressed(Keyboard::Space) && jumpCount != 0 && time.asSeconds() > 0.5)
            {
                vY = -MAXJUMP;
                jumpCount--;
                jumpCounter.restart();
                jumpFrame = 0;

                jumpDustIsVisible = true;
                jumpSprite.setPosition(player.getPosition().x,
                                       player.getPosition().y + (jumpTexture.getSize().y/3));
            }

            if (playerFalling)
            {
                if (vY < 10)
                    vY += 0.3f;
            }

            time = dashTime.getElapsedTime();

            if (!dashX && jumpCount > MAXJUMPS-2)
            {
                if (Keyboard::isKeyPressed(Keyboard::LShift) && time.asSeconds() > 1)
                {
                    if (Keyboard::isKeyPressed(Keyboard::A))
                    {
                        dashVX = -DASHSPEED;
                        dashX = true;
                        dashTime.restart();
                        dashSpriteMovingLeft = true;

                        dashIsVisible = true;
                        dashSprite.setPosition(player.getPosition().x, player.getPosition().y);
                        dashSprite.setTextureRect(player.getTextureRect());
                    }
                    else if (Keyboard::isKeyPressed(Keyboard::D))
                    {
                        dashVX = DASHSPEED;
                        dashX = true;
                        dashTime.restart();
                        dashSpriteMovingLeft = false;

                        dashIsVisible = true;
                        dashSprite.setPosition(player.getPosition().x, player.getPosition().y);
                        dashSprite.setTextureRect(player.getTextureRect());
                    }
                }
            }
            else if (dashX)
            {
                if (dashVX != 0)
                {
                    if (Keyboard::isKeyPressed(Keyboard::A) && dashVX > 0)
                        dashVX = 0;
                    else if (Keyboard::isKeyPressed(Keyboard::D) && dashVX < 0)
                        dashVX = 0;
                }

                if (time.asSeconds() > 0.15)
                {
                    if (dashVX > 0)
                        dashVX -= DASHSPEED / 5;
                    else if (dashVX < 0)
                        dashVX += DASHSPEED / 5;

                    if (dashVX == 0)
                        dashX = false;

                    dashTime.restart();
                }
            }

            player.move(vX+dashVX, vY);
        }

        //Dash Sprite
        if (dashIsVisible)
        {
            Time time = dashFrameTime.getElapsedTime();

            dashSprite.setColor(Color(255, 255, 255, lerp(dashSprite.getColor().a, 255, 0.2f)));

            if (time.asSeconds() > 0.2)
            {
                dashTexture.loadFromFile(dashLocation, IntRect(dashFrame, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT));
                dashFrame += PLAYER_TEXTURE_MOVE;

                if (dashFrame >= 160)
                {
                    dashFrame = 0;
                    dashIsVisible = false;
                }

                if (dashSpriteMovingLeft)
                    dashSprite.move(5, 0);
                else
                    dashSprite.move(-5, 0);

                dashFrameTime.restart();
            }
        }
        else
        {
            dashSprite.setColor(Color(255, 255, 255, lerp(dashSprite.getColor().a, 0, 0.3f)));
        }

        //Jump Dust Sprite
        if (jumpDustIsVisible)
        {
            Time time = jumpFrameTime.getElapsedTime();

            jumpSprite.setColor(Color(255, 255, 255, lerp(jumpSprite.getColor().a, 255, 0.2f)));

            if (time.asSeconds() > 0.2)
            {
                jumpTexture.loadFromFile(jumpLocation, IntRect(jumpFrame, 0, PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT));
                jumpFrame += PLAYER_TEXTURE_MOVE;

                //128 MAX
                if (jumpFrame >= 128)
                {
                    jumpFrame = 0;
                    jumpDustIsVisible = false;
                }

                jumpFrameTime.restart();
            }
        }
        else
        {
            jumpSprite.setColor(Color(255, 255, 255, lerp(jumpSprite.getColor().a, 0, 0.2f)));
        }

        deathState(player, playerS, isDead);

        window.clear(Color(25, 25, 25));
        
        RectangleShape deathFloor;
        deathFloor.setPosition(v2.getCenter().x, 1500+player.getGlobalBounds().height);
        deathFloor.setFillColor(Color(25, 0, 0));
        deathFloor.setSize(Vector2f(v2.getSize().x*2, v2.getSize().y));
        deathFloor.setOrigin(deathFloor.getSize().x/2, 0);

        window.draw(deathFloor);

        for (int i = 0; i < blocks.size(); i++)
            blocks.at(i)->draw(window);

        window.draw(dashSprite);
        window.draw(jumpSprite);

        window.draw(player);
        
        //EDITOR
        if (editorEnabled && window.hasFocus())
        {
            editor(window, blocks);
        }

        window.display();
    }

    return 1;
}